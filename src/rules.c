/*
** rules.c for  in /home/pprost/c/ai/gomoku
** 
** Made by  Prost P.
** Login   <pprost@epitech.net>
** 
** Started on  Sat Dec  1 18:23:29 2012 Prost P.
** Last update Mon Dec  3 00:38:59 2012 Prost P.
*/

#define          _BSD_SOURCE
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include <inttypes.h>
 
#include "board.h"
#include "manip_boards.h"

typedef unsigned __int128 sint;

//		32bits LE
//		0b11101011
#define TAKE_X	(sint)(0b11000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)

//		128bits LE
/* 1000000000000000000 */
/* 1000000000000000000 */
/* 0000000000000000000 */
/* 0000000000000000000 */
/* 0000000000000000000 */
/* 0000000000000000000 */
/* 00000000 */
#define TAKE_Y (sint)(0b10000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)

//		128bits LE
/* 1000000000000000000 */
/* 0100000000000000000 */
/* 0000000000000000000 */
/* 0000000000000000000 */
/* 0000000000000000000 */
/* 0000000000000000000 */
/* 00000000 */
#define TAKE_XY1 (sint)(0b10000000000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)

//		128bits LE
/* 1000000000000000001 */
/* 0000000000000000000 */
/* 0000000000000000000 */
/* 0000000000000000000 */
/* 0000000000000000000 */
/* 0000000000000000000 */
/* 00000000 */
#define TAKE_XY2 (sint)(0b10000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)


//              128bits LE
/* 1000000000000000001 */
/* 0000000000000000000 */
/* 0000000000000000000 */
/* 0000000000000000000 */
/* 0000000000000000000 */
/* 0000000000000000000 */
/* 00000000 */
#define RULE3_X1 (sint)(0b11110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000)

//              128bits LE
/* 1000000000000000001 */
/* 1000000000000000000 */
/* 1000000000000000000 */
/* 1000000000000000000 */
/* 0000000000000000000 */
/* 0000000000000000000 */
/* 00000000 */
#define RULE3_X2   (uint128_t)(10000000000000000000100000000000000000010000000000000000001000000000000000000000000000000000000000000000000000000000000000000000)

typedef __uint128_t uint128_t;

#define TEST (uint128_t)(0xFFFFFFFFFFFFFFFFF)



int print_uint128(uint128_t n) {
  if (n == 0)  return printf("0\n");

  char str[40] = {0}; // log10(1 << 128) + '\0'
  char *s = str + sizeof(str) - 1; // start at the end
  while (n != 0) {
    if (s == str) return -1; // never happens

    *--s = "01"[n % 2]; // save last digit
    n /= 2 ;                     // drop it
  }
  return printf("%s\n", s);
}

#define OPPOSITE(COLOR) (~(COLOR) & 0b00000011)
#define CHCKPOS(X) ((X > 19) ? (19) : (X))
#define TAKE(BOARD, X, Y) (set_board(BOARD, X, Y, EMPTY));

int	prise(t_board *board, unsigned int x, unsigned int y)
{
  /* printf("%i %i\n", WHITE, OPPOSITE(WHITE)); */
  /* printf("%i %i\n", BLACK, OPPOSITE(BLACK)); */

  if ((get_board(board, x + 1, y) == OPPOSITE(get_board(board, x, y))
       && (get_board(board, x + 2, y) == OPPOSITE(get_board(board, x, y)))
       && (get_board(board, x + 3, y) == get_board(board, x, y))))
    {
      printf("prend Hori\n");
     TAKE(board, x + 1, y);
     TAKE(board, x + 2, y);
    }
  if ((get_board(board, x, y + 1) == OPPOSITE(get_board(board, x, y))
       && (get_board(board, x, y + 2) == OPPOSITE(get_board(board, x, y)))
       && (get_board(board, x, y + 3) == get_board(board, x, y))))
    {
      printf("prend V\n");
      TAKE(board, x, y + 1);
      TAKE(board, x, y + 2);
    }
  if ((get_board(board, x + 1, y + 1) == OPPOSITE(get_board(board, x, y))
       && (get_board(board, x + 2, y + 2) == OPPOSITE(get_board(board, x, y)))
       && (get_board(board, x + 3, y + 3) == get_board(board, x, y))))
    {
      printf("prend D\n");
      TAKE(board, x + 1, y + 1);
      TAKE(board, x + 2, y + 2);
    }
  return ((int)"OVER 9000"[2] * unix);
}

