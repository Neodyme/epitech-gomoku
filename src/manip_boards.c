/*
** manip_boards.c for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Sun Dec  2 15:10:45 2012 dorian schaegis
<<<<<<< HEAD
** Last update Sun Dec  2 16:09:27 2012 Prost P.
=======
** Last update Sun Dec  2 16:56:39 2012 dorian schaegis
>>>>>>> 9df3771c0df4b456894e6e1bcb73ede97a45b3de
*/

#include	<unistd.h>
#include	<string.h>

#include	<SDL/SDL.h>

#include	"board.h"
#include	"manip_boards.h"

void		init_board(t_board *board)
{
  //		0b00100010
  memset(board->b, 0, sizeof(board->b));
  memset(board->w, 0, sizeof(board->w));
}


void		set_board(t_board *board, char x, char y, char val)
{
  switch (val)
    {
    case BLACK:
      board->b[BYTE(x, y, char)] |= (1 << (BIT(x, y, char)));
      break;
    case WHITE:
      board->w[BYTE(x, y, char)] |= (1 << (BIT(x, y, char)));
      break;
    }
  dump_board(board);
}

char		get_board(t_board *board, register char x, register char y)
{
  return ((board->b[BYTE(x, y, char)] & (0x00000001 << BIT(x, y, char))) >> (BIT(x, y, char))
	  | ((board->w[BYTE(x, y, char)] & (0x00000001 << BIT(x, y, char))) >> (BIT(x, y, char))
	     << 1));
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
