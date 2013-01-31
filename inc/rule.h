/*
** rule.h for  in /home/pprost/c/cpp/babel/client/qt-test-build-desktop-Qt_4_8_3_in_PATH__System__Release
** 
** Made by  Prost P.
** Login   <pprost@epitech.net>
** 
*/

#ifndef __RULE_H__
#define __RULE_H__

#define		RULE3	0x01	
#define		RULE5	0x02


int	rule3(t_board *board, unsigned int x, unsigned int y, char color);
int	rule5(t_board *board, unsigned int x, unsigned int y, char color);
int	prise(t_board *board, unsigned int x, unsigned int y, int color);
long	prisegetlines(t_board *board, unsigned int x, unsigned int y, int color);

#endif
