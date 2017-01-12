/*
 *      __  __    ___    ___   ___   ___    ___    _  _
 *     |  \/  |  / _ \  | _ \ | _ \ |_ _|  / _ \  | \| |
 *     | |\/| | | (_) | |   / |  _/  | |  | (_) | | .` |
 *     |_|  |_|  \___/  |_|_\ |_|   |___|  \___/  |_|\_|
 *
 *     @Author: Alaeddine.Boussoffara
 *    @File: Displays the game
 */

#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "header.h"
#include "gamelogic.h"

void startgame(int choice, int opt, int size) {

  int result = -1; /*game result*/
  int error = 0; /* for error handling*/
  int ch = 0; /*to store a typed caracter*/
  int max_y = 0, max_x = 0; /*to store frame size*/
  int x, y = 0;
  int selected_x = 0, selected_y = 0; /* selected cell coordinates*/
  int i, j, k; /*generic counters*/
  char players[2] = {
    'x',
    'o'
  }; /* player caracters*/
  int current = 0; /* current player number*/
  char cell[2] = {
    '\0',
    '\0'
  }; /*used to convert chars to strings*/

  /* ncurses initial config*/
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr, TRUE);
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);
  getmaxyx(stdscr, max_y, max_x);

  /* games starts !!*/
  /*initializing empty board*/
  char * * * board = malloc(sizeof * board * size);
  if (board) {
    for (i = 0; i < size; i++) {
      board[i] = malloc(sizeof * board[i] * size);
      if (board[i]) {
        for (j = 0; j < size; j++) {
          board[i][j] = malloc(sizeof * board[i][j] * size * 50);
          for (k = 0; k < size * 2; k++) {
            board[i][j][k] = EMPTYCELL;
          }
        }
      }else {return ;} /* quit on memory error */
    }
  }else {return ;} /* quit on memory error */

  /*initializing summits board*/
  int * * summits = malloc(size * sizeof(int * ));
  if (summits){
    for (i = 0; i < size; i++) {
      summits[i] = malloc(sizeof(int) * size);
      for (j = 0; j < size; j++) {
        summits[i][j] = 0;
      }
    }
  }else {return ;} /* quit on memory error */

  int * * collapse = collapse_prob(summits, size);

  /*
   *  game display
   *  while loop breaks when a game result is determined
   *  or 'q' is pressed
   */

  while (1) {

    clear();
    /*display board*/
    for (x = 0; x < size; x++) {
      for (y = 0; y < size; y++) {
        cell[0] = board[x][y][summits[x][y]];
        attron(COLOR_PAIR(2));
        mvprintw(2 * y + 1, 3 + 3 * x, cell);
      }
    }
    /* display separator*/
    for (x = 0; x < max_x; x++) {
      mvprintw(max_y - 2, x, "-");
    }

    /*
     * display selected pile
     * if frame size is too small a second pile is created
     */

    k = 0;
    j = max_x - 5;
    for (i = 0; i <= summits[selected_x][selected_y]; i++) {
      cell[0] = board[selected_x][selected_y][i];

      if (max_y - i + k - 3 < 4) {
        k = i;
        j = j - 3;
      }
      mvprintw(max_y - i + k - 3, j, cell);
    }

    /*display current player*/
    cell[0] = players[current];
    char tour[80] = "This is player's ";
    strcat(tour, cell);
    strcat(tour, " turn, what is your choice ? ");
    mvprintw(max_y - 1, 3, tour);

    /* if playing seisme display collapsed piles*/
    if (opt) {
      displaycollapse(collapse, size);
    }
    /* print selection decoration*/
    cell[0] = '+';
    mvprintw(2 * selected_y + 1, 3 + 3 * selected_x + 1, cell);
    mvprintw(2 * selected_y + 1, 3 + 3 * selected_x - 1, cell);
    mvprintw(2 * selected_y + 2, 3 + 3 * selected_x, cell);
    mvprintw(2 * selected_y, 3 + 3 * selected_x, cell);
    attroff(COLOR_PAIR(2));

    /*keyboard  handling*/
    ch = getch();
    switch (ch) {
    case 'k':
    case KEY_DOWN:
      selected_y = (selected_y + 1) % size;
      break;
    case 'i':
    case KEY_UP:
      selected_y = selected_y == 0 ? size - 1 : selected_y - 1;
      break;
    case 'l':
    case KEY_RIGHT:
      selected_x = (selected_x + 1) % size;
      break;
    case 'j':
    case KEY_LEFT:
      selected_x = selected_x == 0 ? size - 1 : selected_x - 1;
      break;
    case 'p':
      error = add_piece(size, board, summits, selected_x, selected_y,
        players[current]);
      break;
    case 'r':
      error = remove_piece(board, summits, selected_x, selected_y);
    }

    /*error handling*/
    error_handler(error, size);

    /* check if a valid move have been made if so passes the turn*/

    if ((ch == 'p' || ch == 'r') && error == 0) {
      current = (current + 1) % 2;
      /*check results*/
      result = check_result(choice, board, summits, size);

      /* collapse calculations*/
      if (opt) {
        collapse = collapse_prob(summits, size);
        for (i = 0; i < size; i++) {
          for (j = 0; j < size; j++) {
            if (collapse[i][j] == 1) {
              k = rand() % summits[i][j] + 1;
              /* while loop breaks when k pieces are removed */
              while (k > 0) {
                remove_piece(board, summits, i, j);
                k--;
              }
            }
          }
        }
        /*recalculating result*/
        if (result == -1) {
          result = check_result(choice, board, summits, size);
        }
      }
    }

    error = 0;

    /* refresh display*/
    refresh();
    usleep(DELAY);
    /*handle resizing screen*/
    getmaxyx(stdscr, max_y, max_x);

    /*check if we have a winner or a quitter*/
    if (result != -1 || ch == 'q') {
      break;
    }
  }

  /*celebrate the winner*/
  if (result != -1) {
    celebrate(result);
  }
  endwin();
}
