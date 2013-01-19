/*
** rules.c for  in /home/pprost/c/ai/gomoku
**
** Made by  Prost P.
** Login   <pprost@epitech.net>
**
** Started on  Sat Dec  1 18:23:29 2012 Prost P.
*/

#define          _BSD_SOURCE
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include <inttypes.h>

#include "board.h"
#include "manip_boards.h"

union		u_chemical_cheddar
{
  long		c;	/* cheddar */
  char		l[8];	/* lerdammer */
};
typedef  union u_chemical_cheddar	t_chemical_cheddar;

#define OPPOSITE(COLOR) ((~COLOR) & 0x3)
#define CHCKPOS(X) ((X > 19) ? (19) : (X))
#define TAKE(BOARD, X, Y) (set_board(BOARD, X, Y, EMPTY));
/*
**	consome ketchup
**	renvoie l'incrémentation sur l'axe x (0x80=-1, 0x40=+1, 0xC0=wtf)
*/
#define	GETKETCHUP(X, S) (X  - ((S & 0x80) >> 7) + ((S & 0x40) >> 6))
#define	GETDOUBLEKETCHUP(X, S) (X  - (2 * ((S & 0x80)) >> 7) + (2 * ((S & 0x40)) >> 6))

/*
**	consome mayo
**	même que le ketchup, avec de la mayo.
*/
#define	GETMAYO(Y, S) (Y  - ((S & 0x20) >> 5) + ((S & 0x10) >> 4))
#define	GETDOUBLEMAYO(Y, S) (Y  - (2 * ((S & 0x20) >> 5)) + (2 * ((S & 0x10)) >> 4))

/*
**	revoie la taile des lignes depuis de points dans les 8 directions]
**	consome du ketchup et de la mayo
*/
long	drec(t_board *board, int color, long d, int sen, register unsigned int x, register unsigned int y)
{
  if (get_board(board, x, y) == (OPPOSITE(color)))
    {
       ((char*)&d)[sen & 0x0f] |= BLOCKED;
      return (d);
    }
  if ((x <= 0 && x > 19 && y <= 0 && y > 19) || get_board(board, x, y) != color)
    return (d);
  ((char*)&d)[sen & 0x0f]++;
  return (drec(board, color, d, sen, GETKETCHUP(x, sen), GETMAYO(y, sen)));
}

long	longdrec(t_board *board, int color, long d, int sen, register unsigned int x, register unsigned int y)
{
  if (get_board(board, x, y) == (OPPOSITE(color)))
    {
       ((char*)&d)[sen & 0x0f] |= BLOCKED;
      return (d);
    }
  if ((x <= 0 && x > 19 && y <= 0 && y > 19) || get_board(board, x, y) == EMPTY)
  {
    if (((t_chemical_cheddar)drec(board, color, d, sen, GETKETCHUP(x, sen), GETMAYO(y, sen))).l[sen & 0x0f] == 2)
      ((char*)&d)[sen & 0x0f] += 2;
    return (d);
  }
  ((char*)&d)[sen & 0x0f]++;
   return (drec(board, color, d, sen, GETKETCHUP(x, sen), GETMAYO(y, sen)));
}

long	longgetlines(t_board *board, int color, unsigned int x, unsigned int y)
{
  long	d;

  d = 0;
  d = longdrec(board, color, d, UP_L, x - 1, y - 1);
  d = longdrec(board, color, d, UP_C, x - 1, y);
  d = longdrec(board, color, d, UP_R, x - 1, y + 1);
  d = longdrec(board, color, d, MI_L, x, y - 1);
  d = longdrec(board, color, d, MI_R, x, y + 1);
  d = longdrec(board, color, d, DO_L, x + 1, y - 1);
  d = longdrec(board, color, d, DO_C, x + 1, y);
  d = longdrec(board, color, d, DO_R, x + 1, y + 1);
  return (d);
}

long	getlines(t_board *board, int color, unsigned int x, unsigned int y)
{
  long	d;

  d = 0;
  d = drec(board, color, d, UP_L, x - 1, y - 1);
  d = drec(board, color, d, UP_C, x - 1, y);
  d = drec(board, color, d, UP_R, x - 1, y + 1);
  d = drec(board, color, d, MI_L, x, y - 1);
  d = drec(board, color, d, MI_R, x, y + 1);
  d = drec(board, color, d, DO_L, x + 1, y - 1);
  d = drec(board, color, d, DO_C, x + 1, y);
  d = drec(board, color, d, DO_R, x + 1, y + 1);
  return (d);
}


