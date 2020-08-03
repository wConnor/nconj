#include <ncurses.h>
#include "session.hpp"

int main(int argc, char *argv[])
{
  initscr();
  cbreak();
  noecho();
  std::vector<std::string> front = {"Hallo", "Salut", "Français"};
  std::vector<std::string> back = {"Hello", "Hi", "French"};
  Session *session = new Session();
  session->setFront(front);
  session->setBack(back);
  session->outputContents();

  
  getch();
  endwin();

  return 0;
}
