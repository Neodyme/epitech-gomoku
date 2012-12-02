/*
** display.h for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Sun Dec  2 18:17:00 2012 dorian schaegis
** Last update Sun Dec  2 23:30:40 2012 dorian schaegis
*/

#ifndef		GOMO_DISPLAY_HHHH__
#define		GOMO_DISPLAY_HHHH__

#include	<SDL/SDL.h>
#include	"board.h"

char            init_sdl(t_surfaces *surf);
void		show_background(SDL_Surface *background, SDL_Surface *screen);
void		place_pawns(t_board *board, t_surfaces *surf);

#endif
