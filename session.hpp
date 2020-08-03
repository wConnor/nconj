#ifndef SESSION_HPP
#define SESSION_HPP

#include <ncurses.h>
#include <vector>
#include <string>

class Session
{
public:
  Session(const std::vector<std::string> &words);
  virtual ~Session();
};

#endif
