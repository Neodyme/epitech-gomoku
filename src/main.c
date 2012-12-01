/*
** main.c for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Wed Nov 21 14:26:37 2012 dorian schaegis
** Last update Sat Dec  1 01:01:01 2012 Prost P.
*/

#define		 _BSD_SOURCE
#include	<unistd.h>
#include	<SDL/SDL.h>
#include	<string.h>

#include	"board.h"

void		init_board(t_board *board)
{
  memset(board, 0x33, 91);
}

void		set_board(t_board *board, char x, char y, char val)
{
  unsigned int  byte;
  unsigned int  bit;
  unsigned char *data;

  data = (unsigned char*)board;
  byte = ((19 * (x * 2) + (y * 2)) / 8);
  bit = ((19 * (x * 2) + (y * 2)) % 8);

  data[byte] &= ~(0x00000003 << bit);
  data[byte] |= val << bit;
  (void)val;
}

char		get_board(t_board *board, char x, char y)
{
  unsigned int	byte;
  unsigned int	bit;
  unsigned char	*data;
  
  data = (unsigned char*)board;
  byte = ((19 * (x * 2) + (y * 2)) / 8);
  bit = ((19 * (x * 2) + (y * 2)) % 8);
  printf("\tbyte[%i] bits[%i-%i] >> %i(%X)\n", byte, bit, bit + 1, (data[byte] & (0x00000003 << bit)) >> bit, (0x00000003 << bit));
/* data[(19 * (x * 2) + (y * 2) / 8)] & ((((19 * (x * 2) + (y * 2)) % 8) + 1) | ((((19 * (x * 2) + (y * 2)) % 8) + 1) << 1)) */
/* & data[(19 * (x * 2) + (y * 2) % 8) | ((19 * (x * 2) + (y * 2) % 8) >> 1)]); */
/* largeur des lignes * ligne voulue * 2 + colonne voulue * 2  */
/* / 8 */
/* AND % 4 * 2  */
  (void)data;
  return ((data[byte] & (0x00000003 << bit)) >> bit);
}

void		dump_board(t_board *board)
{
  int		x = 0, y = 0;
  
  while (x < 19)
    {
      y = 0;
      while (y < 19)
	{
	  printf("At %i-%i: ", x, y);
	  get_board(board, x, y);
	  y++;
	}
      x++;
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

  dump_board(&board);

  while (1)
    {
      SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 128, 128, 128));  
      pos.w = 640;
      pos.h = 640;
      pos.x = 1;
      pos.y = 1;
      SDL_BlitSurface(background, NULL, screen, &pos);
      /*      SDL_WaitEvent(&event); */
      SDL_PollEvent(&event);  
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
	      printf("At %i-%i: ", cor.x, cor.y);
	      get_board(&board, cor.x, cor.y);
	      if (current == BLACK)
		current = WHITE;
	      else
		current = BLACK;
	      set_board(&board, cor.x, cor.y, current);
	    }
	}
      SDL_Flip(screen);
    }
  return (0);
}

