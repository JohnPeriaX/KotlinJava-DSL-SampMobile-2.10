package com.rstarx.hexrays.launcher.fragments;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.transition.Fade;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import androidx.recyclerview.widget.SimpleItemAnimator;

import com.joom.paranoid.Obfuscate;
import com.rstarx.hexrays.R;
import com.rstarx.hexrays.launcher.MainActivity;
import com.rstarx.hexrays.launcher.SplashActivity;
import com.rstarx.hexrays.launcher.UpdateActivity;
import com.rstarx.hexrays.launcher.adapters.FavouriteServerAdapter;
import com.rstarx.hexrays.launcher.adapters.ServerAdapter;
import com.rstarx.hexrays.launcher.util.ButtonAnimator;

import java.util.Objects;
@Obfuscate
public class ServerPagesItemFragment extends Fragment {

    private int pagePosition;
    AlertDialog.Builder builder;

    public static ServerPagesItemFragment newInstance(int page)
    {
        ServerPagesItemFragment instance = new ServerPagesItemFragment();
        Bundle bundle = new Bundle();
        bundle.putInt("page", page);
        instance.setArguments(bundle);
        Log.d("InfoJSON", "ServerPagesItemFragment " + page);
        return instance;
    }

    public int getPage() {
        return pagePosition;
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(getArguments() != null)
        {
            pagePosition = getArguments().getInt("page");
            Log.d("InfoJSON", "ServerPagesItemFragment " + pagePosition);
        }

        builder = new AlertDialog.Builder(getContext());
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view;
        RecyclerView recyclerView;

        ((MainActivity)getActivity()).hideKeyboard(getActivity());

        view = inflater.inflate(R.layout.fragment_favorite, container, false);

        if(pagePosition == 1) {
            view = inflater.inflate(R.layout.fragment_internet, container, false);
            ServerAdapter adapter = new ServerAdapter(view.getContext(), this.pagePosition);

            Log.d("AXL", "pagepos == 1");

            RecyclerView recyclerView2 = view.findViewById(R.id.server_recycler);
            if (recyclerView2.getItemAnimator() != null) {
                ((SimpleItemAnimator) Objects.requireNonNull(recyclerView2.getItemAnimator())).setSupportsChangeAnimations(false);
                recyclerView2.setLayoutManager(new LinearLayoutManager(view.getContext()));
                if (view.getContext() != null) {
                    recyclerView2.setAdapter(adapter);
                }
            }

            view.findViewById(R.id.buttonCustomServer).setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    Log.d("gor", "clicked add server button");
                    builder.setMessage("Write me to add here your server (25$ per month)!\nTelegram: @gorgrigoryan18\n" +
                                    "Discord: x1y2z")
                            .setCancelable(false)
                            .setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    dialog.dismiss();
                                }
                            })
                            .setNegativeButton("", null);
                    AlertDialog alert = builder.create();
                    alert.setTitle("Update");
                    alert.show();
                }
            });
        } else {
            view = inflater.inflate(R.layout.fragment_favorite, container, false);

            FavouriteServerAdapter adapter = new FavouriteServerAdapter(getActivity());

            recyclerView = view.findViewById(R.id.server_recycler);
            ((SimpleItemAnimator) recyclerView.getItemAnimator()).setSupportsChangeAnimations(false);
            recyclerView.setLayoutManager(new LinearLayoutManager(recyclerView.getContext()));
            if (recyclerView.getContext() != null) {
                recyclerView.setAdapter(adapter);
            }

            view.findViewById(R.id.buttonServer).setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    Log.d("gor", "clicked add server button");
                    ServerAddFragment newInstance = new ServerAddFragment(getActivity(), adapter);
                    newInstance.show();
                }
            });
        }

        return view;
    }
}
