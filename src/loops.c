/*
** loops.c for gomoku in /a/galaxy/far/far/away
**
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
**
** Started on  Sun Dec  2 23:30:56 2012 dorian schaegis
*/

#include	<SDL/SDL.h>
#include	"board.h"
#include	"rule.h"
#include	"display.h"
#include	"IA.h"
#include	"manip_boards.h"
#include	"getms.h"

#define		OPPOSITE(COLOR) (~(COLOR) & 0b00000011)

long	getlines(t_board *board, int color, unsigned int x, unsigned int y);

char		pose(t_board *board, int x, int y, char current)
{
  char		get;
  
  get = get_board(board, x, y);
  if ((get == EMPTY) &&
      (rule3(board, x, y, current)))
    {
      set_board(board, x, y, current);
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
      printf(" Stone at %i:%i\n", x, y);
    }
  else
    {
      printf("There's ");
      switch (get)
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
  return (current);
}

char		game_loop(t_board *board, t_surfaces *surf, char mode)
{
  int		i;
  t_pos		moveIA;
  char		rules;

  SDL_Rect	pos;
  SDL_Rect	cor;

  SDL_Event     event;
  char		current;

  int		get;

  current = BLACK;
  SDL_ShowCursor(0);
  init_board(board);
  rules = 0;

  cor.x = 0;
  cor.y = 0;

  while (current)
    {
      show_background(surf->background, surf->screen);
      if (mode && current == WHITE)
	{
	  callIA(board, rules, &moveIA);
	  current = pose(board, moveIA.x, moveIA.y, current);
	  for (i = 0; i < 19 * 19; i++)
	    {
	      if (rule5(board, i/19, i%19, OPPOSITE(current)))
		{
		  if (get_board(board, i/19, i%19) == BLACK)
		    printf("Blacks wins with a row!\n");
		  if (get_board(board, i/19, i%19) == WHITE)
		    printf("Whites wins with a row!\n");
		  return (get_board(board, i/19, i%19));
		}
	    }
	}

      SDL_WaitEvent(&event);

      if (board->whites >= 5)
	{
	  printf("Blacks wins with captures!\n");
	  return (BLACK);
	}
      if (board->blacks >= 5)
	{
	  printf("Blacks wins with captures!\n");
	  return (WHITE);
	}

      if (((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE)) ||
	  (event.type == SDL_QUIT))
	current = 0;


      if (event.type == SDL_MOUSEBUTTONUP)
	{
	  if ((cor.x >= 0) && (cor.x < 19) && (cor.y >= 0) && (cor.y < 19))
	    {
	      /* printf("At %i-%i: ", cor.x, cor.y); */	      
	      current = pose(board, cor.x, cor.y, current);
	      get = getprise(board, cor.x, cor.y, current);	      
	      if (get)
		{
		  printf("Taken %i ", get * 2);
		  if (current == BLACK)
		    {
		      board->blacks += get;
		      printf("Black Stones (%i total)\n", board->blacks*2);
		    }
		  else
		    {
		      board->whites += get;
		      printf("Whites Stones (%i total)\n", board->whites*2);
		    }
		  prise(board, cor.x, cor.y, current);
		}
	    }
	  for (i = 0; i < 19 * 19; i++)
	    {
	      if (rule5(board, i/19, i%19, OPPOSITE(current)))
		{
		  if (get_board(board, i/19, i%19) == BLACK)
		    printf("Blacks wins with a row!\n");
		  if (get_board(board, i/19, i%19) == WHITE)
		    printf("Whites wins with a row!\n");
		  return (get_board(board, i/19, i%19));
		}
	    }
	}

      place_pawns(board, surf);

      if ((event.type == SDL_MOUSEMOTION) || (event.type == SDL_MOUSEBUTTONUP))
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
	      if (get_board(board, cor.x, cor.y) == EMPTY)
		{
		  if (!rule3(board, cor.x, cor.y, current))
		    SDL_BlitSurface(surf->nopestone, NULL, surf->screen, &pos);
		  else if (current == BLACK)
		    {
		      SDL_BlitSurface(surf->blackstone, NULL, surf->screen, &pos);
		      SDL_BlitSurface(surf->cursor, NULL, surf->screen, &pos);
		    }
		  else
		    {
		      SDL_BlitSurface(surf->whitestone, NULL, surf->screen, &pos);
		      SDL_BlitSurface(surf->cursor, NULL, surf->screen, &pos);
		    }

		}
	      else
		SDL_BlitSurface(surf->nopestone, NULL, surf->screen, &pos);
	    }
	}


      SDL_Flip(surf->screen);
    }
  return (42);
}

char		menu_loop(t_board *board, t_surfaces *surf)
{
  SDL_Event     event;
  char		loop;
  SDL_Surface	*current;

  loop = 42;
  current = surf->title;
  while (loop)
    {
      show_background(current, surf->screen);
      SDL_ShowCursor(1);
      SDL_WaitEvent(&event);

      if ((((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE)) ||
	   (event.type == SDL_QUIT)) && (current == surf->title))
	loop = 0;

      if ((((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE)) ||
	   (event.type == SDL_QUIT)) && (current != surf->title))
	{
	  loop = 42;
	  current = surf->title;
	}

      if (event.type == SDL_MOUSEBUTTONUP)
	{
	  if (current == surf->title)
	    {
	      if ((event.motion.x > 180) && (event.motion.x < 480))
		{
		  if ((event.motion.y > 400) && (event.motion.y < 450))
		    loop = game_loop(board, surf, 1);
		  if ((event.motion.y > 480) && (event.motion.y < 520))
		    loop = game_loop(board, surf, 0);
		  if ((event.motion.y > 560) && (event.motion.y < 600))
		    loop = 0;

		  if (loop == 1)
		    current = surf->blackwin;
		  if (loop == 2)
		    current = surf->whitewin;
		  printf("%i\n", loop);
		}
	    }
	  else
	    {
	      loop = 42;
	      current = surf->title;
	    }
	}
      SDL_Flip(surf->screen);
    }
  return (0);
}
