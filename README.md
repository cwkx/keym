# Keym
I find using a mouse painful, so made this responsive widget to act as a mouse alternative/replacement using the keyboard. The widget has two modes, normal "keys" mode and "mouse" mode. When in mouse mode, all typical keyboard inputs are gobbled up and replaced with a powerful interface for manipulating the "mouse" at different speeds.

## Usage update after 4 months
I've been using this for 4 months now and am very happy with it, there's just a couple of bugs that crop up and i'm not sure what causes them; if anyone else starts using this i'd appreciate help finding these. In general, Keym greatly enhances my general laptop/desktop usage. I've actually found that I end up using it alongside a mouse/trackpad. In particular, being able to control the mouse just with my left hand is very useful so I can still browse the computer while leaning on my right hand etc. Fast scrolling with 'F' and 'R' keys is great.

## Requirements
1. Awesome WM
2. xdotool (not required but installing it gives access to the majority of features)

## Install Instructions
1. clone next to awesome/rc.lua (there should be a folder called "keym" with widget.lua inside next to rc.lua)
2. local keym = require("keym/widget")                     -- put somewhere near top of awesome rc.lua
3. right_layout:add(keym)                                  -- put in your wibox to add visual mode button to rc.lua
4. awful.key({ }, "`", function () keym.Toggle() end),     -- put somewhere in globalKeys to bind it to rc.lua
5. script requires "xdotool" installed for the majority of features: mouse clicks, scrolls etc

## Usage Instructions
1. Press the backtick key ` to toggle keym mode ON/OFF.
2. keym is intended to be flexible to one-handed, or two-handed usage
2. typical two-handed use is: WASD=move, J=click, K=right-click RF=scroll, ;speed1, '=speed2, h=slow
3. left-handed only use is: WASD=move, Q=click, E=right-click, RF=scroll, \=speed1, z=speed2
4. right-handed only use is limited: Arrows=move, Return=click, /=speed1
5. why no vim-mode bindings? moving along diagnols causes finger-collisions, but do otherwise remap

## Roadmap
1. Fix some rare bugs, possibly with xdotool - e.g. it runs slowly in firefox and sometimes locks
2. Send through other main keys (spacebar, backspace, delete)
3. Find otherways to save/load common mouse locations and optimize clicking on nearby buttons etc

Please let me know if you find this useful, or if you find any bugs/have suggestions.
- Chris G. Willcocks
