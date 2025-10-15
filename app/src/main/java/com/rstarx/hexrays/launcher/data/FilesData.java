package com.rstarx.hexrays.launcher.data;

public class FilesData {

    private String name;
    private long size;
    private String path;
    private String url;

    public FilesData(String name, long size, String path, String url) {
        this.name = name;
        this.size = size;
        this.path = path;
        this.url = url;
    }

    public String getName()
    {
        return name;
    }

    public long getSize()
    {
        return size;
    }

    public  String getPath()
    {
        return path;
    }

    public  String getUrl()
    {
        return url;
    }

}
