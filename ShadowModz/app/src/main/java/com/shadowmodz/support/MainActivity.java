package com.shadowmodz.support;

import android.app.*;
import android.os.*;
import android.content.*;
import android.provider.*;
import android.net.*;
import android.widget.*;
import android.content.res.*;
import java.io.*;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings; 
import android.widget.Toast;
import java.util.ArrayList;
import java.util.List;
import java.io.File;
import android.speech.tts.TextToSpeech;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;
import android.view.View;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.net.URLDecoder;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
public class MainActivity extends Activity {

    /** payload injected into the game - built from jni/Android.mk (LOCAL_MODULE := libmasgik) */
    public static final String PAYLOAD_LIB = "libmasgik.so";

    private android.widget.TextView statusView;
    private android.widget.Button actionButton;
    private Intent makabasa = new Intent();
	static {
        // the native module name (Android.mk LOCAL_MODULE) must match the
        // library injected here: libshadowmodz.so
        // Both must have same name
        //System.loadLibrary("MyLibName");
    }

    // SHADOW MODZ dashboard: unlock with a key, then inject libmasgik into the game
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // 1. activation gate - nothing works without a valid key
        if (!LoginActivity.isActivated(this)) {
            startActivity(new Intent(this, LoginActivity.class));
            finish();
            return;
        }

        setContentView(R.layout.activity_main);

        android.widget.ImageView logo = (android.widget.ImageView) findViewById(R.id.logo);
        if (logo != null && ShadowBranding.logo() != null)
            logo.setImageBitmap(ShadowBranding.logo());

        statusView = (android.widget.TextView) findViewById(R.id.status);
        actionButton = (android.widget.Button) findViewById(R.id.actionButton);
        setStatus("KEY OK  •  READY TO INJECT", 0xFF4ADE80);

        android.widget.TextView tg = (android.widget.TextView) findViewById(R.id.telegram);
        if (tg != null) {
            tg.setText("TELEGRAM  " + ShadowBranding.TELEGRAM_TEXT);
            tg.setOnClickListener(new View.OnClickListener() {
                @Override public void onClick(View v) { openTelegram(); }
            });
        }

