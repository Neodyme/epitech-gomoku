/*
** main.c for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Wed Nov 21 14:26:37 2012 dorian schaegis
** Last update Wed Jan 16 12:37:15 2013 dorian schaegis
*/

#define		 _BSD_SOURCE
#include	<unistd.h>
#include	<SDL/SDL.h>
#include	<string.h>

#include	"board.h"
#include	"display.h"
#include	"loops.h"
#include	"getms.h"

int		main(int ac, char **av)
{
  t_surfaces	surf;
  t_board	board;

  getms("getms");
  getms(NULL);
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
