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

char		pose(t_board *board, t_pos *move, char current, char rules)
{
  char		get;
  int		iget;


  /* printf("At %i-%i: ", cor.move->x, cor.move->y); */

  // Pose
  get = get_board(board, move->x, move->y);
  if ((get == EMPTY) &&
      (!(rules & RULE3) || (rule3(board, move->x, move->y, current))))
    {
      set_board(board, move->x, move->y, current);
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
      printf(" Stone at %i:%i\n", move->x, move->y);
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

  // Prise
  iget = getprise(board, move->x, move->y, current);
  printf("%d\n", iget);
  if (iget)
    {
      printf("Taken %i ", get * 2);
      if (current == BLACK)
	{
	  board->blacks += iget;
	  printf("Black Stones (%i total)\n", board->blacks*2);
	}
      else
	{
	  board->whites += iget;
	  printf("Whites Stones (%i total)\n", board->whites*2);
	}
      prise(board, move->x, move->y, current);
    }

  // Règle de 5
  // (rules & RULE5) && 
  if (rule5(board, move->x, move->y, OPPOSITE(current)))
    {
      if (current == WHITE)
	printf("Blacks wins with a row!\n");
      if (current == BLACK)
	printf("Whites wins with a row!\n");
      return (OPPOSITE(current) + 10);
    }

  return (current);
}

char		game_loop(t_board *board, t_surfaces *surf, char mode)
{
  t_pos		moveIA;
  t_pos		move;
  char		rules;
  char		hint;

  SDL_Rect	pos; // Pour les placements dans la fenêtre
  SDL_Rect	cor; // Pour les placements dans le board

  SDL_Event     event;
  char		current;

  current = BLACK;
  SDL_ShowCursor(0);
  init_board(board);
  rules = RULE3 | RULE5;

  cor.x = 0;
  cor.y = 0;
  hint = 0;

  SDL_WaitEvent(&event);
  printf("New Game\n");
  while (current)
    {
      // Fuite
      if (((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE)) ||
	  (event.type == SDL_QUIT))
	current = 0;

      // Background
      show_background(surf->background, surf->screen);

      // IA
      if (mode && current == WHITE)
	{
	  callIA(board, rules, &moveIA, current);
	  current = pose(board, &moveIA, current, rules);
	}
      else if (hint)
	{
	  if (hint > 1)
	    {
	      /* printf("Hint!\n"); */
	      callIA(board, rules, &moveIA, current);
	      hint--;
	    }
	  pos.x = moveIA.x * 32 +16;
	  pos.y = moveIA.y * 32 +16;
	  pos.w = 32;
	  pos.h = 32;
	  SDL_BlitSurface(surf->cursor, NULL, surf->screen, &pos);
	}

      // Victoire par capture
      if (board->whites >= 5)
	{
	  printf("Blacks wins with captures!\n");
	  return (BLACK);
	}
      if (board->blacks >= 5)
	{
	  printf("Whites wins with captures!\n");
	  return (WHITE);
	}

      // Click
      if (event.type == SDL_MOUSEBUTTONUP)
	{
	  // Mouvements sur le board: position du curseur
	  if ((cor.x >= 0) && (cor.x < 19) && (cor.y >= 0) && (cor.y < 19))
	    {
	      move.x = cor.x;
	      move.y = cor.y;
	      current = pose(board, &move, current, rules);
	      if (hint)
		hint = 2;
	    }
	  // Mouvements en bas du board: sélection des règles
	  else if (event.motion.y > 632)
	    {
	      if (event.motion.x < 160)
		rules = rules ^ RULE3;
	      else if (event.motion.x < 320)
		rules = rules ^ RULE5;
	      else if (event.motion.x < 480)
		{
		  hint = !hint;
		  if (hint)
		    hint++;
		}
	      else if (event.motion.x < 640)
	        return (42);
	    }
	}

      // Affichage de l'état des règles
      if (rules)
	{
     	  pos.w = 320;
	  pos.h = 56;
	  pos.y = 630;
	  if (rules & RULE3)
	    {
	      pos.x = 1;
	      SDL_BlitSurface(surf->rule3, NULL, surf->screen, &pos);
	    }
	  if (rules & RULE5)
	    {
	      pos.x = 161;
	      SDL_BlitSurface(surf->rule5, NULL, surf->screen, &pos);
	    }
	  if (hint)
	    {
	      pos.x = 320;
	      SDL_BlitSurface(surf->hint, NULL, surf->screen, &pos);
	    }
	}

      // Affichage des pions
      place_pawns(board, surf);

      // Move
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

		  if (!getprise(board, cor.x, cor.y, current) && (rules & RULE3)
		      && (!rule3(board, cor.x, cor.y, current)))
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
	      SDL_ShowCursor(0);
	    }
	  else if (event.motion.y > 632)
	    SDL_ShowCursor(1);
	}

      SDL_Flip(surf->screen);
      if (current > 10)
	return (current - 10);
      SDL_PeepEvents(&event, 1337, SDL_GETEVENT, SDL_ALLEVENTS ^ SDL_QUITMASK);
      SDL_WaitEvent(&event);
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


      // Écran de victoire
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
		}
	    }
	  else
	    {
	      loop = 42;
	      current = surf->title;
	    }
	}
      // debug
      if ((loop == 1) || (loop == 2))
	place_pawns(board, surf);
      SDL_Flip(surf->screen);
    }
  return (0);
}
