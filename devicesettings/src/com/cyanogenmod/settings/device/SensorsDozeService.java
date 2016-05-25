/*
 * Copyright (c) 2015 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.cyanogenmod.settings.device;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.media.AudioManager;
import android.os.IBinder;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.os.SystemClock;
import android.os.UserHandle;
import android.os.Vibrator;
import android.preference.PreferenceManager;
import android.provider.Settings;
import android.util.Log;

public class SensorsDozeService extends Service {

    public static final boolean DEBUG = false;
    public static final String TAG = "SensorsDozeService";

    private static final String DOZE_INTENT = "com.android.systemui.doze.pulse";

    private static final int HANDWAVE_DELTA_NS = 1000 * 1000 * 1000;
    private static final int PULSE_MIN_INTERVAL_MS = 5000;
    private static final int SENSORS_WAKELOCK_DURATION = 1000;
    private static final int VIBRATOR_ACKNOWLEDGE = 40;

    private static final String KEY_GESTURE_HAND_WAVE = "gesture_hand_wave";
    private static final String KEY_GESTURE_PICK_UP = "gesture_pick_up";
    private static final String KEY_GESTURE_POCKET = "gesture_pocket";
    private static final String KEY_PROXIMITY_WAKE = "proximity_wake_enable";

    private Context mContext;
    private OrientationSensor mOrientationSensor;
    private PickUpSensor mPickUpSensor;
    private PowerManager mPowerManager;
    private ProximitySensor mProximitySensor;
    private SensorManager mSensorManager;
    private WakeLock mSensorsWakeLock;

    private boolean mDozeEnabled = false;
    private boolean mHandwaveDoze = false;
    private boolean mHandwaveGestureEnabled = false;
    private boolean mPickUpDoze = false;
    private boolean mPickUpGestureEnabled = false;
    private boolean mPickUpState = false;
    private boolean mPocketDoze = false;
    private boolean mPocketGestureEnabled = false;
    private boolean mProximityNear = false;
    private boolean mProximityWake = false;
    private boolean mProximityWakeEnabled = false;
    private long mLastPulseTimestamp = 0;
    private long mLastStowedTimestamp = 0;

    private OrientationSensor.OrientationListener mOrientationListener =
            new OrientationSensor.OrientationListener() {
        public void onEvent() {
            setOrientationSensor(false, false);
            handleOrientation();
        }
    };

    private PickUpSensor.PickUpListener mPickUpListener =
            new PickUpSensor.PickUpListener() {
        public void onEvent() {
            mPickUpState = mPickUpSensor.isPickedUp();
            handlePickUp();
        }
        public void onInit() {
            mPickUpState = mPickUpSensor.isPickedUp();
            if (DEBUG) Log.d(TAG, "Pick-up sensor init : " + mPickUpState);
        }
    };

    private ProximitySensor.ProximityListener mProximityListener =
            new ProximitySensor.ProximityListener() {
        public void onEvent(boolean isNear, long timestamp) {
            mProximityNear = isNear;
            handleProximity(timestamp);
        }
        public void onInit(boolean isNear, long timestamp) {
            if (DEBUG) Log.d(TAG, "Proximity sensor init : " + isNear);
            mLastStowedTimestamp = timestamp;
            mProximityNear = isNear;

            // Pick-up or Orientation sensor initialization
            if (!isEventPending() && !isNear && isPickUpEnabled()) {
                setPickUpSensor(true, false);
            }
        }
    };

    public void onCreate() {
        if (DEBUG) Log.d(TAG, "Creating service");

        super.onCreate();
        mContext = this;

        mPowerManager = (PowerManager) mContext.getSystemService(
                Context.POWER_SERVICE);
        mSensorManager = (SensorManager) mContext.getSystemService(
                Context.SENSOR_SERVICE);
        mSensorsWakeLock = mPowerManager.newWakeLock(
                PowerManager.PARTIAL_WAKE_LOCK, TAG + "WakeLock");

        mOrientationSensor = new OrientationSensor(mContext, mSensorManager,
                mOrientationListener);
        mPickUpSensor = new PickUpSensor(mContext, mSensorManager,
                mPickUpListener);
        mProximitySensor = new ProximitySensor(mContext, mSensorManager,
                mProximityListener);

        SharedPreferences sharedPrefs = PreferenceManager.
                getDefaultSharedPreferences(mContext);
        loadPreferences(sharedPrefs);
        sharedPrefs.registerOnSharedPreferenceChangeListener(mPrefListener);
    }

    public int onStartCommand(Intent intent, int flags, int startId) {
        if (DEBUG) Log.d(TAG, "Starting service");

        IntentFilter intentScreen = new IntentFilter(Intent.ACTION_SCREEN_ON);
        intentScreen.addAction(Intent.ACTION_SCREEN_OFF);
        mContext.registerReceiver(mScreenStateReceiver, intentScreen);
        if (!mPowerManager.isInteractive()) {
            onDisplayOff();
        }

        return START_STICKY;
    }

    public void onDestroy() {
        if (DEBUG) Log.d(TAG, "Destroying service");

        super.onDestroy();
        setOrientationSensor(false, true);
        setPickUpSensor(false, true);
        setProximitySensor(false, true);
    }

    public IBinder onBind(Intent intent) {
        return null;
    }

    private void getDozeEnabled() {
        boolean enabled = true;
        if (android.provider.Settings.Secure.getInt(
                mContext.getContentResolver(), Settings.Secure.DOZE_ENABLED,
                1) == 0) {
            enabled = false;
        }
        mDozeEnabled = enabled;
    }

    private boolean isDozeEnabled() {
        return mDozeEnabled;
    }

    private boolean isHandwaveEnabled() {
        return mHandwaveGestureEnabled && isDozeEnabled();
    }

    private boolean isPickUpEnabled() {
        return mPickUpGestureEnabled && isDozeEnabled();
    }

    private boolean isPocketEnabled() {
        return mPocketGestureEnabled && isDozeEnabled();
    }

    private boolean isProximityWakeEnabled() {
        return mProximityWakeEnabled;
    }

    private boolean isEventPending() {
        return mHandwaveDoze || mPickUpDoze || mPocketDoze || mProximityWake;
    }

    private void handleProximity(long timestamp) {
        long delta = timestamp - mLastStowedTimestamp;
        boolean quickWave = delta < HANDWAVE_DELTA_NS;
        getDozeEnabled();
        if (DEBUG) Log.d(TAG, "Proximity sensor : isNear " + mProximityNear);

        // Proximity sensor released
        if (!mProximityNear) {
            mHandwaveDoze = false;
            mPickUpDoze = false;
            mPocketDoze = false;
            mProximityWake = false;

            // Handwave / Pick-up / Pocket gestures activated
            if (isHandwaveEnabled() && isPickUpEnabled() &&
                    isPocketEnabled()) {
                mHandwaveDoze = quickWave;
                mPickUpDoze = !quickWave;
                mPocketDoze = !quickWave;
                setOrientationSensor(true, false);
            }
            // Proximity Wake detected
            else if (isProximityWakeEnabled() && quickWave) {
                mProximityWake = true;
                setOrientationSensor(true, false);
            }
            // Handwave Doze detected
            else if (isHandwaveEnabled() && quickWave) {
                mHandwaveDoze = true;
                setOrientationSensor(true, false);
            }
            // Pick-up / Pocket Doze detected
            else if ((isPickUpEnabled() || isPocketEnabled()) && !quickWave) {
                mPickUpDoze = isPickUpEnabled();
                mPocketDoze = isPocketEnabled();
                setOrientationSensor(true, false);
            }
            // Start the pick-up sensor
            else if (isPickUpEnabled()) {
                setPickUpSensor(true, false);
            }
        }
        // Proximity sensor stowed
        else {
            mLastStowedTimestamp = timestamp;
            setOrientationSensor(false, false);
            setPickUpSensor(false, false);
        }
    }

    private void handleOrientation() {
        if (DEBUG) Log.d(TAG, "Orientation sensor : " + 
                    "FaceDown " + mOrientationSensor.isFaceDown() +
                    ", FaceUp " + mOrientationSensor.isFaceUp() +
                    ", Vertical " + mOrientationSensor.isVertical());

        // Orientation Doze analysis
        if (!mProximityNear) {
            analyseDoze();
        }
    }

    private void handlePickUp() {
        getDozeEnabled();
        if (DEBUG) Log.d(TAG, "Pick-up sensor : " + mPickUpState);

        // Pick-up Doze analysis
        if (mPickUpState && isPickUpEnabled()) {
            mPickUpDoze = true;
            launchWakeLock();
            analyseDoze();
        }
        // Picked-down
        else {
            mPickUpDoze = false;
        }
    }

    private void analyseDoze() {
        getDozeEnabled();
        if (DEBUG)
            Log.d(TAG, "Doze analysis : HandwaveDoze " + mHandwaveDoze +
                    ", PickUpDoze " + mPickUpDoze +
                    ", PocketDoze " + mPocketDoze +
                    ", ProximityWake " + mProximityWake +
                    ", PickUpState " + mPickUpState);

        // Handwave Doze launch
        if (mHandwaveDoze && !mOrientationSensor.isFaceDown()) {
            launchDozePulse();
        }
        // Pocket Doze launch
        else if (mPickUpDoze &&
                ((mPickUpState && !mProximityNear) ||
                (!mPickUpState && mOrientationSensor.isFaceDown()))) {
            launchDozePulse();
        }
        // Pocket Doze launch
        else if (mPocketDoze && mOrientationSensor.isVertical()) {
            launchDozePulse();
        }
        // Proximity Wake launch
        else if (mProximityWake && !mOrientationSensor.isFaceDown()) {
            launchDeviceWake();
        }

        // Restore the pick-up sensor
        if (!mProximityNear && isPickUpEnabled()) {
            setPickUpSensor(true, false);
        }

        resetValues();
        return;
    }

    private void launchDozePulse() {
        long delta;
        if (mLastPulseTimestamp != 0) {
            delta = SystemClock.elapsedRealtime() - mLastPulseTimestamp;
        } else {
            delta = PULSE_MIN_INTERVAL_MS;
        }

        if (delta >= PULSE_MIN_INTERVAL_MS) {
            if (DEBUG) Log.d(TAG, "Doze launch. Time since last : " + delta);

            launchWakeLock();
            launchAcknowledge();
            mLastPulseTimestamp = SystemClock.elapsedRealtime();
            mContext.sendBroadcastAsUser(new Intent(DOZE_INTENT),
                    UserHandle.ALL);
        }
        else if (DEBUG) Log.d(TAG, "Doze avoided. Time since last : " + delta);
    }

    private void launchDeviceWake() {
        if (DEBUG) Log.d(TAG, "Waking device.");

        mSensorsWakeLock.acquire(SENSORS_WAKELOCK_DURATION);
        launchAcknowledge();
        mPowerManager.wakeUp(SystemClock.uptimeMillis());
    }

    private void launchWakeLock() {
        mSensorsWakeLock.acquire(SENSORS_WAKELOCK_DURATION);
    }

    private void launchAcknowledge() {
        AudioManager audioManager = (AudioManager) mContext.getSystemService(
                Context.AUDIO_SERVICE);
        Vibrator vibrator = (Vibrator) mContext.getSystemService(
                Context.VIBRATOR_SERVICE);

        switch (audioManager.getRingerMode()) {
            case AudioManager.RINGER_MODE_SILENT:
                break;
            case AudioManager.RINGER_MODE_VIBRATE:
            case AudioManager.RINGER_MODE_NORMAL:
            default:
                vibrator.vibrate(VIBRATOR_ACKNOWLEDGE);
                break;
        }
    }

    private void resetValues() {
        mHandwaveDoze = false;
        mPickUpDoze = false;
        mPocketDoze = false;
        mProximityWake = false;
    }

    private void setOrientationSensor(boolean enabled, boolean reset) {
        if (mOrientationSensor == null) return;

        if (reset) {
            mOrientationSensor.reset();
        }
        if (enabled) {
            setPickUpSensor(false, false);
            launchWakeLock();
            mOrientationSensor.enable();
        } else {
            mOrientationSensor.disable();
        }
    }

    private void setPickUpSensor(boolean enabled, boolean reset) {
        if (mPickUpSensor == null) return;

        if (reset) {
            mPickUpSensor.reset();
        }
        if (enabled) {
            setOrientationSensor(false, false);
            mPickUpSensor.enable();
        } else {
            mPickUpSensor.disable();
        }
    }

    private void setProximitySensor(boolean enabled, boolean reset) {
        if (mProximitySensor == null) return;

        if (reset) {
            mProximitySensor.reset();
        }
        if (enabled) {
            mProximitySensor.enable();
        } else {
            mProximitySensor.disable();
        }
    }

    private void onDisplayOn() {
        if (DEBUG) Log.d(TAG, "Display on");

        setOrientationSensor(false, true);
        setPickUpSensor(false, true);
        setProximitySensor(false, true);
    }

    private void onDisplayOff() {
        if (DEBUG) Log.d(TAG, "Display off");

        getDozeEnabled();
        mLastPulseTimestamp = 0;
        if (isHandwaveEnabled() || isPickUpEnabled() || isPocketEnabled() ||
                isProximityWakeEnabled()) {
            resetValues();
            setOrientationSensor(false, true);
            setPickUpSensor(false, true);
            setProximitySensor(true, true);
        }
    }

    private void loadPreferences(SharedPreferences sharedPreferences) {
        mHandwaveGestureEnabled = sharedPreferences.getBoolean(
                KEY_GESTURE_HAND_WAVE, false);
        mPickUpGestureEnabled = sharedPreferences.getBoolean(
                KEY_GESTURE_PICK_UP, false);
        mPocketGestureEnabled = sharedPreferences.getBoolean(
                KEY_GESTURE_POCKET, false);
        mProximityWakeEnabled = sharedPreferences.getBoolean(
                KEY_PROXIMITY_WAKE, false);
    }

    private BroadcastReceiver mScreenStateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(Intent.ACTION_SCREEN_OFF)) {
                onDisplayOff();
            } else if (intent.getAction().equals(Intent.ACTION_SCREEN_ON)) {
                onDisplayOn();
            }
        }
    };

    private SharedPreferences.OnSharedPreferenceChangeListener mPrefListener =
            new SharedPreferences.OnSharedPreferenceChangeListener() {
        @Override
        public void onSharedPreferenceChanged(SharedPreferences
                sharedPreferences, String key) {
            if (KEY_GESTURE_HAND_WAVE.equals(key)) {
                mHandwaveGestureEnabled = sharedPreferences.getBoolean(
                        KEY_GESTURE_HAND_WAVE, false);
            } else if (KEY_GESTURE_PICK_UP.equals(key)) {
                mPickUpGestureEnabled = sharedPreferences.getBoolean(
                        KEY_GESTURE_PICK_UP, false);
            } else if (KEY_GESTURE_POCKET.equals(key)) {
                mPocketGestureEnabled = sharedPreferences.getBoolean(
                        KEY_GESTURE_POCKET, false);
            } else if (KEY_PROXIMITY_WAKE.equals(key)) {
                mProximityWakeEnabled = sharedPreferences.getBoolean(
                        KEY_PROXIMITY_WAKE, false);
            }
        }
    };
}
