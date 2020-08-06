#ifndef MENU_HPP
#define MENU_HPP
#include <memory>
#include <ncurses.h>

class Menu
{
private:

public:
  Menu();
  int print();
  void addOption(std::string key, std::string option, std::unique_ptr<int> &length, WINDOW *win);
  virtual ~Menu();
};



#endif
