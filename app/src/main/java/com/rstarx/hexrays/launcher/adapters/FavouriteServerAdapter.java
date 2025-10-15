package com.rstarx.hexrays.launcher.adapters;

import android.app.Activity;
import android.content.Context;
import android.util.Log;
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
@Obfuscate
public class FavouriteServerAdapter extends RecyclerView.Adapter<FavouriteServerAdapter.ViewHolder>{

    public Context mActivity;
    int position;


    public FavouriteServerAdapter(Activity activity)
    {
        mActivity = activity;
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.server_item, parent, false);

        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {
        Log.d("AXL", "" + holder.getAdapterPosition());
        this.position = holder.getAdapterPosition();
        if (!((MainActivity) mActivity).getFavoriteServerList().get(holder.getAdapterPosition()).getHasPassword())
            holder.mPassword.setImageResource(R.drawable.ic_password_off);
        else
            holder.mPassword.setImageResource(R.drawable.ic_password_on);

        holder.mServerNameText.setText(((MainActivity) mActivity).getFavoriteServerList().get(holder.getAdapterPosition()).getServerName());

        holder.mOnlineText.setText(((MainActivity) mActivity).getFavoriteServerList().get(holder.getAdapterPosition()).getCurrentPlayerCount() + "/" + ((MainActivity) mActivity).getFavoriteServerList().get(holder.getAdapterPosition()).getMaxPlayerCount());
        holder.mServerIP.setText("Address: " + ((MainActivity) mActivity).getFavoriteServerList().get(holder.getAdapterPosition()).getAddress());
        holder.mMode.setText("Mode: " + ((MainActivity) mActivity).getFavoriteServerList().get(holder.getAdapterPosition()).getServerMode());

        holder.mLayout.setOnTouchListener(new ButtonAnimator(mActivity, holder.mLayout));
        holder.mLayout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                FavouriteServerInformationFragment newInstance = new FavouriteServerInformationFragment((Activity) mActivity, FavouriteServerAdapter.this, holder.getAdapterPosition(), ((MainActivity) mActivity).getFavoriteServerList().get(holder.getAdapterPosition()));
                newInstance.show();

            }
        });
    }

    @Override
    public int getItemCount() {
        return ((MainActivity) mActivity).getFavoriteServerList().size();
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
