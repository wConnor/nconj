#include <ncurses.h>
#include "session.hpp"

void layoutOption(std::string key, std::string option,
                  std::unique_ptr<int> &length, WINDOW *win) {

  wattron(win, A_REVERSE);
  mvwprintw(win, 0, *length, key.c_str());
  *length += key.length() + 1;
  wattroff(win, A_REVERSE);
  mvwprintw(win, 0, *length, option.c_str());
  *length += option.length() + 1;
}

int main(int argc, char *argv[])
{
  initscr();
  cbreak();
  noecho();
  start_color();
  
  int yMax, xMax;
  int keyEnd = 0;
  std::unique_ptr<int> keyPtr = std::make_unique<int>(keyEnd);
  getmaxyx(stdscr, yMax, xMax);
  WINDOW *menuWin = newwin(yMax-1, 20, 0, 0);
  WINDOW *keysWin = newwin(1, xMax, yMax-1, 0);
  std::vector<std::string> front = {"Hallo", "Salut", "Français"};
  std::vector<std::string> back = {"Hello", "Hi", "French"};

  // ensures contents in memory are written to the framebuffer.
  refresh();
  wrefresh(menuWin);
  wrefresh(keysWin);

  /* colour pairs for the windows
	 ID1 = menuWin
	 ID2 = keysWin
  */
  init_pair(1, COLOR_CYAN, COLOR_BLUE);
  init_pair(2, COLOR_BLACK, COLOR_CYAN);

  keypad(menuWin, true);

  wbkgd(keysWin, COLOR_PAIR(2));

  // shows the user the usable keys at the bottom of the screen
  layoutOption(" ENTER ", "Select Option ", keyPtr, keysWin);
  layoutOption(" q ", "Quit ", keyPtr, keysWin);
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
	case KEY_UP: case 'k':
	  highlight--;
	  if (highlight == -1)
		highlight = 0;
	  break;
	case KEY_DOWN: case 'j':
	  highlight++;
	  if (highlight == menuOptions.size())
		highlight--;
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
