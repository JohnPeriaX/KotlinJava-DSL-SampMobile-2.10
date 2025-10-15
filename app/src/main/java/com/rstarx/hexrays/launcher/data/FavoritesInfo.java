package com.rstarx.hexrays.launcher.data;

import android.content.Context;
import android.os.Environment;

import com.rstarx.hexrays.launcher.util.Util;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Iterator;
import org.json.JSONArray;
import org.json.JSONObject;

/* renamed from: ru.unisamp_mobile.launcher.FavoritesInfo */
public class FavoritesInfo {
    private static boolean bLoaded = false;
    private static ArrayList<FavoriteServerData> serverList = new ArrayList<>();

    public static void Load(Context mContext) {
        File file = new File(mContext.getExternalFilesDir(null), "SAMP/favorites.json");
        if (!file.exists()) {
            ClearFavorites();
            return;
        }
        try {
            String InputStreamToString = Util.InputStreamToString(new FileInputStream(file));
            if (InputStreamToString.isEmpty()) {
                ClearFavorites();
                return;
            }
            JSONArray jSONArray = new JSONObject(InputStreamToString).getJSONArray("servers");
            for (int i = 0; i < jSONArray.length(); i++) {
                JSONObject jSONObject = jSONArray.getJSONObject(i);
                int i2 = jSONObject.getInt("id");
                int i3 = jSONObject.getInt("serverid");
                String str = "";
                int i4 = 7777;
                if (!jSONObject.isNull("ip")) {
                    str = jSONObject.getString("ip");
                }
                if (!jSONObject.isNull("port")) {
                    i4 = jSONObject.getInt("port");
                }
                serverList.add(new FavoriteServerData(i2, i3, str, i4));
            }
            bLoaded = true;
        } catch (Exception e) {
            e.printStackTrace();
            ClearFavorites();
            Save(mContext);
        }
    }

    public static void Save(Context context) {
        try {
            File file = new File(context.getExternalFilesDir(null), "SAMP/favorites.json");
            if (file.exists()) {
                file.delete();
            }
            JSONObject jSONObject = new JSONObject();
            jSONObject.put("servers", new JSONArray());
            JSONArray jSONArray = jSONObject.getJSONArray("servers");
            for (int i = 0; i < serverList.size(); i++) {
                FavoriteServerData favoriteServerData = serverList.get(i);
                jSONArray.put(i, new JSONObject());
                JSONObject jSONObject2 = jSONArray.getJSONObject(i);
                jSONObject2.put("id", favoriteServerData.id);
                jSONObject2.put("serverid", favoriteServerData.serverid);
                jSONObject2.put("ip", favoriteServerData.ip);
                jSONObject2.put("port", favoriteServerData.port);
            }
            new File(context.getExternalFilesDir(null), "SAMP/").mkdirs();
            file.createNewFile();
            BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(file));
            bufferedWriter.write(jSONObject.toString());
            bufferedWriter.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static boolean AddServer(Context context, int i, int i2, String str, int i3) {
        if (!bLoaded) {
            Load(context);
        }
        Iterator<FavoriteServerData> it = serverList.iterator();
        while (it.hasNext()) {
            FavoriteServerData next = it.next();
            if (next.ip.equals(str) && next.port == i3) {
                return false;
            }
        }
        serverList.add(new FavoriteServerData(i, i2, str, i3));
        return true;
    }

    public static boolean RemoveServer(Context context, String str, int i) {
        if (!bLoaded) {
            Load(context);
        }
        Iterator<FavoriteServerData> it = serverList.iterator();
        while (it.hasNext()) {
            FavoriteServerData next = it.next();
            if (next.ip.equals(str) && next.port == i) {
                serverList.remove(next);
                return true;
            }
        }
        return false;
    }

    public static boolean IsServerExists(Context context, int i, int i2, String str, int i3, boolean z) {
        if (!bLoaded) {
            Load(context);
        }
        Iterator<FavoriteServerData> it = serverList.iterator();
        while (it.hasNext()) {
            FavoriteServerData next = it.next();
            if (next.id == i && next.serverid == i2 && i != 0 && i2 != 0 && next.ip.equals("")) {
                if (z) {
                    next.queried = true;
                }
                next.ip = str;
                next.port = i3;
                Save(context);
                return true;
            } else if (next.ip.equals(str) && next.port == i3) {
                if (z) {
                    next.queried = true;
                }
                return true;
            }
        }
        return false;
    }

    public static FavoriteServerData GetServerByIpAndPort(Context context, String str, int i) {
        if (!bLoaded) {
            Load(context);
        }
        Iterator<FavoriteServerData> it = serverList.iterator();
        while (it.hasNext()) {
            FavoriteServerData next = it.next();
            if (next.ip.equals(str) && next.port == i) {
                return next;
            }
        }
        return null;
    }

    public static ArrayList<FavoriteServerData> getServerList(Context context) {
        if (!bLoaded) {
            Load(context);
        }
        return new ArrayList<>(serverList);
    }

    static void ClearFavorites() {
        serverList.clear();
    }
}