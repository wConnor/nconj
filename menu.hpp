#ifndef MENU_HPP
#define MENU_HPP
#include <memory>
#include <string>
#include <vector>
#include <ncursesw/ncurses.h>
#include "deck.hpp"

class Menu {
private:
  std::string newName = "";
  std::string newFront = "";
  std::string newBack = "";
  
public:
  Menu();
  int print(std::vector<std::string> menuOptions);
  void addOption(std::string key, std::string option,
                 std::unique_ptr<int> &length, WINDOW *win);
  void newDeck(std::unique_ptr<Deck> &deck);
  virtual ~Menu();
};

#endif
