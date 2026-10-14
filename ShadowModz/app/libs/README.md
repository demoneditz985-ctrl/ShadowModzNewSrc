# app/libs

Optional JARs. The project now builds with **no external dependencies** - the
old `com.topjohnwu.superuser` (libsu) requirement was replaced by
`java/com/shadowmodz/support/RootShell.java`.

If you prefer the real libsu, drop its jar here (e.g. `libsu-2.5.0.jar`) and
restore `import com.topjohnwu.superuser.Shell;` in `MainActivity.java`,
`Utils.java` and `exec.java` (replace `RootShell.` with `Shell.`).
