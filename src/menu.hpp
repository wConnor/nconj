#ifndef MENU_HPP
#define MENU_HPP

#include "database.hpp"
#include "deck.hpp"

#include <map>
#include <memory>
#include <ncurses.h>

class Menu
{
private:
	std::vector<std::pair<std::string, std::string>> temp_deck;
	// {option, description}
	std::string new_name = "";
	bool opt_shuffle = true;
	std::shared_ptr<Database> db;

public:
	Menu(std::shared_ptr<Database> db);
	int print(std::vector<std::string> deck_list);
	void add_option(std::string key, std::string option,
					std::unique_ptr<int> &length, WINDOW *win);
	void new_deck(std::unique_ptr<Deck> &deck);
	bool get_shuffle();
	virtual ~Menu();
};

#endif
