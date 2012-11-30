#include	<stdio.h>
#include	<unistd.h>

void		my_putnbr(int i)
{
  int		tmp;
  if (i < 2)
    {
      tmp = (i + '0');
      write(1, &tmp, 1);
    }
  else
    {
      my_putnbr(i / 2);
      tmp = ((i % 2) + '0');
      write(1, &tmp, 1);
    }
}

int		main()
{
  long		size;
  int		i;

  i = 0;
  size = 0;
  while (i < (19*19*2))
    {
      size = size | (1 << i);
      i++;
    }
  /* my_putnbr(size); */
  /* printf("\n"); */
  /* size = 1 << 4; */
  /* size = size | (1 << 8); */
  my_putnbr(size);
  printf("\n");
  return (0);
}
