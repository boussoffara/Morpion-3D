/*
 *      __  __    ___    ___   ___   ___    ___    _  _
 *     |  \/  |  / _ \  | _ \ | _ \ |_ _|  / _ \  | \| |
 *     | |\/| | | (_) | |   / |  _/  | |  | (_) | | .` |
 *     |_|  |_|  \___/  |_|_\ |_|   |___|  \___/  |_|\_|
 *
 *     @Author: squeamish ossifrage
 *    @File: the explosion animation
 */

#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <math.h>
#include "header.h"
#include "explosion.h"

double prng() {
  static long long s = 1;
  s = s * 1488248101 + 981577151;
  return ((s % 65536) - 32768) / 32768.0;
}

void explosion() {
  char * frames[NUM_FRAMES], * p;
  int i, j, x, y, v, t, rows, cols, ith, i0;
  t = 0;
  t += 1;
  t = 0;
  ith = 0;
  ith += 1;
  ith = 0;
  int maxx, minx, maxy, miny, delay = 1E6;
  double bx, by, bz, br, r, th;
  th = 0;
  spaceblob * blobs;

  /* Initialize ncurses and get window dimensions */
  initscr();
  getmaxyx(stdscr, rows, cols);
  minx = -cols / 2;
  maxx = cols + minx - 1;
  miny = -rows / 2;
  maxy = rows + miny - 1;

  /* Generate random blob coordinates */
  blobs = malloc(NUM_BLOBS * sizeof(spaceblob));
  for (i = 0; i < NUM_BLOBS; i++) {
    bx = prng();
    by = prng();
    bz = prng();
    br = sqrt(bx * bx + by * by + bz * bz);
    blobs[i].x = (bx / br) * (1.3 + 0.2 * prng());
    blobs[i].y = (0.5 * by / br) * (1.3 + 0.2 * prng());;
    blobs[i].z = (bz / br) * (1.3 + 0.2 * prng());;
  }

  /* Generate animation frames */
  for (i = 0; i < NUM_FRAMES; i++) {
    t = (1. * i) / NUM_FRAMES;
    p = frames[i] = malloc(cols * rows + 1);
    for (y = miny; y <= maxy; y++) {
      for (x = minx; x <= maxx; x++) {

        /* Show a single '*' in first frame */
        if (i == 0) { * p++ = (x == 0 && y == 0) ? '*' : ' ';
          continue;
        }

        /* Show expanding star in next 7 frames */
        if (i < 8) {
          r = sqrt(x * x + 4 * y * y); * p++ = (r < i * 2) ? '@' : ' ';
          continue;
        }

        /* Otherwise show blast wave */
        r = sqrt(x * x + 4 * y * y) * (0.5 + (prng() / 3.0) * cos(16. * atan2(y * 2. + 0.01, x + 0.01)) * .3);
        ith = 32 + th * 32. * 0.318;
        v = i - r - 7;
        if (v < 0) * p++ = (i < 19) ? "%@W#H=+~-:." [i - 8] : ' '; /* initial flash */
        else if (v < 20) * p++ = " .:!HIOMW#%$&@08O=+-" [v];
        else *p++ = ' ';
      }
    }

    /* Add blobs with perspective effect */
    if (i > 6) {
      i0 = i - 6;
      for (j = 0; j < NUM_BLOBS; j++) {
        bx = blobs[j].x * i0;
        by = blobs[j].y * i0;
        bz = blobs[j].z * i0;
        if (bz < 5 - PERSPECTIVE || bz > PERSPECTIVE) continue;
        x = cols / 2 + bx * PERSPECTIVE / (bz + PERSPECTIVE);
        y = rows / 2 + by * PERSPECTIVE / (bz + PERSPECTIVE);
        if (x >= 0 && x < cols && y >= 0 && y < rows)
          frames[i][x + y * cols] = (bz > 40) ? '.' : (bz > -20) ? 'o' : '@';
      }
    }

    /* Terminate the text string for this frame */
    * p = '\0';
  }

  /* Now play back the frames in sequence */
  for (i = 0; i < NUM_FRAMES; i++) {
    erase();
    mvaddstr(0, 0, frames[i]);
    refresh();
    usleep(delay);
    delay = 33333; /* Change to 30fps after first frame */
  }
}
