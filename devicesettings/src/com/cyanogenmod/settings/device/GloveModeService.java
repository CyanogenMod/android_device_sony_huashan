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
import android.os.IBinder;
import android.util.Log;

import cyanogenmod.providers.CMSettings;

import java.lang.System;
import java.io.FileOutputStream;
import java.io.IOException;

public class GloveModeService extends Service {

    public static final boolean DEBUG = false;
    public static final String TAG = "GloveModeService";

    private static final String GLOVE_PATH = "/sys/devices/i2c-3/3-0024/main_ttsp_core.cyttsp4_i2c_adapter/signal_disparity";
    private static final long GLOVE_UPDATE_DELTA = 900000; // 15 minutes

    private Context mContext;
    private long mLastGloveModeUpdate = 0;
    private boolean mGloveModeEnabled = false;

    public void onCreate() {
        if (DEBUG) Log.d(TAG, "Creating service");

        super.onCreate();
        mContext = this;
    }

    public int onStartCommand(Intent intent, int flags, int startId) {
        if (DEBUG) Log.d(TAG, "Starting service");

        IntentFilter intentScreen = new IntentFilter(Intent.ACTION_SCREEN_ON);
        intentScreen.addAction(Intent.ACTION_SCREEN_OFF);
        mContext.registerReceiver(mScreenStateReceiver, intentScreen);

        mGloveModeEnabled = isGloveModeEnabled();
        if (DEBUG) Log.d(TAG, "Glove Mode state: " + mGloveModeEnabled);

        updateGloveMode();

        return START_STICKY;
    }

    public void onDestroy() {
        if (DEBUG) Log.d(TAG, "Destroying service");

        super.onDestroy();
    }

    public IBinder onBind(Intent intent) {
        return null;
    }

    private void updateGloveMode() {
        if (DEBUG) Log.d(TAG, "Updating Glove Mode: " + mGloveModeEnabled);

        if (!mGloveModeEnabled) {
            return;
        }

        long currentTimeMs = System.currentTimeMillis();
        if (currentTimeMs - mLastGloveModeUpdate > GLOVE_UPDATE_DELTA) {
            mLastGloveModeUpdate = currentTimeMs;
            try {
                FileOutputStream fos = new FileOutputStream(GLOVE_PATH);
                fos.write(String.valueOf(0).getBytes());
                fos.flush();
                fos.close();
            } catch (IOException e) {
                Log.e(TAG, "Could not write to file " + GLOVE_PATH, e);
            }
        }
    }

    private BroadcastReceiver mScreenStateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(Intent.ACTION_SCREEN_OFF)) {
                mGloveModeEnabled = isGloveModeEnabled();
               if (DEBUG) Log.d(TAG, "Glove Mode state: " + mGloveModeEnabled);
            } else if (intent.getAction().equals(Intent.ACTION_SCREEN_ON)) {
                updateGloveMode();
            }
        }
    };

    private boolean isGloveModeEnabled() {
        return (CMSettings.System.getInt(mContext.getContentResolver(),
                CMSettings.System.HIGH_TOUCH_SENSITIVITY_ENABLE, 0) == 1);
    }
}
