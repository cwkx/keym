#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

static const int speeds[5] = {80, 400, 1400, 4000, 10000};
static const int scroll[5] = {1000, 5000, 30000, 50000, 100000};
static const char* unmap[] = {"w", "a", "s", "d", "q", "e", "r", "f", "g", "h", "j", "k", "l", "semicolon", "i", "c", "u", "o", "Shift_L", "backslash", "Tab", "Left", "Right", "Up", "Down", "x", "m", "Control_R"};

static Display *display;
static char keymap[32] = {0};

char pressed(int keycode)
{
    KeyCode c = XKeysymToKeycode(display, keycode);
    return (keymap[c / 8] & (1 << c % 8)) > 0;
}

int main()
{
    KeySym *keysyms, *original;
    fd_set in_fds;
    struct timeval tv;
    int x11_fd;
    int num_ready_fds;
    char idle = 1;
    char key_delta[6] = {0}; /* left, right, up, down, scroll up, scroll down */
    char speed = 2;          /* dash, fast, normal, slow, crawl */
    int first_keycode, max_keycode, ks_per_keystroke;
    int num_keycodes;
    int i,j;
    int len = sizeof(unmap)/sizeof(unmap[0]);

    if (!(display = XOpenDisplay(NULL)))
    {
        fprintf(stderr, "Cannot open display.\n");
        return 1;
    }

    x11_fd = ConnectionNumber(display);

    /* get the full keymap and keep a backup of the original to restore when we're done */
    XDisplayKeycodes(display, &first_keycode, &max_keycode);
    num_keycodes = max_keycode - first_keycode + 1;

    original = XGetKeyboardMapping(display, first_keycode, num_keycodes, &ks_per_keystroke);
    keysyms  = XGetKeyboardMapping(display, first_keycode, num_keycodes, &ks_per_keystroke);

    /* unmap a selection of keys */
    for (i=0; i<num_keycodes; ++i)
    {
        if (keysyms[i*ks_per_keystroke] != NoSymbol)
        {
            KeySym ks = keysyms[i*ks_per_keystroke];
            const char* keysym_str = XKeysymToString(ks);

            /* uncomment if you want a list of strings for unmapping
            printf("%s\n", keysym_str);
            fflush(stdout);
             */

            for (j=0; j<len; ++j)
            {
                if (strcmp(keysym_str, unmap[j]) == 0)
                {
                    keysyms[i*ks_per_keystroke] = NoSymbol;
                }
            }
        }
    }

    /* do the unmapping */
    XChangeKeyboardMapping(display, first_keycode, ks_per_keystroke, keysyms, max_keycode-first_keycode);

    while (1)
    {
        FD_ZERO(&in_fds);
        FD_SET(x11_fd, &in_fds);

        tv.tv_sec = 0;
        tv.tv_usec = (key_delta[4] || key_delta[5]) ? scroll[speed] : idle ? 50000 : speeds[speed];
        num_ready_fds = select(x11_fd + 1, &in_fds, NULL, NULL, &tv);

        XQueryKeymap(display, keymap);

        /* mouse movement */
        key_delta[0] = pressed(XK_Left)  || pressed(XK_a);
        key_delta[1] = pressed(XK_Right) || pressed(XK_d);
        key_delta[2] = pressed(XK_Up)    || pressed(XK_w);
        key_delta[3] = pressed(XK_Down)  || pressed(XK_s);

        /* scrolling */
        key_delta[4] = pressed(XK_r);
        key_delta[5] = pressed(XK_f);

        /* speed adjustment from slow to fast */
        speed = 2;
        speed = (pressed(XK_g)) ? 4 : speed;
        speed = (pressed(XK_h) || pressed(XK_backslash) || pressed(XK_Tab)) ? 3 : speed;
        speed = (pressed(XK_l) || pressed(XK_Shift_L)) ? 1 : speed;
        speed = (pressed(XK_semicolon)) ? 0 : speed;

        /* mouse clicks */
        XTestFakeButtonEvent(display, Button1, (pressed(XK_j) || pressed(XK_q)) ? True : False, CurrentTime);
        XTestFakeButtonEvent(display, Button3, (pressed(XK_k) || pressed(XK_e)) ? True : False, CurrentTime);
        XTestFakeButtonEvent(display, Button2, (pressed(XK_i) || pressed(XK_c)) ? True : False, CurrentTime);
        XTestFakeButtonEvent(display, 8, pressed(XK_u) ? True : False, CurrentTime);
        XTestFakeButtonEvent(display, 9, pressed(XK_o) ? True : False, CurrentTime);

        /* exit */
        if (pressed(XK_x) || pressed(XK_m))
        {
            /* restore the original mapping */
            XChangeKeyboardMapping(display, first_keycode, ks_per_keystroke, original, max_keycode-first_keycode);
            
            XCloseDisplay(display);
            return 0;
        }

        /* option to grab whole keyboard focus - this is useful for some applications that try to do their own input handling */
        if (pressed(XK_Control_R))
        {
            XGrabKeyboard(display, XDefaultRootWindow(display), False, GrabModeAsync, GrabModeAsync, CurrentTime);
        }

        idle = 1;
        if (key_delta[0] || key_delta[1] || key_delta[2] || key_delta[3])
        {
            XWarpPointer(display, None, None, 0, 0, 0, 0, (key_delta[1] - key_delta[0]), (key_delta[3] - key_delta[2]));
            XSync(display, False);
            idle = 0;
        }
        if (key_delta[4])
        {
            XTestFakeButtonEvent(display, Button4, True, 1);
            XTestFakeButtonEvent(display, Button4, False, 1);
        }
        else if (key_delta[5])
        {
            XTestFakeButtonEvent(display, Button5, True, 1);
            XTestFakeButtonEvent(display, Button5, False, 1);
        }
    }

    XCloseDisplay(display);
    return 1;
}
