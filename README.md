# Planning

A list-oriented GTK ideation and daily planner. Works offline and syncs to a NAS when needed.

Milanote and Trello are the two primary options for planning your goals and tasks. But it's too easy to get lost in the customization and organization just to make your planner pixel perfect. Planning is a hyperfocused alternative that is text-only, not browser-based and works offline so you can ideate and plan without any distractions.

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

## Build

I recommend using NixOS for the best development experience. If you want to build on a platform other than Nix, refer to the flake.nix for dependencies.

Enter the Nix devshell: `NIXPKGS_ALLOW_UNFREE=1 NIXPKGS_ACCEPT_ANDROID_SDK_LICENSE=1 nix develop --impure`

*You can append `-c 'code .'` to the end of that command to run your IDE in that devshell for Intellisense*

Setup: `meson setup build` and build: `meson compile -C build && ./build/planning`

Note that you must have Samba installed on your system if you want syncing to a NAS to work. Refer to my [NixOS flake](https://github.com/AmazinAxel/flake/blob/main/modules/desktop.nix) for a one line patch to use `smbclient` without running Samba as a service.

## Build for Android

This project uses Pixiewood for Android support. You can install Pixiewood from source:

```bash
git clone https://github.com/sp1ritCS/gtk-android-builder.git
cd gtk-android-builder
sudo make install
```

If you're on NixOS, run those commands in the devshell provided by this flake in this project directory:

```bash
perl /usr/opt/gtk-android-builder/pixiewood --verbose prepare -s "$ANDROID_SDK_ROOT" -t "$ANDROID_NDK_ROOT"/<version number here like 27.0
.12077973> pixiewood.xml
perl /usr/opt/gtk-android-builder/pixiewood generate

# This fixes all the build errors
meson configure .pixiewood/bin-aarch64 -Dgtk4:media-gstreamer=disabled
meson configure .pixiewood/bin-x86_64 -Dgtk4:media-gstreamer=disabled

meson configure .pixiewood/bin-aarch64 -Dsigc++-3.0:build-documentation=false
meson configure .pixiewood/bin-x86_64 -Dsigc++-3.0:build-documentation=false
meson configure .pixiewood/bin-aarch64 -Dsigc++-3.0:build-documentation=false -Dsigc++-3.0:build-manual=false

# Make sure that pixiewood is including all the files (otherwise the app crashes on start)
cp -r subprojects/gtk/gdk/android/glue/java/org .pixiewood/android/app/src/main/java/
mkdir -p .pixiewood/android/app/src/main/jniLibs/arm64-v8a
mkdir -p .pixiewood/android/app/src/main/jniLibs/x86_64
cp .pixiewood/root/lib/arm64-v8a/*.so .pixiewood/android/app/src/main/jniLibs/arm64-v8a/
cp .pixiewood/root/lib/x86_64/*.so .pixiewood/android/app/src/main/jniLibs/x86_64/

perl /usr/opt/gtk-android-builder/pixiewood build
```

If you encounter conflicting file errors when preparing, go into the conflicting files and delete the `[provide]` section. If on NixOS, add `programs.nix-ld.enable = true;` to your config.

### Android emulation

TOOD

```bash
avdmanager create avd -n test_device -k "system-images;android-34;google_apis;x86_64" --device "pixel"
export ANDROID_AVD_HOME=/home/alec/.config/.android/avd
```

## Tips

Use your arrow keys and press escape to go back. Press `ctrl + d` to delete a plan. This app is best used with the GTK4 Nord theme.
