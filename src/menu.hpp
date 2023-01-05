#ifndef MENU_HPP
#define MENU_HPP

#include "deck.hpp"

#include <filesystem>
#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>

class Menu
{
  private:
	std::string new_name = "";
	std::string new_front = "";
	std::string new_back = "";
	bool opt_shuffle = true;

  public:
	Menu();
	int print(std::vector<std::string> menuOptions);
	void add_option(std::string key, std::string option,
					std::unique_ptr<int> &length, WINDOW *win);
	void new_deck(std::unique_ptr<Deck> &deck);
	bool get_shuffle();
	virtual ~Menu();
};

#endif
