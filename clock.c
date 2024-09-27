#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include "clock.h"

void cntr_printw(WINDOW *win, char *input, int yoffset, int xoffset) {
  int cntr_row, cntr_col;
  getmaxyx(win, cntr_row, cntr_col);
  cntr_row = cntr_row/2 + yoffset;
  cntr_col = cntr_col/2 - strlen(input)/2 + xoffset;
  mvprintw(cntr_row, cntr_col, "%s\n", input);
}

int sgets(char *str, size_t size, char **input)
{
  char *next = *input, bogostr[size];
  int numread = 0;
  if(str == NULL) str = bogostr;
  
  while(numread < size && *next) {
    if(*next == '\n') break;
    *str++ = *next++; // str[i] = next[i]; i++;
    numread++;
  }

  next++;
  if(numread == 0) return 0;  // "eof"
  
  // must have hit the null terminator or end of line
  *str = '\0';  // null terminate this tring
  numread++;
  // set up input for next call
  *input = next;
  return numread;
}

void cntr_printw_multi(WINDOW *win, char *input, size_t size) {
  int yoffset = 0;
  char substr[size];
  
  while(sgets(NULL, size, &input) != 0) yoffset--;
  yoffset = yoffset/2 + (yoffset%2 == 0);
  input -= size+1;

  while(sgets(substr, size, &input) != 0) {
    cntr_printw(win, substr, yoffset, 0);
    yoffset++;
  }
}

void mvprintw_sine(WINDOW *win, double *angle, int *x, char *input) {
  int xmax, y;
  getmaxyx(win, y, xmax);
  if(*x > xmax) *x = 0;
  y = (y + y*sin(*angle))/2;
  *angle += 0.2;
  mvprintw(y, *x, "%s\n", input);
  *x += 2;
}


void *thread_update_clock(void *input) {
  struct st_time *st_time = (struct st_time *)input;
  while(1) {
    time(&st_time->rawtime);
    localtime_r(&st_time->rawtime, &st_time->ftime);
    sleep(1);
  }
}
