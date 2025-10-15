package com.rstarx.hexrays.launcher.util;

import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Build;
import android.widget.RemoteViews;

import androidx.core.app.NotificationCompat;

import com.bumptech.glide.Glide;
import com.rstarx.hexrays.R;
import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

import java.util.concurrent.ExecutionException;

import com.rstarx.hexrays.launcher.SplashActivity;

public class FirebaseMessageReceiver extends FirebaseMessagingService {
    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {
        if(remoteMessage.getData().size()>0){
            showNotification(remoteMessage.getData().get("title"),remoteMessage.getData().get("message"), remoteMessage.getData().get("image"));
        }

        if(remoteMessage.getNotification()!=null){
            if (remoteMessage.getNotification().getImageUrl() != null) {
                showNotification(remoteMessage.getNotification().getTitle(), remoteMessage.getNotification().getBody(), remoteMessage.getNotification().getImageUrl().toString());
            }
            else
            {
                showNotificationNoImage(remoteMessage.getNotification().getTitle(), remoteMessage.getNotification().getBody());
            }
        }

    }

    private RemoteViews getCustomDesign(String title,String message, String urlString){
        RemoteViews remoteViews=new RemoteViews(getApplicationContext().getPackageName(),
                R.layout.notification);
        remoteViews.setTextViewText(R.id.title,title);
        remoteViews.setTextViewText(R.id.message,message);
        //remoteViews.setImageViewResource();
        remoteViews.setImageViewBitmap(R.id.icon, getBitmapFromURL(urlString));
        return remoteViews;
    }

    public void showNotification(String title,String message, String urlString){
        Intent intent=new Intent(this, SplashActivity.class);
        String channel_id="web_app_channel";
        intent.putExtra("notif_message", message );
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        PendingIntent pendingIntent=PendingIntent.getActivity(this,0,intent,PendingIntent.FLAG_ONE_SHOT);

        Uri uri= RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
        NotificationCompat.Builder builder=new NotificationCompat.Builder(getApplicationContext(),channel_id)
                .setSmallIcon(R.mipmap.ic_launcher)
                .setSound(uri)
                .setAutoCancel(true)
                .setVibrate(new long[]{1000,1000,1000,1000,1000})
                .setOnlyAlertOnce(true)
                .setContentIntent(pendingIntent);
        if(Build.VERSION.SDK_INT>=Build.VERSION_CODES.JELLY_BEAN){
            builder = builder.setContent(getCustomDesign(title,message, urlString));
        } else {
            builder = builder.setContentTitle(title)
                    .setContentText(message)
                    .setContentIntent(pendingIntent)
                    .setSmallIcon(R.mipmap.ic_launcher);
        }
        NotificationManager notificationManager= (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
        if(Build.VERSION.SDK_INT>=Build.VERSION_CODES.O){
            NotificationChannel notificationChannel=new NotificationChannel(channel_id,"web_app",NotificationManager.IMPORTANCE_HIGH);
            notificationChannel.setSound(uri,null);
            notificationManager.createNotificationChannel(notificationChannel);
        }
        notificationManager.notify(0,builder.build());
    }

    public void showNotificationNoImage(String title,String message){
        Intent intent=new Intent(this, SplashActivity.class);
        String channel_id="web_app_channel";
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        PendingIntent pendingIntent=PendingIntent.getActivity(this,0,intent,PendingIntent.FLAG_ONE_SHOT);
        Uri uri= RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
        NotificationCompat.Builder builder=new NotificationCompat.Builder(getApplicationContext(),channel_id)
                .setSmallIcon(R.mipmap.ic_launcher)
                .setSound(uri)
                .setAutoCancel(true)
                .setVibrate(new long[]{1000,1000,1000,1000,1000})
                .setOnlyAlertOnce(true)
                .setContentIntent(pendingIntent);
            builder = builder.setContentTitle(title)
                    .setContentText(message)
                    .setSmallIcon(R.mipmap.ic_launcher);
        NotificationManager notificationManager= (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
        if(Build.VERSION.SDK_INT>=Build.VERSION_CODES.O){
            NotificationChannel notificationChannel=new NotificationChannel(channel_id,"web_app",NotificationManager.IMPORTANCE_HIGH);
            notificationChannel.setSound(uri,null);
            notificationManager.createNotificationChannel(notificationChannel);
        }
        notificationManager.notify(0,builder.build());
    }

    public Bitmap getBitmapFromURL(String str) {
        try {
            return Glide.with(this).asBitmap().load(str).submit().get();
        }  catch (InterruptedException e) {
            e.printStackTrace();
            return null;
        } catch (ExecutionException e) {
            e.printStackTrace();
            return null;
        }
    }
}
