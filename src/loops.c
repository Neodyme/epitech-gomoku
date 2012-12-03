/*
** loops.c for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Sun Dec  2 23:30:56 2012 dorian schaegis
** Last update Mon Dec  3 02:16:30 2012 Prost P.
*/

#include	<SDL/SDL.h>
#include	"board.h"
#include	"rule.h"
#include	"display.h"
#include	"manip_boards.h"

char		game_loop(t_board *board, t_surfaces *surf)
{
  int		i;
  SDL_Rect	pos;
  SDL_Rect	cor;

  SDL_Event     event;
  char		current;

  current = BLACK;
  SDL_ShowCursor(0);
  init_board(board);
  while (current)
    {
      show_background(surf->background, surf->screen);
      place_pawns(board, surf);

      SDL_WaitEvent(&event);

      if (((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE)) || 
	  (event.type == SDL_QUIT))
	current = 0;

      if (event.type == SDL_MOUSEMOTION)
	{
     	  pos.w = 32;
	  pos.h = 32;
	  pos.x = (event.motion.x / 32) * 32 - 16;
	  pos.y = (event.motion.y / 32) * 32 - 16;
	  cor.x = event.motion.x / 32 - 1;
	  cor.y = event.motion.y / 32 - 1;
	  /* printf("x:%i y:%i\n", pos.x / 32, pos.y / 32); */
	  if ((cor.x >= 0) && (cor.x < 19) && (cor.y >= 0) && (cor.y < 19))
	    {
	      rule5(board, cor.x, cor.y, current);
	      if ((get_board(board, cor.x, cor.y) != EMPTY) ||
		  (!rule3(board, cor.x, cor.y, current)))
		SDL_BlitSurface(surf->nopestone, NULL, surf->screen, &pos);	  		
	      else if (current == BLACK)
		SDL_BlitSurface(surf->blackstone, NULL, surf->screen, &pos);
	      else
		SDL_BlitSurface(surf->whitestone, NULL, surf->screen, &pos);
	    }
	}

      if (event.type == SDL_MOUSEBUTTONUP)
	{
	  if ((cor.x >= 0) && (cor.x < 19) && (cor.y >= 0) && (cor.y < 19))
	    {
	      /* printf("At %i-%i: ", cor.x, cor.y); */
	      if ((get_board(board, cor.x, cor.y) == EMPTY) && 
		  (rule3(board, cor.x, cor.y, current)))
		{
		  set_board(board, cor.x, cor.y, current);
		  printf("Placed a ");
		  if (current == BLACK)
		    {
		      printf("Black");
		      current = WHITE;
		    }
		  else
		    {
		      printf("White");
		      current = BLACK;
		    }
		  printf(" Stone at %i:%i\n", cor.x, cor.y);		    
		}
	      else
		{
		  printf("There's ");
		  switch (get_board(board, cor.x, cor.y))
		    {
		    case BLACK:
		      printf("a Black Stone");
		      break;
		    case WHITE:
		      printf("a White Stone");
		      break;
		    case EMPTY:
		      printf("nothing");
		      break;
		    }
		  printf(" here\n");
		}
	    }
	}
      usleep(500);
      for (i = 0; i < 19 * 19; i++)
      	{
      	  prise(board, i/19, i%19);
      	}
      SDL_Flip(surf->screen);
    }
  return (0);
}

char		menu_loop(t_board *board, t_surfaces *surf)
{
  SDL_Event     event;
  char		loop;

  loop = 1;
  while (loop)
    {
      show_background(surf->title, surf->screen);
      SDL_ShowCursor(1);
      SDL_WaitEvent(&event);

      if (((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE)) || 
	  (event.type == SDL_QUIT))
	loop = 0;
      if (event.type == SDL_MOUSEBUTTONUP)
	{
	  /* printf("%i:%i\n", event.motion.x, event.motion.y); */
	  if ((event.motion.x > 180) && (event.motion.x < 480))
	    {
	      if ((event.motion.y > 400) && (event.motion.y < 450))
		printf("IA Mode is currently unavailable\n");
	      if ((event.motion.y > 480) && (event.motion.y < 520))
		game_loop(board, surf);
	      if ((event.motion.y > 560) && (event.motion.y < 600))
		loop = 0;
	    }
	}
      SDL_Flip(surf->screen);
    }
  return (0);
}
