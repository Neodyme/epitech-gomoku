/*
** IA.c for rtype in /a/galaxy/far/far/away
**
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
**
** Started on  Tue Jan 15 17:03:24 2013 dorian schaegis
** Last update Sat Jan 19 22:25:00 2013 jonathan martins
*/


#include	"time.h"
#include	"IA.h"
#include	"board.h"
#include	"rule.h"
#include	"manip_boards.h"

#define		INFINITY	2000000
#define		DEPTH		2

#define		FIVE_IN_ROW	INFINITY
#define		FOUR		INFINITY
#define		THREE_IN_ROW	500000
#define		BROKEN_THREE	10000
#define		CAPTURE		9000
#define		TWO_IN_ROW	100
#define		SINGLE_MARK	1

#define GETSAMURAI(X, S, N) (X  - ((N) * ((S & 0x80)) >> 7) + ((N) * ((S & 0x40)) >> 6))
#define GETHARISSA(Y, S, N) (Y  - ((N) * ((S & 0x20) >> 5)) + ((N) * ((S & 0x10)) >> 4))

int		get_val(int size, int blockLeft, int blockRight)
{
  if (size == 5)
    return INFINITY;
  if (size == 4 && (blockLeft == 0 || blockRight == 0))
    return FOUR;
  if (size == 3 && blockLeft == 0 && blockRight == 0)
    return THREE_IN_ROW;
  if (size == 2 && blockLeft == 0 && blockRight == 0)
    return TWO_IN_ROW;
  return SINGLE_MARK * size;
}

void		setvisited(char *visited, int i, int l)
{
/*   int		x = i/19; */
/*   int		y = i%19; */
/*   int		j; */

/*   visited[i] = 1; */

/*   j = GETSAMURAI(x, UP_L, ((char*)&l)[0]) */
/*     * GETHARISSA(y, UP_L, ((char*)&l)[0]); */
/*   j = GETSAMURAI(x, UP_C, ((char*)&l)[1]) */
/* 	  * GETHARISSA(y, UP_C, ((char*)&l)[1])] = 1; */
/*   j = GETSAMURAI(x, UP_R, ((char*)&l)[2]) */
/* 	  * GETHARISSA(y, UP_R, ((char*)&l)[2])] = 1; */
/*   j = GETSAMURAI(x, MI_L, ((char*)&l)[3]) */
/* 	  * GETHARISSA(y, MI_L, ((char*)&l)[3])] = 1; */
/*   j = GETSAMURAI(x, MI_R, ((char*)&l)[4]) */
/* 	  * GETHARISSA(y, MI_R, ((char*)&l)[4])] = 1; */
/*   j = GETSAMURAI(x, DO_L, ((char*)&l)[5]) */
/* 	  * GETHARISSA(y, DO_L, ((char*)&l)[5])] = 1; */
/*   j = GETSAMURAI(x, DO_C, ((char*)&l)[6]) */
/* 	  * GETHARISSA(y, DO_C, ((char*)&l)[6])] = 1; */
/*   j = GETSAMURAI(x, DO_R, ((char*)&l)[7]) */
/* 	  * GETHARISSA(y, DO_R, ((char*)&l)[7])] = 1; */
}

