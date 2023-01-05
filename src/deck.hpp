#ifndef DECK_HPP
#define DECK_HPP

#include <algorithm>
#include <chrono>
#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

class Deck
{
  private:
	std::string file_name = "";
	std::string deck_name = "";
	std::vector<std::pair<std::string, std::string>> deck =
		{}; // { front side, back side }

  public:
	Deck();
	Deck(const std::string &deck_name);
	void create(const std::string &front, const std::string &back);
	void read_contents();

	void set_deck_name(const std::string &deck_name);
	std::string get_deck_name();

	std::vector<std::pair<std::string, std::string>> get_deck();

	virtual ~Deck();
};

#endif
