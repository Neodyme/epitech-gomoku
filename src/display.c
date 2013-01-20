/*
** display.c for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Sun Dec  2 18:14:22 2012 dorian schaegis
** Last update Sun Jan 20 06:35:37 2013 dorian schaegis
*/

#define		_BSD_SOURCE
#include	<SDL/SDL.h>
#include	<unistd.h>

#include	"board.h"
#include	"manip_boards.h"

void		place_pawns(t_board *board, t_surfaces *surf)
{
  SDL_Rect	pos;
  unsigned int	i;

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
	  SDL_BlitSurface(surf->blackstone, NULL, surf->screen, &pos);		  
	  break;
	case WHITE:
	  SDL_BlitSurface(surf->whitestone, NULL, surf->screen, &pos);
	  break;
	}
    }

  for (i = 0; i < (board->blacks*2); i++)
    {
      pos.x = -16; /* WTF */
      pos.y = i * 32;
      SDL_BlitSurface(surf->blackstone, NULL, surf->screen, &pos);
    }

  pos.x = 640 - 16;
  for (i = 0; i < (board->whites*2); i++)
    {
      pos.y = (640 - 32) - (i * 32);
      SDL_BlitSurface(surf->whitestone, NULL, surf->screen, &pos);
    }

}


char		init_sdl(t_surfaces *surf)
{
 
  if (SDL_Init(SDL_INIT_VIDEO))
    return (printf("Video initialization failed\n"));

  if ((surf->screen = SDL_SetVideoMode(640, 684, 24, SDL_HWSURFACE)) == NULL)
    return (printf("Set Video Mode failed\n"));

  if ((surf->background = SDL_LoadBMP("./res/board.bmp")) == NULL)
    return (printf("Texture couldn't be loaded\n"));

  if ((surf->title = SDL_LoadBMP("./res/title.bmp")) == NULL)
    return (printf("Texture couldn't be loaded\n"));
  if ((surf->blackwin = SDL_LoadBMP("./res/blackwin.bmp")) == NULL)
    return (printf("Texture couldn't be loaded\n"));
  if ((surf->whitewin = SDL_LoadBMP("./res/whitewin.bmp")) == NULL)
    return (printf("Texture couldn't be loaded\n"));

  if ((surf->cursor = SDL_LoadBMP("./res/cursor.bmp")) == NULL)
    return (printf("Texture couldn't be loaded\n"));


  if ((surf->blackstone = SDL_LoadBMP("./res/blackstone.bmp")) == NULL)
    return (printf("Texture couldn't be loaded\n"));
  if ((surf->whitestone = SDL_LoadBMP("./res/whitestone.bmp")) == NULL)
    return (printf("Texture couldn't be loaded\n"));
  if ((surf->nopestone = SDL_LoadBMP("./res/nope.bmp")) == NULL)
    return (printf("Texture couldn't be loaded\n"));

  if ((surf->rule3 = SDL_LoadBMP("./res/r3.bmp")) == NULL)
    return (printf("Texture couldn't be loaded\n"));
  if ((surf->rule5 = SDL_LoadBMP("./res/r5.bmp")) == NULL)
    return (printf("Texture couldn't be loaded\n"));


  SDL_SetColorKey(surf->cursor, SDL_SRCCOLORKEY, SDL_MapRGB(surf->cursor->format, 255, 0, 255));

  SDL_SetColorKey(surf->blackstone, SDL_SRCCOLORKEY, SDL_MapRGB(surf->blackstone->format, 255, 0, 255));
  SDL_SetColorKey(surf->whitestone, SDL_SRCCOLORKEY, SDL_MapRGB(surf->whitestone->format, 255, 0, 255));
  SDL_SetColorKey(surf->nopestone, SDL_SRCCOLORKEY, SDL_MapRGB(surf->nopestone->format, 255, 0, 255));

  SDL_WM_SetCaption("Gomoku", NULL);

  return (0);
}

void		free_sdl(t_surfaces *surf)
{
  SDL_FreeSurface(surf->screen);

  SDL_FreeSurface(surf->background);

  SDL_FreeSurface(surf->title);
  SDL_FreeSurface(surf->blackwin);
  SDL_FreeSurface(surf->whitewin);

  SDL_FreeSurface(surf->cursor);

  SDL_FreeSurface(surf->blackstone);
  SDL_FreeSurface(surf->whitestone);
  SDL_FreeSurface(surf->nopestone);

  SDL_FreeSurface(surf->rule3);
  SDL_FreeSurface(surf->rule5);
}

void		show_background(SDL_Surface *background, SDL_Surface *screen)
{
  SDL_Rect	pos;

  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 128, 128, 128));
  pos.w = 640;
  pos.h = 684;
  pos.x = 1;
  pos.y = 1;
  SDL_BlitSurface(background, NULL, screen, &pos);
}
