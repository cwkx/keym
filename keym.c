#include <stdio.h>
#include <sys/select.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

static const int speeds[5] = {80, 400, 1400, 4000, 10000};
static const int scroll[5] = {1000, 5000, 30000, 50000, 100000};

int main()
{
    Display *display;
    Window window;
    XEvent e;
    KeySym k;
    fd_set in_fds;
    struct timeval tv;
    int screen;
    int x11_fd;
    int num_ready_fds;
    char key_delta[6]; /* Left, Right, Up, Down, SCROLL_UP, SCROLL_DOWN */
    char speed = 2; /* Dash, Fast, Normal, Slow, Crawl */

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
        tv.tv_usec = (key_delta[4] || key_delta[5]) ? scroll[speed] : speeds[speed];

        num_ready_fds = select(x11_fd + 1, &in_fds, NULL, NULL, &tv);

        XGrabKeyboard(display, window, False, GrabModeAsync, GrabModeAsync, CurrentTime);

        while (XPending(display))
            XNextEvent(display, &e);

        if ((e.type == KeyPress) || (e.type == KeyRelease))
        {
            k = XLookupKeysym(&e.xkey, 0);

            /* mouse movement */
            if (k == XK_Left || k == XK_a)
                key_delta[0] = (e.type == KeyPress) ? 1.0f : 0.0f;
            if (k == XK_Right || k == XK_d)
                key_delta[1] = (e.type == KeyPress) ? 1.0f : 0.0f;
            if (k == XK_Up || k == XK_w)
                key_delta[2] = (e.type == KeyPress) ? 1.0f : 0.0f;
            if (k == XK_Down || k == XK_s)
                key_delta[3] = (e.type == KeyPress) ? 1.0f : 0.0f;

            /* clicks */
            if (k == XK_j || k == XK_q)
                XTestFakeButtonEvent(display, Button1, (e.type == KeyPress) ? True : False, CurrentTime);
            if (k == XK_k || k == XK_e)
                XTestFakeButtonEvent(display, Button3, (e.type == KeyPress) ? True : False, CurrentTime);

            /* scrolling */
            if (k == XK_r)
                key_delta[4] = (e.type == KeyPress) ? 1.0f : 0.0f;
            if (k == XK_f)
                key_delta[5] = (e.type == KeyPress) ? 1.0f : 0.0f;

            /* speed adjustment from slow to fast */
            if (k == XK_g)
                speed = (e.type == KeyPress) ? 4 : 2;
            if (k == XK_h || k == XK_backslash)
                speed = (e.type == KeyPress) ? 3 : 2;
            if (k == XK_l || k == XK_Shift_L)
                speed = (e.type == KeyPress) ? 1 : 2;
            if (k == XK_semicolon)
                speed = (e.type == KeyPress) ? 0 : 2;

            /* exit */
            if (e.type == KeyRelease && (k == XK_x || k == XK_m))
            {
                XCloseDisplay(display);
                return 0;
            }
            e.type = 0;
        }

        XWarpPointer(display, None, None, 0, 0, 0, 0, (key_delta[1] - key_delta[0]), (key_delta[3] - key_delta[2]));

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

        XSync(display, False);
    }

    XCloseDisplay(display);
    return 1;
}
