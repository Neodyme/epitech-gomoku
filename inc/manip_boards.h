/*
** manip_boards.h for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Sun Dec  2 15:13:36 2012 dorian schaegis
** Last update Thu Jan 17 19:26:13 2013 Prost P.
*/

#ifndef		__GOMO_MANIP_H
#define		__GOMO_MANIP_H

#define UP_L		7
#define UP_C		6
#define UP_R		5
#define MI_L		4
#define MI_R		3
#define DO_L		2
#define DO_C		1
#define DO_R		0

long	getlines(t_board *board, int color, unsigned int x, unsigned int y);

void		init_board(t_board *board);
inline void	set_board(t_board *board, register int x, register int y, int val);
inline char    	get_board(t_board *board, register int x, register int y);
void		dump_board(t_board *board);


#endif
