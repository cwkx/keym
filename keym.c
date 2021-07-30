#include <stdio.h>
#include <sys/select.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

static const int speeds[5] = {80, 400, 1400, 4000, 10000};
static const int scroll[5] = {1000, 5000, 30000, 50000, 100000};

static Display *display;
static char keymap[32] = { 0 };

char pressed(int keycode)
{
    KeyCode c = XKeysymToKeycode(display, keycode);
	return ( keymap[c/8] & (1 << c%8) ) > 0;
}

int main()
{
    Window window;
    XEvent e;
    KeySym k;
    fd_set in_fds;
    struct timeval tv;
    int screen;
    int x11_fd;
    int num_ready_fds;
    char key_delta[6] = { 0 }; /* left, right, up, down, scroll up, scroll down */
    char speed = 2; /* dash, fast, normal, slow, crawl */
    char idle = 1;

    if (!(display = XOpenDisplay(NULL)))
    {
        fprintf(stderr, "Cannot open display.\n");
        return 1;
    }

    window = XDefaultRootWindow(display);
    screen = DefaultScreen(display);

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    x11_fd = ConnectionNumber(display);

    while (1)
    {
        FD_ZERO(&in_fds);
        FD_SET(x11_fd, &in_fds);

        tv.tv_sec = 0;
        tv.tv_usec = (key_delta[4] || key_delta[5]) ? scroll[speed] : idle ? 50000 : speeds[speed];

        num_ready_fds = select(x11_fd + 1, &in_fds, NULL, NULL, &tv);

        XGrabKeyboard(display, window, False, GrabModeAsync, GrabModeAsync, CurrentTime);

        while (XPending(display))
            XNextEvent(display, &e);

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
        speed = (pressed(XK_h) || pressed(XK_backslash)) ? 3 : speed;
        speed = (pressed(XK_l) || pressed(XK_Shift_L)) ? 1 : speed;
        speed = (pressed(XK_semicolon)) ? 0 : speed;

        /* mouse clicks */
        XTestFakeButtonEvent(display, Button1, (pressed(XK_j) || pressed(XK_q)) ? True : False, CurrentTime);
        XTestFakeButtonEvent(display, Button3, (pressed(XK_k) || pressed(XK_e)) ? True : False, CurrentTime);
        XTestFakeButtonEvent(display, Button2, pressed(XK_i) ? True : False, CurrentTime);
        XTestFakeButtonEvent(display, 8, pressed(XK_u) ? True : False, CurrentTime);
        XTestFakeButtonEvent(display, 9, pressed(XK_o) ? True : False, CurrentTime);

        /* exit */
        if (e.type == KeyRelease)
        {
            k = XLookupKeysym(&e.xkey, 0);    
            if (k == XK_x || k == XK_m)
            {
                XCloseDisplay(display);
                return 0;
            }
            e.type = 0;
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
