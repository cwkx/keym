-- Copyright 2013 Chris G. Willcocks
-- MIT License
--
-- Configuration variables
local keyRange = {3,12,30}
local keySpeed = 3;
local width         = 45        -- width in pixels of progressbar
local margin_right  = 3         -- right margin in pixels of progressbar
local margin_left   = 3         -- left margin in pixels of progressbar
local margin_top    = 3         -- top margin in pixels of progressbar
local margin_bottom = 3         -- bottom margin in pixels of progressbar
local color         = '#698f1e' -- foreground color of progessbar
local color_bg      = '#33450f' -- background color
local color_mute    = '#be2a15' -- foreground color when muted
local color_bg_mute = '#532a15' -- background color when muted
local show_text     = true     -- show percentages on progressbar
local text_color    = '#fff' -- color of text

-- End of configuration
local awful = require("awful")
local spawn_with_shell = awful.util.spawn_with_shell or awful.spawn.with_shell
local wibox = require("wibox")
local math = require("math")
local naughty = require("naughty")

local bar = awful.widget.progressbar()
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

local grabber
local widget
local text
local mute = true
local keyDelta = {0,0,0,0}
local keyTimer = timer({ timeout=0.01})
keyTimer:connect_signal("timeout", function()
  mouse.coords({x=mouse.coords().x+(keyDelta[2]-keyDelta[1])*keySpeed,y=mouse.coords().y+(keyDelta[4]-keyDelta[3])*keySpeed})
end)

local mskeysFun = function(mod, key, event)

  if event == "release" then
    if     key == 'f' or key == ';' or key == 'g' or key == '\'' then keySpeed = keyRange[1];
    elseif key == 'w' or key == 'Up'    then keyDelta[3] =  0
    elseif key == 's' or key == 'Down'  then keyDelta[4] =  0
    elseif key == 'a' or key == 'Left'  then keyDelta[1] =  0
    elseif key == 'd' or key == 'Right' then keyDelta[2] =  0
    elseif key == 'j' then awful.util.spawn_with_shell("xdotool mouseup 1")
    elseif key == 'k' then awful.util.spawn_with_shell("xdotool mouseup 3")
    elseif key == 'h' then awful.util.spawn_with_shell("xdotool click 4")
    elseif key == 'l' then awful.util.spawn_with_shell("xdotool click 5") end
  elseif event == "press" then
    if     key == 'f' or key == ';' then keySpeed = keyRange[2];
    elseif key == 'g' or key == '\'' then keySpeed = keyRange[3]
    elseif key == 'w' or key == 'Up'    then keyDelta[3] =  1
    elseif key == 's' or key == 'Down'  then keyDelta[4] =  1
    elseif key == 'a' or key == 'Left'  then keyDelta[1] =  1
    elseif key == 'd' or key == 'Right' then keyDelta[2] =  1
    elseif key == 'j' then awful.util.spawn_with_shell("xdotool mousedown 1")
    elseif key == 'k' then awful.util.spawn_with_shell("xdotool mousedown 3")
    elseif key == 'h' then awful.util.spawn_with_shell("xdotool click 4")
    elseif key == 'l' then awful.util.spawn_with_shell("xdotool click 5")
    elseif key == '`' then widget.Toggle() end
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
	if (mute) then
		bar:set_color(color_mute)
		bar:set_background_color(color_bg_mute)
	else
		bar:set_color(color)
		bar:set_background_color(color_bg)
	end
end

local function _update()
	-- bar:set_value(100)
  widget:setColor()
  if show_text then
      local state_txt = "Mouse"
      if (mute) then
        state_txt = "Keys"
      end
      text:set_markup('<span color="'..text_color..'">'.. state_txt ..'</span>')
  end
end

function widget.Toggle()
  mute = not mute
  if (mute) then
    keyTimer:stop()
    grabber = awful.keygrabber.stop(mskeysFun)
  else
    keyTimer:start()
    grabber = awful.keygrabber.run(mskeysFun)
  end
	_update()
end

widget:buttons(awful.util.table.join(
		awful.button({ }, 1, widget.Toggle)
	)
)

-- initialize
_update()

return widget
