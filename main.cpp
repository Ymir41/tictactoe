#include <ncurses.h>


using namespace std;

class Path
{
  private:
    int * a, * b, *c;
    int f[3];

  public:
    void init(int * board, int f1, int  f2, int f3){
      a = &board[f1];
      b = &board[f2];
      c = &board[f3];
      f[0] =f1; f[1] =f2; f[2] =f3;
    }

    int state(){
      int s = *a + *b + *c;
      if (s == -3 || s == -2 || s== 2 || s== 3) return s;
      if ((*a==0) + (*b==0) + (*c==0) >= 2) return s;
      return 4;
    }

    int free_field(){
      if (*a==0) return f[0];
      if (*b==0) return f[1];
      if (*c==0) return f[2];
      return -1;
    }
};

void game(bool single_player);
void printb();
void print_circle(int * board, int i);
void print_cross(int * board, int i);
int ask_player(int * board, const char * question);
int ask_computer(Path * paths);



int main()
{
  initscr();
  while (true)
  {
    refresh();
    clear();
    mvprintw(0,0, "Menu:\n------------------------\n1. Dwie osoby\n2. Gracz kontra komputer\n3. Wyjdź\nWybór: ");

    switch(wgetch(stdscr))
    {
      case '1': game(false); break;
      case '2': game(true); break;
      case '3': endwin(); return 0;
      default: printw("Nie ma takiej opcji w menu\n");
    }


    wgetch(stdscr);
  }

  return 0;
}


void game(bool single_player)
{
  clear();
  move(0,0);
  
  //board intialisation----------------------------------------------------------------------------------
  int board[9];
  for (int i = 0; i < 9; i++) {
      board[i] = 0;
  }

  const char * pusta = "           |           |           \n";
  const char * pelny = "-----------+-----------+-----------\n";
  for (int i = 1; i<=17; i++) {
    if (i%6==0) mvprintw(i-1, 0, pelny);
    else mvprintw(i-1, 0, pusta);
  }
  

  //paths generating------------------------------------------------------------------------------------
  Path paths[8];
  for (int i = 0; i < 3; i++) {
    paths[i].init(board, 3*i, 3*i+1, 3*i+2);
  }
  for (int i = 0; i < 3; i++) {
    paths[i+3].init(board, i, i+3, i+6);
  }
  paths[6].init(board, 0, 4, 8);
  paths[7].init(board, 2, 4, 6);
  int free_fields = 0;

  //game loop-------------------------------------------------------------------------------------------
  bool runing=1;
  for (int i = 0; runing; i++){
    //asking players-----------------------------------------------------------------------------------
    if (i%2==0) print_cross(board, ask_player(board, "Cross: "));
    else print_circle(board, ask_player(board, "Circle: "));

    //ending game--------------------------------------------------------------------------------------
    for(int j=0; j<8; j++){
      int state = paths[j].state();
      if (state==3) {runing = 0; mvprintw(19, 0, "Cross won!"); break;} // someone won
      if (state==-3) {runing = 0; mvprintw(19, 0, "Circle won!"); break;} // someone won
    }


    free_fields = 0;
    for(int i=0; i<9; i++){
     if (board[i] == 0) free_fields++;
    }
    if (free_fields==0) {mvprintw(19, 0, "Remis!   "); runing =0;}
  }// game loop
}// game function

void print_cross(int * board, int i)
{
 int y = (i/3)*6;
 int x = (i%3)*12;
 mvprintw(y, x,   "   \\   /   ") ;
 mvprintw(y+1, x, "    \\ /    ") ;
 mvprintw(y+2, x, "     X     ") ;
 mvprintw(y+3, x, "    / \\    ") ;
 mvprintw(y+4, x, "   /   \\   ") ;
 board[i] = 1;
}

void print_circle(int * board, int i)
{
 int y = (i/3)*6;
 int x = (i%3)*12;
 mvprintw(y, x,   "    XXX    ") ;
 mvprintw(y+1, x, "  X     X  ") ;
 mvprintw(y+2, x, " X       X ") ;
 mvprintw(y+3, x, "  X     X  ") ;
 mvprintw(y+4, x, "    XXX    ") ;
 board[i] = -1;
}

int ask_player(int * board, const char * question){
  int out;
  mvprintw(18, 0, "           ");
  mvprintw(19, 7, "  ");
  while (true){
    mvprintw(19, 0, question);
    out = getch()-49;
    if (out >= 0 && out <= 8 && board[out] == 0) return out;
    else mvprintw(18, 0, "Wrong value");
 }
}

int ask_computer(Path * paths){
  for(int i = 0; i<8; i++){
    if (paths[i].state()==-2) return paths[i].free_field();
  }
  for(int i = 0; i<8; i++){
    if (paths[i].state()== 2) return paths[i].free_field();
  }
  return 0;
}
