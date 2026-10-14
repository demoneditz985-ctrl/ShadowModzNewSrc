package com.shadowmodz.support;
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

public class exec {
  static public boolean InjectVirtual(String Lib,Context con) {
        try {
            String target = "com.dts.freefireth";
            String injector = con.getApplicationInfo().nativeLibraryDir + File.separator + "libinject.so";
            String payload_source = con.getApplicationInfo().nativeLibraryDir + File.separator + Lib;
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
            int pid = waitForProcess(target, 30000L);
            if (pid <= 0) { lastError = GAME_NOT_FOUND; return false; }
            String command = String.format(Locale.ENGLISH,"%s %d %s", injector, pid, payload_dest);
            RootShell.su(command).exec();
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
	}
    // =====================================================================
    //  NO ROOT PATH
    // ---------------------------------------------------------------------
    //  Inside a virtual space / dual-app container the game runs with the
    //  SAME uid as this app, so ptrace is allowed without root and the
    //  injector can be started as an ordinary process - no "su" anywhere.
    //
    //  Requirements for this path (we can not ship them, they are not part of
    //  this source drop):
    //    * libinject.so     - the ptrace injector, in jniLibs/
    //    * the game run in the same container as this app
    // =====================================================================
    static public boolean InjectDirect(String Lib, Context con) {
        try {
            String target = "com.dts.freefireth";
            String dir      = con.getApplicationInfo().nativeLibraryDir;
            String injector = dir + File.separator + "libinject.so";
            String payload  = dir + File.separator + Lib;

            File inj = new File(injector);
            if (!inj.exists()) { lastError = MISSING_INJECTOR; return false; }
            File pay = new File(payload);
            if (!pay.exists()) { lastError = MISSING_PAYLOAD; return false; }

            int pid = waitForProcess(target, 15000L);
            if (pid <= 0) { lastError = GAME_NOT_FOUND; return false; }

            // the injector prints its own result; a non-zero exit means failure
            Process p = new ProcessBuilder(injector, Integer.toString(pid), payload)
                    .redirectErrorStream(true).start();
            if (!p.waitFor(30, java.util.concurrent.TimeUnit.SECONDS)) { p.destroy(); lastError = TIMEOUT; return false; }
            return p.exitValue() == 0;
        } catch (Throwable e) {
            e.printStackTrace();
            lastError = e.getClass().getSimpleName();
            return false;
        }
    }

    /** wait (bounded) for a process to appear - never loop forever */
    static public int waitForProcess(String pkg, long timeoutMs) {
        long deadline = System.currentTimeMillis() + timeoutMs;
        while (System.currentTimeMillis() < deadline) {
            int pid = Utils.getProcessID(pkg);
            if (pid > 0) return pid;
            try { Thread.sleep(500L); } catch (InterruptedException ignored) { }
        }
        return Utils.getProcessID(pkg);
    }

    /** why the last attempt failed - shown to the user */
    static public String lastError = "";

    public static final String MISSING_INJECTOR = "MISSING libinject.so";
    public static final String MISSING_PAYLOAD  = "MISSING PAYLOAD .so";
    public static final String GAME_NOT_FOUND   = "GAME NOT RUNNING";
    public static final String TIMEOUT          = "INJECTOR TIMED OUT";

    static public boolean InjectRoot(String Lib,Context con) {
        try {
            String target = "com.dts.freefireth";
            String injector = con.getApplicationInfo().nativeLibraryDir + File.separator + "libcatchmeifyoucan.so";
            String payload_source = con.getApplicationInfo().nativeLibraryDir + File.separator + Lib;
            String payload_dest = "/data/local/tmp/"+Lib;
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
            int pid = waitForProcess(target, 30000L);
            if (pid <= 0) { lastError = GAME_NOT_FOUND; return false; }
            String command = String.format(Locale.ENGLISH,"%s %d %s", injector, pid, payload_dest);
            RootShell.su(command).exec();
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
	}
}
