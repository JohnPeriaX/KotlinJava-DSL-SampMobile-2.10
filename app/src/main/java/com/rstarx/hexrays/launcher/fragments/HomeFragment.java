package com.rstarx.hexrays.launcher.fragments;

import android.content.Intent;
import android.media.Image;
import android.net.Uri;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import androidx.fragment.app.Fragment;

import com.joom.paranoid.Obfuscate;
import com.rstarx.hexrays.R;
import com.rstarx.hexrays.launcher.MainActivity;
import com.rstarx.hexrays.launcher.util.ButtonAnimator;
@Obfuscate
public class HomeFragment extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_home, container, false);

        ((MainActivity)getActivity()).hideKeyboard(getActivity());

        ImageView yt_image = (ImageView) view.findViewById(R.id.youtube_logo);
        yt_image.setOnTouchListener(new ButtonAnimator(getContext(), yt_image));
        yt_image.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent link = new Intent(Intent.ACTION_VIEW, Uri.parse("https://bit.ly/x1y2z_yt"));
                startActivity(link);
            }
        });

        ImageView discord_image = (ImageView) view.findViewById(R.id.discord_logo);
        discord_image.setOnTouchListener(new ButtonAnimator(getContext(), discord_image));
        discord_image.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent link = new Intent(Intent.ACTION_VIEW, Uri.parse("https://discord.gg/jvKM7HR3Dc"));
                startActivity(link);
            }
        });

        ImageView internet_logo = (ImageView) view.findViewById(R.id.internet_button);
        internet_logo.setOnTouchListener(new ButtonAnimator(getContext(), internet_logo));
        internet_logo.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent link = new Intent(Intent.ACTION_VIEW, Uri.parse("https://samp-mobile.shop"));
                startActivity(link);
            }
        });

        return view;
    }
}
