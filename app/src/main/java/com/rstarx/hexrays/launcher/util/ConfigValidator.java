package com.rstarx.hexrays.launcher.util;

import android.content.Context;
import android.content.res.AssetManager;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class ConfigValidator {
    public static void validateConfigFiles(Context context) {
        File externalFilesDir = context.getExternalFilesDir(null);
        File file = new File(externalFilesDir, "SAMP/settings.ini");
        if (!file.exists() || file.length() == 0) {
            file.getParentFile().mkdirs();
            boolean copied = copyAsset(context.getAssets(), "settings.ini", file.toString());
            if (!copied) {
                try {
                    FileOutputStream fos = new FileOutputStream(file);
                    StringBuilder sb = new StringBuilder();
                    sb.append("[client]\n");
                    sb.append("name=AndroidUser\n");
                    sb.append("host=127.0.0.1\n");
                    sb.append("port=7777\n");
                    sb.append("password=\n");
                    sb.append("version=0.3.7\n\n");
                    sb.append("[gui]\n");
                    sb.append("androidkeyboard=false\n");
                    sb.append("VoiceChatEnable=true\n");
                    sb.append("fps=0\n");
                    sb.append("ChatMaxMessages=6\n");
                    sb.append("FPSLimit=120\n");
                    fos.write(sb.toString().getBytes());
                    fos.flush();
                    fos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        /*File file2 = new File(externalFilesDir, "gta_sa.set");
        if (!file2.exists()) {
            file2.getParentFile().mkdirs();
            copyAsset(context.getAssets(), "gta_sa.set", file2.toString());
        }*/
    }

    static boolean copyAsset(AssetManager assetManager, String str, String str2) {
        try {
            InputStream open = assetManager.open(str);
            new File(str2).createNewFile();
            FileOutputStream fileOutputStream = new FileOutputStream(str2);
            copyFile(open, fileOutputStream);
            open.close();
            fileOutputStream.flush();
            fileOutputStream.close();
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    static void copyFile(InputStream inputStream, OutputStream outputStream) throws IOException {
        byte[] bArr = new byte[1024];
        while (true) {
            int read = inputStream.read(bArr);
            if (read != -1) {
                outputStream.write(bArr, 0, read);
            } else {
                return;
            }
        }
    }
}
