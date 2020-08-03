#include "session.hpp"

Session::Session() {
  
}

void Session::begin() {
  /* coordinates start from point 0,0 which is the top left corner of the
	 terminal
	 coordinates are in the form of y, x as opposed to the more common x, y
	 y is known as the rank (row)
	 x is known as the file (column)
	 coordinate arithmetic applies; ymax/2 brings the cursor to
	 mid.
  */
  echo();
  int score = 0;
  int maxy, maxx;
  getmaxyx(stdscr, maxy, maxx);
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
	// -10 necessary to bring the text more towards the center.
	if (input == bkTmp) {
	  score += 1;
	  mvprintw(0, maxx/2-10, "Correct! (Score: %d / %d)", score, front.size());
	  attroff(A_BOLD);
	} else {
      mvprintw(0, maxx/2-10, "Incorrect. (Score %d / %d)", score, front.size());
	  attroff(A_BOLD);
	  mvprintw(1, maxx/2-10, "Answer: %s. You input: %s", bkTmp.c_str(), input.c_str());
	}

	refresh();
	echo();
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
