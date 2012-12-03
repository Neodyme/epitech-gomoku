/*
** main.c for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Wed Nov 21 14:26:37 2012 dorian schaegis
** Last update Mon Dec  3 04:55:52 2012 dorian schaegis
*/

#define		 _BSD_SOURCE
#include	<unistd.h>
#include	<SDL/SDL.h>
#include	<string.h>

#include	"board.h"
#include	"display.h"
#include	"loops.h"

int		main(int ac, char **av)
{
  t_surfaces	surf;
  t_board	board;

  if (init_sdl(&surf))
    return (1);
  if ((ac > 1) && (!strcmp(av[1], "-immed")))
    game_loop(&board, &surf);
  else
    menu_loop(&board, &surf);
  printf("Exiting\n");
  free_sdl(&surf);
  return (0);
}
