/*
** display.c for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Sun Dec  2 18:14:22 2012 dorian schaegis
** Last update Sun Dec  2 18:52:13 2012 dorian schaegis
*/

#define		_BSD_SOURCE
#include	<SDL/SDL.h>
#include	<unistd.h>

#include	"board.h"
#include	"manip_boards.h"

void		display_board(t_board *board, SDL_Surface *screen, SDL_Surface *blackstone, SDL_Surface *whitestone)
{
  SDL_Rect	pos;
  int		i;

  /* dump_board2(board); */
  /* printf("\n---\n\n"); */

  pos.w = 32;
  pos.h = 32; 

  for (i = 0; i < 19 * 19; i++)
    {
      pos.x = (i / 19) * 32 + 16;
      pos.y = (i % 19) * 32 + 16;
      switch (get_board(board, (i / 19), (i % 19)))
	    {
	    case BLACK:
	      SDL_BlitSurface(blackstone, NULL, screen, &pos);		  
	      break;
	    case WHITE:
	      SDL_BlitSurface(whitestone, NULL, screen, &pos);
	      break;
	    }
    }
}


char		init_sdl(t_surfaces *surf)
{
  SDL_Init(SDL_INIT_VIDEO);

  surf->screen = SDL_SetVideoMode(640, 640, 24, SDL_HWSURFACE);

  surf->blackstone = SDL_LoadBMP("./res/blackstone.bmp");
  surf->whitestone = SDL_LoadBMP("./res/whitestone.bmp");
  surf->nopestone = SDL_LoadBMP("./res/nope.bmp");

  surf->title = SDL_LoadBMP("./res/title.bmp");
  surf->background = SDL_LoadBMP("./res/board.bmp");

  SDL_SetColorKey(surf->blackstone, SDL_SRCCOLORKEY, SDL_MapRGB(surf->blackstone->format, 255, 0, 255));
  SDL_SetColorKey(surf->whitestone, SDL_SRCCOLORKEY, SDL_MapRGB(surf->whitestone->format, 255, 0, 255));
  SDL_SetColorKey(surf->nopestone, SDL_SRCCOLORKEY, SDL_MapRGB(surf->nopestone->format, 255, 0, 255));

  SDL_WM_SetCaption("Gomoku", NULL);

  SDL_ShowCursor(0);
  return (0);
}

void		show_background(SDL_Surface *background, SDL_Surface *screen)
{
  SDL_Rect	pos;

  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 128, 128, 128));
  pos.w = 640;
  pos.h = 640;
  pos.x = 1;
  pos.y = 1;
  SDL_BlitSurface(background, NULL, screen, &pos);

}

char		game_loop(t_board *board, t_surfaces *surf)
{
  SDL_Rect	pos;
  SDL_Rect	cor;

  SDL_Event     event;
  char		current;

  current = BLACK;

  while (current)
    {
      show_background(surf->background, surf->screen);
      display_board(board, surf->screen, surf->blackstone, surf->whitestone);

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
	      if (get_board(board, cor.x, cor.y) != EMPTY)
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
	      if (get_board(board, cor.x, cor.y) == EMPTY)
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
		  printf("There's a ");
		  if (get_board(board, cor.x, cor.y) == BLACK)
		    printf("Black");
		  else
		    printf("White");
		  printf(" Stone here\n");
		}
	    }
	}
      usleep(500);
      SDL_Flip(surf->screen);
    }
  return (0);
}
