/*
** manip_boards.h for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Sun Dec  2 15:13:36 2012 dorian schaegis
** Last update Thu Jan 31 16:03:32 2013 Prost P.
*/

#ifndef		__GOMO_MANIP_H
#define		__GOMO_MANIP_H

#define UP_L		167
#define UP_C		134
#define UP_R		149
#define MI_L		36
#define MI_R		19
#define DO_L		98
#define DO_C		65
#define DO_R		80

#define BLOCKED		0x80
#define ISBLOCKED(L)	((L) & (0x80))
#define PRENABLE       	0x40
#define ISPRENABLE(L)	((L) & (0x80))
#define L_SIZE		0x3F
#define GETLSIZE(L)	((L) & (0x7F))

int		isprenable(t_board *board, unsigned int x, unsigned int y, int color);
long		longgetlines(t_board *board, int color, unsigned int x, unsigned int y);
long		getlines(t_board *board, int color, unsigned int x, unsigned int y);
long		getprise(t_board *board, int color, unsigned int x, unsigned int y);

void		init_board(t_board *board);
inline void	set_board(t_board *board, register int x, register int y, int val);
inline int    	get_board(t_board *board, register int x, register int y);
void		dump_board(t_board *board);


#endif
