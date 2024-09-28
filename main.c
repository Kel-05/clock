#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "clock.h"

#define INPUT "%02d:%02d:%02d"
#define REFRESH_RATE 16666
#define YSPEED 0.1
#define XSPEED 2
#define LEN 9

void help(FILE *fdesc, char *progname) {
  fprintf(fdesc, "Usage: %s [OPTIONS]\n\n"
	  "  -h\tprint this help message\n"
	  "  -s\tsine wave\n\n", progname);
}

void sine_clock(WINDOW *win, char *input, struct st_time *st_time) {
  size_t bufsize = strlen(input)+1;
  char buf[bufsize];
  double angle = 0;
  pthread_t clock;
  int x = 0;

  time(&st_time->rawtime);
  localtime_r(&st_time->rawtime, &st_time->ftime);
  
  pthread_create(&clock, NULL, thread_update_clock, st_time);
  
  while(1) {
    snprintf(buf, bufsize, input, st_time->ftime.tm_hour, st_time->ftime.tm_min, st_time->ftime.tm_sec);
    clear();
    mvprintw_sine(win, buf, &angle, &x, YSPEED, XSPEED);
    refresh();
    usleep(REFRESH_RATE);
  }
}

int main(int argc, char **argv) {
  struct st_time st_time;
  char c, sineflag = 0;
  char buf[LEN];

  opterr = 0;
  
  while((c = getopt(argc, argv, "hs")) != -1) {
    switch(c) {
      case 's':
	sineflag = 1;
        break;
      case 'h':
	help(stdout, argv[0]);
	return 0;
	break;
      default:
	help(stderr, argv[0]);
	return 1;
    }
  }
  
  if(initscr() == NULL) {
    fprintf(stderr, "Error: initscr(): could not initialize screen");
    return 1;
  }
  cbreak();
  noecho();
  curs_set(0);
 
  if(sineflag) sine_clock(stdscr, INPUT, &st_time);
  
  while(1) {
    time(&st_time.rawtime);
    localtime_r(&st_time.rawtime, &st_time.ftime);
    snprintf(buf, LEN, INPUT, st_time.ftime.tm_hour, st_time.ftime.tm_min, st_time.ftime.tm_sec);
    clear();
    cntr_printw(stdscr, buf, 0, 0);
    refresh();
    sleep(1);
  }

  endwin();
  return 0;
}
