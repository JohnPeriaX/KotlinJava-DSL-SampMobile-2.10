package com.rstarx.hexrays.launcher.fragments;

import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.ToggleButton;

import androidx.appcompat.widget.SwitchCompat;
import androidx.fragment.app.Fragment;

import com.joom.paranoid.Obfuscate;
import com.rstarx.hexrays.R;
import com.rstarx.hexrays.launcher.MainActivity;
import com.rstarx.hexrays.launcher.SplashActivity;
import com.rstarx.hexrays.launcher.util.ButtonAnimator;
import com.rstarx.hexrays.launcher.util.SharedPreferenceCore;
import com.rstarx.hexrays.launcher.util.Util;

import org.ini4j.InvalidFileFormatException;
import org.ini4j.Wini;

import java.io.File;
import java.io.IOException;
@Obfuscate
public class SettingsFragment extends Fragment {

    Wini mWini = null;
    EditText mNickName;
    SwitchCompat mKeyboardSwitch;
    SwitchCompat mVoiceSwitch;
    SwitchCompat mModifySwitch;
    SwitchCompat mFPSSwitch;
    SwitchCompat mMonetSwitch;
    SeekBar mMessagesSeekBar;
    TextView mMessagesText;
    SeekBar mFPSSeekBar;
    TextView mFPSText;

    String[] titles = {"0.3.7", "0.3.7-R1", "0.3.7-R3","0.3.7-R4","0.3.7-R5"};

    Spinner autoCompleteTextView;
    ArrayAdapter<String> adapter;


    @Override
    public View onCreateView(LayoutInflater layoutInflater, ViewGroup viewGroup, Bundle bundle) {
        View view = layoutInflater.inflate(R.layout.fragment_settings, viewGroup, false);

        ((MainActivity)getActivity()).hideKeyboard(getActivity());

        mNickName = view.findViewById(R.id.settings_nickname);
        autoCompleteTextView = view.findViewById(R.id.spinner);
        mKeyboardSwitch = view.findViewById(R.id.keyboard_switch);
        mFPSSwitch = view.findViewById(R.id.fps_switch);
        mMonetSwitch = view.findViewById(R.id.monet_switch);
        mVoiceSwitch = view.findViewById(R.id.voice_switch);
        mModifySwitch = view.findViewById(R.id.modify_switch);
        mMessagesSeekBar = view.findViewById(R.id.messages_seekbar);
        mMessagesText = view.findViewById(R.id.messages_count);
        mFPSSeekBar = view.findViewById(R.id.fps_seekbar);
        mFPSText = view.findViewById(R.id.fps_count);

        adapter = new ArrayAdapter<String>(getActivity(), androidx.appcompat.R.layout.support_simple_spinner_dropdown_item, titles);
        adapter.setDropDownViewResource(androidx.appcompat.R.layout.support_simple_spinner_dropdown_item);
        autoCompleteTextView.setAdapter(adapter);

        File file = new File(getActivity().getExternalFilesDir(null) + "/SAMP/settings.ini");
        try {
            mWini = new Wini(file);

            mNickName.setText(mWini.get("client", "name"));

            mWini.store();
        } catch (IOException e) {
            e.printStackTrace();
        }

        autoCompleteTextView.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                new SharedPreferenceCore().setInt(requireContext().getApplicationContext(), "VERSION", position);
                File file = new File(getActivity().getExternalFilesDir(null) + "/SAMP/settings.ini");
                if(file.exists()) {
                    try {
                        if(mWini != null) {
                            mWini.put("client", "version", titles[new SharedPreferenceCore().getInt(requireContext().getApplicationContext(), "VERSION")]);
                            mWini.store();
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });

        mNickName.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                String text = charSequence.toString();
                File file = new File(getActivity().getExternalFilesDir(null) + "/SAMP/settings.ini");
                if(file.exists()) {
                    try {
                        if(mWini != null) {
                            mWini.put("client", "name", text);
                            mWini.store();
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });

        mModifySwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                new SharedPreferenceCore().setBoolean(requireContext().getApplicationContext(), "MODIFIED_DATA", b);
            }
        });

        mKeyboardSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                new SharedPreferenceCore().setBoolean(requireContext().getApplicationContext(), "ANDROID_KEYBOARD", b);
                try {
                    if(mWini != null) {
                        mWini.put("gui", "androidkeyboard", b);
                        mWini.store();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        mVoiceSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                new SharedPreferenceCore().setBoolean(requireContext().getApplicationContext(), "AIM", b);
                try {
                    if(mWini != null) {
                        mWini.put("gui", "autoaim", b);
                        mWini.store();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        mFPSSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                new SharedPreferenceCore().setBoolean(requireContext().getApplicationContext(), "FPS_DISPLAY", b);
                try {
                    if(mWini != null) {
                        mWini.put("gui", "fps", b ? 1 : 0);
                        mWini.store();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });

        mMonetSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                new SharedPreferenceCore().setBoolean(requireContext().getApplicationContext(), "MLOADER", b);
            }
        });

        // perform seek bar change listener event used for getting the progress value
        mMessagesSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                int realProgress = 0;
                switch(progress)
                {
                    case 0: {
                        realProgress = 6;
                        break;
                    }
                    case 1: {
                        realProgress = 9;
                        break;
                    }
                    case 2: {
                        realProgress = 12;
                        break;
                    }
                    case 3:{
                        realProgress = 15;
                        break;
                    }
                }
                new SharedPreferenceCore().setInt(requireContext().getApplicationContext(), "MESSAGE_COUNT", realProgress);
                File file = new File(getActivity().getExternalFilesDir(null) + "/SAMP/settings.ini");
                if(file.exists()) {
                    try {
                        if(mWini != null) {
                            mWini.put("gui", "ChatMaxMessages", realProgress);
                            mWini.store();
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                mMessagesText.setText(String.valueOf(realProgress));
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
                // TODO Auto-generated method stub
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

        // perform seek bar change listener event used for getting the progress value
        mFPSSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                int realProgress = 0;
                switch(progress)
                {
                    case 0: {
                        realProgress = 30;
                        break;
                    }
                    case 1: {
                        realProgress = 60;
                        break;
                    }
                    case 2: {
                        realProgress = 90;
                        break;
                    }
                    case 3:{
                        realProgress = 120;
                        break;
                    }
                }
                new SharedPreferenceCore().setInt(requireContext().getApplicationContext(), "FPS_LIMIT", realProgress);
                File file = new File(getActivity().getExternalFilesDir(null) + "/SAMP/settings.ini");
                if(file.exists()) {
                    try {
                        if(mWini != null) {
                            mWini.put("gui", "FPSLimit", realProgress);
                            mWini.store();
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                mFPSText.setText(String.valueOf(realProgress));
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
                // TODO Auto-generated method stub
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });


        return view;
    }

    @Override
    public void onResume() {
        super.onResume();

        mKeyboardSwitch.setChecked(new SharedPreferenceCore().getBoolean(requireContext().getApplicationContext(), "ANDROID_KEYBOARD"));
        mVoiceSwitch.setChecked(new SharedPreferenceCore().getBoolean(requireContext().getApplicationContext(), "AIM"));
        mFPSSwitch.setChecked(new SharedPreferenceCore().getBoolean(requireContext().getApplicationContext(), "FPS_DISPLAY"));
        mModifySwitch.setChecked(new SharedPreferenceCore().getBoolean(requireContext().getApplicationContext(), "MODIFIED_DATA"));
        mMonetSwitch.setChecked(new SharedPreferenceCore().getBoolean(requireContext().getApplicationContext(), "MLOADER"));
        autoCompleteTextView.setSelection(new SharedPreferenceCore().getInt(requireContext().getApplicationContext(), "VERSION"));

        int fps = new SharedPreferenceCore().getInt(getContext(), "FPS_LIMIT");
        switch (fps)
        {
            case 30: mFPSSeekBar.setProgress(0); break;
            case 60: mFPSSeekBar.setProgress(1); break;
            case 90: mFPSSeekBar.setProgress(2); break;
            case 120: mFPSSeekBar.setProgress(3); break;
        }
        mFPSText.setText(String.valueOf(fps));

        int message = new SharedPreferenceCore().getInt(getContext(), "MESSAGE_COUNT");
        switch (message)
        {
            case 6: mMessagesSeekBar.setProgress(0); break;
            case 9: mMessagesSeekBar.setProgress(1); break;
            case 12: mMessagesSeekBar.setProgress(2); break;
            case 15: mMessagesSeekBar.setProgress(3); break;
        }
        mMessagesText.setText(String.valueOf(message));
    }
}
