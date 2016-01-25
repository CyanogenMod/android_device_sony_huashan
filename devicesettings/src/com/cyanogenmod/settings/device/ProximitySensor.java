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

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class ProximitySensor implements SensorEventListener {

    private static final int PROXIMITY_DELAY = 1000 * 1000;
    private static final int PROXIMITY_LATENCY = 100 * 1000;

    private boolean mEnabled;
    private boolean mReady;
    private boolean mState;
    private float mMaxRange;
    private ProximityListener mProximityListener;
    private Sensor mProximitySensor;
    private SensorManager mSensorManager;

    public static interface ProximityListener {
        public abstract void onEvent(boolean isNear, long timestamp);
        public abstract void onInit(boolean isNear, long timestamp);
    }

    public ProximitySensor(Context context, SensorManager sensorManager,
            ProximityListener proximitylistener) {
        mEnabled = false;
        reset();
        mProximitySensor = sensorManager.getDefaultSensor(
                Sensor.TYPE_PROXIMITY, true);
        mProximityListener = proximitylistener;
        mMaxRange = mProximitySensor.getMaximumRange();
        mSensorManager = sensorManager;
    }

    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

    public void onSensorChanged(SensorEvent event) {
        if (event.values.length == 0) return;
        boolean isNear = (event.values[0] < mMaxRange);

        // Launch an event
        if (mState != isNear) {
            mState = isNear;
            if (mReady) {
                mProximityListener.onEvent(mState, event.timestamp);
            }
        }

        // Init the sensor
        if (!mReady) {
            mProximityListener.onInit(mState, event.timestamp);
            mReady = true;
        }
    }

    public void enable() {
        if (!mEnabled) {
            mSensorManager.registerListener(this, mProximitySensor,
                    PROXIMITY_DELAY, PROXIMITY_LATENCY);
            mEnabled = true;
        }
    }

    public void reset() {
        mReady = false;
        mState = false;
    }

    public void disable() {
        if (mEnabled) {
            mSensorManager.unregisterListener(this, mProximitySensor);
            mEnabled = false;
        }
    }
}
