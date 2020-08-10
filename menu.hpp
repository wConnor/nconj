#ifndef MENU_HPP
#define MENU_HPP
#include <memory>
#include <string>
#include <vector>
#include <ncursesw/ncurses.h>

class Menu {
private:
public:
  Menu();
  int print(std::vector<std::string> menuOptions);
  void addOption(std::string key, std::string option,
                 std::unique_ptr<int> &length, WINDOW *win);
  virtual ~Menu();
};

#endif
