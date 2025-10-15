package com.rstarx.hexrays.launcher.adapters;

import android.app.Activity;
import android.app.Dialog;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
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
import androidx.fragment.app.Fragment;

import com.joom.paranoid.Obfuscate;
import com.rstarx.hexrays.R;
import com.rstarx.hexrays.game.GTASA;
import com.rstarx.hexrays.game.SAMP;
import com.rstarx.hexrays.launcher.MainActivity;
import com.rstarx.hexrays.launcher.data.FavoritesInfo;
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
import java.util.Arrays;

@Obfuscate
public class FavouriteServerInformationFragment extends Dialog {

    FavouriteServerAdapter mFavouriteServerAdapter;
    SAMPServerInfo sampServerInfo;
    int position;
    Activity act;

    public FavouriteServerInformationFragment(Activity a, FavouriteServerAdapter adapter, int position, SAMPServerInfo sampServerInfo1)
    {
        super(a);
        act = a;
        mFavouriteServerAdapter = adapter;
        sampServerInfo = sampServerInfo1;
        this.position = position;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.alertdialog_server);

        File file = new File(act.getExternalFilesDir(null), "favoriteservers.txt");
        File file2 = new File(act.getExternalFilesDir(null), "favoriteservers_temp.txt");

        TextView mHostName = findViewById(R.id.server_hostname);
        TextView mIP = findViewById(R.id.server_ip);
        TextView mPort = findViewById(R.id.server_port);
        TextView mOnlineServer = findViewById(R.id.server_online);
        TextView mMode = findViewById(R.id.server_mode);
        TextView mLanguage = findViewById(R.id.server_language);
        ImageView mClose = findViewById(R.id.server_close);
        EditText mServerPassword = findViewById(R.id.server_password);
        Button mSave = findViewById(R.id.save_favorites);

        mSave.setVisibility(View.VISIBLE);

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

        mSave.setOnTouchListener(new ButtonAnimator(getContext(), mSave));
        mSave.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                //try {
                    /*BufferedReader bufferedReader = new BufferedReader(new FileReader(file));
                    BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(file2));
                    while (true) {
                        String readLine = bufferedReader.readLine();
                        if (readLine != null) {
                            if (!(mFavouriteServerAdapter.mServersInfo.get(position).getAddress() + ":" + mFavouriteServerAdapter.mServersInfo.get(position).getPort()).equals(readLine)) {
                                bufferedWriter.write(readLine + System.getProperty("line.separator"));
                            }
                        } else {
                            bufferedWriter.close();
                            bufferedReader.close();
                            file.delete();
                            file2.renameTo(file);
                            dismiss();
                            mFavouriteServerAdapter.refreshServers();
                            return;
                        }
                    }*/
                FavoritesInfo.RemoveServer(getContext(), ((MainActivity)act).getFavoriteServerList().get(position).getAddress(), ((MainActivity)act).getFavoriteServerList().get(position).getPort());
                FavoritesInfo.Save(getContext());
                ((MainActivity)act).getFavoriteServerList().remove(position);
                ((MainActivity)act).refreshFavoriteServers();
                dismiss();
                //} catch (IOException e) {
                //    e.printStackTrace();
               // }
            }
        });

        Button mConnect = findViewById(R.id.server_connect);
        mConnect.setOnTouchListener(new ButtonAnimator(getContext(), mConnect));
        mConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                File file = new File(act.getExternalFilesDir(null) + "/SAMP/settings.ini");
                if(file.exists()) {
                    try {
                        Wini wini = new Wini(file);
                        String name = wini.get("client", "name");
                        wini.put("client", "host", sampServerInfo.getAddress());
                        wini.put("client", "port", sampServerInfo.getPort());
                        wini.store();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

                if(new SharedPreferenceCore().getBoolean(getContext(), "MLOADER"))
                {
                    String data = Environment.getExternalStorageDirectory() + "/Android/media/com.rstarx.hexrays";
                    File file4 = new File(data + "/monetloader/compat/profile.json");
                    Log.d("AXL", data + "/monetloader/compat/profile.json");
                    if(file4.isDirectory() || !file4.exists())
                    {
                        file4.delete();
                        try {
                            file.createNewFile();
                            FileWriter writer = new FileWriter(file4);
                            writer.append("{\n" +
                                    "  \"gtasa_name\": \"libGTASA.so\",\n" +
                                    "  \"profile_name\": \"SA-MP 0.3.7\",\n" +
                                    "  \"compat_scripts\": [],\n" +
                                    "  \"samp_name\": \"libsamp.so\",\n" +
                                    "  \"receiveignorerpc_pattern\": \"F0B503AF2DE900????B004460068C16A20468847\",\n" +
                                    "  \"cnetgame_ctor_pattern\": \"F0B503AF2DE9000788B00D46????9146????0446002079447A44\",\n" +
                                    "  \"rakclientinterface_netgame_offset\": 528,\n" +
                                    "  \"use_samp_touch_workaround\": true,\n" +
                                    "  \"nveventinsertnewest_offset\": 2606320\n" +
                                    "}");
                            writer.flush();
                            writer.close();
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }

                File file1 = new File(act.getExternalFilesDir(null) + "/Text/american.dxt");
                if(!file1.exists())
                {
                    File file2 = new File(act.getExternalFilesDir(null) + "/Textures/fonts/RussianFont.png");
                    if(!file2.exists())
                    {
                        Toast.makeText(act, "Some important files in your modified data are missing, such as \"Text\" and \"Textures\"" +
                                "Please, fix it and after try again. ( You can get that files in my discord channel )", Toast.LENGTH_LONG).show();

                        dismiss();
                    }
                    else {
                        act.startActivity(new Intent(act, SAMP.class));
                        act.finish();
                        dismiss();
                    }
                }
                else {
                    act.startActivity(new Intent(act, SAMP.class));
                    act.finish();
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
