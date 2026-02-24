# Planning

A list-oriented GTK ideation and daily planner. Works offline and syncs to a NAS when needed.

Milanote and Trello are the two primary options for planning your goals and tasks. But it's too easy to get lost in the customization and organization just to make your planner pixel perfect. Planning is a hyperfocused alternative that is text-only, not browser-based and works offline so you can ideate and plan without any distractions.

## Install with Nix

If you're on Nix, you're in luck! Just add this to the top of your `flake.nix`:

```nix
planning = {
  url = "github:AmazinAxel/Planning";
  inputs.nixpkgs.follows = "nixpkgs";
};
```

And then include this in your configuration after importing `inputs`:

```nix
environment.systemPackages = [ inputs.planning.packages.${system}.default ];
```

And then rebuild. You can alternatively test out this program by running `nix shell github:AmazinAxel/Planning`

## Sync with a Samba server

This app can upload or download its data from any Samba share.
Make sure you have Samba running and add these options to `~/.config/planning/data.json`:

```json
"smbserver": "alechomelab.local",
"user": "alec",
"group": "WORKGROUP",
"password": "",
"smbshare": "USB",
```

## Broadway web test

I've set up a demo server running Broadway so you can test out this app in your browser. Due to how Broadway functions, the app is windowed like a desktop so you'll need to fullscreen the app.

**The Broadway server is by no means a replacement of the actual application!** You should run the binary locally if you want your data to be saved. Anyone can edit or delete data in the Broadway session. There also seems to be some scrolling bugs and probably other issues which aren't present in the binary.

Access it here: <https://planning.amazinaxel.com>

Android and Linux prebuilt binaries are available in the Releases tab on this repo.

## Run Broadway yourself

Although this kind of defeats the entire purpose of this app being written in native GTK, you can run a Broadway server yourself so you can access the web version of Planning.

I don't suggest this option if you're running Linux/MacOS, Windows, Android or any other OS which natively supports GTK. But since the web version can access a NAS share, this can be a good option if you want to connect all your unsupported devices (e.g. Apple) together.

Open the project dev shell and run

```bash
gtk4-broadwayd :5
GDK_BACKEND=broadway BROADWAY_DISPLAY=:5 gtk4-demo
```

By default the site will be visible at <http://127.0.0.1:8085>

## Tips

Use your arrow keys. Press `ctrl + d` to delete a plan. Press `ctrl +n` (hotkey) to open the popup to create a new plan or list. This app is best used with the GTK4 Nord dark theme.

## Build

I recommend using NixOS for the best development experience. If you want to build on a platform other than Nix, refer to the flake.nix for dependencies.

Enter the Nix devshell: `nix develop`

*You can append `-c 'code .'` to the end of that command to run your IDE in that devshell for Intellisense*

Setup: `meson setup build` and build: `meson compile -C build && ./build/planning`

Note that you must have Samba installed on your system if you want syncing to a NAS to work. Refer to my [NixOS flake](https://github.com/AmazinAxel/flake/blob/main/modules/desktop.nix) for a one line patch to use `smbclient` without running Samba as a service.

## Build for Android

Note that this is experimental and may crash!

This project uses Pixiewood for Android support. You must install Pixiewood from source:

```bash
git clone https://github.com/sp1ritCS/gtk-android-builder.git
cd gtk-android-builder
sudo make install
```

If you're on NixOS, enter the android devshell (`nix-shell androidDevshell.nix`) and then run:

```bash
perl /usr/opt/gtk-android-builder/pixiewood --verbose prepare -s "$ANDROID_SDK_ROOT" -t "$ANDROID_NDK_ROOT"/<version number here like 27.0
.12077973> pixiewood.xml
perl /usr/opt/gtk-android-builder/pixiewood generate

# Pixiewood requires icons for generation so run this entire block to generate some filler icons

RES_DIR=$(find .pixiewood -path "*/app/src/main/res" -type d | head -1)
for density in mdpi hdpi xhdpi xxhdpi xxxhdpi; do
  mkdir -p "$RES_DIR/mipmap-$density"
  printf '\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x01\x00\x00\x00\x01\x08\x02\x00\x00\x00\x90wS\xde\x00\x00\x00\x0cIDATx\x9cc\xa8\x16\x00\x00\x00\x06\x00\x01\xe4\x16\xda\xf2\x00\x00\x00\x00IEND\xaeB`\x82' > "$RES_DIR/mipmap-$density/ic_launcher.png"
done

RES_DIR=".pixiewood/android/app/src/main/res"
for density in mdpi hdpi xhdpi xxhdpi xxxhdpi; do
  cp "$RES_DIR/mipmap-$density/ic_launcher.png" "$RES_DIR/mipmap-$density/ic_launcher_round.png"
done

#---


# This fixes all the build errors
meson configure .pixiewood/bin-aarch64 -Dgtk4:media-gstreamer=disabled
meson configure .pixiewood/bin-x86_64 -Dgtk4:media-gstreamer=disabled

meson configure .pixiewood/bin-aarch64 -Dsigc++-3.0:build-documentation=false
meson configure .pixiewood/bin-x86_64 -Dsigc++-3.0:build-documentation=false
meson configure .pixiewood/bin-aarch64 -Dsigc++-3.0:build-documentation=false -Dsigc++-3.0:build-manual=false


perl /usr/opt/gtk-android-builder/pixiewood build

# Make sure that pixiewood is including all the files (otherwise the app crashes on start)
cp -r subprojects/gtk/gdk/android/glue/java/org .pixiewood/android/app/src/main/java/
mkdir -p .pixiewood/android/app/src/main/jniLibs/arm64-v8a
mkdir -p .pixiewood/android/app/src/main/jniLibs/x86_64
cp .pixiewood/root/lib/arm64-v8a/*.so .pixiewood/android/app/src/main/jniLibs/arm64-v8a/
cp .pixiewood/root/lib/x86_64/*.so .pixiewood/android/app/src/main/jniLibs/x86_64/
cp .pixiewood/bin-x86_64/libplanning.so .pixiewood/android/app/src/main/jniLibs/x86_64/
cp .pixiewood/bin-aarch64/libplanning.so .pixiewood/android/app/src/main/jniLibs/arm64-v8a/

cp $ANDROID_NDK_ROOT/<your version number thing here>/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/x86_64-linux-android/libc++_shared.so .pixiewood/android/app/src/main/jniLibs/x86_64/
cp $ANDROID_NDK_ROOT/<your version number thing here>/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so .pixiewood/android/app/src/main/jniLibs/arm64-v8a/
# Then rebuild again
```

If on NixOS, add `programs.nix-ld.enable = true;` to your config.

Note that the app dynamically adjusts its theme based on the Android dark mode setting. If you want dark mode, enable it in your Android settings!

### Android emulation

```bash
avdmanager create avd -n planningDevice -k "system-images;android-34;google_apis;x86_64" --device "pixel"

emulator -avd planningDevice -cores 4 -memory 4096 -gpu swiftshader_indirect -no-snapshot

# In your IDE:
adb install -r .pixiewood/android/app/build/outputs/apk/debug/app-x86_64-debug.apk && adb shell am start -n com.amazinaxel.planning/org.gtk.android.ToplevelActivity
```

If you're on Wayland, you will need to enable XWayland for your compositor.
