/*
** manip_boards.h for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Sun Dec  2 15:13:36 2012 dorian schaegis
** Last update Sun Dec  2 16:38:50 2012 dorian schaegis
*/

#ifndef		__GOMO_MANIP_H
#define		__GOMO_MANIP_H

void		init_board(t_board *board);
void		set_board(t_board *board, char x, char y, char val);
char		get_board(t_board *board, register char x, register char y);
void		dump_board(t_board *board);


#endif
