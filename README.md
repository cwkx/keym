# Keym
This is a tool for controlling the mouse with the keyboard. It supports five speed settings with fast scrolling and smooth updates. The previous version (in the commit history) was written as a lua widget for ``awesomewm``, which I used for a couple of years, but I wanted something closer to the metal for use with ``dwm`` or other wms so rewrote this in C for X11.

## Installation
Install either from the makefile or compile with gcc:

```gcc keym.c -lX11 -lXtst -o keym```

## Left-handed usage
Simply execute ``keym`` (e.g. use xbindkeys if you want to launch it via a hotkey, or you can launch it from your window manager). It will grab the keyboard focus and you can use ``WASD`` to then move the cursor, ``Q`` to click, ``E`` to right click, ``R/F`` to scroll, hold ``left shift`` to move quickly, and hold ``\`` to move slowly. I found discrete speed settings more efficient/predictable than acceleration. I didn't want to add a config.h, as its easier to modify all the conditionals in the code, e.g. if you wanted to make a better right-handed version. Exit with ``X``.

## Two-handed usage
Also use ``WASD`` to move, but now use ``J`` to click, ``K`` to right click, hold ``G`` to move very slowly, ``H`` to move reasonably slowly, ``L`` to move quickly, and hold ``;`` to move extremely quickly. Also you can press ``I`` for a mouse middle click, ``U`` for the browser back button and ``O`` for the browser forward button. Exit with ``M`` or ``X``. Please let me know if you find any bugs or have any usability suggestions.