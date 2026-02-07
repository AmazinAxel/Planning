# Planning

A list-oriented GTK ideation and daily planner. Works offline and can back up/pull from a NAS when needed.

## Vision

Milanote and Trello are the two primary competitors for tracking and planning your goals and tasks. But there's a fundamental distraction problem with both. It's easy to get lost in the customization and organization of little details to make your planner pixel perfect. Planning is a hyperfocused alternative that is text-only (no images or mind maps - this is what Obsidian is for), not web based and works offline so you can ideate and plan without any distractions.

## Build

Enter the Nix devshell: `nix develop` (run your IDE here if you want intellisense)

Setup: `meson setup build` and build: `meson compile -C build && ./build/planning`
