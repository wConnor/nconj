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
  addOption(" a ", "Create Deck", keyPtr, keysWin);
  addOption(" q ", "Quit ", keyPtr, keysWin);
  addOption(" o ", "Options", keyPtr, keysWin);

  wrefresh(keysWin);

  keypad(menuWin, true);
  box(menuWin, 0, 0);
  wattron(menuWin, A_BOLD);
  mvwprintw(menuWin, 0, 2, "Decks");
  wattroff(menuWin, A_BOLD);
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
    // quit program
    if (choice == 'q') {
      return -2;

      // select the deck, return index to main()
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

      // add a new deck
    } else if (choice == 'a') {
      std::string name = "";
      int deckType = 0;
      std::string front = "";
      std::string back = "";

      int typeHighlight = 0;
      int typeChoice;
      int confirmChoice = 0;
      std::string cardInput = "";
      int yCur, xCur; // holds the current cursor position to simulate the
                      // backspace key for inputs.
      std::vector<std::string> typeOptions = {"- Verb Conjugation",
                                              "- Generic Flashcards"};

      WINDOW *addWindow = newwin(yMax - 2, xMax - 23, 1, 21);
      keypad(addWindow, true);
      box(addWindow, 0, 0);
      wbkgd(addWindow, COLOR_PAIR(1));
      wattron(addWindow, A_BOLD);
      mvwprintw(addWindow, 0, 2, "Create A Deck");
      wattroff(addWindow, A_BOLD);

      wattron(addWindow, A_BOLD);
      mvwprintw(addWindow, 2, 2, "Name:");
      wattroff(addWindow, A_BOLD);
      mvwprintw(addWindow, 3, 2, "___________________");

      wattron(addWindow, A_BOLD);
      mvwprintw(addWindow, 5, 2, "Deck Type:");
      wattroff(addWindow, A_BOLD);

      wattron(addWindow, A_BOLD);
      mvwprintw(addWindow, 9, 2, "Front Card");
      wattroff(addWindow, A_BOLD);
      mvwprintw(addWindow, 10, 2, "_______________");

      wattron(addWindow, A_BOLD);
      mvwprintw(addWindow, 9, 20, "Back Card");
      wattroff(addWindow, A_BOLD);
      mvwprintw(addWindow, 10, 20, "_______________");

      wattron(addWindow, A_BOLD);
      mvwprintw(addWindow, 12, 2, "<Create>");
      wattroff(addWindow, A_BOLD);

      wattron(addWindow, A_BOLD);
      mvwprintw(addWindow, 12, 20, "<Cancel>");
      wattroff(addWindow, A_BOLD);

      // prints the various deck choices.
      for (int i = 0; i < typeOptions.size(); ++i) {
        mvwprintw(addWindow, i + 6, 3, typeOptions[i].c_str());
      }

      /* takes input from the user as they type
             into the 'Name:' field, assigning it to the 'name'
             variable. */
      curs_set(1);
      echo();
      wmove(addWindow, 3, 2);
      wint_t c = wgetch(addWindow);
      while (c != '\n') {
        getyx(addWindow, yCur, xCur);
        if ((c == KEY_BACKSPACE || c == KEY_DC || c == 127) &&
            name.size() != 0) {
          name.pop_back();
          wprintw(addWindow, "_");
          wmove(addWindow, yCur, xCur);
        } else if ((c == KEY_BACKSPACE || c == KEY_DC || c == 127 ||
                    c == 263) &&
                   name.size() == 0) {
          wmove(addWindow, yCur, xCur);
          wprintw(addWindow, " ");
          wrefresh(addWindow);
        } else {
          name.push_back(c);
        }
        c = wgetch(addWindow);
      }
      noecho();
      curs_set(0);

      /* loop to get the choice of deck type from the user,
                 either 'verb conjugation' or 'generic flashcards'. */
      while (1) {
        for (int i = 0; i < typeOptions.size(); ++i) {
          if (i == typeHighlight)
            wattron(addWindow, A_REVERSE);

          mvwprintw(addWindow, i + 6, 3, typeOptions[i].c_str());
          wattroff(addWindow, A_REVERSE);
        }

        typeChoice = wgetch(addWindow);
        switch (typeChoice) {
        case KEY_UP:
        case 'k':
          typeHighlight--;
          if (typeHighlight == -1)
            typeHighlight = 0;
          break;
        case KEY_DOWN:
        case 'j':
          typeHighlight++;
          if (typeHighlight == 2)
            typeHighlight--;
          break;
        default:
          break;
        }

        if (typeChoice == 10) {
          deckType = typeHighlight;
          break;
        }
      }

      curs_set(1);
      wmove(addWindow, 10, 2);
      echo();
      wrefresh(addWindow);

      /* variables:
             yPtr: holds where to put the next input lines
             f: reads the character from the 'front' card input
             b: reads the character from the 'back' card input
      */

      int yPtr = 11;
      wint_t f, b;
      /* loop that constantly takes in input from the
             user as they add cards. terminated once a blank
             entry is input ('\n'); no characters before.
      */
      while (1) {
        f = wgetch(addWindow);
        while (f != '\n') {
          getyx(addWindow, yCur, xCur);
          if ((f == KEY_BACKSPACE || f == KEY_DC || f == 127) &&
              cardInput.size() != 0) {
            cardInput.pop_back();
            wprintw(addWindow, "_");
            wmove(addWindow, yCur, xCur);
          } else if ((f == KEY_BACKSPACE || f == KEY_DC || f == 127 ||
                      f == 263) &&
                     cardInput.size() == 0) {
            wmove(addWindow, yCur, xCur);
            wprintw(addWindow, " ");
            wrefresh(addWindow);
          } else {
            cardInput.push_back(f);
          }
          f = wgetch(addWindow);
        }

        if (f == '\n' && cardInput.size() == 0)
          break;

        front += cardInput + ",";
        cardInput.clear();
        wmove(addWindow, yPtr - 1, 20);

        // takes in the input for each of the back cards.
        b = wgetch(addWindow);
        while (b != '\n') {
          getyx(addWindow, yCur, xCur);
          if ((b == KEY_BACKSPACE || b == KEY_DC || b == 127) &&
              cardInput.size() != 0) {
            wprintw(addWindow, "_");
            wmove(addWindow, yCur, xCur);
            cardInput.pop_back();
          } else if ((b == KEY_BACKSPACE || b == KEY_DC || b == 127 ||
                      b == 263) &&
                     cardInput.size() == 0) {
            wmove(addWindow, yCur, xCur);
            wprintw(addWindow, " ");
            wrefresh(addWindow);
          } else {
            cardInput.push_back(b);
          }
          b = wgetch(addWindow);
        }

        back += cardInput + ",";

        cardInput.clear();

        /* clears the line of the <Create> button, and
           then recreates underneath the new bottom of
           card inputs. */
        wmove(addWindow, yPtr + 1, 2);
        wclrtoeol(addWindow);
        wattron(addWindow, A_BOLD);
        mvwprintw(addWindow, yPtr + 2, 2, "<Create>");
        mvwprintw(addWindow, yPtr + 2, 20, "<Cancel>");
        wattroff(addWindow, A_BOLD);

        mvwprintw(addWindow, yPtr, 2, "_______________");
        mvwprintw(addWindow, yPtr, 20, "_______________");
        wmove(addWindow, yPtr, 2);
        ++yPtr;
      }

      curs_set(0);
      noecho();

      int buttonHighlight = 0;
      int buttonChoice;
      std::vector<std::string> buttons = {"<Create>", "<Cancel>"};

      while (1) {
        for (int i = 0, x = 0; i < buttons.size(); ++i) {
          wattron(addWindow, A_BOLD);
          if (i == buttonHighlight)
            wattron(addWindow, A_REVERSE);

          mvwprintw(addWindow, yPtr + 1, 2 + x, buttons[i].c_str());
          x += buttons[i].size() + 10;
          wattroff(addWindow, A_BOLD);
          wattroff(addWindow, A_REVERSE);
        }

        buttonChoice = wgetch(addWindow);
        switch (buttonChoice) {
        case KEY_LEFT:
        case 'h':
          buttonHighlight--;
          if (buttonHighlight == -1)
            buttonHighlight = 0;
          break;
        case KEY_RIGHT:
        case 'l':
          buttonHighlight++;
          if (buttonHighlight == 2)
            buttonHighlight--;
          break;
        default:
          break;
        }

        if (buttonChoice == 10) {
          confirmChoice = buttonHighlight;
          break;
        }
      }

      if (confirmChoice == 0) {
        newName = name;
        newFront = front;
        newBack = back;
        return -1;
      } else {
        return -3;
      }
      // options menu
    } else if (choice == 'o') {
      const std::vector<std::string> options = {"Randomise Order of Cards",
                                                "Option 2"};
      int optionHighlight = 0;
      int optionChoice;
      WINDOW *optionsWin = newwin(yMax - 2, xMax - 23, 1, 21);
      wbkgd(optionsWin, COLOR_PAIR(1));
      box(optionsWin, 0, 0);
      wattron(optionsWin, A_BOLD);
      mvwprintw(optionsWin, 0, 2, "Options");
      wattroff(optionsWin, A_BOLD);
      wrefresh(optionsWin);

      while (1) {
        for (int i = 0; i < options.size(); ++i) {
          if (i == optionHighlight)
            wattron(optionsWin, A_REVERSE);

          mvwprintw(optionsWin, i + 2, 2, "[ ]");
          wattroff(optionsWin, A_REVERSE);
          mvwprintw(optionsWin, i + 2, 6, options[i].c_str());
        }

        optionChoice = wgetch(optionsWin);
        switch (optionChoice) {
        case KEY_UP:
        case 'k':
          optionHighlight--;
          if (optionHighlight == -1)
            optionHighlight = 0;
          break;
        case KEY_DOWN:
        case 'j':
          optionHighlight++;
          if (optionHighlight == 2)
            optionHighlight--;
          break;
        default:
          break;
        }

		// TODO: move somewhere above, as 'checked' button
		// is being overwritten by the for loop. avoid nested
		// 'if's also.
		if (optionChoice == ' ') {
		  if (optionHighlight == 0) {
			wmove(optionsWin, 2, 2);
			wclrtoeol(optionsWin);
			box(optionsWin, 0, 0);
			wprintw(optionsWin, "[x]");
			wrefresh(optionsWin);
			optShuffle = true;
		  }
		}
        else if (optionChoice == 10) {
          break;
		}

      }
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

void Menu::newDeck(std::unique_ptr<Deck> &deck) {
  deck->setName(newName);
  newFront.pop_back();
  newBack.pop_back();
  deck->create(newFront, newBack);
}

bool Menu::getShuffle() {
  return optShuffle;
}

Menu::~Menu() {}
