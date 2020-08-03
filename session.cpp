#include "session.hpp"

Session::Session(const std::vector<std::string> &words) {
  printw("session object called! contents:\n");

  for (std::vector<std::string>::const_iterator i = words.begin(); i != words.end(); ++i) {
	std::string temp = *i;
	printw("%s \n", i->c_str());
  }
}

Session::~Session() {
  
}