int	rule3(t_board *board,  int x,  int y, char color)
{
  long	res;
  int	counter;
  
  res = longgetlines(board, color, x, y);
  counter = (((t_chemical_cheddar)res).l[UP_L & 0x0f] == 2)
    + (((t_chemical_cheddar)res).l[UP_C & 0x0f] == 2)
    + (((t_chemical_cheddar)res).l[UP_R & 0x0f] == 2)
    + (((t_chemical_cheddar)res).l[MI_L & 0x0f] == 2)
    + (((t_chemical_cheddar)res).l[MI_R & 0x0f] == 2)
    + (((t_chemical_cheddar)res).l[DO_L & 0x0f] == 2)
    + (((t_chemical_cheddar)res).l[DO_C & 0x0f] == 2)
    + (((t_chemical_cheddar)res).l[DO_R & 0x0f] == 2);
  if (counter >= 2)
    return (0);
  return (1);
}


/*
** Parc national d'if - région Ardèche
*/
int	prise(t_board *board, unsigned int x, unsigned int y)
{
  /* printf("%i %i\n", WHITE, OPPOSITE(WHITE)); */
  /* printf("%i %i\n", BLACK, OPPOSITE(BLACK)); */
  if ((get_board(board, x + 1, y) == OPPOSITE(get_board(board, x, y))
       && (get_board(board, x + 2, y) == OPPOSITE(get_board(board, x, y)))
       && (get_board(board, x + 3, y) == get_board(board, x, y))))
    {
      /* printf("prend - 1\n"); */
      TAKE(board, x + 1, y);
      TAKE(board, x + 2, y);
      board->eaten[board->blacks + board->whites + 0] = x + 1;
      board->eaten[board->blacks + board->whites + 1] = y;
      board->eaten[board->blacks + board->whites + 2] = x + 2;
      board->eaten[board->blacks + board->whites + 3] = y;
      return (1);
    }
  if ((get_board(board, x, y + 1) == OPPOSITE(get_board(board, x, y))
       && (get_board(board, x, y + 2) == OPPOSITE(get_board(board, x, y)))
       && (get_board(board, x, y + 3) == get_board(board, x, y))))
    {
      /* printf("prend | 1\n"); */
      TAKE(board, x, y + 1);
      TAKE(board, x, y + 2);
      board->eaten[board->blacks + board->whites + 0] = x;
      board->eaten[board->blacks + board->whites + 1] = y + 1;
      board->eaten[board->blacks + board->whites + 2] = x;
      board->eaten[board->blacks + board->whites + 3] = y + 2;
      return (1);
    }
  if ((get_board(board, x + 1, y + 1) == OPPOSITE(get_board(board, x, y))
       && (get_board(board, x + 2, y + 2) == OPPOSITE(get_board(board, x, y)))
       && (get_board(board, x + 3, y + 3) == get_board(board, x, y))))
    {
      /* printf("prend \ 1\n"); */
      TAKE(board, x + 1, y + 1);
      TAKE(board, x + 2, y + 2);
      board->eaten[board->blacks + board->whites + 0] = x + 1;
      board->eaten[board->blacks + board->whites + 1] = y + 1;
      board->eaten[board->blacks + board->whites + 2] = x + 2;
      board->eaten[board->blacks + board->whites + 3] = y + 2;
      return (1);
    }
  if ((get_board(board, x + 1, y - 1) == OPPOSITE(get_board(board, x, y))
       && (get_board(board, x + 2, y - 2) == OPPOSITE(get_board(board, x, y)))
       && (get_board(board, x + 3, y - 3) == get_board(board, x, y))))
    {
      /* printf("prend / 1\n"); */
      TAKE(board, x + 1, y - 1);
      TAKE(board, x + 2, y - 2);
      board->eaten[board->blacks + board->whites + 0] = x + 1;
      board->eaten[board->blacks + board->whites + 1] = y - 1;
      board->eaten[board->blacks + board->whites + 2] = x + 2;
      board->eaten[board->blacks + board->whites + 3] = y - 2;
      return (1);
    }


  if ((get_board(board, x - 1, y) == OPPOSITE(get_board(board, x, y))
       && (get_board(board, x - 2, y) == OPPOSITE(get_board(board, x, y)))
       && (get_board(board, x - 3, y) == get_board(board, x, y))))
    {
      /* printf("prend - 2\n"); */
      TAKE(board, x - 1, y);
      TAKE(board, x - 2, y);
      board->eaten[board->blacks + board->whites + 0] = x - 1;
      board->eaten[board->blacks + board->whites + 1] = y;
      board->eaten[board->blacks + board->whites + 2] = x - 2;
      board->eaten[board->blacks + board->whites + 3] = y;
      return (1);
    }
  if ((get_board(board, x, y - 1) == OPPOSITE(get_board(board, x, y))
       && (get_board(board, x, y - 2) == OPPOSITE(get_board(board, x, y)))
       && (get_board(board, x, y - 3) == get_board(board, x, y))))
    {
      /* printf("prend | 2\n"); */
      TAKE(board, x, y - 1);
      TAKE(board, x, y - 2);
      board->eaten[board->blacks + board->whites + 0] = x;
      board->eaten[board->blacks + board->whites + 1] = y - 1;
      board->eaten[board->blacks + board->whites + 2] = x;
      board->eaten[board->blacks + board->whites + 3] = y - 2;
      return (1);
    }
  if ((get_board(board, x - 1, y - 1) == OPPOSITE(get_board(board, x, y))
       && (get_board(board, x - 2, y - 2) == OPPOSITE(get_board(board, x, y)))
       && (get_board(board, x - 3, y - 3) == get_board(board, x, y))))
    {
      /* printf("prend \ 2\n"); */
      TAKE(board, x - 1, y - 1);
      TAKE(board, x - 2, y - 2);
      board->eaten[board->blacks + board->whites + 0] = x - 1;
      board->eaten[board->blacks + board->whites + 1] = y - 1;
      board->eaten[board->blacks + board->whites + 2] = x - 2;
      board->eaten[board->blacks + board->whites + 3] = y - 2;
      return (1);
    }
  if ((get_board(board, x - 1, y + 1) == OPPOSITE(get_board(board, x, y))
       && (get_board(board, x - 2, y + 2) == OPPOSITE(get_board(board, x, y)))
       && (get_board(board, x - 3, y + 3) == get_board(board, x, y))))
    {
      /* printf("prend / 2\n"); */
      TAKE(board, x - 1, y + 1);
      TAKE(board, x - 2, y + 2);
      board->eaten[board->blacks + board->whites + 0] = x - 1;
      board->eaten[board->blacks + board->whites + 1] = y + 1;
      board->eaten[board->blacks + board->whites + 2] = x - 2;
      board->eaten[board->blacks + board->whites + 3] = y + 2;
      return (1);
    }
  return (0);
}

