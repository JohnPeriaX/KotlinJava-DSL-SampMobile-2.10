package com.rstarx.hexrays.game.ui.dialog;

import android.app.Activity;
import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.TextView;

import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import androidx.recyclerview.widget.SimpleItemAnimator;

import com.joom.paranoid.Obfuscate;
import com.nvidia.devtech.NvEventQueueActivity;
import com.rstarx.hexrays.R;
import com.rstarx.hexrays.launcher.util.CustomRecyclerView;
import com.rstarx.hexrays.launcher.util.Util;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
@Obfuscate
public class DialogManager {

    private Activity activity;

    public final int DIALOG_STYLE_INPUT;
    public final int DIALOG_STYLE_LIST;
    public final int DIALOG_STYLE_MSGBOX;
    public final int DIALOG_STYLE_PASSWORD;
    public final int DIALOG_STYLE_TABLIST;
    public final int DIALOG_STYLE_TABLIST_HEADERS;

    private String mButtonNegative;
    private String mButtonPositive;
    private String mCaption;
    private int mCurrentDialogId;

    public Animation mAnimBtn;
    private Animation mAnimHide;
    private Animation mAnimShow;

    public String mCurrentInputtext;
    public int mCurrentListitem;

    public ConstraintLayout mMainLayout;
    public View mDialogBody;
    private View mDialogButtonNegative;
    private TextView mDialogButtonNegativeText;
    private View mDialogButtonPositive;
    private TextView mDialogButtonPositiveText;
    private TextView mDialogCaption;
    private EditText mDialogInput;
    private View mDialogInputLayout;

    public View mDialogList;

    public View mDialogListLayout;

    public CustomRecyclerView mDialogListRecycler;

    public int mDialogStyle;

    public final TextView[] mDialogTabField;
    private View mDialogTabListRow;
    private TextView mDialogText;
    private View mDialogTextLayout;

    public DialogAdapter mListAdapter;
    private final ArrayList<String> mRowsList;

    public int[] mTabSizes;
    private String mText;
    public boolean isShow;

    public native void sendDialogResponse(int i, int i2, int i3, byte[] bArr);

    public DialogManager(Activity activity) {
        this.activity = activity;

        ConstraintLayout layout = (ConstraintLayout) activity.getLayoutInflater().inflate(R.layout.dialog, null);
        activity.addContentView(layout, new ConstraintLayout.LayoutParams(-1, -1));

        this.DIALOG_STYLE_MSGBOX = 0;
        this.DIALOG_STYLE_INPUT = 1;
        this.DIALOG_STYLE_LIST = 2;
        this.DIALOG_STYLE_PASSWORD = 3;
        this.DIALOG_STYLE_TABLIST = 4;
        this.DIALOG_STYLE_TABLIST_HEADERS = 5;
        this.mDialogTabField = new TextView[4];
        this.mRowsList = new ArrayList<>();
        this.mCurrentDialogId = 0;
        this.mDialogStyle = 0;
        this.mCaption = "";
        this.mText = "";
        this.mButtonPositive = "";
        this.mButtonNegative = "";
        this.mCurrentListitem = -1;
        this.mCurrentInputtext = "";
        this.mTabSizes = new int[]{0, 0, 0, 0};

        mMainLayout = layout;
        this.mDialogBody = layout.findViewById(R.id.dialog_body);
        this.mDialogTextLayout = layout.findViewById(R.id.dialog_text_layout);
        this.mDialogInputLayout = layout.findViewById(R.id.dialog_input_layout);
        this.mDialogListLayout = layout.findViewById(R.id.dialog_list_layout);
        this.mDialogTabListRow = layout.findViewById(R.id.dialog_tablist_row);
        this.mDialogList = layout.findViewById(R.id.dialog_list);
        this.mDialogCaption = (TextView) layout.findViewById(R.id.dialog_caption);
        this.mDialogText = (TextView) layout.findViewById(R.id.dialog_text);
        this.mDialogInput = (EditText) layout.findViewById(R.id.dialog_input);
        this.mDialogTabField[0] = (TextView) layout.findViewById(R.id.dialog_field1);
        this.mDialogTabField[1] = (TextView) layout.findViewById(R.id.dialog_field2);
        this.mDialogTabField[2] = (TextView) layout.findViewById(R.id.dialog_field3);
        this.mDialogTabField[3] = (TextView) layout.findViewById(R.id.dialog_field4);
        this.mDialogListRecycler = (CustomRecyclerView) layout.findViewById(R.id.dialog_list_recycler);
        this.mDialogButtonPositive = layout.findViewById(R.id.button_positive);
        this.mDialogButtonNegative = layout.findViewById(R.id.button_negative);
        this.mDialogButtonPositiveText = (TextView) layout.findViewById(R.id.button_positive_text);
        this.mDialogButtonNegativeText = (TextView) layout.findViewById(R.id.button_negative_text);

        this.mAnimBtn = AnimationUtils.loadAnimation(this.activity, R.anim.scale);
        this.mAnimShow = AnimationUtils.loadAnimation(this.activity, R.anim.button_show_alpha);
        this.mAnimHide = AnimationUtils.loadAnimation(this.activity, R.anim.button_hide_alpha);

        isShow = false;
        Util.HideLayout(this.mMainLayout, false);
        Util.scaleViewAndChildren(this.activity, this.mMainLayout);
    }

