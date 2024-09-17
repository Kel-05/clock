#include <ncurses.h>
#include <time.h>
#include <unistd.h>

int main(void) {
  time_t rawtime;
  struct tm ftime;
  int row, col;

  initscr();
  cbreak();
  noecho();

  getmaxyx(stdscr, row, col);
  
  while(1) {
    time(&rawtime);
    localtime_r(&rawtime, &ftime);
    mvprintw(row/2, col/2 - 4, "%d:%d:%d", ftime.tm_hour, ftime.tm_min, ftime.tm_sec);
    refresh();
    sleep(1);
  }

  endwin();
  return 0;
}
