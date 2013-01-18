/*
** IA.c for rtype in /a/galaxy/far/far/away
**
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
**
** Started on  Tue Jan 15 17:03:24 2013 dorian schaegis
** Last update Fri Jan 18 16:52:46 2013 jonathan martins
*/


#include	"time.h"
#include	"IA.h"
#include	"board.h"
#include	"rule.h"
#include	"manip_boards.h"

#define		INFINITY 2000000
#define		DEPTH 2

#define		FIVE_IN_ROW	INFINITY
#define		STRAIGHT_FOUR	1500000
#define		FOUR_IN_ROW	1500000
#define		THREE_IN_ROW	250000
#define		BROKEN_THREE	100000
#define		CAPTURE		50000
#define		TWO_IN_ROW	100
#define		SINGLE_MARK	1


/* Eval = w1 × # five-in-row +w2 × # straight-four */
/* +w3 × # four-in-row +w4 × # three-in-row +w5 × # */
/* broken-three +w6 × # two-in-row +w7 × # single marks */

int		heuristic_eval(t_board *board)
{
  int		eval = 0;
  int		x;
  int		y;

  for (y = 0; y < 19; y++)
    {
      for (x = 0; x < 19; x++)
	{

	}
    }
  return eval;
  (void)board;
}

int		leaf(t_board *board)
{
  int		i;

  if (board->whites >= 5 || board->blacks >=5)
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
  unsigned int	saveEaten;

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
		  saveEaten = node->whites;
		  if (prise(node, x, y))
		    node->whites++;
		  val2 = minimax(node, depth - 1, BLACK);
		  set_board(node, x, y, EMPTY);
		  while (saveEaten < node->whites--)
		    {
		      set_board(node, node->eaten[node->blacks + node->whites + 0],
				node->eaten[node->blacks + node->whites + 1], BLACK);
		      set_board(node, node->eaten[node->blacks + node->whites + 2],
				node->eaten[node->blacks + node->whites + 3], BLACK);
		    }
		  node->whites = saveEaten;
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
		  saveEaten = node->blacks;
		  if (prise(node, x, y))
		    node->blacks++;
		  val2 = minimax(node, depth - 1, WHITE);
		  set_board(node, x, y, EMPTY);
		  while (saveEaten < node->blacks--)
		    {
		      set_board(node, node->eaten[node->blacks + node->whites + 0],
				node->eaten[node->blacks + node->whites + 1], BLACK);
		      set_board(node, node->eaten[node->blacks + node->whites + 2],
				node->eaten[node->blacks + node->whites + 3], BLACK);
		    }
		  node->blacks = saveEaten;
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
  unsigned int	saveEaten;

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
	      saveEaten = node->whites;
	      if (prise(node, x, y))
		node->whites++;
	      val2 = minimax(node, DEPTH - 1, BLACK);
	      set_board(node, x, y, EMPTY);
	      while (saveEaten < node->whites--)
		{
		  set_board(node, node->eaten[node->blacks + node->whites + 0],
			    node->eaten[node->blacks + node->whites + 1], BLACK);
		  set_board(node, node->eaten[node->blacks + node->whites + 2],
			    node->eaten[node->blacks + node->whites + 3], BLACK);
		}
	      node->whites = saveEaten;
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
