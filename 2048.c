#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include<time.h>
#include<stdbool.h>
#include<termios.h>
#include<unistd.h>

#define SIZE 4
#define fors(x,n) for(int x=0;x<n;x++)
uint32_t score=0;
bool game_end=false;

void setBufferedInput(bool enable);
void createBoard(uint32_t board[SIZE][SIZE]);
void displayBoard(uint32_t board[SIZE][SIZE]);
void insertColor(uint32_t value, char *color, size_t length);
void add_newnum(uint32_t board[SIZE][SIZE]);
void compress(uint32_t board[SIZE][SIZE]);
void merge(uint32_t board[SIZE][SIZE]);
void rBoard(uint32_t board[SIZE][SIZE]);
void flipBoard(uint32_t board[SIZE][SIZE]);
void left(uint32_t board[SIZE][SIZE]);
void right(uint32_t board[SIZE][SIZE]);
void top(uint32_t board[SIZE][SIZE]);
void down(uint32_t board[SIZE][SIZE]);
void win_lose(uint32_t board[SIZE][SIZE]);

int main() {
  uint32_t board[SIZE][SIZE];
  printf("\033[?25l\033[2J");
  createBoard(board);
  char c;
  setBufferedInput(false);
  while(true){
    c=getchar();
    if(c==68 | c=='a'){
      left(board);
      if(game_end) break;
    }
    else if(c==67 | c=='d'){
      right(board);
      if(game_end) break;
    }
    else if(c==65 | c=='w'){
      top(board);
      if(game_end) break;
    }
    else if(c==66 | c=='s'){
      down(board);
      if(game_end) break;
    }
    else if(c=='q'){
			printf("QUIT? (y/n)\n");
			c=getchar();
			if (c=='y') break;
			displayBoard(board);
    }
    else if (c=='r') {
			printf("RESTART? (y/n)\n");
			c=getchar();
			if (c=='y') {
			  createBoard(board);
			}
      displayBoard(board);
		}
  }
  setBufferedInput(true);
	printf("\033[?25h\033[m");
  return 0;
}

void setBufferedInput(bool enable) {
	static bool enabled = true;
	static struct termios old;
	struct termios new;

	if (enable && !enabled) {
		// restore the former settings
		tcsetattr(STDIN_FILENO,TCSANOW,&old);
		// set the new state
		enabled = true;
	} else if (!enable && enabled) {
		// get the terminal settings for standard input
		tcgetattr(STDIN_FILENO,&new);
		// we want to keep the old setting to restore them at the end
		old = new;
		// disable canonical mode (buffered i/o) and local echo
		new.c_lflag &=(~ICANON & ~ECHO);
		// set the new settings immediately
		tcsetattr(STDIN_FILENO,TCSANOW,&new);
		// set the new state
		enabled = false;
	}
}
//initializing & display
void createBoard(uint32_t board[SIZE][SIZE]){
  fors(i,SIZE){
    fors(j,SIZE){
      board[i][j]=0;
    }
  }
  add_newnum(board);
  add_newnum(board);
  displayBoard(board);
}
void add_newnum(uint32_t board[SIZE][SIZE]){
  static bool initialized = false;
  uint32_t row,col;
  if (!initialized) {
    srand(time(NULL));
    initialized = true;
  }
  row=rand()%4;
  col=rand()%4;
  while(board[row][col]!=0){
    row=rand()%4;
    col=rand()%4;
  }
  board[row][col]=(rand()%2)*2+2;
}
void displayBoard(uint32_t board[SIZE][SIZE]){
  printf("\033[2J");
  char color[20];
  uint32_t i,j;
  printf("\033[H");
  printf("Score: %d\n",score);
  
  fors(i,SIZE){
    fors(j,SIZE){
      insertColor(board[i][j],color,20);
      printf("%s",color);
      printf("       ");
      printf("\033[m");
    }
    printf("\n");
    fors(j,SIZE){
      insertColor(board[i][j],color,20);
      printf("%s",color);
      if(board[i][j]!=0) {
        char s[8];
        snprintf(s,8,"%u",board[i][j]);
        uint32_t t = 7-strlen(s);
        printf("%*s%s%*s",t-t/2,"",s,t/2,"");
      } 
      else{
        printf("   ·   ");
      }
      printf("\033[m");
    }
    printf("\n");
    fors(j,SIZE){
      insertColor(board[i][j],color,20);
      printf("%s",color);
      printf("       ");
      printf("\033[m");
    }
    printf("\n");
  }
  printf("\n");
  printf("        ←,↑,→,↓ or q,r        \n");
}
void insertColor(uint32_t value, char *color, size_t length) {
  uint32_t c[] = {0,255,0,255,10,255,11,255,12,255,13,255,14,255,1,255,2,255,4,255,5,255,6,255,9,255};
  uint32_t *background = c+0;
  uint32_t *foreground = c+1;
  while(value>0){
    background+=2;
    foreground+=2;
    value=value/2;
  }
  snprintf(color,length,"\033[38;5;%d;48;5;%dm",*foreground,*background);
}

