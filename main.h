#ifndef MAIN_H
#define MAIN_H

int gameloop();
int generatemap(int max_x,int max_y, int **map);
int drawmap(int max_x, int max_y, int **map);
int getHighscore();
int drawscore(int score, int hscore, int *CoSc, int max_x, int max_y);
int loopexit(int score);
#endif
