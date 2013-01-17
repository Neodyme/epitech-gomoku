/*
** manip_boards.c for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Sun Dec  2 15:10:45 2012 dorian schaegis
** Last update Thu Jan 17 17:35:29 2013 Prost P.
*/

#include	<unistd.h>
#include	<string.h>

#include	<SDL/SDL.h>

#include	"board.h"
#include	"manip_boards.h"

void		init_board(t_board *board)
{
  //		0b00100010
  board->whites = 0;
  board->blacks = 0;
  memset(board->board, 0x00, sizeof(board->board) );
  memset(board->r3board, 0x00, sizeof(board->r3board) );
  /* memset(board->board, 0xffffffff, sizeof(int)); */
}


inline void	set_board(t_board *board, register int x, register int y, int val)
{
  board->board[BYTE(x, y, char)] = (board->board[BYTE(x, y, char)] & ~(3 << BIT(x, y, char))) | (val << BIT(x, y, char));
}

inline char    	get_board(t_board *board, register int x, register int y)
{
  return ((board->board[BYTE(x, y, char)] & (3 << BIT(x, y, char))) >> BIT(x, y, char));
}

void		dump_board(t_board *board)
{
  unsigned int	i;

  for (i = 0; i < 19 * 19; i++)
    {
      printf("%i ", get_board(board, i / 19, i % 19));
      if ((i % 19) == 18)
	printf("\n");
    }
}