#define HAZHAMBURGER(BOARD, X, Y) (!!!!get_board(BOARD, X, Y)) /* ZOMG EXCLAMATION MARKR */
#define HAZCHEEZBURGER(BOARD, X, Y, COLOR) (get_board(BOARD, X, Y) == COLOR) /* ZOMG EXCLAMATION MARKR */
#define COUNTHAMBURGER(BOARD, X0, Y0, X1, Y1, X2, Y2, X3, Y3, XX, YX, XX2, YX2, COLOR) \
  (HAZCHEEZBURGER(BOARD, X0, Y0, COLOR)					\
   + (HAZCHEEZBURGER(BOARD, X1, Y1, COLOR))				\
   + (HAZCHEEZBURGER(BOARD, X2, Y2, COLOR))				\
   + 1									\
   - (12	* ((((get_board(BOARD, XX, YX) == OPPOSITE(COLOR)) | (XX == -1))) \
		   | ((get_board(BOARD, XX2, YX2) == OPPOSITE(COLOR)) | (XX2 == 19))) ))

int	rule3(t_board *board, unsigned int x, unsigned int y, char color)
{
  /* printf("%d\n", COUNTHAMBURGER(board, x - 3, y,  x - 2, y,  x - 1, y,  x, y,  x - 4, y,  x + 1, y,  color)); */

  if (COUNTHAMBURGER(board, x - 3, y,  x - 2, y,  x - 1, y,  x, y,  x - 4, y,  x + 1, y,  color)  == 4)
    return (0);
  if (COUNTHAMBURGER(board, x + 3, y, x + 2, y,  x + 1, y,  x, y,  x - 1 , y,  x + 4, y,  color)  == 4)
    return (0);
  if (COUNTHAMBURGER(board, x - 1 , y,  x + 1, y,  x + 2, y, x, y,  x - 2 , y,  x + 3, y,  color)  == 4)
    return (0);
  if (COUNTHAMBURGER(board, x - 2 , y,  x - 1, y,  x + 1, y, x, y,  x - 3 , y,  x + 2, y,  color)  == 4)
    return (0);

  if (COUNTHAMBURGER(board, x, y - 3, x, y - 2,  x, y - 1, x, y, x, y - 4,  x, y + 1,  color)  == 4)
    return (0);
  if (COUNTHAMBURGER(board, x, y + 3, x, y + 2,  x, y + 1, x, y, x, y - 1,  x, y + 4,  color)  == 4)
    return (0);
  if (COUNTHAMBURGER(board, x, y - 1, x, y + 1,  x, y + 2, x, y, x, y - 2,  x, y + 3,  color)  == 4)
    return (0);
  if (COUNTHAMBURGER(board, x, y - 2, x, y - 1,  x, y + 1, x, y, x, y - 3,  x, y + 2,  color)  == 4)
    return (0);

  if (COUNTHAMBURGER(board, x - 3, y - 3, x - 2, y - 2,  x - 1, y - 1, x, y, x - 4, y - 4,  x + 1, y + 1,  color)  == 4)
    return (0);
  if (COUNTHAMBURGER(board, x + 3, y + 3, x + 2, y + 2,  x + 1, y + 1, x, y, x - 1, y - 1,  x + 4, y + 4,  color)  == 4)
    return (0);
  if (COUNTHAMBURGER(board, x - 1, y - 1, x + 1, y + 1,  x + 2, y + 2, x, y, x - 2, y - 2,  x + 3, y + 3,  color)  == 4)
    return (0);
  if (COUNTHAMBURGER(board, x - 2, y - 2, x - 1, y - 1,  x + 1, y + 1, x, y, x - 3, y - 3,  x + 2, y + 2,  color)  == 4)
    return (0);

  if (COUNTHAMBURGER(board, x + 3, y + 3, x + 2, y + 2,  x + 1, y + 1, x, y, x + 4, y + 4,  x + 1, y + 1,  color)  == 4)
    return (0);
  if (COUNTHAMBURGER(board, x + 3, y + 3, x + 2, y + 2,  x + 1, y + 1, x, y, x + 1, y + 1,  x + 4, y + 4,  color)  == 4)
    return (0);
  if (COUNTHAMBURGER(board, x + 1, y + 1, x + 1, y + 1,  x + 2, y + 2, x, y, x + 2, y + 2,  x + 3, y + 3,  color)  == 4)
    return (0);
  if (COUNTHAMBURGER(board, x + 2, y + 2, x + 1, y + 1,  x + 1, y + 1, x, y, x + 3, y + 3,  x + 2, y + 2,  color)  == 4)
    return (0);

  return (1);
}
