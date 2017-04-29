#ifndef SOCKET_H
#define SOCKET_H

#ifdef __WIN32__
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>

#endif

typedef struct _bestlist{
  int *score;
  char **playername;
}LIST;

int checkscore(int PLscore,char *PLname,LIST *list);

#endif
