#include <unistd.h>
#include <pthread.h>
#include "clock.h"


void help() {
  puts("Usage: clock [OPTIONS]\n"
       "  -h\tprint this help message\n"
       "  -s\tsine wave");
}

int main(int argc, char **argv) {
  struct st_time st_time;
  size_t bufsize;
  pthread_t clock;
  double angle = 0;
  int x = 0;
  char c;

  opterr = 0;
  
  while((c = getopt(argc, argv, "hs")) != -1) {
    switch(c) {
      case 's':
        break;
      case 'h':
	help();
	return 0;
	break;
      default:
	help();
	return 1;
    }
  }
  
  if(initscr() == NULL) {
    fprintf(stderr, "Error: initscr(): could not initialize screen");
    return 1;
  }
  cbreak();
  noecho();

  time(&st_time.rawtime);
  localtime_r(&st_time.rawtime, &st_time.ftime);

  char fstr[] = "%02d:%02d:%02d";
  bufsize = snprintf(NULL, 0, fstr, st_time.ftime.tm_hour, st_time.ftime.tm_min, st_time.ftime.tm_sec);
  char buf[bufsize];

  pthread_create(&clock, NULL, thread_update_clock, &st_time);
  
  while(1) {
    snprintf(buf, bufsize+1, fstr, st_time.ftime.tm_hour, st_time.ftime.tm_min, st_time.ftime.tm_sec);
    clear();
    mvprintw_sine(stdscr, &angle, &x, buf);
    refresh();
    usleep(75 * 1000);
  }

  endwin();
  return 0;
}
