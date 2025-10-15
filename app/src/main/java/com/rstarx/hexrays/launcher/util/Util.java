package com.rstarx.hexrays.launcher.util;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.app.Activity;
import android.content.Context;
import android.graphics.Point;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.text.Html;
import android.text.Layout;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.view.Display;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.RecyclerView;

import com.rstarx.hexrays.R;
import com.rstarx.hexrays.launcher.MainActivity;
import com.rstarx.hexrays.launcher.config.Config;
import com.rstarx.hexrays.launcher.fragments.ServerPagesItemFragment;
import com.rstarx.hexrays.launcher.fragments.ServersFragment;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.DatagramSocket;
import java.nio.charset.StandardCharsets;
import java.util.Iterator;

public class Util {

    public static String responseFiles = "";
    public static int responseFilesInt = 0;

    public static String InputStreamToString(InputStream inputStream) {
        StringBuilder sb = new StringBuilder();
        try {
            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream, StandardCharsets.UTF_8), 8192);
            while (true) {
                String readLine = bufferedReader.readLine();
                if (readLine == null) {
                    break;
                }
                sb.append(readLine);
                sb.append('\n');
            }
            bufferedReader.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return sb.toString();
    }


    public static void delete(File file) {
        if (file.exists()) {
            if (file.isDirectory()) {
                for (File f : file.listFiles()) {
                    delete(f);
                }
            }
            file.delete();
        }
    }

    public static void ShowLayout(View view, boolean isAnim) {
        if (view != null) {
            view.setAlpha(1.0f);
            view.setVisibility(View.VISIBLE);
        }
    }

    public static void HideLayout(View view, boolean isAnim) {
        if (view != null) {
            view.setAlpha(0.0f);
            view.setVisibility(View.GONE);
        }
    }

    private static void fadeIn(View view) {
        if (view != null) {
            view.animate().setDuration(500).setListener(new AnimatorListenerAdapter() {
                public void onAnimationEnd(Animator animation) {
                    super.onAnimationEnd(animation);
                }
            }).alpha(1.0f);
        }
    }

    private static void fadeOut(final View view) {
        if (view != null) {
            view.animate().setDuration(500).setListener(new AnimatorListenerAdapter() {
                /* class com.brilliant.cr.custom.util.Utils.AnonymousClass2 */

                public void onAnimationEnd(Animator animation) {
                    view.setVisibility(View.GONE);
                    super.onAnimationEnd(animation);
                }
            }).alpha(0.0f);
        }
    }

    public static CharSequence getColoredString(String str) {
        return Html.fromHtml(getStringWithColors(str));
    }

    public static String getStringWithColors(String str) {
        int i;
        int count = 0;
        StringBuilder sb = new StringBuilder();
        int i2 = 0;
        boolean z = false;
        while (i2 < str.length()) {
            if (str.charAt(i2) == '{' && (i = i2 + 7) < str.length() && str.charAt(i) == '}') {
                if (z) {
                    sb.append("</font>");
                }
                sb.append("<font color=#");
                while (true) {
                    i2++;
                    if (i2 >= i) {
                        break;
                    }
                    sb.append(str.charAt(i2));
                }
                sb.append('>');
                i2 = i;
                z = true;
            } else {
                sb.append(str.charAt(i2));
            }
            i2++;
            count++;
        }
        if (z) {
            sb.append("</font>");
        }
        if (count > 0) { return sb.toString().replace("\n", "<br/>"); }
        else return str;
    }

    private static final float MULT_X = 5.2083336E-4f;
    private static final float MULT_Y = 9.259259E-4f;

    public static void scaleViewAndChildren(Activity activity, View view) {
        Display defaultDisplay = activity.getWindowManager().getDefaultDisplay();
        Point point = new Point();
        defaultDisplay.getSize(point);
        float min = Math.min(((float) point.x) * MULT_X, ((float) point.y) * MULT_Y);
        ViewGroup.LayoutParams layoutParams = view.getLayoutParams();
        if (!(layoutParams.width == -1 || layoutParams.width == -2 || ((int) (((float) layoutParams.width) * min)) == 0)) {
            layoutParams.width = (int) (((float) layoutParams.width) * min);
        }
        if (!(layoutParams.height == -1 || layoutParams.height == -2 || ((int) (((float) layoutParams.height) * min)) == 0)) {
            layoutParams.height = (int) (((float) layoutParams.height) * min);
        }
        if (layoutParams instanceof ViewGroup.MarginLayoutParams) {
            ViewGroup.MarginLayoutParams marginLayoutParams = (ViewGroup.MarginLayoutParams) layoutParams;
            marginLayoutParams.leftMargin = (int) (((float) marginLayoutParams.leftMargin) * min);
            marginLayoutParams.rightMargin = (int) (((float) marginLayoutParams.rightMargin) * min);
            marginLayoutParams.topMargin = (int) (((float) marginLayoutParams.topMargin) * min);
            marginLayoutParams.bottomMargin = (int) (((float) marginLayoutParams.bottomMargin) * min);
        }
        if (layoutParams instanceof ConstraintLayout.LayoutParams) {
            ConstraintLayout.LayoutParams layoutParams2 = (ConstraintLayout.LayoutParams) layoutParams;
            layoutParams2.matchConstraintMinHeight = (int) (((float) layoutParams2.matchConstraintMinHeight) * min);
            layoutParams2.matchConstraintMinWidth = (int) (((float) layoutParams2.matchConstraintMinWidth) * min);
            layoutParams2.matchConstraintMaxHeight = (int) (((float) layoutParams2.matchConstraintMaxHeight) * min);
            layoutParams2.matchConstraintMaxWidth = (int) (((float) layoutParams2.matchConstraintMaxWidth) * min);
        }
        view.setLayoutParams(layoutParams);
        view.setPadding((int) (((float) view.getPaddingLeft()) * min), (int) (((float) view.getPaddingTop()) * min), (int) (((float) view.getPaddingRight()) * min), (int) (((float) view.getPaddingBottom()) * min));
        view.setMinimumHeight((int) (((float) view.getMinimumHeight()) * min));
        view.setMinimumWidth((int) (((float) view.getMinimumWidth()) * min));
        if (view instanceof TextView) {
            TextView textView = (TextView) view;
            textView.setTextSize(0, textView.getTextSize() * min);
        }

        if (view instanceof CustomRecyclerView) {
            CustomRecyclerView customRecyclerView = (CustomRecyclerView) view;
            customRecyclerView.setScrollBarSize((int) (((float) customRecyclerView.getScrollBarSize()) * min));
        }

        if (view instanceof ViewGroup) {
            ViewGroup viewGroup = (ViewGroup) view;
            for (int i = 0; i < viewGroup.getChildCount(); i++) {
                scaleViewAndChildren(activity, viewGroup.getChildAt(i));
            }
        }
    }

    public static float scale(Activity activity, float f) {
        Display defaultDisplay = activity.getWindowManager().getDefaultDisplay();
        Point point = new Point();
        defaultDisplay.getSize(point);
        return f * Math.min(((float) point.x) * MULT_X, ((float) point.y) * MULT_Y);
    }

    public static int getTextWidth(String str, TextPaint textPaint) {
        return (int) new StaticLayout(str, textPaint, Integer.MAX_VALUE, Layout.Alignment.ALIGN_NORMAL, 1.0f, 0.0f, false).getLineWidth(0);
    }

    public static String getStringWithoutColors(String str) {
        int i;
        StringBuilder sb = new StringBuilder();
        int i2 = 0;
        while (i2 < str.length()) {
            if (str.charAt(i2) == '{' && (i = i2 + 7) < str.length() && str.charAt(i) == '}') {
                i2 = i;
            } else {
                sb.append(str.charAt(i2));
            }
            i2++;
        }
        return sb.toString();
    }

    public static String convertToReadableValue(int i) {
        StringBuilder sb = new StringBuilder(String.valueOf(i));
        int i2 = 0;
        for (int length = sb.length() - 1; length >= 0; length--) {
            i2++;
            if (!(i2 != 3 || length == 0 || sb.charAt(length - 1) == '-')) {
                sb.insert(length, ' ');
                i2 = 0;
            }
        }
        return sb.toString();
    }

    public static boolean isNetworkConnected(Context context)
    {
        ConnectivityManager cm = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo activeNetwork = cm.getActiveNetworkInfo();
        return activeNetwork != null && activeNetwork.isConnectedOrConnecting();
    }
}
