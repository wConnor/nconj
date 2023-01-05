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
	std::vector<std::string> front = {};
	std::vector<std::string> back = {};

  public:
	Deck();
	Deck(const std::string &name);
	void create(const std::string &front, const std::string &back);
	void read_contents();

	void set_name(const std::string &name);
	std::string get_name();

	std::vector<std::string> get_front();

	std::vector<std::string> get_back();

	virtual ~Deck();
};

#endif
