/*
** IA.c for rtype in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Tue Jan 15 17:03:24 2013 dorian schaegis
** Last update Tue Jan 15 17:13:36 2013 dorian schaegis
*/


#include	"IA.h"
#include	"board.h"

t_pos		callIA(t_board *board)
{
  t_pos		ret;

  (void)board;
  ret = malloc(sizeof(t_pos));
  if (ret != NULL)
    {
      /* call to minmax */
    }
  return (ret);
}
