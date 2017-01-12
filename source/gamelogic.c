/*
 *      __  __    ___    ___   ___   ___    ___    _  _
 *     |  \/  |  / _ \  | _ \ | _ \ |_ _|  / _ \  | \| |
 *     | |\/| | | (_) | |   / |  _/  | |  | (_) | | .` |
 *     |_|  |_|  \___/  |_|_\ |_|   |___|  \___/  |_|\_|
 *
 *     @Author: Alaeddine.Boussoffara
       @File:  gamelogic and display functions
 */

#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <math.h>
#include "explosion.h"
#include "header.h"


/* @requires:nothing
  @assigns: nothing
  @ensures: game title printed
*/

void print_ascii_art(int max_x, int max_y) {
  attron(COLOR_PAIR(1));
  if (max_y > 12) {
    mvprintw(5, max_x / 2 - 20, "__  __  ___  ____  ____ ___ ___  _   _ ");
    mvprintw(6, max_x / 2 - 20, "|  \\/  |/ _ \\|  _ \\|  _ |_ _/ _ \\| \\ | |");
    mvprintw(7, max_x / 2 - 20, "| |\\/| | | | | |_) | |_) | | | | |  \\| |");
    mvprintw(8, max_x / 2 - 20, "| |  | | |_| |  _ <|  __/| | |_| | |\\  |");
    mvprintw(9, max_x / 2 - 20, "|_|  |_|\\___/|_| \\_|_|  |___\\___/|_| \\_|");
  } else {
    mvprintw(2, max_x / 2 - 20, "MORPION");
  }
  attroff(COLOR_PAIR(1));
}

/* @requires:a grid of collapsed cell (int in (0,1))
  @assigns: nothing
  @ensures: collapse decoration is displayed
*/
void displaycollapse(int * * collapse, int size) {
  char cell[2];
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (collapse[j][i] == 1) {
        cell[1] = '\0';
        cell[0] = '*';
        mvprintw(2 * i + 1, 3 + 3 * j + 1, cell);
        mvprintw(2 * i + 1, 3 + 3 * j - 1, cell);
        mvprintw(2 * i + 2, 3 + 3 * j, cell);
        mvprintw(2 * i, 3 + 3 * j, cell);
      }

    }
  }
}

/* @requires: a valid cell coordinates and non empty cell
  @assigns: 0 if no erros else 1
  @ensures: the top tem of a pile is removed
*/

int remove_piece(char * * * board, int * * summits, int i, int j) {
  if (summits[i][j] > 0) {
    board[i][j][summits[i][j]] = EMPTYCELL;
    summits[i][j]--;
    return 0;
  } else {
    return 1;
  }
}

/* @requires: a valid cell coordinates and non full cell
  @assigns: 0 if no erros else 2
  @ensures: the cell item is added to the top of the pile
*/

int add_piece(int size, char * * * board, int * * summits, int i, int j, char cell) {
  if (summits[i][j] <= 3 * size - 1) {
    summits[i][j]++;
    board[i][j][summits[i][j]] = cell;
    return 0;
  } else {
    return 2;
  }
}

/* @requires: a valid error code
  @assigns: nothing
  @ensures: correct error message is displayed
*/

void error_handler(int error, int size) {
  attron(COLOR_PAIR(1));
  switch (error) {
  case 1:
    mvprintw(1, 6 + 3 * size, "ERROR: Empty Cell ! Focus !");
    refresh();
    sleep(1);
    break;
  case 2:
    mvprintw(1, 6 + 3 * size, "ERROR: Full Cell ! sorry '-.-");
    refresh();
    sleep(1);
    break;
  }
  attroff(COLOR_PAIR(1));
}

/* @requires: the board size
              a grid of summits in the correct size
  @assigns: a grid of booleans indicating which piles collapsed
  @ensures: piles collapse according to the required probability
*/
int * * collapse_prob(int * * summits, int size) {
  int i, j;
  /* initializing summits board */
  int * * collapse = malloc(size * sizeof(int * ));
  for (i = 0; i < size; i++) {
    collapse[i] = malloc(sizeof(int) * size);
    for (j = 0; j < size; j++) {
      if (summits[i][j] == 0) {
        collapse[i][j] = 0;
      } else {
        collapse[i][j] = rand() < (1 - pow(2, -summits[i][j] / (2 * size))) * ((double) RAND_MAX + 1.0);
      }
    }
  }
  return collapse;
}

