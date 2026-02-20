# Planning

A list-oriented GTK ideation and daily planner. Works offline and syncs to a NAS when needed.

## Vision

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

Enter the Nix devshell: `NIXPKGS_ALLOW_UNFREE=1 NIXPKGS_ACCEPT_ANDROID_SDK_LICENSE=1 nix develop --impure`

Rrun your IDE here if you want intellisense, e.g. VSCode: `cd ~/Projects/Planning && NIXPKGS_ALLOW_UNFREE=1 NIXPKGS_ACCEPT_ANDROID_SDK_LICENSE=1 nix develop --impure -c 'code .'`

If you're not on Nix, you must install these packages: `gtkmm4 nlohmann_json gcc pkg-config meson ninja`
You must have Samba running if you want syncing to work.

Setup: `meson setup build` and build: `meson compile -C build && ./build/planning`

## Build for Android

This project uses Pixiewood for GTK for Android support. You can install Pixiewood from source:

```bash
git clone https://github.com/sp1ritCS/gtk-android-builder.git
cd gtk-android-builder
sudo make install
```

If you're on NixOS, run those commands in the devshell provided by this flake in this project directory (there's a lot of dependencies!)

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

perl /usr/opt/gtk-android-builder/pixiewood build
```

If you encounter conflicting file errors when preparing, go into the conflicting files and delete the `[provide]` section. You must also enableset `programs.nix-ld.enable = true;` in your Nix config.

Fix icon error:

```bash
# Find the res directory
RES_DIR=$(find .pixiewood -path "*/app/src/main/res" -type d | head -1)
echo "Res dir: $RES_DIR"

# Create mipmap directories and a simple placeholder icon
for density in mdpi hdpi xhdpi xxhdpi xxxhdpi; do
  mkdir -p "$RES_DIR/mipmap-$density"
  # Create a minimal valid PNG (1x1 green pixel)
  printf '\x89PNG\r\n\x1a\n\x00\x00\x00\rIHDR\x00\x00\x00\x01\x00\x00\x00\x01\x08\x02\x00\x00\x00\x90wS\xde\x00\x00\x00\x0cIDATx\x9cc\xa8\x16\x00\x00\x00\x06\x00\x01\xe4\x16\xda\xf2\x00\x00\x00\x00IEND\xaeB`\x82' > "$RES_DIR/mipmap-$density/ic_launcher.png"
done

RES_DIR=".pixiewood/android/app/src/main/res"
for density in mdpi hdpi xhdpi xxhdpi xxxhdpi; do
  cp "$RES_DIR/mipmap-$density/ic_launcher.png" "$RES_DIR/mipmap-$density/ic_launcher_round.png"
done
```

## Tips

Use your arrow keys and press escape to go back. Press `ctrl + d` to delete a plan. This app is best used with the GTK4 Nord theme.
