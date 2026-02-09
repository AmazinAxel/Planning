# Planning

A list-oriented GTK ideation and daily planner. Works offline and can back up/pull from a NAS when needed.

## Vision

Milanote and Trello are the two primary competitors for tracking and planning your goals and tasks. But there's a fundamental distraction problem with both. It's easy to get lost in the customization and organization of little details to make your planner pixel perfect. Planning is a hyperfocused alternative that is text-only (no images or mind maps - this is what Obsidian is for), not web based and works offline so you can ideate and plan without any distractions.

## Sync with a Samba server

This app can upload or download its data from any Samba share.
Add these options to `~/.config/planning/data.json`:

```json
"smbserver": "alechomelab.local",
"user": "alec",
"group": "workgroup",
"password": "",
"smbshare": "USB"
```

## Build

Enter the Nix devshell: `nix develop` (run your IDE here if you want intellisense)

Setup: `meson setup build` and build: `meson compile -C build && ./build/planning`

## Tip

Use your arrow keys and press escape to go back. Press `ctrl + d` to delete a plan.
