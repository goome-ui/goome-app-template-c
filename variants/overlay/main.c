#include <stdio.h>

#include "goome.h"

#define APP_NAME "My Goome App"

int main(void)
{
    goome *g = goome_open(&(goome_opts){.title = APP_NAME}, NULL);
    if (!g) {
        fprintf(stderr, "couldnt open an overlay\n");
        return 1;
    }
    goome_hotkey(g, "alt+h", 1);
    goome_ui *u = goome_ui_new(g);
    double x = 60, y = 60, w = 280, h = 0;
    int clicks = 0, hidden = 0, open = 1;
    while (open) {
        goome_event ev;
        while (goome_poll(g, &ev, 16) > 0) {
            if (ev.type == GOOME_QUIT) open = 0;
            if (ev.type == GOOME_HOTKEY) hidden = !hidden;
            goome_ui_feed(u, &ev);
        }
        if (!open || !goome_ui_begin(u, 0)) continue;
        if (!hidden) {
            if (goome_ui_window(u, APP_NAME, &x, &y, &w, &h, GOOME_WIN_MOVE | GOOME_WIN_CLOSE, &open)) {
                goome_ui_label(u, "im on top of everything");
                if (goome_ui_button(u, "Click me")) clicks++;
                goome_ui_label(u, "clicked %d times", clicks);
                goome_ui_label(u, "alt+h hides me");
            }
            goome_ui_window_end(u);
        }
        goome_ui_end(u);
    }
    goome_ui_free(u);
    goome_close(g);
    return 0;
}
