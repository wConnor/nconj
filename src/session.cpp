#include "session.hpp"

Session::Session()
{
}

void Session::begin(Deck deck, const bool &shuffle)
{
	curs_set(1);
	echo();
	keypad(stdscr, TRUE);

	/* coordinates start from point 0,0 which is the top left corner of the
	   terminal
	   coordinates are in the form of y, x as opposed to the more common x, y
	   - y is known as the rank (row)
	   - x is known as the file (column)
	   coordinate arithmetic applies; ymax/2 brings the cursor to
	   mid.
	*/
	this->quit_flag = false;
	int score = 0;
	int ctr = 3; // line number where to put cursor
	int maxy, maxx;
	auto notes = deck.get_notes();
	std::vector<std::string> incorrect = {};
	getmaxyx(stdscr, maxy, maxx);
	WINDOW *check_win = newwin(2, maxx, 1, 0);

	// unsure how to shuffle a private member variable without making
	// a new vector; hence the use of the 'notes' vector. somewhat
	// inefficient.
	if (shuffle)
	{
		auto seed = std::default_random_engine(
			std::chrono::system_clock::now().time_since_epoch().count());
		std::shuffle(notes.begin(), notes.end(), seed);
	}

	for (const auto &d : notes)
	{
		std::string front = d.first, back = d.second;
		std::string input = "";
		std::string message =
			front + " + _________________ (" + deck.get_name() + ")";
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
			else if (c == 27) // ESC key
			{
				this->quit_flag = true;
				break;
			}
			else
			{
				input.push_back(c);
			}
			c = getch();
		}

		if (this->quit_flag)
		{
			break;
		}

		wclear(check_win);
		wattron(check_win, A_BOLD);

		/* checks whether or not the input that the user
		   has typed is equal to that which is expected, i.e.
		   it's on in the 'back' std::vector.
		*/
		if (input == back)
		{
			score += 1;
			mvwprintw(check_win, 0, maxx / 2 - 10, "Correct! (Score: %i / %lu)",
					  score, notes.size());
			wattroff(check_win, A_BOLD);
		}
		else
		{
			incorrect.push_back(front);
			incorrect.push_back(back);
			mvwprintw(check_win, 0, maxx / 2 - 10, "Incorrect. (Score %i / %lu)",
					  score, notes.size());
			wattroff(check_win, A_BOLD);
			mvwprintw(check_win, 1, maxx / 2 - 10, "Answer: %s. You input: %s",
					  back.c_str(), input.c_str());
		}
		wrefresh(check_win);
	}
	results(score, notes.size(), incorrect);
}

void Session::results(const int &score, const int &total,
					  const std::vector<std::string> incorrect)
{
	int resYMax, resXMax;
	getmaxyx(stdscr, resYMax, resXMax);
	WINDOW *results_win =
		newwin(15, resXMax / 2, resYMax / 2 - 10, resXMax / 2 - 35);
	box(results_win, 0, 0);

	mvwprintw(results_win, 0, 1, "Results");
	wattron(results_win, A_BOLD);
	mvwprintw(results_win, 1, 2, "Session Complete!");
	wattroff(results_win, A_BOLD);

	std::string message =
		"You scored: " + std::to_string(score) + " / " + std::to_string(total);

	mvwprintw(results_win, 2, 2, "%s", message.c_str());

	if (incorrect.size() > 0)
	{
		wattron(results_win, A_ITALIC);
		wattron(results_win, A_BOLD);
		mvwprintw(results_win, 4, 2, "Incorrect: (Front <--> Back)");
		wattroff(results_win, A_BOLD);
		wattroff(results_win, A_ITALIC);
	}

	for (int i = 0; i != incorrect.size(); i += 2)
	{
		mvwprintw(results_win, i + 5, 2, "%s", incorrect[i].c_str());
		mvwprintw(results_win, i + 5, 20, "<-->");
		mvwprintw(results_win, i + 5, 35, "%s", incorrect[i + 1].c_str());
	}
	wgetch(results_win);
	delwin(results_win);
	refresh();
}

Session::~Session()
{
}
