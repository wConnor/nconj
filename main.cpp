#include "session.hpp"
#include "menu.hpp"

int main(int argc, char *argv[])
{
  initscr();
  cbreak();
  start_color();

  Menu *menu = new Menu();

  std::string infinitive = "être";
  std::vector<std::string> front = {"je", "tu", "il, elle, on", "nous", "vous", "ils/elles"};
  std::vector<std::string> back = {"suis", "es", "est", "sommes", "êtes", "sont"};

  while (1) {
	if (menu->print() >= 0) {
	  Session *session = new Session();
	  session->begin(front, back, infinitive);
	} else {
	  break;
	}
  }
  endwin();

  return 0;
}
