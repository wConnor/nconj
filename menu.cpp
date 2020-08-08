#include "menu.hpp"
#include <vector>

Menu::Menu() {}

int Menu::print(std::vector<std::string> menuOptions) {
  clear();
  noecho();
  curs_set(0);
  int yMax, xMax;
  int keyEnd = 0, sesLength = 0;
  std::unique_ptr<int> keyPtr = std::make_unique<int>(keyEnd);
  std::unique_ptr<int> sesPtr = std::make_unique<int>(sesLength);
  getmaxyx(stdscr, yMax, xMax);
  WINDOW *menuWin = newwin(yMax - 2, 20, 1, 0);
  WINDOW *keysWin = newwin(1, xMax, yMax - 1, 0);
  WINDOW *barWin = newwin(1, xMax, 0, 0);

  /* colour pairs for the windows
     ID1 = menuWin
	 ID2 = keysWin
	 ID2 = barWin
  */
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_CYAN);
  wbkgd(menuWin, COLOR_PAIR(1));
  wbkgd(keysWin, COLOR_PAIR(2));
  wbkgd(barWin, COLOR_PAIR(2));

  // ensures contents in memory are written to the framebuffer.
  refresh();
  wrefresh(menuWin);
  wrefresh(keysWin);
  wrefresh(barWin);

  // shows the user the usable keys at the bottom of the screen
  addOption(" ENTER ", "Select Option ", keyPtr, keysWin);
  addOption(" q ", "Quit ", keyPtr, keysWin);
  wrefresh(keysWin);

  keypad(menuWin, true);
  box(menuWin, 0, 0);
  whline(menuWin, ' ', 0);
  mvwprintw(barWin, 0, 1, "nconj");

  wrefresh(menuWin);
  wrefresh(keysWin);
  wrefresh(barWin);

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
      mvwprintw(menuWin, i + 1, 1, menuOptions[i].c_str());
      wattroff(menuWin, A_REVERSE);
    }
    choice = wgetch(menuWin);

    switch (choice) {
    case KEY_UP:
    case 'k':
      highlight--;
      if (highlight == -1)
        highlight = 0;
      break;
    case KEY_DOWN:
    case 'j':
      highlight++;
      if (highlight == menuOptions.size())
        highlight--;
      break;
    default:
      break;
    }
    if (choice == 'q') {
      break;
    } else if (choice == 10) {
      wclear(menuWin);
      wclear(keysWin);
      wrefresh(menuWin);
      wrefresh(keysWin);
      mvwprintw(barWin, 0, 1, "nconj session");
      addOption(" F1 ", "Skip", sesPtr, keysWin);
      wrefresh(keysWin);
      wrefresh(barWin);
      return highlight;
    }
  }

  return -1;
}

void Menu::addOption(std::string key, std::string option,
                     std::unique_ptr<int> &length, WINDOW *win) {
  wattron(win, A_REVERSE);
  mvwprintw(win, 0, *length, key.c_str());
  *length += key.length() + 1;
  wattroff(win, A_REVERSE);
  mvwprintw(win, 0, *length, option.c_str());
  *length += option.length() + 1;
}

Menu::~Menu() {}
