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
  int		i;

  getms("get");
  get_board(board, x, y);
  getms(NULL);
  if ((get_board(board, x, y) == EMPTY) &&
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
      for (i = 0; i < 19 * 19; i++)
	{
	  if (prise(board, x, y))
	    {
	      printf("Taken two ");
	      if (current == BLACK)
		{
		  board->blacks++;
		  printf("Black Stones (%i total)\n", board->blacks*2);
		}
	      else
		{
		  board->whites++;
		  printf("Whites Stones (%i total)\n", board->whites*2);
		}
	    }
	}
    }
  else
    {
      printf("There's ");
      switch (get_board(board, x, y))
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


  current = BLACK;
  SDL_ShowCursor(0);
  init_board(board);
  rules = 3;

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
	  place_pawns(board, surf);
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

      // Click
      if (event.type == SDL_MOUSEBUTTONUP)
	{
	  if ((cor.x >= 0) && (cor.x < 19) && (cor.y >= 0) && (cor.y < 19))
	    {
	      /* printf("At %i-%i: ", cor.x, cor.y); */
	      current = pose(board, cor.x, cor.y, current);
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

      if (event.type == SDL_MOUSEBUTTONUP)
	{
	  long l = getlines(board, OPPOSITE(current), cor.x, cor.y);
	    printf("<%d %d %d %d %d %d %d %d>\n",
	    	   ((char*)&l)[0],
	    	   ((char*)&l)[1],
	    	   ((char*)&l)[2],
	    	   ((char*)&l)[3],
	    	   ((char*)&l)[4],
	    	   ((char*)&l)[5],
	    	   ((char*)&l)[6],
	    	   ((char*)&l)[7]);
	  (void)l;
	  if ((cor.x >= 0) && (cor.x < 19) && (cor.y >= 0) && (cor.y < 19))
	    {
	      /* printf("At %i-%i: ", cor.x, cor.y); */
	      
	      current = pose(board, cor.x, cor.y, current);
	      
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
