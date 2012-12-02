/*
** manip_boards.c for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Sun Dec  2 15:10:45 2012 dorian schaegis
** Last update Sun Dec  2 15:27:55 2012 dorian schaegis
*/

#include	<unistd.h>
#include	<string.h>

#include	<SDL/SDL.h>

#include	"board.h"

void		init_board(t_board *board)
{
  //		0b00100010
  memset(board, 0xAA, sizeof (t_board));
}


void		set_board(t_board *board, char x, char y, char val)
{
  unsigned int  byte;
  unsigned int  bit;
  unsigned char *data;

  data = (unsigned char*)board;
  byte = ((19 * (x * 2) + (y * 2)) / 8);
  bit = ((19 * (x * 2) + (y * 2)) % 8);

  data[byte] &= ~(0x00000003 << bit);
  data[byte] |= val << bit;
  (void)val;
}

char		get_board(t_board *board, register char x, register char y)
{
  return ((board->b[BYTE(x, y, char)] & (0x00000001 << BIT(x, y, char))) >> (BIT(x, y, char)));
}

void		dump_board2(t_board *board)
{
  unsigned int	i;

  for (i = 0; i < 19 * 19; i++)
    {
      printf("%i ", get_board(board, i / 19, i % 19));
      if ((i % 19) == 18)
	printf("\n");
    }
}

void		dump_board(t_board *board)
{
  int		x = 0, y = 0;
  
  while (x < 19)
    {
      y = 0;
      while (y < 19)
	{
	  get_board(board, x, y);
	  y++;
	}
      x++;
    }
}
