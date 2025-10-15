package com.rstarx.hexrays.game.ui.dialog;

import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;

import com.joom.paranoid.Obfuscate;
import com.rstarx.hexrays.R;
import com.rstarx.hexrays.launcher.util.Util;

import java.util.ArrayList;
@Obfuscate
public class DialogAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {
    private Activity mContext;

    public final Animation buttonHide;
    public final Animation buttonScale;
    public final Animation buttonShow;

    public int mCurrentSelectedPosition = 0;
	
    public View mCurrentSelectedView;
    int mFieldsCount = 1;
    boolean mNeedMatchParent = false;
	
    public OnClickListener mOnClickListener;
	
    public OnDoubleClickListener mOnDoubleClickListener;
    private final ArrayList<String> mRowsList;
    int[] mTabSizes = {0, 0, 0, 0};
    boolean mTablist;
    boolean mbSkip = false;

    public interface OnClickListener {
        void onClick(int i, String str);
    }

    public interface OnDoubleClickListener {
        void onDoubleClick();
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        public TextView[] mFields = new TextView[4];
        public ImageView mRowBg;
        private final View mView;

        public ViewHolder(View view) {
            super(view);
            this.mView = view;
            this.mRowBg = (ImageView) view.findViewById(R.id.item_bg);
            this.mFields[0] = (TextView) view.findViewById(R.id.item_field1);
            this.mFields[1] = (TextView) view.findViewById(R.id.item_field2);
            this.mFields[2] = (TextView) view.findViewById(R.id.item_field3);
            this.mFields[3] = (TextView) view.findViewById(R.id.item_field4);
        }

        public View getMView() {
            return this.mView;
        }
    }

    public DialogAdapter(ArrayList<String> arrayList, Activity activity, Animation animation, Animation animation2, Animation animation3, boolean z) {
        this.mRowsList = arrayList;
        this.mContext = activity;
        this.buttonShow = animation;
        this.buttonHide = animation2;
        this.buttonScale = animation3;
        this.mTablist = z;
    }

    public void setOnDoubleClickListener(OnDoubleClickListener onDoubleClickListener) {
        this.mOnDoubleClickListener = onDoubleClickListener;
    }

    public void setOnClickListener(OnClickListener onClickListener) {
        this.mOnClickListener = onClickListener;
    }

    public void setMatchParent(boolean z) {
        if (this.mNeedMatchParent != z) {
            this.mNeedMatchParent = z;
            notifyDataSetChanged();
        }
    }

    public int[] mergeTabSizes(int[] iArr) {
        for (int i = 0; i < 4; i++) {
            int i2 = iArr[i];
            int[] iArr2 = this.mTabSizes;
            if (i2 > iArr2[i]) {
                iArr2[i] = iArr[i];
            }
        }
        this.mbSkip = true;
        notifyDataSetChanged();
        return this.mTabSizes;
    }

