package com.rstarx.hexrays.launcher.util;

import android.content.Context;
import android.content.SharedPreferences;

public class SharedPreferenceCore {
    public static final String APP_PREFERENCES = "configlauncher";
    private static SharedPreferences.Editor editor;
    private static SharedPreferences mSharedPreference;

    public static void main(Context context) {
        SharedPreferences sharedPreferences = context.getSharedPreferences(APP_PREFERENCES, 0);
        mSharedPreference = sharedPreferences;
        editor = sharedPreferences.edit();
    }

    public void setString(Context context, String str, String str2) {
        main(context);
        editor.putString(str, str2);
        editor.apply();
    }

    public void setInt(Context context, String str, int i) {
        main(context);
        editor.putInt(str, i);
        editor.apply();
    }

    public void setBoolean(Context context, String str, boolean z) {
        main(context);
        editor.putBoolean(str, z);
        editor.apply();
    }

    public void setLong(Context context, String str, long j) {
        main(context);
        editor.putLong(str, j);
        editor.apply();
    }

    public void setFloat(Context context, String str, float f) {
        main(context);
        editor.putFloat(str, f);
        editor.apply();
    }

    public String getString(Context context, String str) {
        main(context);
        if (mSharedPreference.contains(str)) {
            return mSharedPreference.getString(str, "");
        }
        return null;
    }

    public int getInt(Context context, String str) {
        main(context);
        if (mSharedPreference.contains(str)) {
            return mSharedPreference.getInt(str, 0);
        }
        return 0;
    }

    public int getInt(Context context, String str, int defaultValue) {
        main(context);
        if (mSharedPreference.contains(str)) {
            return mSharedPreference.getInt(str, defaultValue);
        }
        return 0;
    }

    public boolean getBoolean(Context context, String str) {
        main(context);
        if (mSharedPreference.contains(str)) {
            return mSharedPreference.getBoolean(str, false);
        }
        return false;
    }

    public long getLong(Context context, String str) {
        main(context);
        if (mSharedPreference.contains(str)) {
            return mSharedPreference.getLong(str, 0);
        }
        return 0;
    }

    public float getFloat(Context context, String str) {
        main(context);
        if (mSharedPreference.contains(str)) {
            return mSharedPreference.getFloat(str, 0.0f);
        }
        return 0.0f;
    }
}
