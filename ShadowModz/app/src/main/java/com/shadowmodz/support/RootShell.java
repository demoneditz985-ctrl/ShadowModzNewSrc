package com.shadowmodz.support;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

/**
 * SHADOW MODZ - root shell helper.
 *
 * The original source depended on an external libsu jar
 * (com.topjohnwu.superuser) that was never part of the source drop, so a clean
 * checkout could not compile. This class implements the small slice of that API
 * the injector actually used, with no external dependency:
 *
 *     RootShell.su("cp a b").to(lines).exec()
 *     RootShell.sh("ps -A").exec().getOut()
 *     RootShell.rootAccess()
 *
 * If you prefer the real library, drop its jar into app/libs/ and change these
 * three files back to "import com.topjohnwu.superuser.Shell;".
 */
public final class RootShell {

    private RootShell() { }

    /** result of a shell invocation */
    public static final class Result {
        private final List<String> out;
        private final List<String> err;
        private final int code;

        Result(List<String> out, List<String> err, int code) {
            this.out = out;
            this.err = err;
            this.code = code;
        }

        /** stdout only - never polluted with stderr or synthetic lines */
        public List<String> getOut() { return out; }
        /** stderr, kept separate so parsers of getOut() stay safe */
        public List<String> getErr() { return err; }
        public int getCode() { return code; }
        public boolean isSuccess() { return code == 0; }
    }

    /** fluent command builder: su("cmd").to(list).exec() */
    public static final class Command {
        private final boolean root;
        private final String[] commands;
        private List<String> sink;

        Command(boolean root, String[] commands) {
            this.root = root;
            this.commands = commands;
        }

        /** collect stdout into this list (like libsu's .to(list)) */
        public Command to(List<String> target) {
            this.sink = target;
            return this;
        }

        public Result exec() {
            Result result = run(root, commands);
            if (sink != null) sink.addAll(result.getOut());
            return result;
        }
    }

    /** queue commands for a root shell */
    public static Command su(String... commands) {
        return new Command(true, commands);
    }

    /** queue commands for a normal shell */
    public static Command sh(String... commands) {
        return new Command(false, commands);
    }

    /**
     * true when a root shell is available. Checks for the su binary instead of
     * running it, so opening the app never triggers a superuser prompt.
     */
    public static boolean rootAccess() {
        final String[] dirs = {
                "/system/bin", "/system/xbin", "/sbin", "/su/bin",
                "/system/sbin", "/vendor/bin", "/system/bin/failsafe"
        };
        try {
            for (String dir : dirs) {
                File su = new File(dir, "su");
                if (su.exists() && su.canExecute()) return true;
            }
        } catch (Throwable ignored) { }
        return false;
    }

    private static Result run(boolean root, String[] commands) {
        final List<String> lines = new ArrayList<String>();
        final List<String> errors = new ArrayList<String>();
        int code = -1;
        Process process = null;
        DataOutputStream stdin = null;
        BufferedReader stdout = null;
        Thread drainer = null;

        try {
            ProcessBuilder builder = new ProcessBuilder(root ? "su" : "sh");
            process = builder.start();

            // stderr is drained on its own thread: the child can never block on a
            // full pipe and stdout stays clean for the callers that parse it
            final Process proc = process;
            drainer = new Thread(new Runnable() {
                @Override public void run() {
                    BufferedReader er = null;
                    try {
                        er = new BufferedReader(new InputStreamReader(proc.getErrorStream()));
                        String l;
                        while ((l = er.readLine()) != null) errors.add(l);
                    } catch (Throwable ignored) {
                    } finally {
                        try { if (er != null) er.close(); } catch (Throwable ignored) { }
                    }
                }
            });
            drainer.setDaemon(true);
            drainer.start();

            stdin = new DataOutputStream(process.getOutputStream());
            for (String cmd : commands) {
                if (cmd == null) continue;
                stdin.writeBytes(cmd + "\n");
                stdin.flush();
            }
            stdin.writeBytes("exit\n");
            stdin.flush();
            stdin.close();
            stdin = null;

            stdout = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            while ((line = stdout.readLine()) != null) lines.add(line);

            code = process.waitFor();
        } catch (Throwable t) {
            // no root / su denied / binary missing: report through stderr, never
            // through getOut(), so callers parsing stdout cannot break
            errors.add(t.getClass().getSimpleName()
                    + (t.getMessage() != null ? ": " + t.getMessage() : ""));
        } finally {
            try { if (stdin != null) stdin.close(); } catch (Throwable ignored) { }
            try { if (stdout != null) stdout.close(); } catch (Throwable ignored) { }
            try { if (drainer != null) drainer.join(300); } catch (Throwable ignored) { }
            try { if (process != null) process.destroy(); } catch (Throwable ignored) { }
        }

        return new Result(lines, errors, code);
    }
}