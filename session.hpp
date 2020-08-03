#ifndef SESSION_HPP
#define SESSION_HPP

#include <ncurses.h>
#include <vector>
#include <string>

class Session
{
private:
  std::vector<std::string> front;
  std::vector<std::string> back;
  
public:
  Session();
  void outputContents();
  void setFront(const std::vector<std::string> &words);
  void setBack(const std::vector<std::string> &words);
  std::vector<std::string> getFront();
  std::vector<std::string> getBack();
  virtual ~Session();
  
};

#endif
