
#include <ncurses.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

int main(int argc, char const *argv[])
{
  //Argumentabfrage wenn nötig

  //Curses init
  initscr();
  cbreak();
  noecho();
  nonl();
  if(has_colors() == FALSE)
  	{	endwin();
  		printf("Your terminal does not support color\n");
  		return 1;
  	}
  start_color();

  //Color init
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  //Fähigkeiten color
  init_pair(10, COLOR_BLACK, COLOR_GREEN);
  init_pair(11, COLOR_BLACK, COLOR_YELLOW);
  init_pair(12, COLOR_WHITE, COLOR_RED);
  init_pair(13, COLOR_WHITE, COLOR_BLACK);
  //mapcolor
  init_pair(20, COLOR_BLACK, COLOR_BLUE);
  init_pair(21, COLOR_BLACK, COLOR_GREEN);
  init_pair(22, COLOR_BLACK, COLOR_YELLOW);
  init_pair(23, COLOR_BLACK, COLOR_RED);
  init_pair(24, COLOR_BLACK, COLOR_CYAN);

  init_pair(25, COLOR_BLACK, COLOR_CYAN);

  //GAME
  gameloop();

  //ENDE Curses
  endwin();
  return 0;
}

int gameloop(){
  //Gamevars
  int run = 1;
  int i;
  int *hscore;
  int input = 0;

  //Mapvars
  int max_y = 0,max_x = 0;
  getmaxyx(stdscr, max_y, max_x);
  int **map = (int **) malloc(max_x*sizeof(int *));
  for(i = 0;i < max_x;i++){
    map[i] = (int *) malloc(max_y*sizeof(int));
  }


  //Playervars
  //Score Pattern Gscore 50 | Rscore 50 | Yscore 50| Bombs 0
  int *CoSc = (int *) malloc(4*sizeof(int));
  for (i = 0;i<3;i++){
    CoSc[i] = 50;
  }
  CoSc[3] = 0;
  int score = 0;
  int x = (max_x/4)*2, y = ((max_y-7)/4)*2;

  //Mapgeneration
  generatemap(max_x, max_y, map);

  //Scoreprep
  //getHighscore(hscore);

  //Prescreen
  drawmap(max_x, max_y, map);
  drawscore(score, *hscore, CoSc, max_x, max_y);
  mvprintw(y,x,"()");
  mvprintw(3,(max_x/2)-35," _____                                 _______ _            _        ");
	mvprintw(4,(max_x/2)-35,"/  __ \\                               | | ___ \\ |          | |       ");
	mvprintw(5,(max_x/2)-35,"| /  \\/ ___  _ __ ___   __ _ _ __   __| | |_/ / | ___   ___| | _____ ");
	mvprintw(6,(max_x/2)-35,"| |    / _ \\| '_ ` _ \\ / _` | '_ \\ / _` | ___ \\ |/ _ \\ / __| |/ / __|");
	mvprintw(7,(max_x/2)-35,"| \\__/\\ (_) | | | | | | (_| | | | | (_| | |_/ / | (_) | (__|   <\\__ \\");
	mvprintw(8,(max_x/2)-35," \\____/\\___/|_| |_| |_|\\__,_|_| |_|\\__,_\\____/|_|\\___/ \\___|_|\\_\\___/");
	mvprintw(14,(max_x/2)-12,"Push WASD to start Game!");
  refresh();
  while(run == 1){
    //Key Input 115 = down | 119 = up | 100 = right | 97 = left
    input = getch();
    if (input != 0){
      if (score == 0){
        drawmap(max_x, max_y, map);
      }
      if (input > 64 && input <69){
        score++;
        mvprintw(y,x,"  ");
        map[x][y] = 10;
        x = (input == 67)?x+2:(input == 68)?x-2:x;
        y = (input == 66)?y+1:(input == 65)?y-1:y;
        CoSc[0] += (map[x][y] == 0)?-40:(map[x][y] == 1)?((rand()%9)+1)*10:(map[x][y] == 2)?-10:(map[x][y] == 3)?-2:(map[x][y] == 4)?-50:0;
        CoSc[1] += (map[x][y] == 0)?-10:(map[x][y] == 1)?-4:(map[x][y] == 2)?((rand()%2)+1)*3:(map[x][y] == 3)?-1:(map[x][y] == 4)?20:0;
        CoSc[2] += (map[x][y] == 0)?-15:(map[x][y] == 1)?-1:(map[x][y] == 2)?-4:(map[x][y] == 3)?((rand()%2)+1)*3:(map[x][y] == 4)?0:0;
        drawscore(score, *hscore, CoSc, max_x, max_y);
        if ( CoSc[0] <= 0 || CoSc[1] <= 0|| CoSc[2] <= 0){
    			run = 0;
    			loopexit(score);
    			return 0;
    		}
      }
      mvprintw(y,x,"()");
      refresh();
    }

  }
  return 0;
}

