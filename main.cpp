#include <ncurses.h>
#include "session.hpp"

int main(int argc, char *argv[])
{
  initscr();
  cbreak();
  noecho();
  
  
  


  
  getch();
  endwin();

  return 0;
}
