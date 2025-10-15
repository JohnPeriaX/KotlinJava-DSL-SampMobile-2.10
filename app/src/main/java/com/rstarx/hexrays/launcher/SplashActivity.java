package com.rstarx.hexrays.launcher;

import android.Manifest;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.downloader.Progress;
import com.google.firebase.crashlytics.FirebaseCrashlytics;
import com.joom.paranoid.Obfuscate;
import com.rstarx.hexrays.R;
import com.rstarx.hexrays.launcher.config.Config;
import com.rstarx.hexrays.launcher.util.SharedPreferenceCore;
import com.rstarx.hexrays.launcher.util.SignatureChecker;
import com.rstarx.hexrays.launcher.util.Util;

import java.io.File;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
@Obfuscate
public class SplashActivity extends AppCompatActivity {

    private final String[] permissions = {"android.permission.READ_EXTERNAL_STORAGE", "android.permission.WRITE_EXTERNAL_STORAGE", "android.permission.RECORD_AUDIO"};

    public int mGpuType;

    SharedPreferences prefs = null;

    AlertDialog.Builder builder;

    private final ServiceConnection mConnection = new ServiceConnection() {
        public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
            mService = new Messenger(iBinder);
            checkUpdate();
        }

        public void onServiceDisconnected(ComponentName componentName) {
            mService = null;
        }
    };


    public IncomingHandler mInHandler;
    public Messenger mMessenger;
    public Messenger mService;

    private boolean mIsBind;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_splash);

        if(!SignatureChecker.isSignatureValid(this, getPackageName()))
        {
            ((TextView)findViewById(R.id.launcher_orig_text)).setText("Using not original launcher");
        }
        else {
            Config.currentContext = this;

            prefs = getSharedPreferences("com.rstarx.hexrays", MODE_PRIVATE);

            builder = new AlertDialog.Builder(this);

            if (!Util.isNetworkConnected(Config.currentContext)) {
                builder.setMessage("There is no internet connection.\n" +
                                "Please exit, connect to the internet and reenter the launcher")
                        .setCancelable(false)
                        .setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                FirebaseCrashlytics.getInstance().setCrashlyticsCollectionEnabled(false);

                                finishAndRemoveTask();
                                System.exit(0);
                                dialog.dismiss();
                            }
                        })
                        .setNegativeButton("", null);
                AlertDialog alert = builder.create();
                alert.setTitle("Info");
                alert.show();
                return;
            }

            mInHandler = new IncomingHandler();
            mMessenger = new Messenger(mInHandler);

            GLSurfaceView.Renderer mGlRenderer = new GLSurfaceView.Renderer() {
                @Override
                public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
                    UpdateActivity.eGPUType egputype;
                    String glGetString = gl10.glGetString(GL10.GL_EXTENSIONS);
                    String glGetString2 = gl10.glGetString(GL10.GL_EXTENSIONS);
                    if (glGetString2.contains("GL_IMG_texture_compression_pvrtc")) {
                        egputype = UpdateActivity.eGPUType.PVR;
                        mGpuType = 3;
                    } else if (glGetString2.contains("GL_EXT_texture_compression_dxt1") || glGetString2.contains("GL_EXT_texture_compression_s3tc") || glGetString2.contains("GL_AMD_compressed_ATC_texture")) {
                        egputype = UpdateActivity.eGPUType.DXT;
                        mGpuType = 1;
                    } else {
                        egputype = UpdateActivity.eGPUType.ETC;
                        mGpuType = 2;
                    }
                    Log.e("x1y2z", "GPU name: " + glGetString);
                    Log.e("x1y2z", "GPU type: " + egputype.name());

                    if (isPermissionsGranted()) {
                        mIsBind = bindService(new Intent(SplashActivity.this, UpdateService.class), mConnection, Context.BIND_AUTO_CREATE);
                    } else {
                        ActivityCompat.requestPermissions(SplashActivity.this,
                                permissions,
                                1);
                    }
                }

                @Override
                public void onSurfaceChanged(GL10 gl10, int i, int i1) {

                }

                @Override
                public void onDrawFrame(GL10 gl10) {

                }
            };

            ConstraintLayout gpuLayout = findViewById(R.id.gpu);
            GLSurfaceView mGlSurfaceView = new GLSurfaceView(this);
            mGlSurfaceView.setRenderer(mGlRenderer);
            gpuLayout.addView(mGlSurfaceView);
        }
    }

    public class IncomingHandler extends Handler {
        public IncomingHandler() {}

        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            if (msg.what == 4) {
                UpdateActivity.UpdateStatus valueOf = UpdateActivity.UpdateStatus.valueOf(msg.getData().getString("status", ""));
                if (valueOf == UpdateActivity.UpdateStatus.Undefined) {
                    Message obtain = Message.obtain((Handler) null, 5);
                    obtain.replyTo = mMessenger;
                    try {
                        mService.send(obtain);
                    } catch (RemoteException e5) {
                        e5.printStackTrace();
                    }
                } else if (valueOf == UpdateActivity.UpdateStatus.CheckUpdate) {
                    Message obtain2 = Message.obtain((Handler) null, 4);
                    obtain2.replyTo = mMessenger;
                    try {
                        mService.send(obtain2);
                    } catch (RemoteException e6) {
                        e6.printStackTrace();
                    }
                }
            } else if (msg.what == 5) {
                UpdateActivity.GameStatus valueOf2 = UpdateActivity.GameStatus.valueOf(msg.getData().getString("status", ""));
                Log.i("x1y2z", "gameStatus = " + valueOf2);

                if (valueOf2 == UpdateActivity.GameStatus.UpdateRequired) {
                    if(!new SharedPreferenceCore().getBoolean(SplashActivity.this, "MODIFIED_DATA")) {
                        if(!isFinishing()) {
                            builder.setMessage("Game data update found!\nDo you want to update?")
                                    .setCancelable(false)
                                    .setPositiveButton("Update", new DialogInterface.OnClickListener() {
                                        public void onClick(DialogInterface dialog, int id) {
                                            finish();
                                            Intent intent = new Intent(SplashActivity.this, UpdateActivity.class);
                                            intent.putExtra("mode", UpdateActivity.UpdateMode.GameDataUpdate.name());
                                            startActivity(intent);
                                            finish();
                                            dialog.dismiss();
                                        }
                                    })
                                    .setNegativeButton("No", new DialogInterface.OnClickListener() {
                                        @Override
                                        public void onClick(DialogInterface dialogInterface, int i) {
                                            startActivity(new Intent(SplashActivity.this, MainActivity.class));
                                            finish();
                                            dialogInterface.dismiss();
                                        }
                                    });
                            AlertDialog alert = builder.create();
                            alert.setTitle("Update");
                            alert.show();
                        }
                    }
                    else {
                        startActivity(new Intent(SplashActivity.this, MainActivity.class));
                        finish();
                    }
                }
                else if (valueOf2 == UpdateActivity.GameStatus.GameUpdateRequired) {
                    Intent intent = new Intent(SplashActivity.this, UpdateActivity.class);
                    intent.putExtra("mode", UpdateActivity.UpdateMode.GameUpdate.name());
                    startActivity(intent);
                    finish();
                }
                else {
                    startActivity(new Intent(SplashActivity.this, MainActivity.class));
                    finish();
                }

            }
        }
    }

    public void checkUpdate() {
        Log.d("x1y2z", "checkUpdate");
        Message obtain = Message.obtain((Handler) null, 0);
        obtain.getData().putInt("gputype", mGpuType);
        obtain.replyTo = mMessenger;
        try {
            mService.send(obtain);
        } catch (RemoteException e5) {
            e5.printStackTrace();
        }
    }

    public boolean isPermissionsGranted()
    {
        int size = permissions.length;

        for (int i = 0; i < size; i++) {
            if (ContextCompat.checkSelfPermission(this, permissions[i])
                    == PackageManager.PERMISSION_GRANTED) {
                return true;
            }
        }

        return false;
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode != 1) {
            return;
        }
        if (grantResults.length <= 0 || grantResults[0] != 0) {
            Toast.makeText(this, "Permissions not granted!", Toast.LENGTH_LONG).show();
        } else {
            mIsBind = bindService(new Intent(this, UpdateService.class), mConnection, Context.BIND_AUTO_CREATE);
        }

    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if(mIsBind) {
            unbindService(mConnection);
            mIsBind = false;
        }
    }

    @Override
    protected void onResume() {
        super.onResume();

        if (prefs.getBoolean("firstrun", true)) {
            new SharedPreferenceCore().setInt(getApplicationContext(), "FPS_LIMIT", 60);
            new SharedPreferenceCore().setInt(getApplicationContext(), "MESSAGE_COUNT", 6);
            new SharedPreferenceCore().setBoolean(getApplicationContext(), "AIM", false);
            new SharedPreferenceCore().setBoolean(getApplicationContext(), "MODIFIED_DATA", false);
            new SharedPreferenceCore().setBoolean(getApplicationContext(), "AML", false);
            new SharedPreferenceCore().setBoolean(getApplicationContext(), "CLEO", false);
            new SharedPreferenceCore().setBoolean(getApplicationContext(), "MLOADER", false);
            new SharedPreferenceCore().setInt(getApplicationContext(), "VERSION", 0);
            prefs.edit().putBoolean("firstrun", false).commit();
        }
    }
}
