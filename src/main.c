/*
** main.c for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Wed Nov 21 14:26:37 2012 dorian schaegis
** Last update Sun Dec  2 22:51:23 2012 dorian schaegis
*/

#define		 _BSD_SOURCE
#include	<unistd.h>
#include	<SDL/SDL.h>
#include	<string.h>

#include	"board.h"
#include	"display.h"
#include	"manip_boards.h"

int		main()
{
  t_surfaces	surf;
  t_board	board;

  if (init_sdl(&surf))
    return (1);

  game_loop(&board, &surf);

  return (0);
}
