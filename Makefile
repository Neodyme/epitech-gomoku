##
## Makefile for making files in /a/galaxy/far/far/away
## 
## Made by dorian schaegis
## Login   <schaeg_d@epitech.net>
## 
## Started on  Thu Jan  1 00:00:00 1970 dorian schaegis
##

NAME=			gomoku

CC=			gcc

RM=			rm -f

SRC=			$(notdir $(wildcard src/*.c))

OBJ=			$(addprefix obj/, $(patsubst %.c, %.o, $(SRC)))

INC=			-Iinc

CFLAGS=			-W -Wall -Wextra -Werror\
			-lSDL\
			-O0 -O1 -O2 -O3 -g3
			-lrt

$(NAME):		$(OBJ)
			@echo -en "[BIN] $(NAME) -> \E[33;31m"
			@$(CC) $(CFLAGS) $(OBJ) $(INC) -o $(NAME)
			@echo -e "\E[33;39mDone!"

all:			$(NAME)

clean:
			@echo -n "[RM] $(OBJ) -> "
			@$(RM) $(OBJ)
			@echo "ok"

fclean:			clean
			@echo -n "[RM] $(NAME) -> "
			@$(RM) $(NAME)
			@echo "ok"

re:			fclean $(NAME)

obj/%.o:		src/%.c
			@echo -en "[OBJ] $< -> \E[33;31m"
			@$(CC) $(CFLAGS) $(INC) -c -o $@ $<
			@echo -e "\E[33;39m$@"

.PHONY:			clean fclean all re
