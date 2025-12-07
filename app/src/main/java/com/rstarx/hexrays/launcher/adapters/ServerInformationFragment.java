package com.rstarx.hexrays.launcher.adapters;

import android.app.Activity;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.Fragment;

import com.joom.paranoid.Obfuscate;
import com.rstarx.hexrays.R;
import com.rstarx.hexrays.game.GTASA;
import com.rstarx.hexrays.game.SAMP;
import com.rstarx.hexrays.launcher.MainActivity;
import com.rstarx.hexrays.launcher.util.ButtonAnimator;
import com.rstarx.hexrays.launcher.util.SAMPServerInfo;
import com.rstarx.hexrays.launcher.util.SharedPreferenceCore;

import org.ini4j.Wini;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
@Obfuscate
public class ServerInformationFragment extends Dialog {

    ServerAdapter mServerAdapter;
    SAMPServerInfo sampServerInfo;
    int position;
    Activity activity;

    public ServerInformationFragment(Activity a, ServerAdapter adapter, int position, SAMPServerInfo sampServerInfo1)
    {
        super(a);
        activity = a;
        mServerAdapter = adapter;
        sampServerInfo = sampServerInfo1;
        this.position = position;
    }

    @Nullable
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.alertdialog_server);

        TextView mHostName = findViewById(R.id.server_hostname);
        TextView mIP = findViewById(R.id.server_ip);
        TextView mPort = findViewById(R.id.server_port);
        TextView mOnlineServer = findViewById(R.id.server_online);
        TextView mMode = findViewById(R.id.server_mode);
        TextView mLanguage = findViewById(R.id.server_language);
        ImageView mClose = findViewById(R.id.server_close);
        EditText mServerPassword = findViewById(R.id.server_password);
        Button mSave = findViewById(R.id.save_favorites);

        mSave.setVisibility(View.GONE);

        if(!sampServerInfo.getHasPassword())
            mServerPassword.setVisibility(View.GONE);
        else
            mServerPassword.setVisibility(View.VISIBLE);

        mHostName.setText(sampServerInfo.getServerName());

        mIP.setText(sampServerInfo.getAddress());
        mPort.setText(String.valueOf(sampServerInfo.getPort()));
        mOnlineServer.setText(sampServerInfo.getCurrentPlayerCount() + "/" + sampServerInfo.getMaxPlayerCount());
        mMode.setText(sampServerInfo.getServerMode());

        mLanguage.setText(sampServerInfo.getLanguage());

        Button mConnect = findViewById(R.id.server_connect);
        mConnect.setOnTouchListener(new ButtonAnimator(getContext(), mConnect));
        mConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                File file = new File(activity.getExternalFilesDir(null) + "/SAMP/settings.ini");
                if(file.exists()) {
                    try {
                        Wini wini = new Wini(file);
                        wini.put("client", "host", sampServerInfo.getAddress());
                        wini.put("client", "port", sampServerInfo.getPort());
                        wini.store();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }



                File file1 = new File(activity.getExternalFilesDir(null) + "/Text/american.dxt");
                if(!file1.exists())
                {
                    File file2 = new File(activity.getExternalFilesDir(null) + "/Textures/fonts/RussianFont.png");
                    if(!file2.exists())
                    {
                        Toast.makeText(activity, "Some important files in your modified data are missing, such as \"Text\" and \"Textures\"" +
                                "Please, fix it and after try again. ( You can get that files in my discord channel )", Toast.LENGTH_LONG).show();

                        dismiss();
                    }
                    else {
                        activity.startActivity(new Intent(activity, SAMP.class));
                        activity.finish();
                        dismiss();
                    }
                }
                else {
                    activity.startActivity(new Intent(activity, SAMP.class));
                    activity.finish();
                    dismiss();
                }
            }
        });


        mClose.setOnTouchListener(new ButtonAnimator(getContext(), mClose));
        mClose.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                dismiss();
            }
        });
    }

}
