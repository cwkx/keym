--[[
   The MIT License (MIT)
   Copyright (c) 2016 Chris G. Willcocks

   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
--]]

-- Configuration variables, inspired by the APW widget
local keyRange       = {1,3,12,30}
local keySpeed       = 3;
local width          = 45        -- width in pixels of progressbar
local margin_right   = 3         -- right margin in pixels of progressbar
local margin_left    = 3         -- left margin in pixels of progressbar
local margin_top     = 3         -- top margin in pixels of progressbar
local margin_bottom  = 3         -- bottom margin in pixels of progressbar
local color          = '#698f1e' -- foreground color of progessbar
local color_bg       = '#33450f' -- background color
local color_keys     = '#be2a15' -- foreground color when keysd
local color_bg_keys  = '#532a15' -- background color when keysd
local show_text      = true      -- show mouse/keys text
local text_color     = '#fff'    -- color of text

-- End of configuration
local awful = require("awful")
local spawn_with_shell = awful.util.spawn_with_shell or awful.spawn.with_shell
local wibox = require("wibox")
local math = require("math")
local bar = awful.widget.progressbar()
local naughty = require("naughty")
bar:set_width(width)

local function make_stack(w1, w2)
    local ret = wibox.widget.base.make_widget()
    ret.fit = function(self, ...) return w1:fit(...) end
    ret.draw = function(self, wibox, cr, width, height)
        w1:draw(wibox, cr, width, height)
        w2:draw(wibox, cr, width, height)
    end
    local update = function() ret:emit_signal("widget::updated") end
    w1:connect_signal("widget::updated", update)
    w2:connect_signal("widget::updated", update)
    return ret
end

keys = true
local grabber
local widget
local text
local scrlSkip = 0
local keyDelta = {0,0,0,0,0,0,0,0}
local keyTimer = timer({ timeout=0.01})

keyTimer:connect_signal("timeout", function()
  mouse.coords({x=mouse.coords().x+(keyDelta[2]-keyDelta[1])*keySpeed,y=mouse.coords().y+(keyDelta[4]-keyDelta[3])*keySpeed})
  local scrlMod = keySpeed>keyRange[2] and 1 or (keySpeed>keyRange[1] and 3 or 6) -- numbers 1 or 3 here controls scroll skip, change for different scroll speeds
  if (scrlSkip % scrlMod == 0) then
    if (keyDelta[5] ~= 0) then awful.util.spawn_with_shell("xdotool click 4") end
    if (keyDelta[6] ~= 0) then awful.util.spawn_with_shell("xdotool click 5") end
  end
  scrlSkip = scrlSkip + 1
  if (keyDelta[7] ~= 0 or keyDelta[8] ~= 0) then
    awful.tag.incmwfact( 0.008 * (keyDelta[8]-keyDelta[7]))
  end
end)

local mskeysFun = function(mod, key, event)

  if event == "release" then
    if     key == ';' or key == '\\' or key == '/' or key =='z' or key == 'g' or key == '\'' or key =='h' then keySpeed = keyRange[2] bar:set_value(0) text:set_markup('<span color="'..text_color..'">Mouse</span>')
    elseif key == 'w' or key == 'Up'    then keyDelta[3] =  0
    elseif key == 's' or key == 'Down'  then keyDelta[4] =  0
    elseif key == 'a' or key == 'Left'  then keyDelta[1] =  0
    elseif key == 'd' or key == 'Right' then keyDelta[2] =  0
    elseif key == 'j' or key == 'q'     or key == 'Return' then awful.util.spawn_with_shell("xdotool mouseup 1")
    elseif key == 'k' or key == 'e'     then awful.util.spawn_with_shell("xdotool mouseup 3")
    elseif key == 'r' then keyDelta[5] = 0
    elseif key == 'f' then keyDelta[6] = 0
    elseif key == 'n' then keyDelta[7] = 0
    elseif key == 'm' then keyDelta[8] = 0 end
  elseif event == "press" then
    if     key == ';' or key == '\\' or key == '/' then keySpeed = keyRange[3] bar:set_value(0.5) text:set_markup('<span color="'..text_color..'">Fast</span>')
    elseif key == 'g' or key == '\'' or key == 'z' then keySpeed = keyRange[4] bar:set_value(1.0) text:set_markup('<span color="'..text_color..'">Dash</span>')
    elseif key == 'h' then keySpeed = keyRange[1] bar:set_value(0) text:set_markup('<span color="'..text_color..'">Slow</span>')
    elseif key == 'w' or key == 'Up'    then keyDelta[3] =  1
    elseif key == 's' or key == 'Down'  then keyDelta[4] =  1
    elseif key == 'a' or key == 'Left'  then keyDelta[1] =  1
    elseif key == 'd' or key == 'Right' then keyDelta[2] =  1
    elseif key == 'j' or key == 'q' or key == 'Return' then awful.util.spawn_with_shell("xdotool mousedown 1")
    elseif key == 'k' or key == 'e' then awful.util.spawn_with_shell("xdotool mousedown 3")
    elseif key == 'r' then keyDelta[5] = 1
    elseif key == 'f' then keyDelta[6] = 1
    elseif key == '`' then keys=false widget.Toggle()
    elseif key == 'i' then awful.util.spawn_with_shell("xdotool click 2")
    elseif key == 'n' then keyDelta[7] = 1
    elseif key == 'm' then keyDelta[8] = 1 end
  end
end

if show_text then
    text = wibox.widget.textbox()
    text:set_align("center")
    widget = wibox.layout.margin(make_stack(bar, text), margin_right, margin_left, margin_top, margin_bottom)
else
    widget = wibox.layout.margin(bar, margin_right, margin_left, margin_top, margin_bottom)
end

function widget.setColor()
	if (keys) then
		bar:set_color(color_keys)
		bar:set_background_color(color_bg_keys)
	else
		bar:set_color(color)
		bar:set_background_color(color_bg)
	end
end

local function _update()
  widget:setColor()
  if show_text then
      local state_txt = "Mouse"
      if (keys) then
        state_txt = "Keys"
      end
      text:set_markup('<span color="'..text_color..'">'.. state_txt ..'</span>')
  end
end

function widget.Toggle()
  keys = not keys
  if (keys) then
    keyTimer:stop()
    grabber = awful.keygrabber.stop(mskeysFun)
  else
    keyTimer:start()
    grabber = awful.keygrabber.run(mskeysFun)
  end
	_update()
end

-- initialize
_update()

return widget