/* @requires: result in (0..2)
  @assigns: nothing
  @ensures: celebration animation displayed
*/
void celebrate(int result) {
  explosion();
  clear();
  initscr();
  attroff(COLOR_PAIR(1));
  if (result == 0) {
    mvprintw(2, 10, "                                    ");
    mvprintw(3, 10, "            |\\     /|               ");
    mvprintw(4, 10, "            ( \\   / )               ");
    mvprintw(5, 10, "             \\ (_) /                ");
    mvprintw(6, 10, "              ) _ (                 ");
    mvprintw(7, 10, "             / ( ) \\                ");
    mvprintw(8, 10, "            ( /   \\ )               ");
    mvprintw(9, 10, "            |/     \\|               ");
    mvprintw(10, 10,"                                    ");
    mvprintw(11, 10,"         _________ _        _______ ");
  } else if (result == 1) {
    mvprintw(2, 10, "             _______                ");
    mvprintw(3, 10, "            (  ___  )               ");
    mvprintw(4, 10, "            | (   ) |               ");
    mvprintw(5, 10, "            | |   | |               ");
    mvprintw(6, 10, "            | |   | |               ");
    mvprintw(7, 10, "            | |   | |               ");
    mvprintw(8, 10, "            | (___) |               ");
    mvprintw(9, 10, "            (_______)               ");
    mvprintw(10, 10, "                                    ");
  }
  if (result != 3) {
    mvprintw(12, 10, "|\\     /|\\__   __/( (    /|(  ____ \\");
    mvprintw(13, 10, "| )   ( |   ) (   |  \\  ( || (    \\/");
    mvprintw(14, 10, "| | _ | |   | |   |   \\ | || (_____ ");
    mvprintw(15, 10, "| |( )| |   | |   | (\\ \\) |(_____  )");
    mvprintw(16, 10, "| || || |   | |   | | \\   |      ) |");
    mvprintw(17, 10, "| () () |___) (___| )  \\  |/\\____) |");
    mvprintw(18, 10, "(_______)\\_______/|/    )_)\\_______)");
    mvprintw(19, 10, "                                    ");
  } else {
    mvprintw(5, 5, "Draw !");
  }
  attroff(COLOR_PAIR(2));
  refresh();
  sleep(4);
}

/* @requires: a valid error choice in (0,2)
              the board size
              a board and a grid of summits in the correct size
  @assigns: 0 if x win 1 if 0 wins 2 if its a draw -1 else
  @ensures: the player with most correct rows wins

            May the force be with you :p
*/