#define HAZHAMBURGER(BOARD, X, Y) (!!!!get_board(BOARD, X, Y)) /* ZOMG EXCLAMATION MARKR */
#define HAZCHEEZBURGER(BOARD, X, Y, COLOR) (get_board(BOARD, X, Y) == COLOR) /* ZOMG EXCLAMATION MARKR */
#define COUNTHAMBURGER(BOARD, X0, Y0, X1, Y1, X2, Y2, X3, Y3, XX, YX, XX2, YX2, COLOR) \
  (HAZCHEEZBURGER(BOARD, X0, Y0, COLOR)					\
   + (HAZCHEEZBURGER(BOARD, X1, Y1, COLOR))				\
   + (HAZCHEEZBURGER(BOARD, X2, Y2, COLOR))				\
   + 1									\
   - (12	* ((((get_board(BOARD, XX, YX)) || (XX == -1) || YX == -1)) \
		   || ((get_board(BOARD, XX2, YX2)) || (XX2 == 19) || YX2 == 19))))

/*
** banc de macros élevés en pleine mer 
*/
/* int	rule3(t_board *board,  int x,  int y, char color) */
/* { */
/*   /\* printf("%d\n", COUNTHAMBURGER(board, x - 3, y,  x - 2, y,  x - 1, y,  x, y,  x - 4, y,  x + 1, y,  color)); *\/ */

/*   if (COUNTHAMBURGER(board, x - 3, y,  x - 2, y,  x - 1, y,  x, y,  x - 4, y,  x + 1, y,  color)  == 4) */
/*     return (0); */
/*   if (COUNTHAMBURGER(board, x + 3, y, x + 2, y,  x + 1, y,  x, y,  x - 1 , y,  x + 4, y,  color)  == 4) */
/*     return (0); */
/*   if (COUNTHAMBURGER(board, x - 1 , y,  x + 1, y,  x + 2, y, x, y,  x - 2 , y,  x + 3, y,  color)  == 4) */
/*     return (0); */
/*   if (COUNTHAMBURGER(board, x - 2 , y,  x - 1, y,  x + 1, y, x, y,  x - 3 , y,  x + 2, y,  color)  == 4) */
/*     return (0); */

