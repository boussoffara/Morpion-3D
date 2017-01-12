/*
 *      __  __    ___    ___   ___   ___    ___    _  _
 *     |  \/  |  / _ \  | _ \ | _ \ |_ _|  / _ \  | \| |
 *     | |\/| | | (_) | |   / |  _/  | |  | (_) | | .` |
 *     |_|  |_|  \___/  |_|_\ |_|   |___|  \___/  |_|\_|
 *
 *     @Author: Alaeddine.Boussoffara
 *     @File: Main,Display the selection menu
 */

#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "game.h"
#include "gamelogic.h"
#include "explosion.h"

int main() {
  int size = 0; /* n */
  int ch = 0; /*to store a typed caracter */
  int choice = -1;
  int current = 0; /* currently selected choice */
  int max_y = 0, max_x = 0; /* to store frame size */
  int bound = 100; /* max acceptable size */
  int x;
  int opt = -1;
  char * options[] = {
    "no",
    "yes",
  };
  char str[100];
  char * choices[] = {
    "Vue du dessus",
    "3D",
  };

  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);
  getmaxyx(stdscr, max_y, max_x);

  /* game variant  selection menu
   *  while loop breaks when a choice is made
   */
  while (choice == -1) {
    clear();
    print_ascii_art(max_x, max_y);
    attron(COLOR_PAIR(2));
    mvprintw(13, max_x / 2 - 16, "Choose your variant:");
    mvprintw(max_y - 1, 1, "Help: use UP/DOWN keys to switch, ENTER to select, Q to quit");
    mvprintw(13, max_x / 2 + 5, choices[current]);
    /* display separator*/
    for (x = 0; x < max_x; x++) {
      mvprintw(max_y - 2, x, "-");
    }
    attroff(COLOR_PAIR(2));
    /* keyboard handling */
    ch = getch();
    switch (ch) {
    case KEY_UP:
    case 'i':
      current = (current + 1) % 2;
      break;
    case KEY_DOWN:
    case 'k':
      current = current == 0 ? 1 : current - 1;
      break;
    case 10:
      choice = current;
      break;
    }
    if (ch == 'q') {
      clear();
      endwin();
      return 0;
    }
    /* refresh and windows resizing handling */
    refresh();
    getmaxyx(stdscr, max_y, max_x);
  }

  /* seisme option selection menu
   *  while loop breaks when a choice is made
   */

  while (opt == -1) {
    clear();
    print_ascii_art(max_x, max_y);
    attron(COLOR_PAIR(2));
    mvprintw(12, (max_x - strlen(choices[choice])) / 2, choices[choice]);
    mvprintw(13, max_x / 2 - 16, "Do you want the seisme option? :");
    mvprintw(max_y - 1, 1, "Help: use UP/DOWN keys to switch, ENTER to select, Q to quit");
    mvprintw(13, max_x / 2 + 16, options[current]);
    /* display separator*/
    for (x = 0; x < max_x; x++) {
      mvprintw(max_y - 2, x, "-");
    }
    attroff(COLOR_PAIR(2));
    ch = getch();
    switch (ch) {
    case KEY_UP:
    case 'i':
      current = (current + 1) % 2;
      break;
    case KEY_DOWN:
    case 'k':
      current = current == 0 ? 1 : current - 1;
      break;
    case 10:
      opt = current;
      break;
    }
    refresh();
    getmaxyx(stdscr, max_y, max_x);
    if (ch == 'q') {
      clear();
      endwin();
      return 0;
    }
  }

  /* game  size selection menu
   *  while loop breaks when a valid game size  given
   *  check that size is a valid number and within screen size range
   */
  int outofbound = 0, idiot = 0;
  while (size <= 0) {
    ch = 0;
    current = 0;
    clear();
    print_ascii_art(max_x, max_y);
    echo();
    curs_set(1);
    nocbreak();
    attron(COLOR_PAIR(2));
    mvprintw(12, (max_x - strlen(choices[choice])) / 2, choices[choice]);
    mvprintw(max_y - 1, 1, "Help:ENTER to select, q + ENTER to quit ");
    /* display separator*/
    for (x = 0; x < max_x; x++) {
      mvprintw(max_y - 2, x, "-");
    }
    if (idiot) {
      attron(COLOR_PAIR(1));
      mvprintw(15, max_x / 2 - 16, "I said a number! idiot!");
      attroff(COLOR_PAIR(1));
    }
    if (outofbound) {
      attron(COLOR_PAIR(1));
      bound = max_x/4 < (max_y - 2) / 2 ? max_x/4 : (max_y - 2) / 2;
      strcpy(str, "terminal window too small, max size : ");
      char bounds[5];
      sprintf(bounds, "%d", bound-1);
      strcat(str, bounds);
      mvprintw(15, (max_x - strlen(str)) / 2, str);
      attroff(COLOR_PAIR(1));
    }
    if (idiot) {
      outofbound = 0;
    } else {
      idiot = 0;
    }
    mvprintw(13, max_x / 2 - 16, "Enter game size:");
    attroff(COLOR_PAIR(2));
    getstr(str);
    char * ptr = NULL;
    current = strtol(str, & ptr, 10);
    if (ptr[0] == '\0' && current > 0) {
      if (current < (max_x / 4) - 2 && current < (max_y - 2) / 2) {
        size = current;
      } else {
        outofbound = 1;
      }
    } else {
      idiot = 1;
    }
    refresh();
    getmaxyx(stdscr, max_y, max_x);
    if (str[0] == 'q') {
      clear();
      endwin();
      return 0;
    }
  }

  /* let the games begin ! */
  startgame(choice, opt, size);
  endwin();
  return 0;
}
