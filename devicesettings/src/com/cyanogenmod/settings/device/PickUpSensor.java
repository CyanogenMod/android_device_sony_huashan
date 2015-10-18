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

public class PickUpSensor implements SensorEventListener {

    public static final int PICK_UP_UNKNOWN = 0;
    public static final int PICK_UP_FALSE = 1;
    public static final int PICK_UP_TRUE = 2;

    private static final int PICKUP_DELAY = 500 * 1000;
    private static final int PICKUP_LATENCY = 100 * 1000;
    private static final float PICK_UP_SAFEZONE = 5;
    private static final float PICK_UP_THRESHOLD = 6;

    private boolean mEnabled;
    private boolean mReady;
    private int mState;
    private Sensor mPickUpSensor;
    private PickUpListener mPickUpListener;
    private SensorManager mSensorManager;

    public static interface PickUpListener {
        public abstract void onEvent();
        public abstract void onInit();
    }

    public boolean isPickedUp() {
        return mReady && mState == PICK_UP_TRUE;
    }

    public PickUpSensor(Context context, SensorManager sensorManager,
            PickUpListener pickUpListener) {
        mEnabled = false;
        reset();
        mPickUpSensor = sensorManager.getDefaultSensor(
                Sensor.TYPE_ACCELEROMETER, false);

        mPickUpListener = pickUpListener;
        mSensorManager = sensorManager;
    }

    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

    public void onSensorChanged(SensorEvent event) {
        if (event.values.length == 0) return;
        float x = event.values[0];
        float y = event.values[1];
        float z = event.values[2];

        // Device is picked-up
        if (isPickUpAbove(x, y, PICK_UP_SAFEZONE)) {
            if (isPickUpAbove(x, y, PICK_UP_THRESHOLD)) {
                if (mState != PICK_UP_TRUE) {
                    mState = PICK_UP_TRUE;
                    if (mReady) {
                        mPickUpListener.onEvent();
                    }
                }
            }
        }
        // Device is layed down
        else if (mState != PICK_UP_FALSE) {
            mState = mState = PICK_UP_FALSE;
            if (mReady) {
                mPickUpListener.onEvent();
            }
        }

        // Init the sensor
        if (!mReady) {
            mReady = true;
            mPickUpListener.onInit();
        }
    }

    public boolean isPickUpAbove(float x, float y, float threshold) {
        return (x < -threshold || x > threshold || y > threshold);
    }

    public void enable() {
        if (!mEnabled && mPickUpSensor != null) {
            reset();
            mSensorManager.registerListener(this, mPickUpSensor,
                    PICKUP_DELAY, PICKUP_LATENCY);
            mEnabled = true;
        }
    }

    public void reset() {
        mReady = false;
        mState = PICK_UP_UNKNOWN;
    }

    public void disable() {
        if (mEnabled && mPickUpSensor != null) {
            mSensorManager.unregisterListener(this, mPickUpSensor);
            mEnabled = false;
        }
    }
}
