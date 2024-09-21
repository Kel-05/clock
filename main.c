#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void cntr_printw(WINDOW *win, char *str, int yoffset, int xoffset);
void cntr_printw_multi(WINDOW *win, char str[], size_t size);
int sgets(char *str, size_t size, char **input);

int main(void) {
  time_t rawtime; // unix time
  struct tm ftime; // formatted time
  size_t bufsize;
  
  if(initscr() == NULL) {
    fprintf(stderr, "Error: initscr(): could not initialize screen");
    return 1;
  }
  cbreak();
  noecho();

  time(&rawtime);
  localtime_r(&rawtime, &ftime);

  char fstr[] = "%02d:%02d:%02d\nHell has frozen over\nmreow :3";
  bufsize = snprintf(NULL, 0, fstr, ftime.tm_hour, ftime.tm_min, ftime.tm_sec);
  char buf[bufsize];
  
  while(1) {
    snprintf(buf, bufsize+1, fstr, ftime.tm_hour, ftime.tm_min, ftime.tm_sec);
    cntr_printw_multi(stdscr, buf, bufsize);
    refresh();
    sleep(1);
    time(&rawtime);
    localtime_r(&rawtime, &ftime);
  }

  endwin();
  return 0;
}

void cntr_printw(WINDOW *win, char *input, int yoffset, int xoffset) {
  int cntr_row, cntr_col;
  getmaxyx(win, cntr_row, cntr_col);
  cntr_row = cntr_row/2 + yoffset;
  cntr_col = cntr_col/2 - strlen(input)/2 + xoffset;
  mvprintw(cntr_row, cntr_col, "%s\n", input);
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
