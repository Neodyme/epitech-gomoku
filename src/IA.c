/*
** IA.c for rtype in /a/galaxy/far/far/away
**
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
**
** Started on  Tue Jan 15 17:03:24 2013 dorian schaegis
** Last update Sun Feb 10 16:30:41 2013 Shauny
*/


#include	"time.h"
#include	"IA.h"
#include	"board.h"
#include	"rule.h"
#include	"manip_boards.h"

#define		INFINITY	400000000
#define		DEPTH		2

#define		FIVE_IN_ROW	300
#define		BROKEN_FIVE	200
#define		FOUR_IN_ROW	250
#define		BROKEN_FOUR	50
#define		THREE_IN_ROW	33
#define		CAPTURE		798
#define		BROKEN_THREE	5
#define		TWO_IN_ROW	3
#define		SINGLE_MARK	1

#define         OPPOSITE(COLOR) (~(COLOR) & 0b00000011)


/* A RETIRER */

#include <stdio.h>
#include <stdint.h>
#include <time.h>

/* int64_t timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p) */
/* { */
/*   return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) - */
/*     ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec); */
/* } */

/* A RETIRER */

int		get_val(int size, int blockLeft, int blockRight)
{
  if (blockLeft != 0 && blockRight != 0)
    return 0;
  if (size >= 5 && blockLeft == 0 && blockRight == 0)
    return FIVE_IN_ROW;
  if (size >= 5)
    return BROKEN_FIVE;
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
  return SINGLE_MARK;
}

