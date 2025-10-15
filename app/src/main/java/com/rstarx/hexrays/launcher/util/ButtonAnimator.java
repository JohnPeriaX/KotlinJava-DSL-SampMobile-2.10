package com.rstarx.hexrays.launcher.util;

import android.animation.AnimatorInflater;
import android.animation.AnimatorSet;
import android.content.Context;
import android.view.MotionEvent;
import android.view.View;

import com.rstarx.hexrays.R;

public class ButtonAnimator implements View.OnTouchListener {
    private final Context mContext;
    private final View mViewToReduce;
    private final AnimatorSet reducer;
    private final AnimatorSet regainer;

    public ButtonAnimator(Context context, View view) {
        this.mContext = context;
        this.mViewToReduce = view;
        view.setClickable(true);
        this.reducer = (AnimatorSet) AnimatorInflater.loadAnimator(context, R.animator.reduce_size);
        this.regainer = (AnimatorSet) AnimatorInflater.loadAnimator(context, R.animator.regain_size);
    }

    public boolean onTouch(View view, MotionEvent motionEvent) {
        int action = motionEvent.getAction() & 255;
        if (action == 0) {
            if (this.regainer.isRunning()) {
                this.regainer.end();
            }
            this.reducer.setTarget(this.mViewToReduce);
            this.reducer.start();
            return false;
        } else if (action != 1 && action != 3) {
            return false;
        } else {
            if (this.reducer.isRunning()) {
                this.reducer.end();
            }
            this.regainer.setTarget(this.mViewToReduce);
            this.regainer.start();
            return false;
        }
    }
}
