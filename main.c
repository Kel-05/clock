#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void cntr_printw(WINDOW *win, char str[]) {
  int cntr_row, cntr_col;
  getmaxyx(win, cntr_row, cntr_col);
  cntr_row /= 2;
  cntr_col = cntr_col/2 - strlen(str)/2;
  mvprintw(cntr_row, cntr_col, "%s\n", str);
}

int main(void) {
  time_t rawtime;
  struct tm ftime;
  WINDOW *win;
  char buf[10] = {0};

  if( ( win = initscr() ) == NULL) {
    fprintf(stderr, "Error: initscr(): could not initialize screen");
    return 1;
  }
  cbreak();
  noecho();
  
  while(1) {
    time(&rawtime);
    localtime_r(&rawtime, &ftime);
    snprintf(buf, 10, "%02d:%02d:%02d", ftime.tm_hour, ftime.tm_min, ftime.tm_sec);
    cntr_printw(win, buf);
    refresh();
    sleep(1);
  }

  endwin();
  return 0;
}