/*   if (COUNTHAMBURGER(board, x, y - 3, x, y - 2,  x, y - 1, x, y, x, y - 4,  x, y + 1,  color)  == 4) */
/*     return (0); */
/*   if (COUNTHAMBURGER(board, x, y + 3, x, y + 2,  x, y + 1, x, y, x, y - 1,  x, y + 4,  color)  == 4) */
/*     return (0); */
/*   if (COUNTHAMBURGER(board, x, y - 1, x, y + 1,  x, y + 2, x, y, x, y - 2,  x, y + 3,  color)  == 4) */
/*     return (0); */
/*   if (COUNTHAMBURGER(board, x, y - 2, x, y - 1,  x, y + 1, x, y, x, y - 3,  x, y + 2,  color)  == 4) */
/*     return (0); */

/*   if (COUNTHAMBURGER(board, x - 3, y - 3, x - 2, y - 2,  x - 1, y - 1, x, y, x - 4, y - 4,  x + 1, y + 1,  color)  == 4) */
/*     return (0); */
/*   if (COUNTHAMBURGER(board, x + 3, y + 3, x + 2, y + 2,  x + 1, y + 1, x, y, x - 1, y - 1,  x + 4, y + 4,  color)  == 4) */
/*     return (0); */
/*   if (COUNTHAMBURGER(board, x - 1, y - 1, x + 1, y + 1,  x + 2, y + 2, x, y, x - 2, y - 2,  x + 3, y + 3,  color)  == 4) */
/*     return (0); */
/*   if (COUNTHAMBURGER(board, x - 2, y - 2, x - 1, y - 1,  x + 1, y + 1, x, y, x - 3, y - 3,  x + 2, y + 2,  color)  == 4) */
/*     return (0); */

/*   if (COUNTHAMBURGER(board, x - 1, y + 1, x - 2, y + 2,  x - 3, y + 3, x, y, x + 1, y - 1,  x - 4, y + 4,  color)  == 4) */
/*     return (0); */
/*   if (COUNTHAMBURGER(board, x - 2, y + 2, x - 1, y + 1,  x + 1, y - 1, x, y, x - 3, y + 3,  x + 2, y - 2,  color)  == 4) */
/*     return (0); */
/*   if (COUNTHAMBURGER(board, x + 3, y - 3, x + 2, y - 2,  x + 1, y - 1, x, y, x + 4, y - 4,  x - 1, y + 1,  color)  == 4) */
/*     return (0); */

/*   return (1); */
/* } */


/*
** car on ne sait pas si il a prit un cheezburger ou un l'opposé d'u hamburger
*/
#define VERIFIELECHAT(BOARD, X, Y, COLOR)				\
   ((HAZCHEEZBURGER(BOARD, X - 1, Y, COLOR) && (HAZCHEEZBURGER(BOARD, X - 2, Y, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X + 1, Y))) \
    || (HAZCHEEZBURGER(BOARD, X + 1, Y, COLOR) && (HAZCHEEZBURGER(BOARD, X + 2, Y, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X - 1, Y))) \
									\
   || (HAZCHEEZBURGER(BOARD, X, Y - 1, COLOR) && (HAZCHEEZBURGER(BOARD, X, Y - 2, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X, Y + 1))) \
   || (HAZCHEEZBURGER(BOARD, X, Y + 1, COLOR) && (HAZCHEEZBURGER(BOARD, X, Y + 2, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X, Y - 1))) \
   									\
   || (HAZCHEEZBURGER(BOARD, X + 1, Y + 1, COLOR) && (HAZCHEEZBURGER(BOARD, X + 2, Y + 2, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X - 1, Y + 1))) \
   || (HAZCHEEZBURGER(BOARD, X - 1, Y - 1, COLOR) && (HAZCHEEZBURGER(BOARD, X - 2, Y - 2, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X + 1, Y + 1))) \
   									\
   || (HAZCHEEZBURGER(BOARD, X - 1, Y + 1, COLOR) && (HAZCHEEZBURGER(BOARD, X - 2, Y + 2, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X + 1, Y - 1))) \
   || (HAZCHEEZBURGER(BOARD, X + 1, Y - 1, COLOR) && (HAZCHEEZBURGER(BOARD, X + 2, Y - 2, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X - 1, Y + 1)))\
									\
   || (HAZCHEEZBURGER(BOARD, X - 1, Y, COLOR) && (HAZCHEEZBURGER(BOARD, X + 1, Y, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X - 2, Y))) \
   || (HAZCHEEZBURGER(BOARD, X + 1, Y, COLOR) && (HAZCHEEZBURGER(BOARD, X - 1, Y, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X + 2, Y))) \
									\
   || (HAZCHEEZBURGER(BOARD, X, Y - 1, COLOR) && (HAZCHEEZBURGER(BOARD, X, Y + 1, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X, Y - 2))) \
   || (HAZCHEEZBURGER(BOARD, X, Y + 1, COLOR) && (HAZCHEEZBURGER(BOARD, X, Y - 1, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X, Y + 2))) \
   									\
   || (HAZCHEEZBURGER(BOARD, X + 1, Y + 1, COLOR) && (HAZCHEEZBURGER(BOARD, X - 1, Y - 1, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X + 2, Y + 2))) \
   || (HAZCHEEZBURGER(BOARD, X - 1, Y - 1, COLOR) && (HAZCHEEZBURGER(BOARD, X + 1, Y + 1, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X - 2, Y - 2))) \
   									\
   || (HAZCHEEZBURGER(BOARD, X - 1, Y + 1, COLOR) && (HAZCHEEZBURGER(BOARD, X + 1, Y - 1, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X - 2, Y + 2))) \
    || (HAZCHEEZBURGER(BOARD, X + 1, Y - 1, COLOR) && (HAZCHEEZBURGER(BOARD, X - 1, Y + 1, OPPOSITE(COLOR)) && !HAZHAMBURGER(BOARD, X + 2, Y - 2))))

