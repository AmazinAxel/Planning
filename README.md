# Planning

A list-oriented GTK ideation and daily planner. Works offline and syncs to a NAS when needed.

## Vision

Milanote and Trello are the two primary options for planning your goals and tasks. But it's too easy to get lost in the customization and organization just to make your planner pixel perfect. Planning is a hyperfocused alternative that is text-only, not browser-based and works offline so you can ideate and plan without any distractions.

## Sync with a Samba server

This app can upload or download its data from any Samba share.
Make sure you have samba running and add these options to `~/.config/planning/data.json`:

```json
"smbserver": "alechomelab.local",
"user": "alec",
"group": "WORKGROUP",
"password": "",
"smbshare": "USB",
```

## Build

Enter the Nix devshell: `nix develop` (run your IDE here if you want intellisense)

If you're not on Nix, you must install these packages: `gtkmm4 nlohmann_json gcc pkg-config meson ninja`
You must have Samba running if you want syncing to work.

Setup: `meson setup build` and build: `meson compile -C build && ./build/planning`

## Tips

Use your arrow keys and press escape to go back. Press `ctrl + d` to delete a plan. This app is best used with the GTK4 Nord theme.