    public void updateSizes(TextView textView) {
        int[] iArr = this.mTabSizes;
        iArr[0] = 0;
        iArr[1] = 0;
        iArr[2] = 0;
        iArr[3] = 0;
        this.mFieldsCount = 1;
        for (int i = 0; i < this.mRowsList.size(); i++) {
            String str = this.mRowsList.get(i);
            if (!this.mTablist) {
                float textWidth = ((float) Util.getTextWidth(Util.getColoredString(str).toString(), textView.getPaint())) + Util.scale(this.mContext, 32.0f);
                int[] iArr2 = this.mTabSizes;
                if (((float) iArr2[0]) < textWidth) {
                    iArr2[0] = (int) textWidth;
                }
            } else {
                String[] split = str.split("\t");
                int length = split.length;
                if (length > 4) {
                    length = 4;
                }
                if (this.mFieldsCount < length) {
                    this.mFieldsCount = length;
                }
                for (int i2 = 0; i2 < length; i2++) {
                    float textWidth2 = ((float) Util.getTextWidth(Util.getColoredString(split[i2]).toString(), textView.getPaint())) + Util.scale(this.mContext, 32.0f);
                    int[] iArr3 = this.mTabSizes;
                    if (((float) iArr3[i2]) < textWidth2) {
                        iArr3[i2] = (int) textWidth2;
                    }
                }
            }
        }
    }

    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup viewGroup, int i) {
        View inflate = LayoutInflater.from(viewGroup.getContext()).inflate(R.layout.dialog_item, viewGroup, false);
        return new ViewHolder(inflate);
    }

    public void onBindViewHolder(RecyclerView.ViewHolder viewHolder, int i) {
        onBindViewHolder((ViewHolder) viewHolder);
    }

    public void onBindViewHolder(ViewHolder viewHolder) {
        if (viewHolder.getAdapterPosition() == 0 && !this.mbSkip) {
            updateSizes(viewHolder.mFields[0]);
        }
        for (int i2 = 0; i2 < 4; i2++) {
            if (this.mFieldsCount <= i2) {
                viewHolder.mFields[i2].setVisibility(View.GONE);
            } else {
                viewHolder.mFields[i2].setVisibility(View.VISIBLE);
                ViewGroup.LayoutParams layoutParams = viewHolder.mFields[i2].getLayoutParams();
                layoutParams.width = this.mTabSizes[i2];
                viewHolder.mFields[i2].setLayoutParams(layoutParams);
                viewHolder.mFields[i2].setText("");
            }
        }
        String str = this.mRowsList.get(viewHolder.getAdapterPosition());
        if (!this.mTablist) {
            viewHolder.mFields[0].setText(Util.getColoredString(str));
        } else {
            String[] split = str.split("\t");
            int length = split.length;
            if (length > 4) {
                length = 4;
            }
            for (int i3 = 0; i3 < 4; i3++) {
                if (length > i3) {
                    viewHolder.mFields[i3].setText(Util.getColoredString(split[i3]));
                }
            }
        }
        if (this.mCurrentSelectedPosition == viewHolder.getAdapterPosition()) {
            this.mCurrentSelectedView = viewHolder.mRowBg;
            viewHolder.mRowBg.setVisibility(View.VISIBLE);
            this.mOnClickListener.onClick(viewHolder.getAdapterPosition(), viewHolder.mFields[0].getText().toString());
        } else {
            viewHolder.mRowBg.setVisibility(View.GONE);
        }
        ViewGroup.LayoutParams layoutParams2 = viewHolder.getMView().getLayoutParams();
        layoutParams2.width = this.mNeedMatchParent ? -1 : -2;
        viewHolder.getMView().setLayoutParams(layoutParams2);

        viewHolder.getMView().setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                if (viewHolder.getAdapterPosition() != DialogAdapter.this.mCurrentSelectedPosition) {
                    if (DialogAdapter.this.mCurrentSelectedView != null) {
                        DialogAdapter.this.mCurrentSelectedView.startAnimation(DialogAdapter.this.buttonHide);
                        DialogAdapter.this.mCurrentSelectedView.setVisibility(View.GONE);
                    }
                    int unused = DialogAdapter.this.mCurrentSelectedPosition = viewHolder.getAdapterPosition();
                    View unused2 = DialogAdapter.this.mCurrentSelectedView = viewHolder.mRowBg;
                    viewHolder.mRowBg.startAnimation(DialogAdapter.this.buttonShow);
                    viewHolder.mRowBg.setVisibility(View.VISIBLE);
                    DialogAdapter.this.mOnClickListener.onClick(viewHolder.getAdapterPosition(), viewHolder.mFields[0].getText().toString());
                }
                else if (DialogAdapter.this.mOnDoubleClickListener != null) {
                    view.startAnimation(DialogAdapter.this.buttonScale);
                    DialogAdapter.this.mOnDoubleClickListener.onDoubleClick();
                }
            }
        });
    }

    public int getItemCount() {
        return this.mRowsList.size();
    }
}
