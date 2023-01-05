#ifndef SESSION_HPP
#define SESSION_HPP

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
  public:
	Session();
	void begin(std::vector<std::string> front, std::vector<std::string> back, const std::string &infinitive,
			   const bool &shuffle);
	void results(const int &score, const int &total, const std::vector<std::string> incorrect);
	virtual ~Session();
};

#endif