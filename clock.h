#ifndef CLOCK_H_
#define CLOCK_H_

#include <curses.h>
#include <time.h>

struct st_time {
  time_t rawtime;
  struct tm ftime;
};

void cntr_printw(WINDOW *win, char *input, int yoffset, int xoffset);
void mvprintw_sine(WINDOW *win, double *angle, int *x, char *input);
void cntr_printw_multi(WINDOW *win, char *input, size_t size);
int sgets(char *str, size_t size, char **input);
void *thread_update_clock(void *input);

#endif
