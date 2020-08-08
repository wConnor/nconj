#ifndef SESSION_HPP
#define SESSION_HPP

#include <algorithm>
#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>

class Session {
private:
public:
  Session();
  void begin(const std::vector<std::string> &front,
             const std::vector<std::string> &back,
             const std::string &infinitive);
  void results(const int &score, const int &total,
               const std::vector<std::string> incorrect);
  virtual ~Session();
};

#endif
