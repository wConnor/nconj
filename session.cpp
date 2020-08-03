#include "session.hpp"

Session::Session() {
  
}

void Session::begin() {
  int score = 0;
  for (auto i = front.begin(), z = back.begin(); i != front.end(); ++i, ++z) {
	std::string frTmp = *i, bkTmp = *z;
	std::string input = "";
	mvprintw(1, 0, "Translate: %s", frTmp.c_str());
	refresh();
	
	move(2,0);

	char c = getch();
	while (c != '\n') {
	  input.push_back(c);
	  c = getch();
	}

	clear();
	attron(A_BOLD);
	if (input == bkTmp) {
	  score += 1;
	  printw("Correct! (Score: %d / %d)", score, front.size());
	  attroff(A_BOLD);
	} else {
      printw("Incorrect. (Score %d / %d)", score, front.size());
	  attroff(A_BOLD);
	  printw("Answer: %s. You input: %s", bkTmp.c_str(), input.c_str());
	}

	refresh();
  }
}

void Session::outputContents() {
  
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
