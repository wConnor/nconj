#ifndef SESSION_HPP
#define SESSION_HPP

#include "deck.hpp"

#include <algorithm>
#include <chrono>
#include <memory>
#include <ncurses.h>
#include <random>
#include <string>
#include <vector>

class Session
{
private:
	bool quit_flag = false;

public:
	Session();
	void begin(Deck deck, const bool &shuffle);
	void results(const int &score, const int &total,
				 const std::vector<std::string> incorrect);
	virtual ~Session();
};

#endif
