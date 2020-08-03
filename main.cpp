#include <ncurses.h>
#include "session.hpp"

int main(int argc, char *argv[])
{
  initscr();
  nocbreak();
  echo();
  //raw();  
  
  std::vector<std::string> front = {"Hallo", "Salut", "Français"};
  std::vector<std::string> back = {"Hello", "Hi", "French"};
  Session *session = new Session();
  session->setFront(front);
  session->setBack(back);

  refresh();
  session->begin();
	
  getch();
  endwin();

  return 0;
}