int	rule5(t_board *board,  int x,  int y, char color)
{
  if ((!VERIFIELECHAT(board, x, y, color) && HAZCHEEZBURGER(board, x, y, color))
      && (!VERIFIELECHAT(board, x + 1, y, color) && HAZCHEEZBURGER(board, x + 1, y, color))
      && (!VERIFIELECHAT(board, x + 2, y, color) && HAZCHEEZBURGER(board, x + 2, y, color))
      && (!VERIFIELECHAT(board, x + 3, y, color) && HAZCHEEZBURGER(board, x + 3, y, color))
      && (!VERIFIELECHAT(board, x + 4, y, color) && HAZCHEEZBURGER(board, x + 4, y, color)))
    return (1);
  if ((!VERIFIELECHAT(board, x, y, color) && HAZCHEEZBURGER(board, x, y, color))
      && (!VERIFIELECHAT(board, x, y + 1, color) && HAZCHEEZBURGER(board, x, y + 1, color))
      && (!VERIFIELECHAT(board, x, y + 2, color) && HAZCHEEZBURGER(board, x, y + 2, color))
      && (!VERIFIELECHAT(board, x, y + 3, color) && HAZCHEEZBURGER(board, x, y + 3, color))
      && (!VERIFIELECHAT(board, x, y + 4, color) && HAZCHEEZBURGER(board, x, y + 4, color)))
    return (1);

  if ((!VERIFIELECHAT(board, x, y, color) && HAZCHEEZBURGER(board, x, y, color))
      && (!VERIFIELECHAT(board, x + 1, y + 1, color) && HAZCHEEZBURGER(board, x + 1, y + 1, color))
      && (!VERIFIELECHAT(board, x + 2, y + 2, color) && HAZCHEEZBURGER(board, x + 2, y + 2, color))
      && (!VERIFIELECHAT(board, x + 3, y + 3, color) && HAZCHEEZBURGER(board, x + 3, y + 3, color))
      && (!VERIFIELECHAT(board, x + 4, y + 4, color) && HAZCHEEZBURGER(board, x + 4, y + 4, color)))
    return (1);
  if ((!VERIFIELECHAT(board, x, y, color) && HAZCHEEZBURGER(board, x, y, color))
      && (!VERIFIELECHAT(board, x + 1, y - 1, color + 1) && HAZCHEEZBURGER(board, x + 1, y - 1, color))
      && (!VERIFIELECHAT(board, x + 2, y - 2, color + 2) && HAZCHEEZBURGER(board, x + 2, y - 2, color))
      && (!VERIFIELECHAT(board, x + 3, y - 3, color + 3) && HAZCHEEZBURGER(board, x + 3, y - 3, color))
      && (!VERIFIELECHAT(board, x + 4, y - 4, color + 4) && HAZCHEEZBURGER(board, x + 4, y - 4, color)))
    return (1);
  return (0);
}