    public void show(int i, int i2, String str, String str2, String str3, String str4) {
        isShow = true;
        this.mCurrentDialogId = i;
        this.mDialogStyle = i2;
        this.mCaption = str;
        this.mText = str2;
        this.mButtonPositive = str3;
        this.mButtonNegative = str4;

        ConstraintLayout.LayoutParams layoutParams = (ConstraintLayout.LayoutParams) this.mDialogTextLayout.getLayoutParams();
        layoutParams.height = -2;
        this.mDialogTextLayout.setLayoutParams(layoutParams);
        FrameLayout.LayoutParams layoutParams2 = (FrameLayout.LayoutParams) this.mMainLayout.getLayoutParams();
        layoutParams2.gravity = 17;
        this.mMainLayout.setLayoutParams(layoutParams2);

        loadDialog();
        Util.ShowLayout(this.mMainLayout, true);
    }

    public void hideWithoutReset() {
        this.mMainLayout.setVisibility(View.GONE);
    }

    public void showWithOldContent() {
        this.mMainLayout.setVisibility(View.VISIBLE);
    }

    public void Hide() {
        if (this.activity.getCurrentFocus() != null) {
            ((InputMethodManager) this.activity.getSystemService(Context.INPUT_METHOD_SERVICE)).hideSoftInputFromWindow(this.activity.getCurrentFocus().getWindowToken(), 0);
            System.out.println("[Hide] closed input");
        }

        isShow = false;
        Util.HideLayout(this.mMainLayout, true);
    }

    public void onHeightChanged(int i) {
        if (mMainLayout.getVisibility() == View.VISIBLE) {
            int i2 = this.mDialogStyle;
            if (i2 != 1 && i2 != 3) {
                return;
            }
            if (i <= 150) {
                ConstraintLayout.LayoutParams layoutParams = (ConstraintLayout.LayoutParams) this.mDialogTextLayout.getLayoutParams();
                layoutParams.height = -2;
                this.mDialogTextLayout.setLayoutParams(layoutParams);
                FrameLayout.LayoutParams layoutParams2 = (FrameLayout.LayoutParams) this.mMainLayout.getLayoutParams();
                layoutParams2.gravity = 17;
                this.mMainLayout.setLayoutParams(layoutParams2);
                return;
            }
            ConstraintLayout.LayoutParams layoutParams3 = (ConstraintLayout.LayoutParams) this.mDialogTextLayout.getLayoutParams();
            layoutParams3.height = (int) Util.scale(this.activity, 104.0f);
            this.mDialogTextLayout.setLayoutParams(layoutParams3);
            FrameLayout.LayoutParams layoutParams4 = (FrameLayout.LayoutParams) this.mMainLayout.getLayoutParams();
            layoutParams4.gravity = 49;
            this.mMainLayout.setLayoutParams(layoutParams4);
        }
    }

