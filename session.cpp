#include "session.hpp"

Session::Session() {
  
}

void Session::outputContents() {
  for (std::vector<std::string>::const_iterator i = front.begin(), z = back.begin(); i != front.end(); ++i, ++z) {
	std::string frTmp = *i, bkTmp = *z;
	mvprintw(std::distance<std::vector<std::string>::const_iterator>(front.begin(), i), 1, "%s, %s", frTmp.c_str(), bkTmp.c_str());
  }
}

void Session::setFront(const std::vector<std::string> &words){
  front = words;
}

void Session::setBack(const std::vector<std::string> &words){
  back = words;
}

std::vector<std::string> Session::getFront() { return front; }

std::vector<std::string> Session::getBack() { return back; }

Session::~Session() {
  
}
