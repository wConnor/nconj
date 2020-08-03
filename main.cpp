#include <ncurses.h>
#include "session.hpp"

int main(int argc, char *argv[])
{
  initscr();
  cbreak();
  noecho();
  start_color();
  
  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);
  WINDOW *menuWin = newwin(yMax-1, 20, 0, 0);
  WINDOW *keysWin = newwin(1, xMax, yMax-1, 0);
  std::vector<std::string> front = {"Hallo", "Salut", "Français"};
  std::vector<std::string> back = {"Hello", "Hi", "French"};


  // ensures contents in memory are written to the framebuffer.
  refresh();
  wrefresh(menuWin);
  wrefresh(keysWin);


  init_pair(1, COLOR_WHITE, COLOR_CYAN);
  init_pair(2, COLOR_CYAN, COLOR_BLUE);

  keypad(menuWin, true);
  wbkgd(keysWin, COLOR_PAIR(1));
  mvwprintw(keysWin, 0, 1, "ENTER: Select Option\tq: Quit");

  wrefresh(keysWin);
  std::vector<std::string> menuOptions = {"Deck 1", "Deck 2", "Deck 3"};

  int choice;
  int highlight = 0;

  /* main menu. loop constantly reads for user input
	 until one of the three options are selected.
	 '10' is the ENTER key.
  */
  
  while (1) {
	for (int i = 0; i < menuOptions.size(); ++i) {
	  if (i == highlight)
		wattron(menuWin, A_REVERSE);
	  mvwprintw(menuWin, i+1, 1, menuOptions[i].c_str());
	  wattroff(menuWin, A_REVERSE);
	}
	choice = wgetch(menuWin);

	switch(choice) {
	case KEY_UP:
	  highlight--;
	  if (highlight == -1)
		highlight = 0;
	  break;
	case KEY_DOWN:
	  highlight++;
	  if (highlight == menuOptions.size() + 1)
		highlight = 2;
	  break;
	default:
	  break;
	}
	if (choice == 'q') {
	  endwin();
	  return 0;
	}
	else if (choice == 10)
	  break;
  }

  // Session *session = new Session();
  // session->setFront(front);
  // session->setBack(back);
  
  getch();
  endwin();

  return 0;
}
