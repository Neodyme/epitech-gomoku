/*
** board.h<2> for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Tue Nov 27 18:59:18 2012 dorian schaegis
** Last update Sun Dec  2 18:07:08 2012 dorian schaegis
*/

#ifndef		GOMO_BOARD
# define		GOMO_BOARD

# define		EMPTY	0x00 /* 00 */
# define		BLACK	0x01 /* 01 */
# define		WHITE	0x02 /* 10 */

# define		OMFGBUG	0x03 /* ZZOMG TIHS IS IMPOSSSIBRRUUUUUUU */

struct		s_board
{
  unsigned char	b[46];
  unsigned char	w[46];
};

typedef struct s_board t_board;

struct		s_dl
{
  SDL_Surface	*screen;

  SDL_Surface	*background;
  SDL_Surface	*title;

  SDL_Surface	*blackstone;
  SDL_Surface	*whitestone;
  SDL_Surface	*nopestone;
};

typedef struct s_dl t_surfaces;


//typedef unsigned char t_board[91];

# define BYTE(X, Y, TYPE) ((19 * X + Y) / 8)
# define BIT(X, Y, TYPE)  ((19 * X + Y) % 8)

#endif
