#include <stdio.h>
#include <time.h>

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
    long start = goome_now_ms(), last = start;
    double fps = 0;
    int hidden = 0, running = 1;
    goome_pacer pace = {0};
    while (running) {
        goome_event ev;
        int wait = goome_pace(&pace, 30);
        while (goome_poll(g, &ev, wait) > 0) {
            if (ev.type == GOOME_QUIT) running = 0;
            if (ev.type == GOOME_HOTKEY) hidden = !hidden;
            wait = goome_pace(&pace, 30);
        }
        if (!running) break;
        long now = goome_now_ms();
        if (now > last) fps = fps * 0.9 + 100.0 / (double)(now - last);
        last = now;
        goome_canvas *c = goome_begin(g, 0);
        if (!c) continue;
        if (!hidden) {
            char clock[32], up[32], rate[32];
            time_t t = time(NULL);
            strftime(clock, sizeof(clock), "%H:%M:%S", localtime(&t));
            snprintf(up, sizeof(up), "up %lds", (now - start) / 1000);
            snprintf(rate, sizeof(rate), "%.0f fps", fps);
            goome_draw_round_rect(c, 20, 20, 160, 92, 10, 0xB0141820);
            goome_draw_text(c, 34, 30, clock, 1, 0xFFFFFFFF);
            goome_draw_text(c, 34, 62, up, 0, 0xFFC0C8D0);
            goome_draw_text(c, 34, 84, rate, 0, 0xFF6FDC5F);
        }
        goome_end(g, c, NULL, 0);
    }
    goome_close(g);
    return 0;
}
