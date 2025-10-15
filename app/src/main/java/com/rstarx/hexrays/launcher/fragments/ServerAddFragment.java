package com.rstarx.hexrays.launcher.fragments;

import android.app.Activity;
import android.app.Dialog;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.RecyclerView;

import com.joom.paranoid.Obfuscate;
import com.rstarx.hexrays.R;
import com.rstarx.hexrays.launcher.MainActivity;
import com.rstarx.hexrays.launcher.adapters.FavouriteServerAdapter;
import com.rstarx.hexrays.launcher.adapters.ServerAdapter;
import com.rstarx.hexrays.launcher.data.FavoritesInfo;
import com.rstarx.hexrays.launcher.util.ButtonAnimator;
import com.rstarx.hexrays.launcher.util.SAMPServerInfo;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.Objects;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
@Obfuscate
public class ServerAddFragment extends Dialog {

    FavouriteServerAdapter mFavouriteServerAdapter;
    Activity activity;

    public ServerAddFragment(Activity a, FavouriteServerAdapter adapter)
    {
        super(a);
        activity = a;
        mFavouriteServerAdapter = adapter;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.alertdialog_addserver);

        EditText editText = findViewById(R.id.server_host);
        ImageView mClose = findViewById(R.id.server_close);

        findViewById(R.id.server_add).setOnTouchListener(new ButtonAnimator(getContext(), findViewById(R.id.server_add)));
        findViewById(R.id.server_add).setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                Pattern compile = Pattern.compile("^(((?!-)[A-Za-z0-9-]{1,63}(?<!-)\\.)+[A-Za-z]{2,6}|localhost|(([0-9]{1,3}\\.){3})[0-9]{1,3}):[0-9]{1,5}$");
                Pattern compile2 = Pattern.compile("^(((?!-)[A-Za-z0-9-]{1,63}(?<!-)\\.)+[A-Za-z]{2,6}|localhost|(([0-9]{1,3}\\.){3})[0-9]{1,3})$");
                Matcher matcher = compile.matcher(editText.getText().toString());
                Matcher matcher2 = compile2.matcher(editText.getText().toString());
                if (matcher.find() || matcher2.find()) {
                    if (!editText.getText().toString().contains(":")) {
                        editText.setText(editText.getText().toString() + ":7777");
                    }
                    /*try {
                        BufferedReader bufferedReader = new BufferedReader(new FileReader(mFavouriteServerAdapter.mServerFavouriteFile));
                        boolean z = false;
                        while (true) {
                            String readLine = bufferedReader.readLine();
                            if (readLine == null) {
                                break;
                            } else if (editText.getText().toString().equals(readLine)) {
                                Toast.makeText(activity, "This server is already added", Toast.LENGTH_LONG).show();
                                z = true;
                            }
                        }
                        if (!z) {
                            BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(mFavouriteServerAdapter.mServerFavouriteFile, true));
                            bufferedWriter.write(editText.getText().toString() + System.getProperty("line.separator"));
                            bufferedWriter.close();

                            mFavouriteServerAdapter.mServersInfo.add(new SAMPServerInfo(0,0,"Loading...", editText.getText().toString().split(":")[0], Integer.parseInt(editText.getText().toString().split(":")[1]), 0,0,0,0,0,"English"));
                            mFavouriteServerAdapter.refreshServers();
                        }
                        bufferedReader.close();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }*/
                    if (FavoritesInfo.IsServerExists(getContext(), 0, 0, editText.getText().toString().split(":")[0], Integer.parseInt(editText.getText().toString().split(":")[1]), false)) {
                        Toast.makeText(getContext(), "This server has been already added!", Toast.LENGTH_SHORT).show();
                        return;
                    }
                    FavoritesInfo.AddServer(getContext(), 0, 0, editText.getText().toString().split(":")[0], Integer.parseInt(editText.getText().toString().split(":")[1]));
                    FavoritesInfo.Save(getContext());
                    ((MainActivity)activity).getFavoriteServerList().add(new SAMPServerInfo(0,0,"Loading...", editText.getText().toString().split(":")[0], Integer.parseInt(editText.getText().toString().split(":")[1]), 0,0,0,0,0,"English"));
                    ((MainActivity)activity).refreshFavoriteServers();
                } else {
                    Toast.makeText(activity, "Invalid ip and port. (Use IP:PORT or IP)", Toast.LENGTH_LONG).show();
                }

               /* if(((MainActivity)activity).interstitialAd.isReady())
                {
                    ((MainActivity)activity).interstitialAd.showAd();
                }*/

                dismiss();
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
