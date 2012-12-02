/*
** main.c for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Wed Nov 21 14:26:37 2012 dorian schaegis
*/

#define		 _BSD_SOURCE
#include	<unistd.h>
#include	<SDL/SDL.h>
#include	<string.h>

#include	"board.h"
#include	"manip_boards.h"

void		display_board(t_board *board, SDL_Surface *screen, SDL_Surface *blackstone, SDL_Surface *whitestone)
{
  SDL_Rect	pos;
  int		i;

  pos.w = 32;
  pos.h = 32;

  for (i = 0; i < 19 * 19; i++)
    {
      pos.x = (i / 19) * 32 + 16;
      pos.y = (i % 19) * 32 + 16;
      printf("%i:%i\n", i/19, i%19);
      printf("caca %x\n", get_board(board, (i / 19), (i % 19)));
      switch (get_board(board, (i / 19), (i % 19)))
	    {
	    case BLACK:
	      printf("BLACK");
	      SDL_BlitSurface(blackstone, NULL, screen, &pos);		  
	      break;
	    case WHITE:
	      printf("WHITE");
	      SDL_BlitSurface(whitestone, NULL, screen, &pos);
	      break;
	    }
    }
}


int		main()
{
  SDL_Surface	*screen;

  SDL_Surface	*blackstone;
  SDL_Surface	*whitestone;
  SDL_Surface	*background;

  SDL_Rect	pos;
  SDL_Rect	cor;
  SDL_Event     event;
  char		current;

  t_board	board;


  init_board(&board);

  dump_board2(&board);

  SDL_Init(SDL_INIT_VIDEO);

  screen = SDL_SetVideoMode(640, 640, 24, SDL_HWSURFACE);

  blackstone = SDL_ConvertSurface(SDL_LoadBMP("./res/blackstone.bmp"), screen->format, SDL_HWSURFACE);
  whitestone = SDL_ConvertSurface(SDL_LoadBMP("./res/whitestone.bmp"), screen->format, SDL_HWSURFACE);
  background = SDL_LoadBMP("./res/board.bmp");

  SDL_SetColorKey(blackstone, SDL_SRCCOLORKEY, SDL_MapRGB(blackstone->format, 255, 0, 255));
  SDL_SetColorKey(whitestone, SDL_SRCCOLORKEY, SDL_MapRGB(whitestone->format, 255, 0, 255));

  SDL_WM_SetCaption("Gomoku", NULL);

  current = BLACK;
  SDL_ShowCursor(0);
  /* set_board(&board, 2, 2, BLACK); */
  /* get_board(&board, 2, 2); */

  while (1)
    {
      SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 128, 128, 128));
      pos.w = 640;
      pos.h = 640;
      pos.x = 1;
      pos.y = 1;
      SDL_BlitSurface(background, NULL, screen, &pos);

      display_board(&board, screen, blackstone, whitestone);
      SDL_WaitEvent(&event);
      if (((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE)) || 
	  (event.type == SDL_QUIT))
	  return (0);

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
	      if (current == BLACK)
		SDL_BlitSurface(blackstone, NULL, screen, &pos);
	      else
		SDL_BlitSurface(whitestone, NULL, screen, &pos);
	    }
	}

      if (event.type == SDL_MOUSEBUTTONUP)
	{
	  if ((cor.x >= 0) && (cor.x < 19) && (cor.y >= 0) && (cor.y < 19))
	    {
	      /* printf("At %i-%i: ", cor.x, cor.y); */
	      if (get_board(&board, cor.x, cor.y) == EMPTY)
		{
		  set_board(&board, cor.x, cor.y, current);
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
		  if (get_board(&board, cor.x, cor.y) == BLACK)
		    printf("Black");
		  else
		    printf("White");
		  printf(" Stone here\n");
		}
	    }
	}
      usleep(500);
      SDL_Flip(screen);
    }
  return (0);
}
