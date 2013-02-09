/*
** main.c for gomoku in /home/shauny/local/gomoku/src
** 
** Made by Shauny
** Login   <shauny@epitech.net>
** 
** Started on  Wed Jan 16 16:37:43 2013 Shauny
** Last update Sat Feb  9 17:03:41 2013 Shauny
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

void			toggle_rules(char *str)
{
  // Doit set les règles dans la structure
  if (strlen(str) >= 11)
    {
      str[6]; // DOUBLE_3
      str[8]; // BREAKING 5
      &str[10]; // TIMEOUT
    }
}

void			play(int s, char *buffer)
{
  char			str[64];

  snprintf(str, 64, "PLAY %s %s\n", "42", "42");
  if (write(s, str, strlen(str)) == -1)
    exit(EXIT_FAILURE);
}

void			main(int ac, char **av)
{
  struct protoent	*pe;
  struct sockaddr_in	sin;
  char			buffer[256];
  int			s;

  pe = getprotobyname("TCP");
  if ((s = socket(AF_INET, SOCK_STREAM, pe->p_proto)) == -1)
    exit(EXIT_FAILURE);
  sin.sin_family = AF_INET;
  sin.sin_port = htons(atoi(av[2]));
  sin.sin_addr.s_addr = inet_addr(av[1]);
  if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) == -1)
    {
      close(s);
      exit(EXIT_FAILURE);
    }
  if (write(s, "CONNECT CLIENT\n", 15) == -1)
    {
      close(s);
      exit(EXIT_FAILURE);
    }
  bzero(&buffer, 256);
  if (read(s, buffer, 255) < 1)
    {
      close(s);
      exit(EXIT_FAILURE);
    }
  if (strncmp(buffer, "RULES", 5) == 0)
    {
      toggle_rules(buffer);
      while (strncmp(buffer, "WIN", 3) != 0 || strncmp(buffer, "LOSE", 4) != 0)
	{
	  printf("Et j'attends\n");
	  if (strncmp(buffer, "YOURTURN\n", 9) == 0)
	    {
	      printf("Je joue\n");
	      play(s, buffer);
	    }
	  /* if (strncmp(buffer, "REM", 3) == 0) */
	  // rem(s, buffer);
	  /* if (strncmp(buffer, "ADD", 3) == 0) */
	  // add(s, buffer);
	  bzero(&buffer, 256);
	  if (read(s, buffer, 255) < 1)
	    {
	      close(s);
	      exit(EXIT_FAILURE);
	    }
	  printf("%s\n", buffer);
	}
      // Envoi du Win ou du Lose
    }
  close(s);
}