//functions
void compress(uint32_t board[SIZE][SIZE]){
  uint32_t n_board[SIZE][SIZE]={0,};
  fors(i,SIZE){
    uint32_t tmp=0;
    fors(j,SIZE){
      if(board[i][j]!=0){
        n_board[i][tmp]=board[i][j];
        tmp++;
      }
    }
  }
  fors(i,SIZE){
    fors(j,SIZE){
      board[i][j]=n_board[i][j];
    }
  }
}
void merge(uint32_t board[SIZE][SIZE]){
  fors(i,SIZE){
    fors(j,SIZE-1){
      if(board[i][j]!=0 && board[i][j]==board[i][j+1]){
        board[i][j]*=2;
        board[i][j+1]=0;
        score+=board[i][j];
      }
    }
  }
}
void rBoard(uint32_t board[SIZE][SIZE]){
  uint32_t n_board[SIZE][SIZE];
  fors(i,SIZE){
    fors(j,SIZE){
      n_board[i][j]=board[i][3-j];
    }
  }
  fors(i,SIZE){
    fors(j,SIZE){
      int tmp = n_board[i][j];
      n_board[i][j]= board[i][j];
      board[i][j]= tmp;
    }
  }
}
void flipBoard(uint32_t board[SIZE][SIZE]){
  uint32_t n_board[SIZE][SIZE]={0,};
  fors(i,SIZE){
    fors(j,SIZE){
      n_board[i][j]=board[j][i];
    }
  }
  fors(i,SIZE){
    fors(j,SIZE){
      int tmp = n_board[i][j];
      n_board[i][j]= board[i][j];
      board[i][j]= tmp;
    }
  }
}

//arrows
void left(uint32_t board[SIZE][SIZE]){
  compress(board);
  merge(board);
  compress(board);
  
  add_newnum(board);
  displayBoard(board);
  win_lose(board);
}

void right(uint32_t board[SIZE][SIZE]){
  rBoard(board);
  compress(board);
  merge(board);
  compress(board);
  rBoard(board);

  add_newnum(board);
  displayBoard(board);
  win_lose(board);  
}
void top(uint32_t board[SIZE][SIZE]){
  flipBoard(board);
  compress(board);
  merge(board);
  compress(board);
  flipBoard(board);

  add_newnum(board);
  displayBoard(board);
  win_lose(board);
}
void down(uint32_t board[SIZE][SIZE]){
  flipBoard(board);
  rBoard(board);
  compress(board);
  merge(board);
  compress(board);
  rBoard(board);
  flipBoard(board);

  add_newnum(board);
  displayBoard(board);
  win_lose(board);
}
//game
void win_lose(uint32_t board[SIZE][SIZE]){
  bool exist=false;
  fors(i,SIZE){
    fors(j,SIZE){
      if(board[i][j]==2048){
        printf("You Win\n");
        printf("Final score: %d\n",score);
        game_end=true;
        break;
      }
      else if(board[i][j]==0){
        exist=true;
      }
    }
  }
  fors(i,SIZE){
    fors(j,3){
      if(board[i][j]==board[i][j+1]){
        exist=true;
      }
    }
  }
  fors(i,3){
    fors(j,SIZE){
      if(board[i][j]==board[i+1][j]){
        exist=true;
      }
    }
  }
  if(!exist){
    printf("Game Over-You lose\n");
    printf("Final score: %d\n",score);
    game_end=true;
  }
}

