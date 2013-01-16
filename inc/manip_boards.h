/*
** manip_boards.h for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Sun Dec  2 15:13:36 2012 dorian schaegis
** Last update Wed Jan 16 11:44:43 2013 Prost P.
*/

#ifndef		__GOMO_MANIP_H
#define		__GOMO_MANIP_H

void		init_board(t_board *board);
inline void	set_board(t_board *board, register int x, register int y, int val);
inline char    	get_board(t_board *board, register int x, register int y);
void		dump_board(t_board *board);


#endif
