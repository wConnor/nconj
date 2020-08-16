#ifndef SESSION_HPP
#define SESSION_HPP

#include <algorithm>
#include <memory>
#include <ncursesw/ncurses.h>
#include <string>
#include <vector>
#include <chrono>
#include <random>

class Session {
private:
public:
  Session();
  void begin(std::vector<std::string> front,
             std::vector<std::string> back,
             const std::string &infinitive, const bool &shuffle);
  void results(const int &score, const int &total,
               const std::vector<std::string> incorrect);
  virtual ~Session();
};

#endif
