/*
** main.c for gomoku in /home/shauny/local/gomoku/src
** 
** Made by Shauny
** Login   <shauny@epitech.net>
** 
** Started on  Wed Jan 16 16:37:43 2013 Shauny
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

char			**message_to_wordtab(char *buffer)
{
  char			**ret;
  int			i;
  int			j;
  int			k;

  i = 0;
  j = 0;
  k = 0;
  while (buffer[i] != '\0')
    {
      if (buffer[i] == '\n')
	k++;
      i++;
    }
  ret = malloc(sizeof (**ret) * (j + 1));
  i = 0;
  k = 0;
  while (buffer[0] != '\0')
    {
      ret[i++] = strndup(buffer, strchr(buffer, '\n') - buffer);
      buffer = strchr(buffer, '\n') + 1;
    }
  ret[i] = NULL;
  return (ret);
}

int			main(int ac, char **av)
{
  struct timespec	start, end;
  struct protoent	*pe;
  struct sockaddr_in	sin;
  t_board		board;
  t_pos			move;
  char			**message;
  char			buffer[256];
  char			play[64];
  int			s;
  char			rules;
  unsigned int		timeout;
  int			x1, y1, x2, y2;
  int			i, mess;
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
	  while (1)
	    {
	      mess = 0;
	      printf("-----------------------\nEt j'attends\n");
	      bzero(&buffer, 256);
	      if (read(s, buffer, 255) < 1)
		{
		  close(s);
		  printf("Connection close\n");
		  return (EXIT_FAILURE);
		}
	      printf("%s\n", buffer);
	      message = message_to_wordtab(buffer);
	      while (message[mess] != 0)
		{
		  if (strncmp(message[mess], "YOURTURN", 8) == 0)
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
		  else if (strncmp(message[mess], "REM", 3) == 0)
		    {
		      printf("Il y a une prise\n");
		      i = 4;
		      x1 = atoi(&message[mess][i]);
		      while (message[mess][i] != ' ' && message[mess][i] != '\0')
			i++;
		      if (message[mess][i] == '\0')
			{
			  close(s);
			  printf("Erreur dans la commande REM\n");
			  return (EXIT_FAILURE);
			}
		      y1 = atoi(&message[mess][i]);
		      while (message[mess][i] != ' ' && message[mess][i] != '\0')
			i++;
		      if (message[mess][i] == '\0')
			{
			  close(s);
			  printf("Erreur dans la commande REM\n");
			  return (EXIT_FAILURE);
			}
		      x2 = atoi(&message[mess][i]);
		      while (message[mess][i] != ' ' && message[mess][i] != '\0')
			i++;
		      if (message[mess][i] == '\0')
			{
			  close(s);
			  printf("Erreur dans la commande REM\n");
			  return (EXIT_FAILURE);
			}
		      y2 = atoi(&message[mess][i]);
		      set_board(&board, x1, y1, EMPTY);
		      set_board(&board, x2, y2, EMPTY);
		    }
		  else if (strncmp(message[mess], "ADD", 3) == 0)
		    {
		      printf("Il y a un ajout par %d\n", current_color);
		      i = 4;
		      x1 = atoi(&message[mess][i]);
		      while (message[mess][i] != ' ' && message[mess][i] != '\0')
			i++;
		      if (message[mess][i] == '\0')
			{
			  close(s);
			  printf("Erreur dans la commande ADD\n");
			  return (EXIT_FAILURE);
			}
		      y1 = atoi(&message[mess][i]);
		      set_board(&board, x1, y1, current_color);
		      current_color = OPPOSITE(current_color);
		    }
		  else if (strncmp(message[mess], "WIN", 3) == 0 || strncmp(message[mess], "LOSE", 4) == 0)
		    {
		      i = 0;
		      if (strncmp(message[mess], "WIN", 3) == 0)
			printf("You win !\nVictoire");
		      else
			printf("You loose !\nDefaite");
		      while (message[mess][i] != ' ' && message[mess][i] != '\0')
			i++;
		      if (message[mess][i] == '\0')
			{
			  close(s);
			  printf("Erreur pour connaitre la raison de WIN ou LOSE\n");
			  return (EXIT_FAILURE);
			}
		      while (message[mess][i] == ' ' && message[mess][i] != '\0')
			i++;
		      if (message[mess][i] == '\0')
			{
			  close(s);
			  printf("Erreur pour connaitre la raison de WIN ou LOSE\n");
			  return (EXIT_FAILURE);
			}
		      if (strncmp(&message[mess][i], "CAPTURE", 7) == 0)
			{
			  printf(" par capture de 10 pierres ou plus.\n");
			  return (EXIT_SUCCESS);
			}
		      if (strncmp(&message[mess][i], "FIVEALIGN", 9) == 0)
			{
			  printf(" par alignement de 5 pierres.\n");
			  return (EXIT_SUCCESS);
			}
		      if (strncmp(&message[mess][i], "RULEERR", 7) == 0)
			{
			  printf(" pour non-respect des regles.\n");
			  return (EXIT_SUCCESS);
			}
		      if (strncmp(&message[mess][i], "TIMEOUT", 7) == 0)
			{
			  printf(" par timeout.\n");
			  return (EXIT_SUCCESS);
			}
		    }
		  else
		    printf("Erreur commande bizarre\n");
		  mess++;
		}
	    }
	}
      close(s);
      printf("Erreur lors de la reception des regles\n");
      return (EXIT_FAILURE);
    }
  else
    {
      printf("Usage: network IP_ADDESS PORT\n");
      return (EXIT_FAILURE);
    }
}
