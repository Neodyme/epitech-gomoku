/*
** getms.c for gomoku in /a/galaxy/far/far/away
** 
** Made by dorian schaegis
** Login   <schaeg_d@epitech.net>
** 
** Started on  Tue Jan 15 18:08:14 2013 dorian schaegis
** Last update Tue Jan 15 20:02:31 2013 dorian schaegis
*/

#include	<time.h>
#include	<stdio.h>
#include	<stdlib.h>

void		getms(char *str)
{
  struct timespec		tp;
  static struct timespec	lastcall;

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp);
  if (str != NULL)
    {
      lastcall.tv_nsec = tp.tv_nsec;
      printf("%s: ", str);
    }    
  else
    {
      if ((tp.tv_nsec - lastcall.tv_nsec) < 0)
	tp.tv_nsec = 1000000000 + tp.tv_nsec - lastcall.tv_nsec;
      else
	tp.tv_nsec = tp.tv_nsec - lastcall.tv_nsec;
      printf("%i\n", (int)tp.tv_nsec);
    }
}