int		heuristic_eval(t_board *board)
{
  int		eval = 0;
  int		ret;
  int		i;
  long		l;
  char		visited[361];

  memset(visited, 0, 360);
  for (i = 0; i < 19 * 19; i++)
    {
      if (visited[i] == 0)
	{
	  if (get_board(board, i/19, i%19) == WHITE)
	    {
	      l = getlines(board, WHITE, i/19, i%19);
	      ret = get_val(GETLSIZE(((char*)&l)[0]) + GETLSIZE(((char*)&l)[7]),
			    ISBLOCKED(((char*)&l)[0]), ISBLOCKED(((char*)&l)[7]));
	      if (ret == INFINITY)
		return INFINITY;
	      eval += ret;
	      ret = get_val(GETLSIZE(((char*)&l)[1]) + GETLSIZE(((char*)&l)[6]),
			    ISBLOCKED(((char*)&l)[1]), ISBLOCKED(((char*)&l)[6]));
	      if (ret == INFINITY)
		return INFINITY;
	      eval += ret;
	      ret = get_val(GETLSIZE(((char*)&l)[2]) + GETLSIZE(((char*)&l)[5]),
			    ISBLOCKED(((char*)&l)[2]), ISBLOCKED(((char*)&l)[5]));
	      if (ret == INFINITY)
		return INFINITY;
	      eval += ret;
	      ret = get_val(GETLSIZE(((char*)&l)[3]) + GETLSIZE(((char*)&l)[4]),
			    ISBLOCKED(((char*)&l)[3]), ISBLOCKED(((char*)&l)[4]));
	      if (ret == INFINITY)
		return INFINITY;
	      eval += ret;
	      setvisited(visited, i, l);
	    }
	  if (get_board(board, i/19, i%19) == BLACK)
	    {
	      l = getlines(board, BLACK, i/19, i%19);
	      ret = get_val(GETLSIZE(((char*)&l)[0]) + GETLSIZE(((char*)&l)[7]),
			    ISBLOCKED(((char*)&l)[0]), ISBLOCKED(((char*)&l)[7]));
	      if (ret == INFINITY)
		return -INFINITY;
	      eval -= ret;
	      ret = get_val(GETLSIZE(((char*)&l)[1]) + GETLSIZE(((char*)&l)[6]),
			    ISBLOCKED(((char*)&l)[1]), ISBLOCKED(((char*)&l)[6]));
	      if (ret == INFINITY)
		return -INFINITY;
	      eval -= ret;
	      ret = get_val(GETLSIZE(((char*)&l)[2]) + GETLSIZE(((char*)&l)[5]),
			    ISBLOCKED(((char*)&l)[2]), ISBLOCKED(((char*)&l)[5]));
	      if (ret == INFINITY)
		return -INFINITY;
	      eval -= ret;
	      ret = get_val(GETLSIZE(((char*)&l)[3]) + GETLSIZE(((char*)&l)[4]),
			    ISBLOCKED(((char*)&l)[3]), ISBLOCKED(((char*)&l)[4]));
	      if (ret == INFINITY)
		return -INFINITY;
	      eval -= ret;
	    }
	}
    }
  eval += board->whites * CAPTURE;
  eval -= board->blacks * CAPTURE;
  return eval;
}

int		leaf(t_board *board)
{
  int		i;

  if (board->whites >= 5)
    return 1;
  if (board->blacks >= 5)
    return 1;
  for (i = 0; i < 19 * 19; i++)
    {
      if (rule5(board, i/19, i%19, BLACK))
	return 1;
      if (rule5(board, i/19, i%19, WHITE))
	return 1;
    }
  return 0;
}

int		minimax(t_board *node, int depth, char current)
{
  int		val;
  int		val2;
  int		x;
  int		y;

  if (depth == 0 || leaf(node) == 1)
    return heuristic_eval(node);
  if (current == WHITE)
    {
      val = -INFINITY;
      for (x = 0; x < 19; x++)
	{
	  for (y = 0; y < 19; y++)
	    {
	      if ((get_board(node, x, y) == EMPTY) &&
		  (rule3(node, x, y, current)))
		{
		  set_board(node, x, y, current);
		  val2 = minimax(node, depth - 1, BLACK);
		  set_board(node, x, y, EMPTY);
		  if (val2 > val)
		    val = val2;
		}
	    }
	}
      return val;
    }
  if (current == BLACK)
    {
      val = INFINITY;
      for (x = 0; x < 19; x++)
	{
	  for (y = 0; y < 19; y++)
	    {
	      if ((get_board(node, x, y) == EMPTY) &&
		  (rule3(node, x, y, current)))
		{
		  set_board(node, x, y, current);
		  val2 = minimax(node, depth - 1, WHITE);
		  set_board(node, x, y, EMPTY);
		  if (val2 < val)
		    val = val2;
		}
	    }
	}
      return val;
    }
  return 0;
}

void		minmax(t_board *node, t_pos *bestMove)
{
  int		val;
  int		val2;
  int		x;
  int		y;

  val = -INFINITY;
  bestMove->x = 0;
  bestMove->y = 0;
  for (x = 0; x < 19; x++)
    {
      for (y = 0; y < 19; y++)
	{
	  if ((get_board(node, x, y) == EMPTY) &&
	      (rule3(node, x, y, WHITE)))
	    {
	      set_board(node, x, y, WHITE);
	      val2 = minimax(node, DEPTH - 1, BLACK);
	      set_board(node, x, y, EMPTY);
	      if (val2 > val)
		{
		  val = val2;
		  bestMove->x = x;
		  bestMove->y = y;
		}
	    }
	}
    }
}

#include <stdio.h>

void		callIA(t_board *board, char rules, t_pos *ret)
{
  if (ret != NULL)
    {
      minmax(board, ret);
      printf("IA move : %d | %d\n", ret->x, ret->y);
    }
  (void)rules;
}
