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

public class OrientationSensor implements SensorEventListener {

    public static final int ORIENTATION_UNKNOWN = 0;
    public static final int ORIENTATION_FACE_DOWN = 1;
    public static final int ORIENTATION_FACE_UP = 2;
    public static final int ORIENTATION_VERTICAL = 3;

    private static final int ORIENTATION_DELAY = 60 * 1000;
    private static final int ORIENTATION_LATENCY = 0;
    private static final float MATH_PI_1_4 = 0.785398F;
    private static final float MATH_PI_3_4 = 2.35619F;

    private boolean mEnabled;
    private boolean mReady;
    private int mState;
    private float mGravity[];
    private float mMagnetic[];
    private OrientationListener mOrientationListener;
    private Sensor mAccelerometerSensor;
    private Sensor mMagneticFieldSensor;
    private SensorManager mSensorManager;

    public static interface OrientationListener {
        public abstract void onEvent();
    }

    public boolean isFaceDown() {
        return mReady && mState == ORIENTATION_FACE_DOWN;
    }

    public boolean isFaceUp() {
        return mReady && mState == ORIENTATION_FACE_UP;
    }

    public boolean isVertical() {
        return mReady && mState == ORIENTATION_VERTICAL;
    }

    public OrientationSensor(Context context, SensorManager sensorManager,
            OrientationListener orientationListener) {
        mEnabled = false;
        reset();
        mAccelerometerSensor = sensorManager.getDefaultSensor(
                Sensor.TYPE_ACCELEROMETER, false);
        mMagneticFieldSensor = sensorManager.getDefaultSensor(
                Sensor.TYPE_MAGNETIC_FIELD, false);

        mOrientationListener = orientationListener;
        mSensorManager = sensorManager;
    }

    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

    public void onSensorChanged(SensorEvent event) {
        if (event.values.length == 0) return;

        switch (event.sensor.getType()) {
            case Sensor.TYPE_ACCELEROMETER:
                mGravity = event.values;
                break;
            case Sensor.TYPE_MAGNETIC_FIELD:
                mMagnetic = event.values;
                break;
            default:
                break;
        }

        if (mGravity != null && mMagnetic != null) {
            float[] rotationMatrix = new float[9];
            if (SensorManager.getRotationMatrix(rotationMatrix, new float[9],
                    mGravity, mMagnetic))
            {
                float[] values = new float[3];
                mState = ORIENTATION_UNKNOWN;
                SensorManager.getOrientation(rotationMatrix, values);

                // Orientation is flat
                if (values[1] > -MATH_PI_1_4 && values[1] < MATH_PI_1_4) {
                    // Device is face up
                    if (values[2] > -MATH_PI_1_4 && values[2] < MATH_PI_1_4) {
                        mState = ORIENTATION_FACE_UP;
                    }
                    // Device is face down
                    else if (values[2] < -MATH_PI_3_4 || values[2] > MATH_PI_3_4) {
                        mState = ORIENTATION_FACE_DOWN;
                    }
                }
                // Orientation is vertical
                if (values[1] < -MATH_PI_1_4 || values[1] > MATH_PI_1_4 ||
                        (values[2] > MATH_PI_1_4 && values[2] < MATH_PI_3_4) ||
                        (values[2] > -MATH_PI_3_4 && values[2] < -MATH_PI_1_4)) {
                    mState = ORIENTATION_VERTICAL;
                }

                //  Launch an event
                mReady = true;
                mOrientationListener.onEvent();
            }
        }
    }

    public void enable() {
        if (!mEnabled && mAccelerometerSensor != null && mMagneticFieldSensor != null) {
            reset();
            mState = ORIENTATION_UNKNOWN;
            mSensorManager.registerListener(this, mAccelerometerSensor,
                    ORIENTATION_DELAY, ORIENTATION_LATENCY);
            mSensorManager.registerListener(this, mMagneticFieldSensor,
                    ORIENTATION_DELAY, ORIENTATION_LATENCY);
            mEnabled = true;
        }
    }

    public void reset() {
        mGravity = null;
        mMagnetic = null;
        mReady = false;
    }

    public void disable() {
        if (mEnabled && mAccelerometerSensor != null && mMagneticFieldSensor != null) {
            mSensorManager.unregisterListener(this,mAccelerometerSensor);
            mSensorManager.unregisterListener(this, mMagneticFieldSensor);
            mEnabled = false;
        }
    }
}
