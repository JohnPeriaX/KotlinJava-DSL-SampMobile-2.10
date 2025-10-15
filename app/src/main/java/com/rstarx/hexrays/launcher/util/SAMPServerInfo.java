package com.rstarx.hexrays.launcher.util;

import android.os.Parcel;
import android.os.Parcelable;

public final class SAMPServerInfo implements Parcelable {
    public static final CREATOR CREATOR = new CREATOR();
    private String address;
    private int currentPlayerCount;
    private boolean favorite;
    private boolean hasPassword;
    private int id;
    private int maxPlayerCount;
    private int ping;
    private int port;
    private int projId;
    private boolean queryed;
    private String serverMode;
    private String serverName;
    private Status serverStatus;
    private Official type;
    private String language;

    public enum Official {
        OFFICIAL,
        HOSTED,
        CUSTOM
    }

    public enum Status {
        OFFLINE,
        ONLINE
    }

    public int describeContents() {
        return 0;
    }

    public static final class CREATOR implements Creator<SAMPServerInfo> {
        @Override
        public SAMPServerInfo createFromParcel(Parcel parcel) {
            return new SAMPServerInfo(parcel);
        }

        @Override
        public SAMPServerInfo[] newArray(int i) {
            return new SAMPServerInfo[i];
        }
    }

    public SAMPServerInfo(int i, int i2, String str, String str2, int i3, int i4, int i5, int i6, int i7, int i8, String language) {
        projId = i;
        id = i2;
        boolean z = false;
        queryed = false;
        address = str2;
        serverName = str;
        port = i3;
        currentPlayerCount = i4;
        maxPlayerCount = i5;
        hasPassword = i6 != 0;
        serverMode = "Android";
        this.language = language;
        serverStatus = i7 != 0 ? Status.ONLINE : Status.OFFLINE;
        ping = 1;
        favorite = i8 != 0 ? true : z;
        type = i == 0 ? Official.OFFICIAL : i == 1 ? Official.HOSTED : Official.CUSTOM;
    }

    public SAMPServerInfo(Parcel parcel) {
        this(parcel.readInt(), parcel.readInt(), parcel.readString(), parcel.readString(), parcel.readInt(), parcel.readInt(), parcel.readInt(), parcel.readInt(), parcel.readInt(), parcel.readInt(), parcel.readString());
    }

    public SAMPServerInfo() {
        this(0, 0, "", "0.0.0.0", 0, 0, 0, 0, 0, 0, "");
    }

    public final int getProjId() {
        return projId;
    }

    public final void setProjId(int i) {
        projId = i;
    }

    public final int getId() {
        return id;
    }

    public final void setId(int i) {
        id = i;
    }

    public final boolean getQueried() {
        return queryed;
    }

    public final void setQueried(boolean z) {
        queryed = z;
    }

    public final String getAddress() {
        return address;
    }

    public final void setAddress(String str) {
        address = str;
    }

    public final String getServerName() {
        return serverName;
    }

    public final void setServerName(String str) {
        serverName = str;
    }

    public final int getPort() {
        return port;
    }

    public final void setPort(int i) {
        port = i;
    }

    public final int getCurrentPlayerCount() {
        return currentPlayerCount;
    }

    public final void setCurrentPlayerCount(int i) {
        currentPlayerCount = i;
    }

    public final int getMaxPlayerCount() {
        return maxPlayerCount;
    }

    public final void setMaxPlayerCount(int i) {
        maxPlayerCount = i;
    }

    public final boolean getHasPassword() {
        return hasPassword;
    }

    public final void setHasPassword(boolean z) {
        hasPassword = z;
    }

    public final String getServerMode() {
        return serverMode;
    }

    public final void setServerMode(String str) {
        serverMode = str;
    }

    public final String getLanguage() {
        return language;
    }

    public final void setLanguage(String str) {
        language = str;
    }

    public final Status getServerStatus() {
        return serverStatus;
    }

    public final void setServerStatus(Status status) {
        serverStatus = status;
    }

    public final int getPing() {
        return ping;
    }

    public final void setPing(int i) {
        ping = i;
    }

    public final boolean getFavorite() {
        return favorite;
    }

    public final void setFavorite(boolean z) {
        favorite = z;
    }

    public final Official getType() {
        return type;
    }

    public final void setType(Official official) {
        type = official;
    }

    public void writeToParcel(Parcel parcel, int i) {
        parcel.writeString(String.valueOf(id));
        parcel.writeString(address);
        parcel.writeInt(port);
        parcel.writeInt(hasPassword ? 1 : 0);
        parcel.writeInt(currentPlayerCount);
        parcel.writeInt(maxPlayerCount);
        parcel.writeString(serverName);
        parcel.writeString(serverMode);
        parcel.writeInt(serverStatus.ordinal());
        parcel.writeInt(ping);
        parcel.writeInt(type.ordinal());
    }

    public final String json() {
        return "address : " + address + "\n port : " + port;
    }
}
