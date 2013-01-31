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
  unsigned char	fl[8];	/* fake lerdammer */
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
  if ((x >= 19 || y >= 19) || (get_board(board, x, y) == (OPPOSITE(color))))
    {
       ((char*)&d)[sen & 0x0f] |= BLOCKED;
      return (d);
    }
  if (get_board(board, x, y) != color)
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
  if ((x >= 19 || y >= 19) || get_board(board, x, y) == EMPTY)
  {
    if (((t_chemical_cheddar)drec(board, color, d, sen, GETKETCHUP(x, sen), GETMAYO(y, sen))).l[sen & 0x0f] == 2)
      ((char*)&d)[sen & 0x0f] += 2;
    return (d);
  }
  ((char*)&d)[sen & 0x0f]++;
   return (drec(board, color, d, sen, GETKETCHUP(x, sen), GETMAYO(y, sen)));
}

long	prisedrec(t_board *board, int color, long d, int sen, register unsigned int x, register unsigned int y)
{
  if ((x >= 19 || y >= 19) || (get_board(board, x, y) == (OPPOSITE(color))))
    {
      ((char*)&d)[sen & 0x0f] |= BLOCKED;
      return (d);
    }
  if (isprenable(board, color, x, y))
    ((char*)&d)[sen & 0x0f] |= PRENABLE;
  if (get_board(board, x, y) != color)
    return (d);
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

long	prisegetlines(t_board *board, int color, unsigned int x, unsigned int y)
{
  long	d;

  d = 0;
  d = prisedrec(board, color, d, UP_L, x - 1, y - 1);
  d = prisedrec(board, color, d, UP_C, x - 1, y);
  d = prisedrec(board, color, d, UP_R, x - 1, y + 1);
  d = prisedrec(board, color, d, MI_L, x, y - 1);
  d = prisedrec(board, color, d, MI_R, x, y + 1);
  d = prisedrec(board, color, d, DO_L, x + 1, y - 1);
  d = prisedrec(board, color, d, DO_C, x + 1, y);
  d = prisedrec(board, color, d, DO_R, x + 1, y + 1);
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

int	getprise(t_board *board, unsigned int x, unsigned int y, int color)
{
  long  res;

  if (color == EMPTY)
    return (0);
  res = getlines(board, color, x, y);
  return ((((t_chemical_cheddar)res).fl[UP_L & 0x0fl] == 0x82)
	  + (((t_chemical_cheddar)res).fl[UP_C & 0x0f] == 0x82)
	  + (((t_chemical_cheddar)res).fl[UP_R & 0x0f] == 0x82)
	  + (((t_chemical_cheddar)res).fl[MI_L & 0x0f] == 0x82)
	  + (((t_chemical_cheddar)res).fl[MI_R & 0x0f] == 0x82)
	  + (((t_chemical_cheddar)res).fl[DO_R & 0x0f] == 0x82)
	  + (((t_chemical_cheddar)res).fl[DO_C & 0x0f] == 0x82)
	  + (((t_chemical_cheddar)res).fl[DO_L & 0x0f] == 0x82));
}

#define DOUBLETAKE(BOARD, X1, Y1, X2, Y2) (set_board(BOARD, X1, Y1, EMPTY)); \
  (set_board(BOARD, X2, Y2, EMPTY));

int	rule3(t_board *board,  int x,  int y, char color)
{
  long	res;
  int	counter = 0;
  
  res = longgetlines(board, color, x, y);
  
  counter = (((t_chemical_cheddar)res).l[UP_L & 0x0f] == 2 && ((t_chemical_cheddar)res).l[DO_R & 0x0f] != -128)
    + (((t_chemical_cheddar)res).l[UP_C & 0x0f] == 2 && ((t_chemical_cheddar)res).l[DO_C & 0x0f] != -128)
    + (((t_chemical_cheddar)res).l[UP_R & 0x0f] == 2 && ((t_chemical_cheddar)res).l[DO_L & 0x0f] != -128)
    + (((t_chemical_cheddar)res).l[MI_L & 0x0f] == 2 && ((t_chemical_cheddar)res).l[MI_R & 0x0f] != -128)
    + (((t_chemical_cheddar)res).l[MI_R & 0x0f] == 2 && ((t_chemical_cheddar)res).l[MI_L & 0x0f] != -128)
    + (((t_chemical_cheddar)res).l[DO_L & 0x0f] == 2 && ((t_chemical_cheddar)res).l[UP_R & 0x0f] != -128)
    + (((t_chemical_cheddar)res).l[DO_C & 0x0f] == 2 && ((t_chemical_cheddar)res).l[UP_C & 0x0f] != -128)
    + (((t_chemical_cheddar)res).l[DO_R & 0x0f] == 2 && ((t_chemical_cheddar)res).l[UP_L & 0x0f] != -128);
  if (counter >= 2)
    return (0);
  return (1);
}

int	prise(t_board *board, unsigned int x, unsigned int y, int color)
{
  long	res;

  if (color == EMPTY)
    return (0);
  res = getlines(board, color, x, y);
  if (((t_chemical_cheddar)res).fl[UP_L & 0x0f] == 0x82)
    {DOUBLETAKE (board, x - 1, y - 1, x - 2, y - 2);}
  if (((t_chemical_cheddar)res).fl[UP_C & 0x0f] == 0x82)
    {DOUBLETAKE (board, x - 1, y, x - 2, y);}
  if (((t_chemical_cheddar)res).fl[UP_R & 0x0f] == 0x82)
    {DOUBLETAKE (board, x - 1, y + 1, x - 2, y + 2);}
  if (((t_chemical_cheddar)res).fl[MI_L & 0x0f] == 0x82)
    {DOUBLETAKE (board, x, y - 1, x, y - 2);}
  if (((t_chemical_cheddar)res).fl[MI_R & 0x0f] == 0x82)
    {DOUBLETAKE (board, x, y + 1, x, y + 2);}
  if (((t_chemical_cheddar)res).fl[DO_L & 0x0f] == 0x82)
    {DOUBLETAKE (board, x + 1, y - 1, x + 2, y - 2);}
  if (((t_chemical_cheddar)res).fl[DO_C & 0x0f] == 0x82)
    {DOUBLETAKE (board, x + 1, y, x + 2, y);}
  if (((t_chemical_cheddar)res).fl[DO_R & 0x0f] == 0x82)
    {DOUBLETAKE (board, x + 1, y + 1, x + 2, y + 2);}
  return (0);
}


int	isprenable(t_board *board, int color, unsigned int x, unsigned int y)
{
  long	res;

  if (color == EMPTY)
    return (0);
  res = getlines(board, color, x, y);
  return ((((t_chemical_cheddar)res).fl[UP_L & 0x0f] == 0x81) && ((t_chemical_cheddar)res).fl[DO_R & 0x0f] == BLOCKED
	  + (((t_chemical_cheddar)res).fl[UP_C & 0x0f] == 0x81 && ((t_chemical_cheddar)res).fl[UP_C & 0x0f] == BLOCKED)
	  + (((t_chemical_cheddar)res).fl[UP_R & 0x0f] == 0x81 && ((t_chemical_cheddar)res).fl[UP_L & 0x0f] == BLOCKED)
	  + (((t_chemical_cheddar)res).fl[MI_L & 0x0f] == 0x81 && ((t_chemical_cheddar)res).fl[MI_R & 0x0f] == BLOCKED)
	  + (((t_chemical_cheddar)res).fl[MI_R & 0x0f] == 0x81 && ((t_chemical_cheddar)res).fl[MI_L & 0x0f] == BLOCKED)
	  + (((t_chemical_cheddar)res).fl[DO_L & 0x0f] == 0x81 && ((t_chemical_cheddar)res).fl[UP_R & 0x0f] == BLOCKED)
	  + (((t_chemical_cheddar)res).fl[DO_C & 0x0f] == 0x81 && ((t_chemical_cheddar)res).fl[UP_C & 0x0f] == BLOCKED)
	  + (((t_chemical_cheddar)res).fl[DO_R & 0x0f] == 0x81 && ((t_chemical_cheddar)res).fl[UP_L & 0x0f] == BLOCKED));
}

int	rule5(t_board *board,  int x,  int y, char color)
{
  long	res;
  int	counter1;
  int	counter2;

  if (color == EMPTY)
    return (0);
  res = prisegetlines(board, color, x, y);
  counter1 = (GETLSIZE(((t_chemical_cheddar)res).l[UP_L & 0x0f]) >= 5)
    + (GETLSIZE(((t_chemical_cheddar)res).l[UP_C & 0x0f]) >= 5) 
    + (GETLSIZE(((t_chemical_cheddar)res).l[UP_R & 0x0f]) >= 5)
    + (GETLSIZE(((t_chemical_cheddar)res).l[MI_L & 0x0f]) >= 5)
    + (GETLSIZE(((t_chemical_cheddar)res).l[MI_R & 0x0f]) >= 5)
    + (GETLSIZE(((t_chemical_cheddar)res).l[DO_L & 0x0f]) >= 5)
    + (GETLSIZE(((t_chemical_cheddar)res).l[DO_C & 0x0f]) >= 5)
    + (GETLSIZE(((t_chemical_cheddar)res).l[DO_R & 0x0f]) >= 5);
  counter2 = (GETLSIZE(((t_chemical_cheddar)res).l[UP_L & 0x0f]) >= 5 && ISPRENABLE(((t_chemical_cheddar)res).l[UP_L & 0x0f]))
    + (GETLSIZE(((t_chemical_cheddar)res).l[UP_C & 0x0f]) >= 5 && ISPRENABLE(((t_chemical_cheddar)res).l[UP_C & 0x0f]))
    + (GETLSIZE(((t_chemical_cheddar)res).l[UP_R & 0x0f]) >= 5 && ISPRENABLE(((t_chemical_cheddar)res).l[UP_R & 0x0f]))
    + (GETLSIZE(((t_chemical_cheddar)res).l[MI_L & 0x0f]) >= 5 && ISPRENABLE(((t_chemical_cheddar)res).l[MI_L & 0x0f]))
    + (GETLSIZE(((t_chemical_cheddar)res).l[MI_R & 0x0f]) >= 5 && ISPRENABLE(((t_chemical_cheddar)res).l[MI_R & 0x0f]))
    + (GETLSIZE(((t_chemical_cheddar)res).l[DO_L & 0x0f]) >= 5 && ISPRENABLE(((t_chemical_cheddar)res).l[DO_L & 0x0f]))
    + (GETLSIZE(((t_chemical_cheddar)res).l[DO_C & 0x0f]) >= 5 && ISPRENABLE(((t_chemical_cheddar)res).l[DO_C & 0x0f]))
    + (GETLSIZE(((t_chemical_cheddar)res).l[DO_R & 0x0f]) >= 5 && ISPRENABLE(((t_chemical_cheddar)res).l[DO_R & 0x0f]));
  /* printf("%d %d %d\n", counter1, counter2, isprenable(board, color, x, y)); */
  return (counter1 && counter2 != 1);
}
