#ifndef SOCKET_H
#define SOCKET_H

typedef struct _bestlist{
  int *score;
  char **playername;
}LIST;

int checkscore(int PLscore,char *PLname,LIST *list);

#endif
