package com.rstarx.hexrays.launcher.fragments;

import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentPagerAdapter;
import androidx.viewpager2.widget.ViewPager2;

import com.google.android.material.tabs.TabLayout;
import com.joom.paranoid.Obfuscate;
import com.rstarx.hexrays.R;
import com.rstarx.hexrays.launcher.MainActivity;
import com.rstarx.hexrays.launcher.util.ViewPagerWithoutSwipe;
@Obfuscate
public class ServersFragment extends Fragment {

    public final class PagerAdapter extends FragmentPagerAdapter {
        private int pageCount;

        public PagerAdapter(FragmentManager fragmentManager, int i) {
            super(fragmentManager);
            pageCount = i;
            Log.d("InfoJSON", "PagerAdapter " + i);
        }

        @Override
        public Fragment getItem(int position) {
            Log.d("InfoJSON", "PagerAdapter "+position);
            return ServerPagesItemFragment.newInstance(position);
        }

        @Override
        public int getCount() {
            return pageCount;
        }

        @Override
        public CharSequence getPageTitle(int position) {
            if (position == 0) {
                return "Favorites";
            }
            else if (position == 1) {
                return "Hosted";
            }

            return "None";
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_servers, container, false);

        ((MainActivity)getActivity()).hideKeyboard(getActivity());

        return view;
    }

    @Override
    public void onStart() {
        super.onStart();
        View view = getView();
        if(view !=null) {
            TabLayout tabLayout = view.findViewById(R.id.tabLayout_servers);
            ViewPagerWithoutSwipe viewPager2 = view.findViewById(R.id.viewPager_servers);

            FragmentManager fm = getChildFragmentManager();
            PagerAdapter sa = new PagerAdapter(fm, 2);
            viewPager2.setAdapter(sa);
            tabLayout.setupWithViewPager(viewPager2);
        }
    }
}
