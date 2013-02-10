/*
** main.c for gomoku in /home/shauny/local/gomoku/src
** 
** Made by Shauny
** Login   <shauny@epitech.net>
** 
** Started on  Wed Jan 16 16:37:43 2013 Shauny
** Last update Sun Feb 10 17:42:24 2013 Shauny
*/

#include		<sys/types.h>
#include		<sys/socket.h>
#include		<netinet/in.h>
#include		<arpa/inet.h>
#include		<netdb.h>
#include		<stdio.h>
#include		<stdlib.h>
#include		<unistd.h>
#include		<string.h>
#include		<time.h>
#include		<stdint.h>

#include		"board.h"
#include		"rule.h"
#include		"manip_boards.h"
#include		"IA.h"

int64_t timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
    ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

/* void			play(int s, char *buffer) */
/* { */
/*   char			str[64]; */

/*   snprintf(str, 64, "PLAY %s %s\n", "42", "42"); */
/*   if (write(s, str, strlen(str)) == -1) */
/*     exit(EXIT_FAILURE); */
/* } */

int			main(int ac, char **av)
{
  struct timespec start, end;
  struct protoent	*pe;
  struct sockaddr_in	sin;
  t_board		board;
  t_pos			move;
  char			buffer[256];
  char			play[64];
  int			s;
  char			rules;
  unsigned int		timeout;
  int			x1, y1, x2, y2;
  int			i;
  int			current_color;

  if (ac == 3)
    {
      init_board(&board);
      current_color = WHITE;
      pe = getprotobyname("TCP");
      if ((s = socket(AF_INET, SOCK_STREAM, pe->p_proto)) == -1)
	{
	  perror("gomoku");
	  return (EXIT_FAILURE);
	}
      sin.sin_family = AF_INET;
      sin.sin_port = htons(atoi(av[2]));
      sin.sin_addr.s_addr = inet_addr(av[1]);
      if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
	  close(s);
	  perror("gomoku");
	  return (EXIT_FAILURE);
	}
      if (write(s, "CONNECT CLIENT\n", 15) == -1)
	{
	  close(s);
	  perror("gomoku");
	  return (EXIT_FAILURE);
	}
      bzero(&buffer, 256);
      if (read(s, buffer, 255) < 1)
	{
	  close(s);
	  printf("Connection close\n");
	  return (EXIT_FAILURE);
	}
      if (strncmp(buffer, "RULES", 5) == 0)
	{
	  // Recupère les regles
	  if (strlen(buffer) >= 11)
	    {
	      rules = (buffer[6] & 1) | ((buffer[8] & 1) << 1);
	      timeout = atoi(&buffer[10]);
	    }
	  else
	    {
	      close(s);
	      printf("Erreur dans les règles\n");
	      return (EXIT_FAILURE);
	    }
	  // Boucle jusqu'a une victoire ou une defaite
	  while (strncmp(buffer, "WIN", 3) != 0 || strncmp(buffer, "LOSE", 4) != 0)
	    {
	      printf("-----------------------\nEt j'attends\n");
	      bzero(&buffer, 256);
	      if (read(s, buffer, 255) < 1)
		{
		  close(s);
		  printf("Connection close\n");
		  return (EXIT_FAILURE);
		}
	      printf("%s\n", buffer);
	      if (strncmp(buffer, "YOURTURN\n", 9) == 0)
		{
		  printf("C'est mon tour en %d\n", current_color);
		  clock_gettime(CLOCK_MONOTONIC, &start);
		  // play(s, buffer);
		  callIA(&board, rules, &move, current_color);
		  set_board(&board, move.x, move.y, current_color);
		  clock_gettime(CLOCK_MONOTONIC, &end);
		  bzero(&play, 64);
		  snprintf(play, 16, "PLAY %d %d\n", move.x, move.y);
		  if (write(s, play, strlen(play)) == -1)
		    {
		      close(s);
		      perror("gomoku");
		      return (EXIT_FAILURE);
		    }
		  if (((unsigned int)timespecDiff(&end, &start) / 1000000) > timeout)
		    printf("Prout\n");
		  printf("time: '%d'ms\n", (unsigned int)timespecDiff(&end, &start) / 1000000);
		}
	      else if (strncmp(buffer, "REM", 3) == 0)
		{
		  printf("Il y a une prise\n");
		  i = 4;
		  x1 = atoi(&buffer[i]);
		  while (buffer[i] != ' ' && buffer[i] != '\0')
		    i++;
		  if (buffer[i] == '\0')
		    {
		      close(s);
		      printf("Erreur dans la commande REM\n");
		      return (EXIT_FAILURE);
		    }
		  y1 = atoi(&buffer[i]);
		  while (buffer[i] != ' ' && buffer[i] != '\0')
		    i++;
		  if (buffer[i] == '\0')
		    {
		      close(s);
		      printf("Erreur dans la commande REM\n");
		      return (EXIT_FAILURE);
		    }
		  x2 = atoi(&buffer[i]);
		  while (buffer[i] != ' ' && buffer[i] != '\0')
		    i++;
		  if (buffer[i] == '\0')
		    {
		      close(s);
		      printf("Erreur dans la commande REM\n");
		      return (EXIT_FAILURE);
		    }
		  y2 = atoi(&buffer[i]);
		  set_board(&board, x1, y1, EMPTY);
		  set_board(&board, x2, y2, EMPTY);
		}
	      else if (strncmp(buffer, "ADD", 3) == 0)
		{
		  printf("Il y a un ajout par %d\n", current_color);
		  i = 4;
		  x1 = atoi(&buffer[i]);
		  while (buffer[i] != ' ' && buffer[i] != '\0')
		    i++;
		  if (buffer[i] == '\0')
		    {
		      close(s);
		      printf("Erreur dans la commande ADD\n");
		      return (EXIT_FAILURE);
		    }
		  y1 = atoi(&buffer[i]);
		  set_board(&board, x1, y1, current_color);
		  current_color = OPPOSITE(current_color);
		}
	    }
	  // Envoi du Win ou du Lose
	}
      close(s);
      return (EXIT_SUCCESS);
    }
  else
    {
      printf("Usage: network IP_ADDESS PORT\n");
      return (EXIT_FAILURE);
    }
}
