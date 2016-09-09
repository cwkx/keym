#Keym
I find using a mouse painful, so made this responsive widget to act as a powerful mouse replacement using the keyboard. The widget has two modes, normal "keys" mode and "mouse" mode. When in mouse mode, all typical keyboard inputs are gobbled up and replaced with a powerful interface for manipulating the cursor at different speeds. 

## INSTALL INSTRUCTIONS:
1. create a folder next to awesome/rc.lua called "keym" and extract widget.lua into it
2. local keym = require("keym/widget")                      -- put somewhere near top of awesome rc.lua
3. right_layout:add(mkeys)                                  -- put in your wibox to add visual mode button
4. awful.key({ }, "`", function () mkeys.Toggle() end),     -- put somewhere in globalKeys to bind it
5. script requires "xdotool" installed for the majority of features: mouse clicks, scrolls etc

## USAGE INSTRUCTIONS:
1. keym is intended to be flexible to one-handed, or two-handed usage
2. typical two-handed use is: WASD=move, J=click, K=right-click RF=scroll, ;speed1, '=speed2
3. left-handed only use is: WASD=move, Q=click, E=right-click, RF=scroll, \=speed1, z=speed2
4. right-handed only use is limited: Arrows=move, Return=click, /=speed1
5. why no vim-mode bindings? moving along diagnols causes finger-collisions, but do otherwise remap
