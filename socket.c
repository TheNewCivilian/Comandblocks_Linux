

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include "socket.h"

int checkscore(int PLscore,char *PLname,LIST *input)
{
      int sockfd,n;
      int i,j,point;
      //int score = 121;
      char sendline[100];
      char recvline[100];
      struct sockaddr_in servaddr;

      char *sendname = (char *) malloc(30*sizeof(char));
      char *score = (char *) malloc(10*sizeof(char));

      sockfd=socket(AF_INET,SOCK_STREAM,0);
      bzero(&servaddr,sizeof servaddr);

      servaddr.sin_family=AF_INET;
      servaddr.sin_port=htons(22000);

      inet_pton(AF_INET,"92.222.88.89",&(servaddr.sin_addr));

      if (connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1){
        printf("Mega Error!\n");
        return 1;
      }

          bzero( sendline, 10);
          bzero( recvline, 100);

          //fgets(sendline,100,stdin); /*stdin = 0 , for standard input */
          sprintf(score,"%10i",PLscore);
          strcpy(sendname,PLname);

          write(sockfd,score,10);
          bzero( sendline, 30);
          write(sockfd,sendname,30);
          sleep(1);
          point = 0;

          do{
            //printf("Debug %i\n",point);

            read(sockfd,recvline,10*sizeof(char));
            input->score[point] = atoi(recvline);
            read(sockfd,recvline,30*sizeof(char));
            strcpy(input->playername[point],recvline);
            point++;

          }while(point <10);
          read(sockfd,recvline,10*sizeof(char));

          //printf("Platz: %i\n",atoi(recvline));

      // for(i=0;i<10;i++){
      //   printf("%i | %s | %i \n",i+1,input->playername[i],input->score[i]);
      // }
    return atoi(recvline); //0 no win, 9-10 Places
}
