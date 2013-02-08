/*
** IA.c for rtype in /a/galaxy/far/far/away
**
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
**
** Started on  Tue Jan 15 17:03:24 2013 dorian schaegis
** Last update Fri Feb  8 04:22:15 2013 jonathan martins
*/


#include	"time.h"
#include	"IA.h"
#include	"board.h"
#include	"rule.h"
#include	"manip_boards.h"

#define		INFINITY	40000000
#define		DEPTH		2


#define		FIVE_IN_ROW	40000
#define		FOUR_IN_ROW    	100
#define		BROKEN_FOUR	20
#define		THREE_IN_ROW	20
#define		CAPTURE		50
#define		BROKEN_THREE	7
#define		TWO_IN_ROW	5
#define		SINGLE_MARK	1

#define         OPPOSITE(COLOR) (~(COLOR) & 0b00000011)

int		get_val(int size, int blockLeft, int blockRight)
{
  if (blockLeft != 0 && blockRight != 0)
    return 0;
  if (size == 4 && blockLeft == 0 && blockRight == 0)
    return FOUR_IN_ROW;
  if (size == 4)
    return BROKEN_FOUR;
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

int		heuristic_eval(t_board *board, char rules)
{
  int       	eval = 0;
  int		x;
  int		y;
  long		l;

  for (x = 0; x < 19; x++)
    {
      for (y = 0; y < 19; y++)
	{
	  if (get_board(board, x, y) == WHITE)
	    {
	      if (rule5(board, x, y, WHITE, rules))
		eval += FIVE_IN_ROW;
	      else
		{
		  l = getlines(board, WHITE, x, y);
		  eval += get_val(GETLSIZE(((char*)&l)[0]) + GETLSIZE(((char*)&l)[7])
				  + 1, ISBLOCKED(((char*)&l)[0]),
				  ISBLOCKED(((char*)&l)[7]));
		  eval += get_val(GETLSIZE(((char*)&l)[1]) + GETLSIZE(((char*)&l)[6])
				  + 1, ISBLOCKED(((char*)&l)[1]),
				  ISBLOCKED(((char*)&l)[6]));
		  eval += get_val(GETLSIZE(((char*)&l)[2]) + GETLSIZE(((char*)&l)[5])
				  + 1, ISBLOCKED(((char*)&l)[2]),
				  ISBLOCKED(((char*)&l)[5]));
		  eval += get_val(GETLSIZE(((char*)&l)[3]) + GETLSIZE(((char*)&l)[4])
				  + 1, ISBLOCKED(((char*)&l)[3]),
				  ISBLOCKED(((char*)&l)[4]));
		  }
	    }
	  if (get_board(board, x, y) == BLACK)
	    {
	      if (rule5(board, x, y, BLACK, rules))
		eval -= FIVE_IN_ROW;
	      else
		{
		  l = getlines(board, BLACK, x, y);
		  eval -= get_val(GETLSIZE(((char*)&l)[0]) + GETLSIZE(((char*)&l)[7])
				  + 1, ISBLOCKED(((char*)&l)[0]),
				  ISBLOCKED(((char*)&l)[7]));
		  eval -= get_val(GETLSIZE(((char*)&l)[1]) + GETLSIZE(((char*)&l)[6])
				  + 1, ISBLOCKED(((char*)&l)[1]),
				  ISBLOCKED(((char*)&l)[6]));
		  eval -= get_val(GETLSIZE(((char*)&l)[2]) + GETLSIZE(((char*)&l)[5])
				  + 1, ISBLOCKED(((char*)&l)[2]),
				  ISBLOCKED(((char*)&l)[5]));
		  eval -= get_val(GETLSIZE(((char*)&l)[3]) + GETLSIZE(((char*)&l)[4])
				  + 1, ISBLOCKED(((char*)&l)[3]),
				  ISBLOCKED(((char*)&l)[4]));
		}
	    }
	}
    }
  eval += board->blacks * CAPTURE;
  eval -= board->whites * CAPTURE;
  return eval;
}

int		check(t_board *board, int x, int y)
{
  int		xx;
  int		yy;

  for (xx = x - 2; xx < x + 2; xx++)
    {
      for (yy = y - 2; yy < y + 2; yy++)
	{
	  if (get_board(board, xx, yy) != EMPTY)
	    return 1;
	}
    }
  return 0;
}

int		minimax(t_board *node, int depth, char current, char rules)
{
  int		val;
  int		val2;
  int		x;
  int		y;
  int		get;

  if (depth == 0)
    return heuristic_eval(node, rules);
  val = INFINITY;
  if (current == WHITE)
    val = -INFINITY;
  for (x = 0; x < 19; x++)
    {
      for (y = 0; y < 19; y++)
	{
	  if (check(node, x, y) == 1)
	    {
	      if ((get_board(node, x, y) == EMPTY) &&
                  (!(rules & RULE3) || (rule3(node, x, y, current))))
		{
		  set_board(node, x, y, current);
		  get = getprise(node, x, y, OPPOSITE(current));
		  if (current == WHITE)
		    node->blacks += get;
		  if (current == BLACK)
		    node->whites += get;
		  val2 = minimax(node, depth - 1, OPPOSITE(current), rules);
		  if (current == WHITE)
		    node->blacks -= get;
		  if (current == BLACK)
		    node->whites -= get;
		  set_board(node, x, y, EMPTY);
		  if ((current == WHITE && val2 > val)
		      || (current == BLACK && val2 < val))
		    val = val2;
		}
	    }
	}
    }
  return val;
}

void		minmax(t_board *node, t_pos *bestMove, char current, char rules)
{
  int		val;
  int		val2;
  int		x;
  int		y;
  int		get;

  val = INFINITY;
  if (current == WHITE)
    val = -INFINITY;
  bestMove->x = 0;
  bestMove->y = 0;
  for (x = 0; x < 19; x++)
    {
      for (y = 0; y < 19; y++)
	{
	  if (check(node, x, y) == 1)
	    {
	      if ((get_board(node, x, y) == EMPTY) &&
                  (!(rules & RULE3) || (rule3(node, x, y, current))))
		{
		  set_board(node, x, y, current);
		  get = getprise(node, x, y, OPPOSITE(current));
		  if (current == WHITE)
		    node->blacks += get;
		  if (current == BLACK)
		    node->whites += get;
		  val2 = minimax(node, DEPTH - 1, OPPOSITE(current), rules);
		  if (current == WHITE)
		    node->blacks -= get;
		  if (current == BLACK)
		    node->whites -= get;
		  set_board(node, x, y, EMPTY);
		  if (current == WHITE && val2 > val)
		    {
		      val = val2;
		      bestMove->x = x;
		      bestMove->y = y;
		    }
		  if (current == BLACK && val2 < val)
		    {
		      val = val2;
		      bestMove->x = x;
		      bestMove->y = y;
		    }
		}
	    }
	}
    }
}

void		callIA(t_board *board, char rules, t_pos *ret, char current)
{
  minmax(board, ret, current, rules);
}