int check_result(int choice, char * * * board, int * * summits, int size) {
  int i, j, k; /*the usual generic counters */
  int xwin = 0;
  int ywin = 0;
  /* checking score for Vue du dessus is sperate */
  if (choice == 0) {
    /* check columns */
    for (i = 0; i < size; i++) {
      for (j = 1; j < size; j++) {
        if (board[i][0][summits[i][0]] == EMPTYCELL) {
          break;
        }
        if (board[i][j][summits[i][j]] != board[i][0][summits[i][0]]) {
          break;
        }
      }
      if (j == size && board[i][0][summits[i][0]] != EMPTYCELL) {
        if (board[i][0][summits[i][0]] == 'x') {
          xwin = +1;
        } else {
          ywin += 1;
        }
      }
    }
    /* check rows */
    for (i = 0; i < size; i++) {
      for (j = 1; j < size; j++) {
        if (board[0][i][summits[0][i]] == EMPTYCELL) {
          break;
        }
        if (board[j][i][summits[j][i]] != board[0][i][summits[0][i]]) {
          break;
        }
      }
      if (j == size && board[0][i][summits[0][i]] != EMPTYCELL) {
        if (board[0][i][summits[0][i]] == 'x') {
          xwin = +1;
        } else {
          ywin += 1;
        }
      }
    }
    /* check diagonal */
    for (i = 0; i < size; i++) {
      if (board[0][0][summits[0][0]] == EMPTYCELL) {
        break;
      }
      if (board[i][i][summits[i][i]] != board[0][0][summits[0][0]]) {
        break;
      }
    }
    if (i == size && board[0][0][summits[0][0]] != EMPTYCELL) {
      if (board[0][0][summits[0][0]] == 'x') {
        xwin = +1;
      } else {
        ywin += 1;
      }
    }
    /*check anti diagonal */
    for (i = 1; i <= size; i++) {
      if (board[size - 1][0][summits[size - 1][0]] == EMPTYCELL) {
        break;
      }
      if (board[size - i][i - 1][summits[size - i][i - 1]] !=
        board[size - 1][0][summits[size - 1][0]]) {
        break;
      }
    }
    if (i == size + 1 && board[size - 1][0][summits[size - 1][0]] != EMPTYCELL) {
      if (board[size - 1][0][summits[size - 1][0]] == 'x') {
        xwin = +1;
      } else {
        ywin += 1;
      }
    }
  } else {
    /*checking  result  for 3d */
    /*find biggest pile */
    int biggest_pile = 0;
    for (i = 0; i < size; i++) {
      for (j = 1; j < size; j++) {
        if (summits[i][j] > biggest_pile) {
          biggest_pile = summits[i][j];
        }
      }
    }

    /*for each level check result */
    for (k = 1; k <= biggest_pile; k++) {
      /* check columns */
      for (i = 0; i < size; i++) {

        for (j = 1; j < size; j++) {
          if (board[i][0][k] == EMPTYCELL) {
            break;
          }
          if (board[i][j][k] != board[i][0][k]) {
            break;
          }
        }
        if (j == size && board[i][0][k] != EMPTYCELL) { /*win cond */
          if (board[i][0][k] == 'x') {
            xwin = +1;
          } else {
            ywin += 1;
          }
        }
      }
      /*check rows */
      for (i = 0; i < size; i++) {
        for (j = 1; j < size; j++) {
          if (board[0][i][k] == EMPTYCELL) {
            break;
          }
          if (board[j][i][k] != board[0][i][k]) {
            break;
          }
        }
        if (j == size && board[0][i][k] != EMPTYCELL) {
          if (board[0][i][k] == 'x') {
            xwin = +1;
          } else {
            ywin += 1;
          }
        }
      }
      /* check diagonals */
      for (i = 0; i < size; i++) {

        if (board[0][0][k] == EMPTYCELL) {
          break;
        }
        if (board[i][i][k] != board[0][0][k]) {
          break;
        }
      }
      if (i == size && board[0][0][k] != EMPTYCELL) {
        if (board[0][0][k] == 'x') {
          xwin = +1;
        } else {
          ywin += 1;
        }
      }
      /* check anti diagonals */
      for (i = 1; i <= size; i++) {

        if (board[size - 1][0][k] == EMPTYCELL) {
          break;
        }
        if (board[size - i][i - 1][k] !=
          board[size - 1][0][k]) {
          break;
        }
      }
      if (i == size + 1 && board[size - 1][0][k] != EMPTYCELL) {
        if (board[size - 1][0][k] == 'x') {
          xwin = +1;
        } else {
          ywin += 1;
        }
      }
      /* check piles */
      char current = 'x';
      int counter = 1;
      for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
          counter = 1;
          if (summits[i][j] < size) {
            break;
          }
          for (k = 1; k <= summits[i][j]; k++) {
            if (board[i][j][k] == current) {
              counter++;
            } else {
              current = board[i][j][k];
              counter = 1;
            }
            if (counter == size) {
              if (current == 'x') {
                xwin = +1;
              } else {
                ywin += 1;
              }
            }
          }
        }
      }
      /* check steps asc column */
      counter = 0;
      current = 'x';
      for (i = 0; i < size; i++) {
        for (k = 1; k < biggest_pile; k++) {
          counter = 0;
          current = board[i][0][0 + k];
          for (j = 0; j < size; j++) {
            if (board[i][j][j + k] == current) {
              counter++;
            } else {
              break;
            }
          }
          if (counter == size) {
            if (current == 'x') {
              xwin = +1;
            } else if (current == 'o') {
              ywin += 1;
            }
          }
        }

      }
      /* check steps asc rows */
      counter = 0;
      current = 'x';
      for (i = 0; i < size; i++) {
        for (k = 1; k < biggest_pile; k++) {
          counter = 0;
          current = board[0][i][0 + k];
          for (j = 0; j < size; j++) {
            if (board[j][i][j + k] == current) {
              counter++;
            } else {
              break;
            }
          }
          if (counter == size) {
            if (current == 'x') {
              xwin = +1;
            } else if (current == 'o') {
              ywin += 1;
            }
          }
        }

      }
      /* check steps desc rows */
      counter = 0;
      current = 'x';
      for (i = 0; i < size; i++) {
        for (k = 1; k < biggest_pile; k++) {
          counter = 0;
          current = board[0][i][0 + k];
          for (j = 1; j <= size; j++) {
            if (board[size - j][i][j + k - 1] == current) {
              counter++;
            } else {
              break;
            }
          }
          if (counter == size) {
            if (current == 'x') {
              xwin = +1;
            } else if (current == 'o') {
              ywin += 1;
            }
          }
        }

      }
      /*check steps desc column */
      counter = 0;
      current = 'x';
      for (i = 0; i < size; i++) {
        for (k = 1; k < biggest_pile; k++) {
          counter = 0;
          current = board[i][0][0 + k];
          for (j = 1; j <= size; j++) {
            if (board[i][size - j][j + k - 1] == current) {
              counter++;
            } else {
              break;
            }
          }
          if (counter == size) {
            if (current == 'x') {
              xwin = +1;
            } else if (current == 'o') {
              ywin += 1;
            }
          }
        }

        /* check steps diags des */
        counter = 0;
        current = 'x';
        for (k = 1; k < biggest_pile; k++) {
          counter = 0;
          current = board[size - 1][size - 1][0 + k];
          for (j = 1; j <= size; j++) {
            if (board[size - j][size - j][j + k - 1] == current) {
              counter++;
            } else {
              break;
            }
          }
          if (counter == size) {
            if (current == 'x') {
              xwin = +1;
            } else if (current == 'o') {
              ywin += 1;
            }
          }

        }
        /* check steps diags asc */
        counter = 0;
        current = 'x';
        for (k = 1; k < biggest_pile; k++) {
          counter = 0;
          current = board[0][0][0 + k];
          for (j = 0; j < size; j++) {
            if (board[j][j][j + k] == current) {
              counter++;
            } else {
              break;
            }
          }
          if (counter == size) {
            if (current == 'x') {
              xwin = +1;
            } else if (current == 'o') {
              ywin += 1;
            }
          }
        }
        /* check steps anti diags des */
        counter = 0;
        current = 'x';
        for (k = 1; k < biggest_pile; k++) {
          counter = 0;
          current = board[0][size - 1][0 + k];
          for (j = 1; j <= size; j++) {
            if (board[j - 1][size - j][j + k - 1] == current) {
              counter++;
            } else {
              break;
            }
          }
          if (counter == size) {
            if (current == 'x') {
              xwin = +1;
            } else if (current == 'o') {
              ywin += 1;
            }
          }

        }
        /*check steps anti diags asc */
        counter = 0;
        current = 'x';
        for (k = 1; k < biggest_pile; k++) {
          counter = 0;
          current = board[size - 1][0][0 + k];
          for (j = 0; j < size; j++) {
            if (board[size - j - 1][j][j + k] == current) {
              counter++;
            } else {
              break;
            }
          }
          if (counter == size) {
            if (current == 'x') {
              xwin = +1;
            } else if (current == 'o') {
              ywin += 1;
            }
          }
        }

      }
    }
  }
  if (xwin > ywin) {
    return 0;
  } else if (ywin > xwin) {
    return 1;
  } else if (ywin == xwin && xwin > 0) {
    return 2;
  }
  return -1;
}
