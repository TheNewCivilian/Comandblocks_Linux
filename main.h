#ifndef MAIN_H
#define MAIN_H

#ifdef __WIN32__
#include <pdcurses.h>
#else
#include <ncurses.h>
#endif

#include "socket.h"

int gameloop();
int generatemap(int max_x,int max_y, int **map);
int drawmap(int max_x, int max_y, int **map);
int getHighscore();
int drawscore(int score, int hscore, int *CoSc, int max_x, int max_y,LIST * HSinput);
int loopexit(int score,LIST *HSinput);
int drawhscorelist(int max_x,int max_y,LIST *HSinput);
int enterplayername(char *player, int max_x);
#endif
