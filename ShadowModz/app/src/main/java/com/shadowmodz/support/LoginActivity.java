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

    /**
     * Start the gate again for a key change: the dashboard logo sends this, so
     * tapping the logo really does open the login page (field editable, the
     * stored key is not pre-filled).
     */
    public static final String EXTRA_RELOGIN = "shadowmodz_relogin";

    /** stored value: SHA-256( SALT + key ) - the key itself is never written down */
    private static final String SALT       = "SHADOWMODZ::v4::GATE";
    private static final String KEY_HASH   = "ee6370ffb4f3e26df4a4b4f02a74778dfc840cb1903ff312ac31f00b120ac3df";
    private static final String PREF_KEY   = "shadowmodz_activation_key";
    private static final String PREF_REDIR = "shadowmodz_redirected";

    private EditText keyField;
    private TextView status;
    private Button   verifyBtn;
    private boolean  relogin = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        relogin = getIntent() != null && getIntent().getBooleanExtra(EXTRA_RELOGIN, false);

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
                    // "re-enter key" mode: a typed key is always verified, even
                    // when this device is already activated
                    String typed = keyField == null ? "" : keyField.getText().toString().trim();
                    if (relogin && typed.length() > 0) {
                        verify();
                    } else if (isActivated(LoginActivity.this)) {
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

        // long press anywhere on the key field pastes as well
        if (keyField != null)
            keyField.setOnLongClickListener(new View.OnLongClickListener() {
                @Override public boolean onLongClick(View v) { pasteFromClipboard(); return true; }
            });

        if (relogin) {
            // opened from the dashboard logo - let the user paste / type a key again
            setStatus("ENTER YOUR KEY AGAIN  •  TAP VERIFY", 0xFFCB98FE);
            if (verifyBtn != null) verifyBtn.setText("VERIFY KEY");
            if (keyField != null) {
                keyField.setText("");
                keyField.setEnabled(true);
                keyField.requestFocus();
            }
            return;
        }

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
        // the typed text goes through the same clean-up as a paste, so spaces,
        // dashes and lower case can not make a correct key fail
        String input = normalizeKey(keyField == null ? "" : keyField.getText().toString());
        hideKeyboard();

        if (input.length() == 0) {
            fail("ENTER OR PASTE A KEY FIRST");
            return;
        }

        if (constantTimeEquals(sha256(SALT + input), KEY_HASH)) {
            Prefs.with(this).write(PREF_KEY, input);
            setStatus((relogin ? "NEW KEY ACCEPTED  •  " : "KEY ACCEPTED  •  WELCOME TO ")
                      + ShadowBranding.BRAND, 0xFF4ADE80);
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
        Intent i = new Intent(this, MainActivity.class);
        // CLEAR_TOP: arriving here from the dashboard logo leaves exactly one
        // dashboard on the back stack, never two
        i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        startActivity(i);
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

    /**
     * PASTE button: takes whatever is on the clipboard and keeps only the key.
     *
     * People copy "SHADOWMODZ", "@ShadowModz_Bot SHADOWMODZ", a whole Telegram
     * message or a key with spaces/dashes in it - all of those end up as the
     * clean key here, so the button always does something useful.
     */
    private void pasteFromClipboard() {
        String raw = null;
        try {
            ClipboardManager cm = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);
            if (cm != null && cm.hasPrimaryClip()) {
                ClipData clip = cm.getPrimaryClip();
                if (clip != null) {
                    for (int i = 0; i < clip.getItemCount() && raw == null; i++) {
                        ClipData.Item item = clip.getItemAt(i);
                        if (item == null) continue;
                        CharSequence cs = item.coerceToText(this);
                        if (cs != null && cs.length() > 0) raw = cs.toString();
                    }
                }
            }
        } catch (Throwable t) {
            raw = null;
        }

        if (raw == null || raw.trim().length() == 0) {
            setStatus("CLIPBOARD IS EMPTY  •  COPY YOUR KEY FIRST", 0xFFFFC53D);
            return;
        }

        String key = normalizeKey(raw);
        if (key.length() == 0) {
            setStatus("NO KEY IN THE CLIPBOARD TEXT", 0xFFFFC53D);
            return;
        }

        if (keyField != null) {
            keyField.setEnabled(true);
            keyField.setText(key);
            keyField.setSelection(key.length());
            keyField.requestFocus();
        }
        if (verifyBtn != null) verifyBtn.setText(isActivated(this) && !relogin ? "ENTER" : "VERIFY KEY");
        setStatus("KEY PASTED  •  TAP VERIFY", 0xFFCB98FE);
    }

    /** A-Z / 0-9 only, upper case - the same rule the native keypad uses */
    private static String normalizeKey(String raw) {
        if (raw == null) return "";
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < raw.length() && sb.length() < 32; i++) {
            char c = Character.toUpperCase(raw.charAt(i));
            if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) sb.append(c);
        }
        return sb.toString();
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