int		heuristic_eval(t_board *board, char rules)
{
  int       	eval = 0;
  int		x;
  int		y;
  long		l;
  int		ret;
  int		get;

  for (x = 0; x < 19; x++)
    {
      for (y = 0; y < 19; y++)
	{
	  get = get_board(board, x, y);
	  if (get == EMPTY)
	    continue;
	  else if (get == WHITE)
	    {
	      l = getlines(board, WHITE, x, y);
	      ret = get_val(GETLSIZE(((char*)&l)[0]) + GETLSIZE(((char*)&l)[7])
			    + 1, ISBLOCKED2(((char*)&l)[0]),
			    ISBLOCKED2(((char*)&l)[7]));
	      if (ret >= BROKEN_FIVE && (rule5(board, x, y, WHITE, rules)))
		return INFINITY;
	      eval += ret;
	      ret = get_val(GETLSIZE(((char*)&l)[1]) + GETLSIZE(((char*)&l)[6])
			    + 1, ISBLOCKED2(((char*)&l)[1]),
			    ISBLOCKED2(((char*)&l)[6]));
	      if (ret >= BROKEN_FIVE && (rule5(board, x, y, WHITE, rules)))
		return INFINITY;
	      eval += ret;
	      ret = get_val(GETLSIZE(((char*)&l)[2]) + GETLSIZE(((char*)&l)[5])
			    + 1, ISBLOCKED2(((char*)&l)[2]),
			    ISBLOCKED2(((char*)&l)[5]));
	      if (ret >= BROKEN_FIVE && (rule5(board, x, y, WHITE, rules)))
		return INFINITY;
	      eval += ret;
	      ret = get_val(GETLSIZE(((char*)&l)[3]) + GETLSIZE(((char*)&l)[4])
			    + 1, ISBLOCKED2(((char*)&l)[3]),
			    ISBLOCKED2(((char*)&l)[4]));
	      if (ret >= BROKEN_FIVE && (rule5(board, x, y, WHITE, rules)))
		return INFINITY;
	      eval += ret;
	    }
	  else
	    {
	      l = getlines(board, BLACK, x, y);
	      ret = get_val(GETLSIZE(((char*)&l)[0]) + GETLSIZE(((char*)&l)[7])
			    + 1, ISBLOCKED2(((char*)&l)[0]),
			    ISBLOCKED2(((char*)&l)[7]));
	      if (ret >= BROKEN_FIVE && (rule5(board, x, y, BLACK, rules)))
		return -INFINITY;
	      eval -= ret;
	      ret = get_val(GETLSIZE(((char*)&l)[1]) + GETLSIZE(((char*)&l)[6])
			    + 1, ISBLOCKED2(((char*)&l)[1]),
			    ISBLOCKED2(((char*)&l)[6]));
	      if (ret >= BROKEN_FIVE && (rule5(board, x, y, BLACK, rules)))
		return -INFINITY;
	      eval -= ret;
	      ret = get_val(GETLSIZE(((char*)&l)[2]) + GETLSIZE(((char*)&l)[5])
			    + 1, ISBLOCKED2(((char*)&l)[2]),
			    ISBLOCKED2(((char*)&l)[5]));
	      if (ret >= BROKEN_FIVE && (rule5(board, x, y, BLACK, rules)))
		return -INFINITY;
	      eval -= ret;
	      ret = get_val(GETLSIZE(((char*)&l)[3]) + GETLSIZE(((char*)&l)[4])
			    + 1, ISBLOCKED2(((char*)&l)[3]),
			    ISBLOCKED2(((char*)&l)[4]));
	      if (ret >= BROKEN_FIVE && (rule5(board, x, y, BLACK, rules)))
		return -INFINITY;
	      eval -= ret;
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

	  if (get_board(board, xx, yy) != EMPTY
	      && xx >= 0 && xx < 19 && yy >= 0 && yy < 19)
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
  val = heuristic_eval(node, rules);
  if (val == INFINITY || val == -INFINITY)
    return val;
  for (x = 0; x < 19; x++)
    {
      for (y = 0; y < 19; y++)
	{
	  if ((get_board(node, x, y) == EMPTY) && check(node, x, y) == 1 &&
	      (!(rules & RULE3) || (rule3(node, x, y, current))))
	    {
	      set_board(node, x, y, current);
	      get = getprise(node, x, y, OPPOSITE(current));
	      if (current == WHITE)
		node->blacks += get;
	      else
		node->whites += get;
	      val2 = minimax(node, depth - 1, OPPOSITE(current), rules);
	      if (current == WHITE)
		node->blacks -= get;
	      else
		node->whites -= get;
	      set_board(node, x, y, EMPTY);
	      if ((current == WHITE && val2 > val)
		  || (current == BLACK && val2 < val))
		val = val2;
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

  if (current == WHITE)
    val = -INFINITY - 50;
  else
    val = INFINITY + 50;
  for (x = 0; x < 19; x++)
    {
      for (y = 0; y < 19; y++)
	{
	  if ((get_board(node, x, y) == EMPTY) && check(node, x, y) == 1 &&
	      (!(rules & RULE3) || (rule3(node, x, y, current))))
	    {
	      if (bestMove->x == -1 || bestMove->y == -1)
		{
		  bestMove->x = x;
		  bestMove->y = y;
		}
	      set_board(node, x, y, current);
	      get = getprise(node, x, y, OPPOSITE(current));
	      if (current == WHITE)
		node->blacks += get;
	      else
		node->whites += get;
	      val2 = minimax(node, DEPTH - 1, OPPOSITE(current), rules);
	      if (current == WHITE)
		node->blacks -= get;
	      else
		node->whites -= get;
	      set_board(node, x, y, EMPTY);
	      if ((current == WHITE && val2 > val)
		  || (current == BLACK && val2 < val))
		{
		  val = val2;
		  bestMove->x = x;
		  bestMove->y = y;
		}
	    }
	}
    }
}

void		set_pos(t_board *board, t_pos *pos)
{
  int	i;

  for (i = 180; i > 0; i--)
    if (get_board(board, i/19, i%19) == EMPTY)
      {
	pos->x = i / 19;
	pos->y = i % 19;
	return;
      }
for (i = 180; i < 361; i++)
    if (get_board(board, i/19, i%19) == EMPTY)
      {
	pos->x = i / 19;
	pos->y = i % 19;
	return;
      }
}

void		callIA(t_board *board, char rules, t_pos *ret, char current)
{
  /* struct timespec start, end; */
  /* clock_gettime(CLOCK_MONOTONIC, &start); */


  set_pos(board, ret);
  minmax(board, ret, current, rules);


  /* clock_gettime(CLOCK_MONOTONIC, &end); */
  /* printf("time: '%d'ms\n", (int)timespecDiff(&end, &start) / 1000000); */
}