int generatemap(int max_x,int max_y, int **map){
  int x,y;
  srand(time(NULL));
  for(x= 0;x < max_x;x++){
		for(y= 0;y < max_y;y++){
        //printf("Do for %i %i\n",x,y);
	     	map[x][y] = rand()%5;
		}
	}
  return 0;
}

int drawmap(int max_x, int max_y, int **map){
  int x,y;
  for (x = 0; x < max_x; x++){
    	for (y = 0; y < max_y-7; y++){
        attron(COLOR_PAIR(map[x][y]+20));
        mvprintw(y,x*2,"  ");
        attroff(COLOR_PAIR(map[x][y]+20));
      }
    }
  return 0;
}

int getHighscore(){
	char textfrom[100];
	FILE *f = fopen("highscore.hs","rb");
	fseek(f, SEEK_SET, 0);
	int r = fread(textfrom,1,strlen(textfrom)+1,f);
	fclose(f);
	textfrom[r] = 0;
	return atoi(textfrom);
}

int drawscore(int score, int hscore, int *CoSc, int max_x, int max_y){
  int i;
  const char *name;
  int abst = (max_x/6), x_off = 2;

  for(i = 0;i < 3 && x_off < max_x;i++){
    name = (i == 0)?"Greenpower":(i == 1)?"Yellowpower":"Redpower";
    attron(COLOR_PAIR(i+10));
    mvprintw(max_y-1,x_off,"%s",name);
    mvprintw(max_y-2,x_off,"--------");
    mvprintw(max_y-3,x_off,"|      |");
    mvprintw(max_y-4,x_off,"|%5i |",CoSc[i]);
    mvprintw(max_y-5,x_off,"|      |");
    mvprintw(max_y-6,x_off,"--------");
    attroff(COLOR_PAIR(i+10));
    x_off += abst;
  }

  mvprintw(max_y-6,x_off,"Game Stats:");
  mvprintw(max_y-5,x_off,"Your Score: %i",score);
  mvprintw(max_y-4,x_off,"Highscore: %i",hscore);
  //mvprintw(max_y-3,x_off,"");
  mvprintw(max_y-2,x_off,"For Fieldinfo Press: I");
  attron(COLOR_PAIR(25));
  mvprintw(max_y-1,x_off,"Build by TheNewCivillian");
  attroff(COLOR_PAIR(25));
  return 0;
}

int loopexit(int score){
  int max_y = 0,max_x = 0;
  int input;
  getmaxyx(stdscr, max_y, max_x);
  attron(COLOR_PAIR(1));
	mvprintw(10,(max_x/2)-15,"              YOU LOST!!!!           ");
	mvprintw(11,(max_x/2)-15,"         Your Score %5i           ",score);
	mvprintw(12,(max_x/2)-15,"Press SPACE to Restart Str+E to Exit ");
	attroff(COLOR_PAIR(1));
  //READ AND WRITE Score
  while(1 == 1){
    noecho();
    input = getch();
    if (input == 'E'){
      endwin();
      return 0;
    }else if (input == ' '){
      gameloop();
      return 0;
    }

  }
  return 0;
}
