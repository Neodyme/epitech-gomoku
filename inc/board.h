/*
** board.h<2> for gomoku in /a/galaxy/far/far/away
**
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
**
** Started on  Tue Nov 27 18:59:18 2012 dorian schaegis
*/


#ifndef			GOMO_BOARD
# define		GOMO_BOARD

# include	<unistd.h>
# include	<SDL/SDL.h>
# include	<string.h>

# define		EMPTY	0x00 /* 00 */
# define		BLACK	0x01 /* 01 */
# define		WHITE	0x02 /* 10 */

# define		OMFGBUG	0x03 /* ZZOMG TIHS IS IMPOSSSIBRRUUUUUUU */

struct		s_board
{
  unsigned char	board[92];
  unsigned char	r3board[92];
  unsigned int	blacks;
  unsigned int	whites;
  char		eaten[64];
};

typedef struct s_board t_board;

struct		s_dl
{
  SDL_Surface	*screen;

  SDL_Surface	*background;

  SDL_Surface	*title;
  SDL_Surface	*blackwin;
  SDL_Surface	*whitewin;

  SDL_Surface	*cursor;

  SDL_Surface	*blackstone;
  SDL_Surface	*whitestone;
  SDL_Surface	*nopestone;
};

typedef struct s_dl t_surfaces;


//typedef unsigned char t_board[91];

# define BYTE(X, Y, TYPE) ((38 * X + Y * 2) / 8)
# define BIT(X, Y, TYPE)  ((38 * X + Y * 2) % 8)

struct		s_pos
{
  int		x;
  int		y;
};

typedef	struct s_pos	t_pos;

#endif
