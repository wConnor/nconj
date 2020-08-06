#include "session.hpp"
#include "menu.hpp"

int main(int argc, char *argv[])
{
  initscr();
  cbreak();
  start_color();

  Menu *menu = new Menu();
  
  std::vector<std::string> front = {"oneF", "twoF", "threeF"};
  std::vector<std::string> back = {"oneB", "twoB", "threeB"};

  while (1) {
	if (menu->print() >= 0) {
	  Session *session = new Session();
	  session->begin(front, back);
	} else {
	  break;
	}
  }
  endwin();

  return 0;
}
