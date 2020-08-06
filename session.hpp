#ifndef SESSION_HPP
#define SESSION_HPP

#include <ncurses.h>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

class Session
{
private:
  
public:
  Session();
  void begin(const std::vector<std::string> &front, const std::vector<std::string> &back);
  virtual ~Session();
  
};

#endif
