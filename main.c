
#include <ncurses.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "main.h"
#include "socket.h"

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
  init_pair(9, COLOR_WHITE, COLOR_MAGENTA);
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
  int *hscore = (int *) malloc(sizeof(int));
  int input = 0;

  //Mapvars
  int max_y = 0,max_x = 0;
  getmaxyx(stdscr, max_y, max_x);
  int **map = (int **) malloc(max_x*sizeof(int *));
  LIST *HSinput = (LIST *) malloc(sizeof(LIST));
  HSinput->score = (int *) calloc(10,sizeof(int));
  HSinput->playername = (char **) malloc(10*sizeof(char *));
  for(i = 0;i<10;i++){
    HSinput->playername[i] = (char *) calloc(30,sizeof(char));
  }
  if(checkscore(-1,(char *) " ",HSinput) != -1){
    printf("Something fucked up!\n");
    return 1;
  }
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
  *hscore = getHighscore();

  //Prescreen
  drawmap(max_x, max_y, map);
  drawscore(score, *hscore, CoSc, max_x, max_y,HSinput);
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
        drawscore(score, *hscore, CoSc, max_x, max_y,HSinput);
        if ( CoSc[0] <= 0 || CoSc[1] <= 0|| CoSc[2] <= 0){
    			run = 0;
    			loopexit(score,HSinput);
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
	char *textfrom = (char *) malloc(11*sizeof(char));
	FILE *f = fopen("highscore.hs","r+");
  if (f == NULL){
    endwin();
    printf("FILE NOT FOUND\n");
    return 1;
  }
	fseek(f, SEEK_SET, 0);
	fread(textfrom,1,10,f);
	fclose(f);
	return atoi(textfrom);
}

int setHighscore(int x){
	FILE *f = fopen("highscore.hs","w+");
  if (f == NULL){
    endwin();
    printf("FILE NOT FOUND\n");
    return 1;
  }
	fprintf(f, "%i",x);
	fclose(f);
  return 0;
}

int drawscore(int score, int hscore, int *CoSc, int max_x, int max_y,LIST * HSinput){
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
  mvprintw(max_y-4,x_off,"Online Highscore: %i",HSinput->score[0]);
  //mvprintw(max_y-3,x_off,"");
  mvprintw(max_y-2,x_off,"For Fieldinfo Press: I");
  attron(COLOR_PAIR(25));
  mvprintw(max_y-1,x_off,"Build by TheNewCivillian");
  attroff(COLOR_PAIR(25));
  return 0;
}

int loopexit(int score,LIST * HSinput){
  int max_y = 0,max_x = 0;
  int input;
  int place;
  char *player = (char *) calloc(30,sizeof(char));
  getmaxyx(stdscr, max_y, max_x);
  attron(COLOR_PAIR(1));
	mvprintw(10,(max_x/2)-15,"            YOU LOST!!           ");
	mvprintw(11,(max_x/2)-15,"         Your Score %5i        ",score);
	mvprintw(12,(max_x/2)-15,"Press SPACE to Restart E to Exit ");
	attroff(COLOR_PAIR(1));
  refresh();
  //READ AND WRITE Score
  if(getHighscore()<score){
    attron(COLOR_PAIR(12));
    mvprintw(10,(max_x/2)-15,"         New Highscore!!         ");
    attron(COLOR_PAIR(12));
    setHighscore(score);
    refresh();
  }
  enterplayername(player,max_x);
  place = checkscore(score,player,HSinput);
  if (place > -1){
    attron(COLOR_PAIR(9));
    mvprintw(10,(max_x/2)-15,"      New ONLINE Highscore!!      ");
    attroff(COLOR_PAIR(9));
    refresh();
  }

  drawhscorelist(max_x,max_y,HSinput);

  while(1 == 1){
    input = getchar();
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

int enterplayername(char *player, int max_x){
  int i = 0;
  attron(COLOR_PAIR(13));
  mvprintw(14,(max_x/2)-15,"                                  ");
  mvprintw(15,(max_x/2)-15,"      Enter your Name:            ");
  attroff(COLOR_PAIR(13));
  attron(COLOR_PAIR(1));
  mvprintw(16,(max_x/2)-15,"      %-20s        ",player);
  attroff(COLOR_PAIR(1));
  attron(COLOR_PAIR(13));
  mvprintw(17,(max_x/2)-15,"                                  ");
  attroff(COLOR_PAIR(13));
  refresh();
  char c;
  while((c = getch()) != 13 && c != EOF && i<20)
  {
    if(isalpha(c)){
      if(i<20){
        player[i] = c;
      }
      attron(COLOR_PAIR(1));
      mvprintw(16,(max_x/2)-15,"   %-20s       ",player,c);
      attroff(COLOR_PAIR(1));
      refresh();
      i++;
    }
    if(c == 127){
      if(i>=0){
        i--;
      }
      player[i] = ' ';
      attron(COLOR_PAIR(1));
      mvprintw(16,(max_x/2)-15,"   %-20s        ",player,c);
      attroff(COLOR_PAIR(1));
      refresh();
    }
  }
  return 0;
}

int drawhscorelist(int max_x,int max_y,LIST *HSinput){
  int i;
  int y_off = 0;
  attron(COLOR_PAIR(13));
  mvprintw(14,(max_x/2)-15," Rank      Playername       Score ");
  attroff(COLOR_PAIR(13));
  if(HSinput != NULL){
    for(i = 0;i < 10 && y_off < max_y;i++){
      if(i+10<=13){
          attron(COLOR_PAIR(i+10));
      }else{
        attron(COLOR_PAIR(13));
      }
      mvprintw(15+y_off,(max_x/2)-15,"                                  ");
      mvprintw(16+y_off,(max_x/2)-15," %2i   %-20s %6i ",i+1,HSinput->playername[i],HSinput->score[i]);
      mvprintw(17+y_off,(max_x/2)-15,"                                  ");
      attroff(COLOR_PAIR(i+10));
      attroff(COLOR_PAIR(13));
      y_off += 3;
    }
  }else{
    mvprintw(15,(max_x/2)-15,"        ERROR       ");
  }
  refresh();
  return 0;
}
