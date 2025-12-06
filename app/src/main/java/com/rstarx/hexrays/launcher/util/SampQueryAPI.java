package com.rstarx.hexrays.launcher.util;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Random;
import java.util.StringTokenizer;

/* renamed from: g5.b */
public final class SampQueryAPI {

    /* renamed from: a */
    public DatagramSocket f7277a = null;

    /* renamed from: b */
    public InetAddress f7278b = null;

    /* renamed from: c */
    public String f7279c = "";

    /* renamed from: d */
    public int f7280d;

    /* renamed from: e */
    public Random f7281e = new Random();

    /* renamed from: f */
    public boolean f7282f = true;

    public SampQueryAPI(String str, int i) {
        try {
            InetAddress byName = InetAddress.getByName(str);
            this.f7278b = byName;
            this.f7279c = byName.getHostAddress();
        } catch (UnknownHostException unused) {
            this.f7282f = false;
        }
        try {
            DatagramSocket datagramSocket = new DatagramSocket();
            this.f7277a = datagramSocket;
            datagramSocket.setSoTimeout(2000);
        } catch (SocketException unused2) {
            this.f7282f = false;
        }
        this.f7280d = i;
    }

    /* renamed from: a */
    public static String m5851a(int i, ByteBuffer byteBuffer) throws UnsupportedEncodingException {
        byte[] bArr = new byte[i];
        for (int i2 = 0; i2 < i; i2++) {
            try {
                bArr[i2] = byteBuffer.get();
            } catch (Exception unused) {
            }
        }
        return new String(bArr, "windows-874");
    }

    /* renamed from: b */
    public final String[] mo7164b() {
        byte[] bArr;
        DatagramPacket datagramPacket;
        DatagramSocket datagramSocket = this.f7277a;
        String[] strArr = new String[6];
        try {
            DatagramPacket c = mo7165c("i");
            if (datagramSocket != null) {
                try {
                    datagramSocket.send(c);
                } catch (IOException unused) {
                }
            }
            if (datagramSocket == null) {
                bArr = new byte[3072];
            } else {
                datagramPacket = new DatagramPacket(new byte[3072], 3072);
                try {
                    datagramSocket.receive(datagramPacket);
                } catch (IOException unused2) {
                }
                bArr = datagramPacket.getData();
            }
            ByteBuffer wrap = ByteBuffer.wrap(bArr);
            wrap.order(ByteOrder.LITTLE_ENDIAN);
            wrap.position(11);
            if (wrap.get() == 0) {
                strArr[0] = "0";
            } else {
                strArr[0] = "1";
            }
            strArr[1] = String.valueOf((int) wrap.getShort());
            strArr[2] = String.valueOf((int) wrap.getShort());
            strArr[3] = m5851a(wrap.getInt(), wrap);
            strArr[4] = m5851a(wrap.getInt(), wrap);
            strArr[5] = m5851a(wrap.getInt(), wrap);
            return strArr;
        } catch (Exception unused4) {
            return null;
        }
    }

    /* renamed from: c */
    public final DatagramPacket mo7165c(String str) {
        int i = this.f7280d;
        try {
            byte[] bytes = ("SAMPzalupa" + str).getBytes("windows-874");
            StringTokenizer stringTokenizer = new StringTokenizer(this.f7279c, ".");
            bytes[4] = (byte) Integer.parseInt(stringTokenizer.nextToken());
            bytes[5] = (byte) Integer.parseInt(stringTokenizer.nextToken());
            bytes[6] = (byte) Integer.parseInt(stringTokenizer.nextToken());
            bytes[7] = (byte) Integer.parseInt(stringTokenizer.nextToken());
            bytes[8] = (byte) (i & 255);
            bytes[9] = (byte) ((i >> 8) & 255);
            return new DatagramPacket(bytes, bytes.length, this.f7278b, i);
        } catch (Exception unused) {
            return null;
        }
    }

    /* renamed from: d */
    public boolean mo7166d() {
        DatagramSocket datagramSocket;
        DatagramPacket datagramPacket;
        byte[] data;
        if (!this.f7282f || (datagramSocket = this.f7277a) == null) {
            return false;
        }
        byte[] bArr = new byte[4];
        this.f7281e.nextBytes(bArr);
        bArr[0] = (byte) (((bArr[0] % 100) + 110) & 255);
        bArr[1] = (byte) (bArr[1] % Byte.MIN_VALUE);
        bArr[2] = (byte) (bArr[2] % Byte.MIN_VALUE);
        bArr[3] = (byte) ((bArr[3] % 50) & 255);
        byte[] bArr2 = null;
        try {
            DatagramPacket c = mo7165c("p".concat(new String(bArr, "windows-874")));
            if (datagramSocket != null) {
                try {
                    datagramSocket.send(c);
                } catch (IOException unused) {
                }
            }
            if (datagramSocket == null) {
                data = new byte[3072];
            } else {
                datagramPacket = new DatagramPacket(new byte[3072], 3072);
                try {
                    datagramSocket.receive(datagramPacket);
                } catch (IOException unused2) {
                }
                data = datagramPacket.getData();
            }
            bArr2 = data;
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        if (bArr2[10] == 112 && bArr2[11] == bArr[0] && bArr2[12] == bArr[1] && bArr2[13] == bArr[2] && bArr2[14] == bArr[3]) {
            return true;
        }
        return false;
    }
}