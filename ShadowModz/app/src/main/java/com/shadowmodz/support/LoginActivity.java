package com.shadowmodz.support;

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Vibrator;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.security.MessageDigest;

/**
 * SHADOW MODZ - activation gate.
 *
 * Flow:
 *   1. the user pastes (or types) an activation key
 *   2. VERIFY hashes it and compares against the stored hash
 *   3. on success the key is remembered and the user is redirected to the
 *      SHADOW MODZ Telegram channel, then the injector screen is unlocked
 *
 * Telegram : https://t.me/+BBimnHMiSvpiYTBl
 */
public class LoginActivity extends Activity {

    /** stored value: SHA-256( SALT + key ) - the key itself is never written down */
    private static final String SALT       = "SHADOWMODZ::v4::GATE";
    private static final String KEY_HASH   = "ee6370ffb4f3e26df4a4b4f02a74778dfc840cb1903ff312ac31f00b120ac3df";
    private static final String PREF_KEY   = "shadowmodz_activation_key";
    private static final String PREF_REDIR = "shadowmodz_redirected";

    private EditText keyField;
    private TextView status;
    private Button   verifyBtn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        ImageView logo = (ImageView) findViewById(R.id.logo);
        if (logo != null && ShadowBranding.logo() != null)
            logo.setImageBitmap(ShadowBranding.logo());

        TextView brand   = (TextView) findViewById(R.id.brand);
        TextView version = (TextView) findViewById(R.id.version);
        TextView tg      = (TextView) findViewById(R.id.telegram);
        if (brand != null)   brand.setText(ShadowBranding.BRAND);
        if (version != null) version.setText(ShadowBranding.VERSION);
        if (tg != null)      tg.setText("TELEGRAM  " + ShadowBranding.TELEGRAM_TEXT);

        keyField  = (EditText) findViewById(R.id.keyField);
        status    = (TextView) findViewById(R.id.status);
        verifyBtn = (Button)   findViewById(R.id.verifyBtn);

        Button pasteBtn = (Button) findViewById(R.id.pasteBtn);
        Button getKeyBtn = (Button) findViewById(R.id.getKeyBtn);

        if (pasteBtn != null)
            pasteBtn.setOnClickListener(new View.OnClickListener() {
                @Override public void onClick(View v) { pasteFromClipboard(); }
            });

        if (getKeyBtn != null)
            getKeyBtn.setOnClickListener(new View.OnClickListener() {
                @Override public void onClick(View v) {
                    openTelegram();
                }
            });

        if (verifyBtn != null)
            verifyBtn.setOnClickListener(new View.OnClickListener() {
                @Override public void onClick(View v) {
                    if (isActivated(LoginActivity.this)) {
                        enterDashboard();
                    } else {
                        verify();
                    }
                }
            });

        if (tg != null)
            tg.setOnClickListener(new View.OnClickListener() {
                @Override public void onClick(View v) { openTelegram(); }
            });

