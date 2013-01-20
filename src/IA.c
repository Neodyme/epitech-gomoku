/*
** IA.c for rtype in /a/galaxy/far/far/away
**
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
**
** Started on  Tue Jan 15 17:03:24 2013 dorian schaegis
** Last update Sun Jan 20 02:16:49 2013 jonathan martins
*/


#include	"time.h"
#include	"IA.h"
#include	"board.h"
#include	"rule.h"
#include	"manip_boards.h"

#define		INFINITY	200000000
#define		DEPTH		2

#define		FIVE_IN_ROW	INFINITY
#define		FOUR		50000000
#define		THREE_IN_ROW	2000000
#define		CAPTURE		1000000
#define		BROKEN_THREE	10000
#define		TWO_IN_ROW	4000
#define		SINGLE_MARK	100

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
  if (size == 1 && blockLeft == 0 && blockRight == 0)
    return SINGLE_MARK;
  return size;
}

int		heuristic_eval(t_board *board)
{
  int		eval = 0;
  int		ret;
  int		i;
  long		l;

  for (i = 0; i < 19 * 19; i++)
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
  eval += board->blacks * CAPTURE;
  eval -= board->whites * CAPTURE;
  eval -= random() % 5;
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

int		minimax(t_board *node, int depth, char current, int min, int max)
{
  int		val;
  int		val2;
  int		x;
  int		y;
  int		get;

  if (depth == 0 || leaf(node) == 1)
    return heuristic_eval(node);
  if (current == WHITE)
    {
      val = min;
      for (x = 0; x < 19; x++)
	{
	  for (y = 0; y < 19; y++)
	    {
	      if ((get_board(node, x, y) == EMPTY) &&
		  (rule3(node, x, y, current)))
		{
		  set_board(node, x, y, current);
		  get = getprise(node, x, y, WHITE);
		  node->blacks += get;
		  val2 = minimax(node, depth - 1, BLACK, val, max);
		  node->blacks -= get;
		  set_board(node, x, y, EMPTY);
		  if (val2 > val)
		    val = val2;
		  if (val > max)
		    return max;
		}
	    }
	}
      return val;
    }
  if (current == BLACK)
    {
      val = max;
      for (x = 0; x < 19; x++)
	{
	  for (y = 0; y < 19; y++)
	    {
	      if ((get_board(node, x, y) == EMPTY) &&
		  (rule3(node, x, y, current)))
		{
		  set_board(node, x, y, current);
		  get = getprise(node, x, y, BLACK);
		  node->whites += get;
		  val2 = minimax(node, depth - 1, WHITE, min, val);
		  node->whites -= get;
		  set_board(node, x, y, EMPTY);
		  if (val2 < val)
		    val = val2;
		  if (val > max)
		    return max;
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
  int		get;

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
	      get = getprise(node, x, y, WHITE);
	      if (get)
		node->blacks += get;
	      val2 = minimax(node, DEPTH - 1, BLACK, -INFINITY, INFINITY);
	      if (get)
		node->blacks -= get;
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
  if (get_board(node, bestMove->x, bestMove->y) != EMPTY)
    for (bestMove->x = 0; bestMove->x < 19; bestMove->x++)
      for(bestMove->y = 0; bestMove->y < 19; bestMove->y++)
	if (get_board(node, bestMove->x, bestMove->y) == EMPTY)
	  return;
}

void		callIA(t_board *board, char rules, t_pos *ret)
{
  if (ret != NULL)
    minmax(board, ret);
  (void)rules;
}
