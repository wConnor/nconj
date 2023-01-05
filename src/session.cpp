#include "session.hpp"

Session::Session()
{
}

void Session::begin(std::vector<std::pair<std::string, std::string>> deck,
					const std::string &infinitive, const bool &shuffle)
{
	curs_set(1);
	echo();
	keypad(stdscr, TRUE);

	raw();

	/* coordinates start from point 0,0 which is the top left corner of the
	   terminal
	   coordinates are in the form of y, x as opposed to the more common x, y
	   - y is known as the rank (row)
	   - x is known as the file (column)
	   coordinate arithmetic applies; ymax/2 brings the cursor to
	   mid.
	*/
	int score = 0;
	int ctr = 3; // line number where to put cursor
	int maxy, maxx;
	std::vector<std::string> incorrect = {};
	getmaxyx(stdscr, maxy, maxx);
	WINDOW *checkWin = newwin(2, maxx, 1, 0);

	if (shuffle)
	{
		std::shuffle(
			deck.begin(), deck.end(),
			std::default_random_engine(
				std::chrono::system_clock::now().time_since_epoch().count()));
	}

	for (const auto &d : deck)
	{
		std::string front = d.first, back = d.second;
		std::string input = "";
		std::string message = front + " + _________________ (" + infinitive + ")";
		mvprintw(ctr, 0, "%s", message.c_str());
		refresh();

		// moves cursor to the start of the underscores
		move(ctr, front.size() + 3);
		++ctr;

		int y_cur, x_cur;
		/* takes in the input from the user. if the input is NOT
		   a key that deletes a character, then it's appended to the
		   std::string. else, it'll delete the last character that
		   was input.
		*/
		wint_t c = getch();
		while (c != '\n')
		{
			getyx(stdscr, y_cur, x_cur);
			if ((c == KEY_BACKSPACE || c == KEY_DC || c == 127 || c == 263) &&
				input.size() != 0)
			{
				input.pop_back();
				printw("_");
				move(y_cur, x_cur);
			}
			else if ((c == KEY_BACKSPACE || c == KEY_DC || c == 127 || c == 263) &&
					 input.size() == 0)
			{
				move(y_cur, x_cur);
				printw(" ");
				refresh();
			}
			else if (c == KEY_F(1))
			{
				break;
			}
			else
			{
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
		if (input == back)
		{
			score += 1;
			mvwprintw(checkWin, 0, maxx / 2 - 10, "Correct! (Score: %i / %lu)",
					  score, deck.size());
			wattroff(checkWin, A_BOLD);
		}
		else
		{
			incorrect.push_back(front);
			incorrect.push_back(back);
			mvwprintw(checkWin, 0, maxx / 2 - 10, "Incorrect. (Score %i / %lu)",
					  score, deck.size());
			wattroff(checkWin, A_BOLD);
			mvwprintw(checkWin, 1, maxx / 2 - 10, "Answer: %s. You input: %s",
					  back.c_str(), input.c_str());
		}
		wrefresh(checkWin);
	}
	results(score, deck.size(), incorrect);
}

void Session::results(const int &score, const int &total,
					  const std::vector<std::string> incorrect)
{
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

	mvwprintw(resultsWin, 2, 2, "%s", message.c_str());

	if (incorrect.size() > 0)
	{
		wattron(resultsWin, A_ITALIC);
		wattron(resultsWin, A_BOLD);
		mvwprintw(resultsWin, 4, 2, "Incorrect: (Front <--> Back)");
		wattroff(resultsWin, A_BOLD);
		wattroff(resultsWin, A_ITALIC);
	}

	for (int i = 0; i != incorrect.size(); i += 2)
	{
		mvwprintw(resultsWin, i + 5, 2, "%s", incorrect[i].c_str());
		mvwprintw(resultsWin, i + 5, 20, "<-->");
		mvwprintw(resultsWin, i + 5, 35, "%s", incorrect[i + 1].c_str());
	}
	wgetch(resultsWin);
	delwin(resultsWin);
	refresh();
}

Session::~Session()
{
}