        // already activated in an earlier session -> straight to the button state
        if (isActivated(this)) {
            setStatus("ACCESS GRANTED  •  " + ShadowBranding.BRAND, 0xFF4ADE80);
            verifyBtn.setText("ENTER");
            if (keyField != null) {
                keyField.setText(Prefs.with(this).read(PREF_KEY, ""));
                keyField.setEnabled(false);
            }
        }
    }

    // ------------------------------------------------------------------ gate

    /** true when a valid key has already been stored on this device */
    public static boolean isActivated(Context ctx) {
        String stored = Prefs.with(ctx).read(PREF_KEY, "");
        return stored.length() > 0 && constantTimeEquals(sha256(SALT + stored), KEY_HASH);
    }

    private void verify() {
        String input = keyField == null ? "" : keyField.getText().toString().trim();
        hideKeyboard();

        if (input.length() == 0) {
            fail("ENTER OR PASTE A KEY FIRST");
            return;
        }

        if (constantTimeEquals(sha256(SALT + input.toUpperCase()), KEY_HASH)) {
            Prefs.with(this).write(PREF_KEY, input.toUpperCase());
            setStatus("KEY ACCEPTED  •  WELCOME TO " + ShadowBranding.BRAND, 0xFF4ADE80);
            if (verifyBtn != null) verifyBtn.setText("ENTER");
            if (keyField != null) keyField.setEnabled(false);
            Toast.makeText(this, "ACTIVATED - JOIN THE TELEGRAM CHANNEL", Toast.LENGTH_LONG).show();

            // first activation: redirect to the Telegram channel
            if (!"1".equals(Prefs.with(this).read(PREF_REDIR, "0"))) {
                Prefs.with(this).write(PREF_REDIR, "1");
                openTelegram();
            } else {
                enterDashboard();
            }
        } else {
            fail("INVALID KEY  •  GET A KEY FROM TELEGRAM");
        }
    }

    private void enterDashboard() {
        startActivity(new Intent(this, MainActivity.class));
        finish();
    }

    private void fail(String message) {
        setStatus(message, 0xFFFF4D6A);
        shake();
        try {
            Vibrator vib = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
            if (vib != null) vib.vibrate(120);
        } catch (Throwable ignored) { }
    }

    // ------------------------------------------------------------------ ui

    private void setStatus(String text, int color) {
        if (status == null) return;
        status.setText(text);
        status.setTextColor(color);
        status.setVisibility(View.VISIBLE);
    }

    private void shake() {
        if (keyField == null) return;
        try {
            Animation anim = AnimationUtils.loadAnimation(this, R.anim.shake);
            keyField.startAnimation(anim);
        } catch (Throwable ignored) { }
    }

    private void hideKeyboard() {
        try {
            InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
            if (imm != null && keyField != null) imm.hideSoftInputFromWindow(keyField.getWindowToken(), 0);
        } catch (Throwable ignored) { }
    }

    private void pasteFromClipboard() {
        try {
            ClipboardManager cm = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);
            CharSequence clip = "";
            if (cm != null && cm.hasPrimaryClip() && cm.getPrimaryClip() != null) {
                ClipData.Item item = cm.getPrimaryClip().getItemAt(0);
                if (item != null && item.getText() != null) clip = item.getText();
            }
            if (clip != null && clip.length() > 0) {
                if (keyField != null) keyField.setText(clip.toString().trim());
                setStatus("KEY PASTED  •  TAP VERIFY", 0xFFCB98FE);
            } else {
                setStatus("CLIPBOARD IS EMPTY", 0xFFFFC53D);
            }
        } catch (Throwable t) {
            setStatus("PASTE FAILED - TYPE THE KEY MANUALLY", 0xFFFFC53D);
        }
    }

    /** opens the SHADOW MODZ Telegram channel (falls back to the clipboard) */
    private void openTelegram() {
        try {
            Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(ShadowBranding.TELEGRAM_URL));
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            startActivity(intent);
        } catch (ActivityNotFoundException e) {
            try {
                ClipboardManager cm = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);
                if (cm != null) cm.setPrimaryClip(ClipData.newPlainText("SHADOW MODZ", ShadowBranding.TELEGRAM_URL));
                setStatus("LINK COPIED  •  " + ShadowBranding.TELEGRAM_TEXT, 0xFFCB98FE);
            } catch (Throwable ignored) { }
        }
    }

    // ------------------------------------------------------------------ crypto

    private static String sha256(String value) {
        try {
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            byte[] bytes = digest.digest(value.getBytes("UTF-8"));
            StringBuilder sb = new StringBuilder(bytes.length * 2);
            for (byte b : bytes) {
                sb.append(Character.forDigit((b >> 4) & 0xF, 16));
                sb.append(Character.forDigit(b & 0xF, 16));
            }
            return sb.toString();
        } catch (Exception e) {
            return "";
        }
    }

    private static boolean constantTimeEquals(String a, String b) {
        if (a == null || b == null || a.length() != b.length()) return false;
        int diff = 0;
        for (int i = 0; i < a.length(); i++) diff |= a.charAt(i) ^ b.charAt(i);
        return diff == 0;
    }
}
