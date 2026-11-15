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
    goome_hotkey(g, "alt+m", 1);
    goome_ui *u = goome_ui_new(g);
    double x = 80, y = 80, w = 300, h = 0, speed = 1;
    int menu = 0, fly = 0, god = 0, running = 1;
    while (running) {
        goome_event ev;
        while (goome_poll(g, &ev, 16) > 0) {
            if (ev.type == GOOME_QUIT) running = 0;
            if (ev.type == GOOME_HOTKEY) menu = !menu;
            goome_ui_feed(u, &ev);
        }
        if (!running) break;
        goome_canvas *c = goome_ui_begin(u, 0);
        if (!c) continue;
        if (menu) {
            if (goome_ui_window(u, APP_NAME, &x, &y, &w, &h, GOOME_WIN_MOVE | GOOME_WIN_CLOSE, &menu)) {
                goome_ui_toggle(u, "Fly", &fly);
                goome_ui_checkbox(u, "God mode", &god);
                goome_ui_slider(u, "Speed", &speed, 1, 10, "%.1f");
                if (goome_ui_button(u, "Respawn")) goome_ui_toast(u, "respawned", 2000);
            }
            goome_ui_window_end(u);
        } else {
            goome_draw_round_rect(c, 20, 20, 170, 30, 8, 0xC0202838);
            goome_draw_text(c, 32, 26, "alt+m opens the menu", 0, 0xFFFFFFFF);
        }
        goome_ui_end(u);
    }
    goome_ui_free(u);
    goome_close(g);
    return 0;
}
