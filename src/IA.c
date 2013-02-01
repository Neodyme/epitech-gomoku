/*
** IA.c for rtype in /a/galaxy/far/far/away
**
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
**
** Started on  Tue Jan 15 17:03:24 2013 dorian schaegis
** Last update Sat Feb  2 00:00:30 2013 jonathan martins
*/


#include	"time.h"
#include	"IA.h"
#include	"board.h"
#include	"rule.h"
#include	"manip_boards.h"

#define		INFINITY	200000000
#define		DEPTH		2
#define		ENEMY_RATE	2


#define		FIVE_IN_ROW	40000000
#define		FOUR		10000000
#define		THREE_IN_ROW	500000
#define		CAPTURE		500000
#define		BROKEN_THREE	3333
#define		TWO_IN_ROW	250
#define		SINGLE_MARK	100

#define         OPPOSITE(COLOR) (~(COLOR) & 0b00000011)

#define GETSAMURAI(X, S, N) (X  - ((N) * ((S & 0x80)) >> 7) + ((N) * ((S & 0x40)) >> 6))
#define GETHARISSA(Y, S, N) (Y  - ((N) * ((S & 0x20) >> 5)) + ((N) * ((S & 0x10)) >> 4))

int		get_val(int size, int blockLeft, int blockRight)
{
  if (blockLeft != 0 && blockRight != 0)
    return 0;
  if (size >= 5)
    return FIVE_IN_ROW;
  if (size == 4)
    return FOUR;
  if (size == 3 && blockLeft == 0 && blockRight == 0)
    return THREE_IN_ROW;
  if (size == 3)
    return BROKEN_THREE;
  if (size == 2 && blockLeft == 0 && blockRight == 0)
    return TWO_IN_ROW;
  if (size == 1 && blockLeft == 0 && blockRight == 0)
    return SINGLE_MARK;
  return 0;
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
	  ret = get_val(GETLSIZE(((char*)&l)[0]) + GETLSIZE(((char*)&l)[7]) + 1,
			ISBLOCKED(((char*)&l)[0]), ISBLOCKED(((char*)&l)[7]));
	  eval += ret;
	  ret = get_val(GETLSIZE(((char*)&l)[1]) + GETLSIZE(((char*)&l)[6]) + 1,
			ISBLOCKED(((char*)&l)[1]), ISBLOCKED(((char*)&l)[6]));
	  eval += ret;
	  ret = get_val(GETLSIZE(((char*)&l)[2]) + GETLSIZE(((char*)&l)[5]) + 1,
			ISBLOCKED(((char*)&l)[2]), ISBLOCKED(((char*)&l)[5]));
	  eval += ret;
	  ret = get_val(GETLSIZE(((char*)&l)[3]) + GETLSIZE(((char*)&l)[4]) + 1,
			ISBLOCKED(((char*)&l)[3]), ISBLOCKED(((char*)&l)[4]));
	  eval += ret;
	}
      if (get_board(board, i/19, i%19) == BLACK)
	{
	  l = getlines(board, BLACK, i/19, i%19);
	  ret = get_val(GETLSIZE(((char*)&l)[0]) + GETLSIZE(((char*)&l)[7]) + 1,
			ISBLOCKED(((char*)&l)[0]), ISBLOCKED(((char*)&l)[7]));
	  eval -= ret * ENEMY_RATE;
	  ret = get_val(GETLSIZE(((char*)&l)[1]) + GETLSIZE(((char*)&l)[6]) + 1,
			ISBLOCKED(((char*)&l)[1]), ISBLOCKED(((char*)&l)[6]));
	  eval -= ret * ENEMY_RATE;
	  ret = get_val(GETLSIZE(((char*)&l)[2]) + GETLSIZE(((char*)&l)[5]) + 1,
			ISBLOCKED(((char*)&l)[2]), ISBLOCKED(((char*)&l)[5]));
	  eval -= ret * ENEMY_RATE;
	  ret = get_val(GETLSIZE(((char*)&l)[3]) + GETLSIZE(((char*)&l)[4]) + 1,
			ISBLOCKED(((char*)&l)[3]), ISBLOCKED(((char*)&l)[4]));
	  eval -= ret * ENEMY_RATE;
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
      if (rule5(board, i/19, i%19, BLACK, RULE5))
	return 1;
      if (rule5(board, i/19, i%19, WHITE, RULE5))
	return 1;
    }
  return 0;
}

int		minimax(t_board *node, int depth, char current,
			int min, int max, char rules)
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
		  (!(rules & RULE3) || (rule3(node, x, y, current))))
		{
		  set_board(node, x, y, current);
		  get = getprise(node, x, y, WHITE);
		  node->blacks += get;
		  val2 = minimax(node, depth - 1, BLACK, val, max, rules);
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
		  (!(rules & RULE3) || (rule3(node, x, y, current))))
		{
		  set_board(node, x, y, current);
		  get = getprise(node, x, y, BLACK);
		  node->whites += get;
		  val2 = minimax(node, depth - 1, WHITE, min, val, rules);
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

void		minmax(t_board *node, t_pos *bestMove, char current, char rules)
{
  int		val;
  int		val2;
  int		x;
  int		y;
  int		get;

  if (current == WHITE)
    val = -INFINITY;
  else
    val = INFINITY;
  bestMove->x = 0;
  bestMove->y = 0;
  for (x = 0; x < 19; x++)
    {
      for (y = 0; y < 19; y++)
	{
	  if ((get_board(node, x, y) == EMPTY) &&
	      (!(rules & RULE3) || (rule3(node, x, y, current))))
	    {
	      set_board(node, x, y, current);
	      get = getprise(node, x, y, current);
	      if (current == WHITE)
		node->blacks += get;
	      else
		node->whites += get;
	      val2 = minimax(node, DEPTH - 1, OPPOSITE(current),
			     -INFINITY, INFINITY, rules);
	      if (current == WHITE)
		node->blacks -= get;
	      else
		node->whites -= get;
	      set_board(node, x, y, EMPTY);
	      if (current == WHITE)
		{
		  if (val2 > val)
		    {
		      val = val2;
		      bestMove->x = x;
		      bestMove->y = y;
		    }
		}
	      else
		{
		  if (val2 < val)
		    {
		      val = val2;
		      bestMove->x = x;
		      bestMove->y = y;
		    }
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

void		callIA(t_board *board, char rules, t_pos *ret, char current)
{
  if (ret != NULL)
    minmax(board, ret, current, rules);
}