        if (actionButton != null) {
            actionButton.setOnClickListener(new View.OnClickListener() {
                @Override public void onClick(View v) { startInjection(); }
            });
        }
    }

    /** launches Free Fire, waits for its process and injects the payload */
    private void startInjection() {
        final String target = "com.dts.freefireth";
        if (actionButton != null) actionButton.setEnabled(false);
        setStatus("WAITING FOR " + target + " ...", 0xFFCB98FE);

        new Thread(new Runnable() {
            @Override public void run() {
                final String result = doInject(target);
                runOnUiThread(new Runnable() {
                    @Override public void run() {
                        if (actionButton != null) actionButton.setEnabled(true);
                        setStatus(result, result.startsWith("INJECT")
                                ? 0xFF4ADE80 : 0xFFFF4D6A);
                        Toast.makeText(getApplicationContext(), result,
                                result.startsWith("INJECT") ? Toast.LENGTH_LONG : Toast.LENGTH_LONG).show();
                    }
                });
            }
        }).start();
    }

    private String doInject(String target) {
        try {
            // start the game if it is not running yet
            if (Utils.getProcessID(target) <= 0) {
                Intent launch = getPackageManager().getLaunchIntentForPackage(target);
                if (launch != null) {
                    launch.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                    startActivity(launch);
                }
            }

            // wait (bounded) for the game process to appear
            long deadline = System.currentTimeMillis() + 120000L;
            while (Utils.getProcessID(target) <= 0 && System.currentTimeMillis() < deadline) {
                try { Thread.sleep(1000L); } catch (InterruptedException ignored) { }
            }
            if (Utils.getProcessID(target) <= 0)
                return "GAME NOT RUNNING  •  OPEN FREEFIRE AND RETRY";

            OnUiThread("INJECTING " + PAYLOAD_LIB + " ...");

            // root path first, then the virtual (no root) path
            boolean ok = InjectRoot(PAYLOAD_LIB);
            if (!ok) ok = exec.InjectRoot(PAYLOAD_LIB, this);
            if (!ok) ok = exec.InjectVirtual(PAYLOAD_LIB, this);

            if (ok) {
                DeletedCMODs(PAYLOAD_LIB);
                return "INJECT SUCCESS  •  " + PAYLOAD_LIB;
            }
            return "INJECT FAILED  •  NEED ROOT (or missing injector .so)";
        } catch (Throwable e) {
            e.printStackTrace();
            return "INJECT FAILED  •  " + e.getClass().getSimpleName();
        }
    }

    private void OnUiThread(final String message) {
        runOnUiThread(new Runnable() { @Override public void run() { setStatus(message, 0xFFCB98FE); } });
    }

    private void setStatus(String message, int color) {
        if (statusView == null) return;
        statusView.setText(message);
        statusView.setTextColor(color);
        statusView.setVisibility(View.VISIBLE);
    }

    private void openTelegram() {
        try {
            Intent intent = new Intent(Intent.ACTION_VIEW,
                    Uri.parse(ShadowBranding.TELEGRAM_URL));
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            startActivity(intent);
        } catch (Exception e) {
            Toast.makeText(this, ShadowBranding.TELEGRAM_TEXT, Toast.LENGTH_LONG).show();
        }
    }

    private boolean InjectRoot(String Lib) {
        try {
            String target = "com.dts.freefireth";
            String injector = this.getApplicationInfo().nativeLibraryDir + File.separator + "libcatchmeifyoucan.so"; // add your injector .so to jniLibs
            String payload_source = this.getApplicationInfo().nativeLibraryDir + File.separator + Lib;
            String payload_dest = "/dev/"+Lib;
            String context = "u:object_r:system_lib_file:s0";
            List<String> STDOUT = new ArrayList<>();
            RootShell.su("ls -lZ /system/lib/libandroid_runtime.so").to(STDOUT).exec();
            for (String line : STDOUT) {
                if (line.contains(" u:object_r:") && line.contains(":s0 ")) {
                    context = line.substring(line.indexOf("u:object_r:"));
                    context = context.substring(0, context.indexOf(' '));
                }
            }
            RootShell.su("cp " + payload_source + " " + payload_dest).exec();
            RootShell.su("chmod 777 " + payload_dest).exec();
            RootShell.su("chcon " + context + " " + payload_dest).exec();
            while (Utils.getProcessID(target) <= 0) {}
            Thread.sleep(1000);
            int pid = Utils.getProcessID(target);
          //  writeFile("/storage/emulated/0/getPid.txt",Integer.toString(pid));
           // Toast.makeText(this, pid, Toast.LENGTH_LONG).show();
            String command = String.format(Locale.ENGLISH,"%s %d %s", injector, pid, payload_dest);
            RootShell.su(command).exec();
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            Toast.makeText(getApplicationContext(), "INJECT FAILED PLEASE ROOT", Toast.LENGTH_LONG).show();
            return false;
        }
	}	
	public void DeletedCMODs(String remv) {
        cmods_exec("rm -rf " + "/dev/" + remv);      
        
        Toast.makeText(getApplicationContext(), "BYPASS SUCCESS", Toast.LENGTH_LONG).show();
        
    }
    public static int cmods_exec(String cmd) {
        int result = -1;
        DataOutputStream dos = null;

        try {
            java.lang.Process p = Runtime.getRuntime().exec("su");
            dos = new DataOutputStream(p.getOutputStream());
            dos.writeBytes(cmd + "\n");
            dos.flush();
            dos.writeBytes("exit\n");
            dos.flush();
            p.waitFor();
            result = p.exitValue();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (dos != null) {
                try {
                    dos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return result;
    }
    private static void createNewFile(String path) {
        int lastSep = path.lastIndexOf(File.separator);
        if (lastSep > 0) {
            String dirPath = path.substring(0, lastSep);
            makeDir(dirPath);
        }

        File file = new File(path);

        try {
            if (!file.exists()) file.createNewFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public static String readFile(String path) {
        createNewFile(path);

        StringBuilder sb = new StringBuilder();
        FileReader fr = null;
        try {
            fr = new FileReader(new File(path));

            char[] buff = new char[1024];
            int length = 0;

            while ((length = fr.read(buff)) > 0) {
                sb.append(new String(buff, 0, length));
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (fr != null) {
                try {
                    fr.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }

        return sb.toString();
    }
    public static void writeFile(String path, String str) {
        createNewFile(path);
        FileWriter fileWriter = null;

        try {
            fileWriter = new FileWriter(new File(path), false);
            fileWriter.write(str);
            fileWriter.flush();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (fileWriter != null)
                    fileWriter.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static void copyFile(String sourcePath, String destPath) {
        if (!isExistFile(sourcePath)) return;
        createNewFile(destPath);

        FileInputStream fis = null;
        FileOutputStream fos = null;

        try {
            fis = new FileInputStream(sourcePath);
            fos = new FileOutputStream(destPath, false);

            byte[] buff = new byte[1024];
            int length = 0;

            while ((length = fis.read(buff)) > 0) {
                fos.write(buff, 0, length);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (fis != null) {
                try {
                    fis.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (fos != null) {
                try {
                    fos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static void copyDir(String oldPath, String newPath) {
        File oldFile = new File(oldPath);
        File[] files = oldFile.listFiles();
        File newFile = new File(newPath);
        if (!newFile.exists()) {
            newFile.mkdirs();
        }
        for (File file : files) {
            if (file.isFile()) {
                copyFile(file.getPath(), newPath + "/" + file.getName());
            } else if (file.isDirectory()) {
                copyDir(file.getPath(), newPath + "/" + file.getName());
            }
        }
    }

    public static void moveFile(String sourcePath, String destPath) {
        copyFile(sourcePath, destPath);
        deleteRecursive(sourcePath);
    }

    public static void deleteRecursive(String path) {
        File file = new File(path);

        if (!file.exists()) return;

        if (file.isFile()) {
            file.delete();
            return;
        }

        File[] fileArr = file.listFiles();

        if (fileArr != null) {
            for (File subFile : fileArr) {
                if (subFile.isDirectory()) {
                    deleteRecursive(subFile.getAbsolutePath());
                }

                if (subFile.isFile()) {
                    subFile.delete();
                }
            }
        }

        file.delete();
    }

    public static boolean isExistFile(String path) {
        File file = new File(path);
        return file.exists();
    }

    public static void makeDir(String path) {
        if (!isExistFile(path)) {
            File file = new File(path);
            file.mkdirs();
        }
    }

    public static void listDir(String path, ArrayList<String> list) {
        File dir = new File(path);
        if (!dir.exists() || dir.isFile()) return;

        File[] listFiles = dir.listFiles();
        if (listFiles == null || listFiles.length <= 0) return;

        if (list == null) return;
        list.clear();
        for (File file : listFiles) {
            list.add(file.getAbsolutePath());
        }
    }

    public static boolean isDirectory(String path) {
        if (!isExistFile(path)) return false;
        return new File(path).isDirectory();
    }
} 
