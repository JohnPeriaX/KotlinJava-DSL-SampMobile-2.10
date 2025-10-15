package com.rstarx.hexrays.game.ui;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.google.android.material.button.MaterialButton;
import com.joom.paranoid.Obfuscate;
import com.rstarx.hexrays.R;
import com.rstarx.hexrays.launcher.util.Util;
@Obfuscate
public class AttachEdit {
    private final int POSITION_Y = 0;
    private final int POSITION_Z = 1;
    private final int POSITION_X = 2;
    private final int BUTTON_SCALE = 3;
    private final int BUTTON_ROT_X = 4;
    private final int BUTTON_ROT_Y = 5;
    private final int BUTTON_ROT_Z = 6;

    private int active_button = -1;

    private Activity activity;
    private ConstraintLayout attach_main_layot;
    private Button attach_exit_butt;
    private Button attach_save_butt;
    private ConstraintLayout attach_leftright;
    private ConstraintLayout attach_topbott;
    private ConstraintLayout attach_pushpull;
    private ConstraintLayout attach_scale;
    private ConstraintLayout attach_rotX;
    private ConstraintLayout attach_rotY;
    private ConstraintLayout attach_rotZ;
    //private TextView attach_text_description;
    private Button attach_left_butt;
    private Button attach_right_button;

    native void Exit();
    native void AttachClick(int buttonType, boolean buttonID);
    native void Save();
    
    public boolean isShow;

    @SuppressLint("ClickableViewAccessibility")
    public AttachEdit(Activity activity)
    {
        this.activity = activity;

        ConstraintLayout layout = (ConstraintLayout) activity.getLayoutInflater().inflate(R.layout.attachedit, null);
        activity.addContentView(layout, new ConstraintLayout.LayoutParams(-1, -1));

        attach_left_butt = activity.findViewById(R.id.attach_left_butt);
        attach_left_butt.setOnTouchListener(touchListener);

        attach_right_button = activity.findViewById(R.id.attach_right_button);
        attach_right_button.setOnTouchListener(touchListener);

        //attach_text_description = activity.findViewById(R.id.attach_text_description);

        attach_save_butt = activity.findViewById(R.id.attach_save_butt);
        attach_save_butt.setOnClickListener(view -> {
            Util.HideLayout(attach_main_layot, true);
            Save();
        });

        attach_exit_butt = activity.findViewById(R.id.attach_exit_butt);
        attach_exit_butt.setOnClickListener(view -> {
            Util.HideLayout(attach_main_layot, true);
            Exit();
        });

        attach_leftright = activity.findViewById(R.id.attach_leftright);
        attach_leftright.setOnClickListener(view -> {
            active_button = POSITION_X;
            //attach_text_description.setText("Установите смещение по оси X");
            SelectetItem(view);
        });

        attach_topbott = activity.findViewById(R.id.attach_topbott);
        attach_topbott.setOnClickListener(view -> {
            active_button = POSITION_Y;
            //attach_text_description.setText("Установите смещение по оси Y");
            SelectetItem(view);
        });

        attach_pushpull = activity.findViewById(R.id.attach_pushpull);
        attach_pushpull.setOnClickListener(view -> {
            active_button = POSITION_Z;
            //attach_text_description.setText("Установите смещение по оси Z");
            SelectetItem(view);
        });

        attach_scale = activity.findViewById(R.id.attach_scale);
        attach_scale.setOnClickListener(view -> {
            //attach_text_description.setText("Установите размер");
            active_button = BUTTON_SCALE;
            SelectetItem(view);
        });

        attach_rotX = activity.findViewById(R.id.attach_rotX);
        attach_rotX.setOnClickListener(view -> {
            //attach_text_description.setText("Установите поворот X");
            active_button = BUTTON_ROT_X;
            SelectetItem(view);
        });

        attach_rotY = activity.findViewById(R.id.attach_rotY);
        attach_rotY.setOnClickListener(view -> {
            //attach_text_description.setText("Установите поворот Y");
            active_button = BUTTON_ROT_Y;
            SelectetItem(view);
        });

        attach_rotZ = activity.findViewById(R.id.attach_rotZ);
        attach_rotZ.setOnClickListener(view -> {
            //attach_text_description.setText("Установите поворот Z");
            active_button = BUTTON_ROT_Z;
            SelectetItem(view);
        });

        attach_main_layot = activity.findViewById(R.id.attach_main_layot);

        isShow = false;
        Util.HideLayout(attach_main_layot, true);
    }

    public void show()
    {
        isShow = true;
        Util.ShowLayout(attach_main_layot, true);
    }

    public void hide()
    {
        isShow = false;
        Util.HideLayout(attach_main_layot, true);
    }

    public void showWithoutReset()
    {
        attach_main_layot.setVisibility(View.VISIBLE);
    }

    public void hideWithoutReset()
    {
        attach_main_layot.setVisibility(View.GONE);
    }

    public void SelectetItem(View view){
        attach_leftright.getChildAt(0).setBackgroundTintList(null);
        //attach_leftright.getChildAt(2).setBackgroundTintList(null);

        attach_topbott.getChildAt(0).setBackgroundTintList(null);
        //attach_topbott.getChildAt(2).setBackgroundTintList(null);

        attach_pushpull.getChildAt(0).setBackgroundTintList(null);
        //attach_pushpull.getChildAt(2).setBackgroundTintList(null);

        attach_scale.getChildAt(0).setBackgroundTintList(null);
        //attach_scale.getChildAt(2).setBackgroundTintList(null);

        attach_rotX.getChildAt(0).setBackgroundTintList(null);
        //attach_rotX.getChildAt(2).setBackgroundTintList(null);

        attach_rotY.getChildAt(0).setBackgroundTintList(null);
        //attach_rotY.getChildAt(2).setBackgroundTintList(null);

        attach_rotZ.getChildAt(0).setBackgroundTintList(null);
        //attach_rotZ.getChildAt(2).setBackgroundTintList(null);

        ConstraintLayout tmp = (ConstraintLayout) view;
        tmp.getChildAt(0).setBackgroundTintList(ColorStateList.valueOf(activity.getResources().getColor(R.color.colorAccent)));
        //tmp.getChildAt(2).setBackgroundTintList(ColorStateList.valueOf(Color.parseColor("#000000")));
    }
    //
    int fix_move = 0;
    private View.OnTouchListener touchListener = new View.OnTouchListener() {
        @Override public boolean onTouch(View view, MotionEvent event) {

            switch (event.getAction() & MotionEvent.ACTION_MASK) {
                case MotionEvent.ACTION_DOWN: {
                    fix_move = 10;
                    break;
                }
                case MotionEvent.ACTION_UP: {
                    if (view == attach_left_butt) {
                        AttachClick(active_button, false);
                    } else {
                        AttachClick(active_button, true);
                    }
                    break;
                }
                case MotionEvent.ACTION_MOVE: {
                    if(fix_move > 0){
                        fix_move --;
                        if(fix_move > 0)return true;
                    }
                    if(view == attach_left_butt){
                        AttachClick(active_button, false);
                    }else{
                        AttachClick(active_button, true);
                    }
                    break;
                }
            }
            return true;
        }
    };
}
