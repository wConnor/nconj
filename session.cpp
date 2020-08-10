#include "session.hpp"

Session::Session() {}

void Session::begin(const std::vector<std::string> &front,
                    const std::vector<std::string> &back,
                    const std::string &infinitive) {
  curs_set(1);
  echo();
  keypad(stdscr, TRUE);

  raw();

  /* coordinates start from point 0,0 which is the top left corner of the
         terminal
         coordinates are in the form of y, x as opposed to the more common x, y
         y is known as the rank (row)
         x is known as the file (column)
         coordinate arithmetic applies; ymax/2 brings the cursor to
         mid.
  */
  int score = 0;
  int ctr = 3;
  int maxy, maxx;
  std::vector<std::string> incorrect = {};
  getmaxyx(stdscr, maxy, maxx);
  WINDOW *checkWin = newwin(2, maxx, 1, 0);
  for (auto i = front.begin(), z = back.begin(); i != front.end(); ++i, ++z) {
    std::string frTmp = *i, bkTmp = *z;
    std::string input = "";
    std::string message = frTmp + " + _________________ (" + infinitive + ")";
    mvprintw(ctr, 0, message.c_str());
    refresh();

    // moves cursor to the start of the underscores
    move(ctr, frTmp.size() + 3);
    ++ctr;

    int yCur, xCur;
    /* takes in the input from the user. if the input is NOT
       a key that deletes a character, then it's appended to the
       std::string. else,  it'll delete the last character that
       was input.
    */
    wint_t c = getch();
    while (c != '\n') {
      getyx(stdscr, yCur, xCur);
      if ((c == KEY_BACKSPACE || c == KEY_DC || c == 127 || c == 263) &&
          input.size() != 0) {
        input.pop_back();
        printw("_");
        move(yCur, xCur);
      } else if ((c == KEY_BACKSPACE || c == KEY_DC || c == 127 || c == 263) &&
                 input.size() == 0) {
        move(yCur, xCur);
        printw(" ");
        refresh();
      } else if (c == KEY_F(1)) {
        break;
      } else {
        input.push_back(c);
      }
      c = getch();
    }

    wclear(checkWin);
    wattron(checkWin, A_BOLD);

    /* checks whether or not the input that the user
       has typed is equal to that which is expected, i.e.
       it's on in the 'back' std::vector.
    */
    if (input == bkTmp) {
      score += 1;
      mvwprintw(checkWin, 0, maxx / 2 - 10, "Correct! (Score: %d / %d)", score,
                front.size());
      wattroff(checkWin, A_BOLD);
    } else {
      incorrect.push_back(frTmp);
      incorrect.push_back(bkTmp);
      mvwprintw(checkWin, 0, maxx / 2 - 10, "Incorrect. (Score %d / %d)", score,
                front.size());
      wattroff(checkWin, A_BOLD);
      mvwprintw(checkWin, 1, maxx / 2 - 10, "Answer: %s. You input: %s",
                bkTmp.c_str(), input.c_str());
    }
    wrefresh(checkWin);
  }
  results(score, front.size(), incorrect);
}

void Session::results(const int &score, const int &total,
                      const std::vector<std::string> incorrect) {
  int resYMax, resXMax;
  getmaxyx(stdscr, resYMax, resXMax);
  WINDOW *resultsWin =
      newwin(15, resXMax / 2 - 10, resYMax / 2 - 10, resXMax / 2 - 35);
  box(resultsWin, 0, 0);

  mvwprintw(resultsWin, 0, 1, "Results");
  wattron(resultsWin, A_BOLD);
  mvwprintw(resultsWin, 1, 2, "Session Complete!");
  wattroff(resultsWin, A_BOLD);

  std::string message =
      "You scored: " + std::to_string(score) + " / " + std::to_string(total);

  mvwprintw(resultsWin, 2, 2, message.c_str());

  if (incorrect.size() > 0) {
    wattron(resultsWin, A_ITALIC);
    wattron(resultsWin, A_BOLD);
    mvwprintw(resultsWin, 4, 2, "Incorrect: (Front <--> Back)");
    wattroff(resultsWin, A_BOLD);
    wattroff(resultsWin, A_ITALIC);
  }

  for (int i = 0; i != incorrect.size(); i += 2) {
    mvwprintw(resultsWin, i + 5, 2, incorrect[i].c_str());
    mvwprintw(resultsWin, i + 5, 20, "<-->");
    mvwprintw(resultsWin, i + 5, 35, incorrect[i + 1].c_str());
  }
  wgetch(resultsWin);
  delwin(resultsWin);
  refresh();
}

Session::~Session() {}