    public void SendDialogResponse(int i, int i2, String str) {
        if (i2 == -1) {
            int i3 = this.mDialogStyle;
            if (i3 == 2 || i3 == 4 || i3 == 5) {
                i2 = 0;
            }
        }

        try {
            byte[] Str2 = str.getBytes("windows-874");

            Hide();
            sendDialogResponse(i, DialogManager.this.mCurrentDialogId, i2, Str2);
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
    }

    public void loadDialog() {
        if (this.activity.getCurrentFocus() != null) {
            ((InputMethodManager) this.activity.getSystemService(Context.INPUT_METHOD_SERVICE)).hideSoftInputFromWindow(this.activity.getCurrentFocus().getWindowToken(), 0);
            System.out.println("[loadDialog] closed input");
        }

        clearDialogData();
        loadDialogData();
        loadButtons();
        loadRecycler();
        loadSizes();
    }

    public void clearDialogData() {
        this.mRowsList.clear();
        this.mDialogInput.setText("");
        this.mCurrentListitem = -1;
        this.mCurrentInputtext = "";
        int[] iArr = this.mTabSizes;
        iArr[0] = 0;
        iArr[1] = 0;
        iArr[2] = 0;
        iArr[3] = 0;
    }

    public void loadDialogData() {
        setDialogStyle(this.mDialogStyle);
        this.mDialogCaption.setText(Util.getColoredString(this.mCaption));
        int i = this.mDialogStyle;
        if (i == 0 || i == 1 || i == 3) {
            this.mDialogText.setText(Util.getColoredString(this.mText));
            return;
        }
        if (i != 2) {
            StringBuilder sb = new StringBuilder(this.mText);
            for (int i2 = 0; i2 < sb.length(); i2++) {
                if (sb.charAt(i2) == 9 && i2 < sb.length() - 1) {
                    while (true) {
                        int i3 = i2 + 1;
                        if (sb.charAt(i3) != 9) {
                            break;
                        }
                        sb.deleteCharAt(i3);
                    }
                }
            }
            this.mText = sb.toString();
        }
        this.mCurrentListitem = 0;
        String[] split = this.mText.split("\n");
        for (int i4 = 0; i4 < split.length; i4++) {
            if (i4 == 0 && this.mDialogStyle == 5) {
                String[] split2 = split[i4].split("\t");
                int length = split2.length;
                if (length > 4) {
                    length = 4;
                }
                for (int i5 = 0; i5 < 4; i5++) {
                    if (length <= i5) {
                        this.mDialogTabField[i5].setVisibility(View.GONE);
                    } else {
                        this.mDialogTabField[i5].setVisibility(View.VISIBLE);
                        this.mTabSizes[i5] = Util.getTextWidth(Util.getColoredString(split2[i5]).toString(), this.mDialogTabField[i5].getPaint()) + ((int) Util.scale(this.activity, 32.0f));
                        this.mDialogTabField[i5].setText(Util.getColoredString(split2[i5]));
                    }
                }
            } else {
                if ((i4 == 1 && this.mDialogStyle == 5) || i4 == 0) {
                    String stringWithoutColors = Util.getStringWithoutColors(split[i4]);
                    int i6 = this.mDialogStyle;
                    if (i6 == 5 || i6 == 4) {
                        stringWithoutColors = stringWithoutColors.split("\n")[0];
                    }
                    this.mCurrentInputtext = stringWithoutColors;
                }
                this.mRowsList.add(split[i4]);
            }
        }
    }

    public void loadSizes() {
        ConstraintLayout.LayoutParams layoutParams = (ConstraintLayout.LayoutParams) this.mDialogListLayout.getLayoutParams();
        layoutParams.width = -2;
        this.mDialogListLayout.setLayoutParams(layoutParams);
        ConstraintLayout.LayoutParams layoutParams2 = (ConstraintLayout.LayoutParams) this.mDialogList.getLayoutParams();
        layoutParams2.width = -2;
        this.mDialogList.setLayoutParams(layoutParams2);
        ConstraintLayout.LayoutParams layoutParams3 = (ConstraintLayout.LayoutParams) this.mDialogListRecycler.getLayoutParams();
        layoutParams3.width = -2;
        this.mDialogListRecycler.setLayoutParams(layoutParams3);
        this.mDialogBody.post(new Runnable() {
            public void run() {
                if (DialogManager.this.mDialogBody.getWidth() > DialogManager.this.mDialogList.getWidth()) {
                    if (DialogManager.this.mDialogBody.getWidth() > DialogManager.this.mDialogListLayout.getWidth()) {
                        ConstraintLayout.LayoutParams layoutParams = (ConstraintLayout.LayoutParams) DialogManager.this.mDialogListLayout.getLayoutParams();
                        layoutParams.width = 0;
                        DialogManager.this.mDialogListLayout.setLayoutParams(layoutParams);
                    }
                    ConstraintLayout.LayoutParams layoutParams2 = (ConstraintLayout.LayoutParams) DialogManager.this.mDialogList.getLayoutParams();
                    layoutParams2.width = 0;
                    DialogManager.this.mDialogList.setLayoutParams(layoutParams2);
                    ConstraintLayout.LayoutParams layoutParams3 = (ConstraintLayout.LayoutParams) DialogManager.this.mDialogListRecycler.getLayoutParams();
                    layoutParams3.width = 0;
                    DialogManager.this.mDialogListRecycler.setLayoutParams(layoutParams3);
                    DialogManager.this.mListAdapter.setMatchParent(true);
                } else {
                    DialogManager.this.mListAdapter.setMatchParent(false);
                }
                if (DialogManager.this.mDialogStyle == 5) {
                    DialogManager dialogManager = DialogManager.this;
                    int[] unused = dialogManager.mTabSizes = dialogManager.mListAdapter.mergeTabSizes(DialogManager.this.mTabSizes);
                    for (int i = 0; i < 4; i++) {
                        ViewGroup.LayoutParams layoutParams4 = DialogManager.this.mDialogTabField[i].getLayoutParams();
                        layoutParams4.width = DialogManager.this.mTabSizes[i];
                        DialogManager.this.mDialogTabField[i].setLayoutParams(layoutParams4);
                    }
                }
            }
        });
    }

    public void loadButtons() {
        this.mDialogButtonPositive.setVisibility(View.VISIBLE);
        this.mDialogButtonNegative.setVisibility(View.VISIBLE);
        this.mDialogButtonPositiveText.setText(this.mButtonPositive);
        this.mDialogButtonNegativeText.setText(this.mButtonNegative);
        this.mDialogButtonPositive.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                view.startAnimation(DialogManager.this.mAnimBtn);
                DialogManager dialogManager = DialogManager.this;
                dialogManager.SendDialogResponse(1, dialogManager.mCurrentListitem, DialogManager.this.mCurrentInputtext);
            }
        });
        if (this.mButtonNegative.length() == 0) {
            this.mDialogButtonNegative.setVisibility(View.GONE);
        } else {
            this.mDialogButtonNegative.setOnClickListener(new View.OnClickListener() {
                public void onClick(View view) {
                    view.startAnimation(DialogManager.this.mAnimBtn);
                    DialogManager dialogManager = DialogManager.this;
                    dialogManager.SendDialogResponse(0, dialogManager.mCurrentListitem, DialogManager.this.mCurrentInputtext);
                }
            });
        }
        this.mDialogInput.addTextChangedListener(new TextWatcher() {
            public void afterTextChanged(Editable editable) {
            }

            public void beforeTextChanged(CharSequence charSequence, int i, int i2, int i3) {
            }

            public void onTextChanged(CharSequence charSequence, int i, int i2, int i3) {
                String unused = DialogManager.this.mCurrentInputtext = charSequence.toString();
            }
        });
    }

    public void setDialogStyle(int i) {
        if (i == 0) {
            this.mDialogTextLayout.setVisibility(View.VISIBLE);
            this.mDialogInputLayout.setVisibility(View.GONE);
            this.mDialogListLayout.setVisibility(View.GONE);
        }
        else if (i == 1) {
            this.mDialogTextLayout.setVisibility(View.VISIBLE);
            this.mDialogInputLayout.setVisibility(View.VISIBLE);
            this.mDialogInput.setInputType(524353);
            this.mDialogListLayout.setVisibility(View.GONE);
        }
        else if (i == 2) {
            this.mDialogTextLayout.setVisibility(View.GONE);
            this.mDialogInputLayout.setVisibility(View.GONE);
            this.mDialogListLayout.setVisibility(View.VISIBLE);
            this.mDialogTabListRow.setVisibility(View.GONE);
        }
        else if (i == 3) {
            this.mDialogTextLayout.setVisibility(View.VISIBLE);
            this.mDialogInputLayout.setVisibility(View.VISIBLE);
            this.mDialogInput.setInputType(524417);
            this.mDialogListLayout.setVisibility(View.GONE);
        }
        else if (i == 4) {
            this.mDialogTextLayout.setVisibility(View.GONE);
            this.mDialogInputLayout.setVisibility(View.GONE);
            this.mDialogListLayout.setVisibility(View.VISIBLE);
            this.mDialogTabListRow.setVisibility(View.GONE);
        }
        else if (i == 5) {
            this.mDialogTextLayout.setVisibility(View.GONE);
            this.mDialogInputLayout.setVisibility(View.GONE);
            this.mDialogListLayout.setVisibility(View.VISIBLE);
            this.mDialogTabListRow.setVisibility(View.VISIBLE);
        }
    }

    public void loadRecycler() {
        SimpleItemAnimator simpleItemAnimator = (SimpleItemAnimator) this.mDialogListRecycler.getItemAnimator();
        if (simpleItemAnimator != null) {
            simpleItemAnimator.setSupportsChangeAnimations(false);
            this.mDialogListRecycler.setItemAnimator(simpleItemAnimator);
        }
        this.mDialogListRecycler.setLayoutManager(new LinearLayoutManager(this.activity, RecyclerView.VERTICAL, false));
        ArrayList<String> arrayList = this.mRowsList;
        Activity activity = this.activity;
        Animation animation = this.mAnimShow;
        Animation animation2 = this.mAnimHide;
        Animation animation3 = this.mAnimBtn;
        int i = this.mDialogStyle;
        this.mListAdapter = new DialogAdapter(arrayList, activity, animation, animation2, animation3, i == 4 || i == 5);
        this.mDialogListRecycler.setAdapter(this.mListAdapter);

        this.mListAdapter.setOnClickListener(new DialogAdapter.OnClickListener() {
            public void onClick(int i, String str) {
                if (i != -1) {
                    int unused = DialogManager.this.mCurrentListitem = i;
                    String unused2 = DialogManager.this.mCurrentInputtext = str;
                }
            }
        });
        this.mListAdapter.setOnDoubleClickListener(new DialogAdapter.OnDoubleClickListener() {
            public void onDoubleClick() {
                DialogManager dialogManager = DialogManager.this;
                dialogManager.SendDialogResponse(1, dialogManager.mCurrentListitem, DialogManager.this.mCurrentInputtext);
            }
        });
    }
    
}
