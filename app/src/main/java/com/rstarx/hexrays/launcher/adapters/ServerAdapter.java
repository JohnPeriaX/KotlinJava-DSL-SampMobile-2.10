package com.rstarx.hexrays.launcher.adapters;

import android.app.Activity;
import android.content.Context;
import android.os.AsyncTask;
import android.transition.Fade;
import android.util.Log;
import android.util.Pair;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.RecyclerView;

import com.joom.paranoid.Obfuscate;
import com.rstarx.hexrays.R;
import com.rstarx.hexrays.launcher.MainActivity;
import com.rstarx.hexrays.launcher.util.ButtonAnimator;
import com.rstarx.hexrays.launcher.util.SAMPServerInfo;
import com.rstarx.hexrays.launcher.util.SampQueryAPI;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
@Obfuscate
public class ServerAdapter extends RecyclerView.Adapter<ServerAdapter.ViewHolder> {

    public Context mContext;
    public ArrayList<SAMPServerInfo> mServersInfo = new ArrayList<>();
    public ArrayList<SAMPServerInfo> mServersInfoFavourite = new ArrayList<>();
    public File mServerFavouriteFile;
    int position = 0;

    private int pagePosition;
    private final HashMap<Integer, Pair<String, Integer>> holderPosition = new HashMap<>();

    public ServerAdapter(Context context, int page)
    {
        mContext = context;
        pagePosition = page;
        refreshServers();
        Log.d("InfoJSON", "ServerAdapter " + mServersInfo.size());
    }

    public int getPagePosition()
    {
        return pagePosition;
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.server_item, parent, false);

        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {

        this.position = holder.getAdapterPosition();

        /*new Thread(new Runnable() {
                public void run() {
                    new ProcessInfo().execute(Integer.valueOf(holder.getAdapterPosition()));
                }
            }).start();*/

        if (!mServersInfo.get(holder.getAdapterPosition()).getHasPassword())
            holder.mPassword.setImageResource(R.drawable.ic_password_off);
        else
            holder.mPassword.setImageResource(R.drawable.ic_password_on);

        holder.mServerNameText.setText(mServersInfo.get(holder.getAdapterPosition()).getServerName());

        holder.mOnlineText.setText(mServersInfo.get(holder.getAdapterPosition()).getCurrentPlayerCount() + "/" + mServersInfo.get(holder.getAdapterPosition()).getMaxPlayerCount());
        holder.mServerIP.setText("Adress: " + mServersInfo.get(holder.getAdapterPosition()).getAddress());
        holder.mMode.setText("Mode: " + mServersInfo.get(holder.getAdapterPosition()).getServerMode());

        holder.mLayout.setOnTouchListener(new ButtonAnimator(mContext, holder.mLayout));
        holder.mLayout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                ServerInformationFragment newInstance = new ServerInformationFragment((Activity) mContext, ServerAdapter.this, holder.getAdapterPosition(), mServersInfo.get(holder.getAdapterPosition()));
        /*newInstance.setEnterTransition(new Fade());
        if (holder.itemView.getContext() != null) {
            ((MainActivity) holder.itemView.getContext()).getSupportFragmentManager().beginTransaction().replace(R.id.main_layout, newInstance).addToBackStack(null).commit();
        }*/
                //StartAppAd.showAd(mContext);
                newInstance.show();

            }
        });

        holderPosition.put(Integer.valueOf(holder.getAdapterPosition()), new Pair<>(mServersInfo.get(holder.getAdapterPosition()).getAddress(), Integer.valueOf(mServersInfo.get(holder.getAdapterPosition()).getPort())));
    }

    public void refreshServers() {
        mServersInfo.clear();
        getLocalServers();
        notifyDataSetChanged();
    }

    public void getLocalServers() {
        mServersInfo = ((MainActivity)mContext).getServerList();
    }

    @Override
    public int getItemCount() {
        Log.d("AXL" , "size: " + ((MainActivity)mContext).getServerList().size());
        return ((MainActivity)mContext).getServerList().size();
    }

    public class ViewHolder extends RecyclerView.ViewHolder
    {
        private View mView;
        public ImageView mPassword;
        public TextView mServerIP;
        public TextView mServerNameText;
        public TextView mOnlineText;
        public TextView mMode;
        public ConstraintLayout mLayout;

        public ViewHolder(View view) {
            super(view);
            mView = view;
            mLayout = view.findViewById(R.id.server_item_layout);
            mServerIP = view.findViewById(R.id.textView16);
            mPassword = view.findViewById(R.id.password);
            mServerNameText = view.findViewById(R.id.textView6);
            mOnlineText = view.findViewById(R.id.online_text);
            mMode = view.findViewById(R.id.textView18);
        }

        public View getView() {
            return mView;
        }
    }
}
