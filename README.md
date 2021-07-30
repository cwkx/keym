# Keym
This is a tool for controlling the mouse with the keyboard. It supports four speed settings with fast scrolling with smooth updates. The previous version (in the commit history) was written as a lua widget for ``awesomewm``, which I used for a couple of years, but I wanted something closer to the metal for use with ``dwm`` or other wms so rewrote this in C for X11.

## Left-handed usage
Simply execute ``keym`` (e.g. use xbindkeys if you want to launch it via a hotkey, or you can launch it from your window manager). It will grab the keyboard focus and you can use WASD to then move the cursor, Q to click, E to right click, R/F to scroll, and left shift to move quickly. I found discrete speed settings more efficient/predictable than acceleration. I didn't want to add a config.h, as its easier to modify all the conditionals in the code, e.g. if you wanted to make a better right-handed version. Exit with X.

## Two-handed usage
Also use WASD to move, but now use J to click, K to right click, G to move very slowly, H to move reasonably slowly, L to move quickly, ; to move extremely quickly. Exit with M or X.

## Requirements
This only requires linkage with X11 and Xtst. CPU usage is based on the speed setting, e.g. faster movement requires more updates. The key events could be optimised further but its fine for my needs.

Please let me know if you find any bugs or have usability suggestions. Code is MIT licensed.
- Chris G. Willcocks
