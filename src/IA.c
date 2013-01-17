/*
** IA.c for rtype in /a/galaxy/far/far/away
**
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
**
** Started on  Tue Jan 15 17:03:24 2013 dorian schaegis
** Last update Thu Jan 17 16:50:45 2013 jonathan martins
*/


#include	"IA.h"
#include	"board.h"
#include	"rule.h"
#include	"manip_boards.h"

#define		OPPOSITE(COLOR) (~(COLOR) & 0b00000011)
#define		INFINITY 2000000
#define		DEPTH 2

int		heuristic_eval(t_board *board)
{
  return 0;
  (void)board;
}

int		leaf(t_board *board)
{
  int		i;
  char		current;

  if (board->whites >= 5 || board->blacks >=5)
    return 1;
  current = BLACK;
  for (i = 0; i < 19 * 19; i++)
    {
      if (rule5(board, i/19, i%19, OPPOSITE(current)))
	return 1;
    }
  current = WHITE;
  for (i = 0; i < 19 * 19; i++)
    {
      if (rule5(board, i/19, i%19, OPPOSITE(current)))
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
	  /* poser un pion a chaque position possible */
	  val2 = minimax(node, DEPTH - 1, BLACK);
	  if (val2 > val)
	    {
	      val = val2;
	      bestMove->x = x;
	      bestMove->y = y;
	    }
	}
    }
}

#include <stdio.h>

t_pos		*callIA(t_board *board, char rules)
{
  t_pos		*ret;

  (void)board;
  (void)rules;

  ret = malloc(sizeof(t_pos));
  if (ret != NULL)
    {
      minmax(board, ret);
      printf("IA move : %d | %d\n", ret->x, ret->y);
      /* call to minmax */
    }
  return (ret);
}
