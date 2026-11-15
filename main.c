#include <stdio.h>

#include "goome.h"

#define APP_NAME "My Goome App"

static int clicks, overlay;
static double volume = 0.5;
static double wx, wy = 60, ww = 300, wh;

static goome *open_mode(int ov)
{
    if (!ov) return goome_open_app(APP_NAME, 640, 420, NULL);
    goome *g = goome_open(&(goome_opts){.title = APP_NAME}, NULL);
    if (g) {
        goome_monitor m;
        wx = goome_monitors(g, &m, 1) > 0 ? m.area.x + m.area.w - ww - 40 : 40;
    }
    return g;
}

static void contents(goome_ui *u)
{
    goome_ui_toggle(u, "Overlay mode", &overlay);
    goome_ui_separator(u);
    goome_ui_heading(u, "Hello from " APP_NAME);
    if (goome_ui_button(u, "Click me")) clicks++;
    goome_ui_label(u, "clicked %d times", clicks);
    goome_ui_slider(u, "Volume", &volume, 0, 1, "%.2f");
}

int main(void)
{
    int mode = 0;
    goome *g = open_mode(0);
    if (!g) {
        fprintf(stderr, "couldnt open a window\n");
        return 1;
    }
    goome_ui *u = goome_ui_new(g);
    for (int running = 1; running;) {
        goome_event ev;
        while (goome_poll(g, &ev, 16) > 0) {
            if (ev.type == GOOME_QUIT || ev.type == GOOME_CLOSE) running = 0;
            goome_ui_feed(u, &ev);
        }
        if (!running) break;
        if (overlay != mode) {
            goome_ui_free(u);
            goome_close(g);
            g = open_mode(overlay);
            if (!g) {
                fprintf(stderr, "no overlay here, staying in window mode\n");
                overlay = 0;
                g = open_mode(0);
                if (!g) return 1;
            }
            mode = overlay;
            u = goome_ui_new(g);
            printf("%s mode\n", mode ? "overlay" : "window");
            fflush(stdout);
            continue;
        }
        if (!goome_ui_begin(u, 0)) continue;
        if (mode) {
            if (goome_ui_window(u, APP_NAME, &wx, &wy, &ww, &wh, GOOME_WIN_MOVE | GOOME_WIN_COLLAPSE, NULL)) contents(u);
            goome_ui_window_end(u);
        } else {
            goome_ui_app(u, APP_NAME, 0);
            contents(u);
            goome_ui_app_end(u);
        }
        goome_ui_end(u);
    }
    goome_ui_free(u);
    goome_close(g);
    return 0;
}
