#include "menu.hpp"

Menu::Menu(std::shared_ptr<Database> db)
{
	this->db = db;
}

int Menu::print(std::vector<std::string> deck_list)
{
	auto enabled_options = db->retrieve_options();

	clear();
	noecho();
	curs_set(0);
	int yMax, xMax;
	int keyEnd = 0, sesLength = 0;
	std::unique_ptr<int> key = std::make_unique<int>(keyEnd);
	std::unique_ptr<int> ses = std::make_unique<int>(sesLength);
	getmaxyx(stdscr, yMax, xMax);

	// newwin(lines, cols, begin_y, begin_x);
	WINDOW *menu_win = newwin(yMax - 3, 20, 1, 0);
	WINDOW *keys_win = newwin(1, xMax, yMax - 2, 0);
	WINDOW *bar_win = newwin(1, xMax, 0, 0);
	WINDOW *message_win = newwin(1, xMax, yMax - 1, 0);

	// colour pairs for the windows
	//     ID1 = menu_win
	//	   ID2 = keys_win
	//	   ID2 = bar_win
	//	   ID1 (norm), ID3 (err) = message_win
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_CYAN);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	wbkgd(menu_win, COLOR_PAIR(1));
	wbkgd(keys_win, COLOR_PAIR(2));
	wbkgd(bar_win, COLOR_PAIR(2));

	// ensures contents in memory are written to the framebuffer.
	refresh();
	wrefresh(menu_win);
	wrefresh(keys_win);
	wrefresh(bar_win);
	wrefresh(message_win);

	// shows the user the usable keys at the bottom of the screen
	add_option(" ENTER ", "Select Option ", key, keys_win);
	add_option(" a ", "Create Deck", key, keys_win);
	add_option(" d ", "Delete Deck", key, keys_win);
	add_option(" o ", "Options", key, keys_win);
	add_option(" q ", "Quit ", key, keys_win);

	wrefresh(keys_win);

	keypad(menu_win, true);
	box(menu_win, 0, 0);
	wattron(menu_win, A_BOLD);
	mvwprintw(menu_win, 0, 2, "Decks");
	wattroff(menu_win, A_BOLD);
	whline(menu_win, ' ', 0);
	mvwprintw(bar_win, 0, 1, "nconj");

	wrefresh(menu_win);
	wrefresh(keys_win);
	wrefresh(bar_win);

	int choice;
	int highlight = 0;

	/* main menu. loop constantly reads for user input
		   until one of the three options are selected.
		   '10' is the ENTER key.
	*/
	while (1)
	{
		for (size_t i = 0; i < deck_list.size(); ++i)
		{
			if (i == highlight)
				wattron(menu_win, A_REVERSE);

			mvwprintw(menu_win, i + 1, 1, "%s", deck_list[i].c_str());
			wattroff(menu_win, A_REVERSE);
		}
		choice = wgetch(menu_win);

		switch (choice)
		{
		case KEY_UP:
		case 'k':
			highlight--;
			if (highlight == -1)
				highlight = 0;
			break;
		case KEY_DOWN:
		case 'j':
			highlight++;
			if (highlight == deck_list.size())
				highlight--;
			break;
		default:
			break;
		}
		// quit program
		if (choice == 'q')
		{
			return -2;
		}
		// select the deck, return index to main()
		else if (choice == 10)
		{
			wclear(menu_win);
			wclear(keys_win);
			wrefresh(menu_win);
			wrefresh(keys_win);
			mvwprintw(bar_win, 0, 1, "nconj session");
			add_option(" ESC ", "Quit", ses, keys_win);
			add_option(" F1 ", "Skip", ses, keys_win);
			wrefresh(keys_win);
			wrefresh(bar_win);

			return highlight;
		}
		// add a new deck
		else if (choice == 'a')
		{
			std::string name = "";
			int deck_type = 0;
			std::string front = "";
			std::string back = "";

			int type_highlight = 0;
			int typeChoice;
			int confirmChoice = 0;
			std::string card_input = "";
			int yCur, xCur; // holds the current cursor position to simulate the
							// backspace key for inputs.
			std::vector<std::string> typeOptions = {"- Verb Conjugation",
													"- Generic Flashcards"};

			WINDOW *add_window = newwin(yMax - 3, xMax - 23, 1, 21);
			mvwprintw(menu_win, 0, 2, "Create Deck");

			keypad(add_window, true);
			box(add_window, 0, 0);
			wbkgd(add_window, COLOR_PAIR(1));
			wattron(add_window, A_BOLD);
			mvwprintw(add_window, 0, 2, "Create A Deck");
			wattroff(add_window, A_BOLD);

			wattron(add_window, A_BOLD);
			mvwprintw(add_window, 2, 2, "Name:");
			wattroff(add_window, A_BOLD);
			mvwprintw(add_window, 3, 2, "___________________");

			wattron(add_window, A_BOLD);
			mvwprintw(add_window, 5, 2, "Deck Type:");
			wattroff(add_window, A_BOLD);

			wattron(add_window, A_BOLD);
			mvwprintw(add_window, 9, 2, "Front Card");
			wattroff(add_window, A_BOLD);
			mvwprintw(add_window, 10, 2, "_______________");

			wattron(add_window, A_BOLD);
			mvwprintw(add_window, 9, 20, "Back Card");
			wattroff(add_window, A_BOLD);
			mvwprintw(add_window, 10, 20, "_______________");

			wattron(add_window, A_BOLD);
			mvwprintw(add_window, 12, 2, "<Create>");
			wattroff(add_window, A_BOLD);

			wattron(add_window, A_BOLD);
			mvwprintw(add_window, 12, 20, "<Cancel>");
			wattroff(add_window, A_BOLD);

			// prints the various deck choices.
			for (size_t i = 0; i < typeOptions.size(); ++i)
			{
				mvwprintw(add_window, i + 6, 3, "%s", typeOptions[i].c_str());
			}

			/* takes input from the user as they type
				   into the 'Name:' field, assigning it to the 'name'
				   variable. */
			curs_set(1);
			echo();
			wmove(add_window, 3, 2);
			wint_t c = wgetch(add_window);
			while (c != '\n')
			{
				getyx(add_window, yCur, xCur);
				if ((c == KEY_BACKSPACE || c == KEY_DC || c == 127) &&
					name.size() != 0)
				{
					name.pop_back();
					wprintw(add_window, "_");
					wmove(add_window, yCur, xCur);
				}
				else if ((c == KEY_BACKSPACE || c == KEY_DC || c == 127 ||
						  c == 263) &&
						 name.size() == 0)
				{
					wmove(add_window, yCur, xCur);
					wprintw(add_window, " ");
					wrefresh(add_window);
				}
				else
				{
					name.push_back(c);
				}
				c = wgetch(add_window);
			}
			noecho();
			curs_set(0);

			// loop to get the choice of deck type from the user,
			// either 'verb conjugation' or 'generic flashcards'.
			while (1)
			{
				for (size_t i = 0; i < typeOptions.size(); ++i)
				{
					if (i == type_highlight)
						wattron(add_window, A_REVERSE);

					mvwprintw(add_window, i + 6, 3, "%s", typeOptions[i].c_str());
					wattroff(add_window, A_REVERSE);
				}

				typeChoice = wgetch(add_window);
				switch (typeChoice)
				{
				case KEY_UP:
				case 'k':
					type_highlight--;
					if (type_highlight == -1)
						type_highlight = 0;
					break;
				case KEY_DOWN:
				case 'j':
					type_highlight++;
					if (type_highlight == 2)
						type_highlight--;
					break;
				default:
					break;
				}

				if (typeChoice == 10)
				{
					deck_type = type_highlight;
					break;
				}
			}

			curs_set(1);
			wmove(add_window, 10, 2);
			echo();
			wrefresh(add_window);

			// variables:
			// - yPtr: holds where to put the next input lines
			// - f: reads the character from the 'front' card input
			// - b: reads the character from the 'back' card input
			int yPtr = 11;
			wint_t f, b;
			/* loop that constantly takes in input from the
				   user as they add cards. terminated once a blank
				   entry is input ('\n'); no characters before.
			*/
			while (1)
			{
				f = wgetch(add_window);
				while (f != '\n')
				{
					getyx(add_window, yCur, xCur);
					if ((f == KEY_BACKSPACE || f == KEY_DC || f == 127) &&
						card_input.size() != 0)
					{
						card_input.pop_back();
						wprintw(add_window, "_");
						wmove(add_window, yCur, xCur);
					}
					else if ((f == KEY_BACKSPACE || f == KEY_DC || f == 127 ||
							  f == 263) &&
							 card_input.size() == 0)
					{
						wmove(add_window, yCur, xCur);
						wprintw(add_window, " ");
						wrefresh(add_window);
					}
					else
					{
						card_input.push_back(f);
					}
					f = wgetch(add_window);
				}

				if (f == '\n' && card_input.size() == 0)
					break;

				std::string temp_front = card_input;
				card_input.clear();
				wmove(add_window, yPtr - 1, 20);

				// takes in the input for each of the back cards.
				b = wgetch(add_window);
				while (b != '\n')
				{
					getyx(add_window, yCur, xCur);
					if ((b == KEY_BACKSPACE || b == KEY_DC || b == 127) &&
						card_input.size() != 0)
					{
						wprintw(add_window, "_");
						wmove(add_window, yCur, xCur);
						card_input.pop_back();
					}
					else if ((b == KEY_BACKSPACE || b == KEY_DC || b == 127 ||
							  b == 263) &&
							 card_input.size() == 0)
					{
						wmove(add_window, yCur, xCur);
						wprintw(add_window, " ");
						wrefresh(add_window);
					}
					else
					{
						card_input.push_back(b);
					}
					b = wgetch(add_window);
				}

				std::string temp_back = card_input;
				temp_deck.push_back({temp_front, temp_back});

				card_input.clear();

				/* clears the line of the <Create> button, and
				   then recreates underneath the new bottom of
				   card inputs. */
				wmove(add_window, yPtr + 1, 2);
				wclrtoeol(add_window);
				box(add_window, 0, 0);
				wattron(add_window, A_BOLD);
				mvwprintw(add_window, yPtr + 2, 2, "<Create>");
				mvwprintw(add_window, yPtr + 2, 20, "<Cancel>");
				wattroff(add_window, A_BOLD);

				mvwprintw(add_window, yPtr, 2, "_______________");
				mvwprintw(add_window, yPtr, 20, "_______________");
				wmove(add_window, yPtr, 2);
				++yPtr;
			}

			curs_set(0);
			noecho();

			int button_highlight = 0;
			int button_choice;
			std::vector<std::string> buttons = {"<Create>", "<Cancel>"};

			while (1)
			{
				for (size_t i = 0, x = 0; i < buttons.size(); ++i)
				{
					wattron(add_window, A_BOLD);
					if (i == button_highlight)
						wattron(add_window, A_REVERSE);

					mvwprintw(add_window, yPtr + 1, 2 + x, "%s", buttons[i].c_str());
					x += buttons[i].size() + 10;
					wattroff(add_window, A_BOLD);
					wattroff(add_window, A_REVERSE);
				}

				button_choice = wgetch(add_window);
				switch (button_choice)
				{
				case KEY_LEFT:
				case 'h':
					button_highlight--;
					if (button_highlight == -1)
						button_highlight = 0;
					break;
				case KEY_RIGHT:
				case 'l':
					button_highlight++;
					if (button_highlight == 2)
						button_highlight--;
					break;
				default:
					break;
				}

				if (button_choice == 10)
				{
					confirmChoice = button_highlight;
					break;
				}
			}

			if (confirmChoice == 0)
			{
				if (!name.empty() && !front.empty() && !back.empty())
				{
					new_name = name;
					wbkgd(message_win, COLOR_PAIR(1));
					wattron(message_win, A_BOLD);
					mvwprintw(message_win, 0, 1, "Deck %s has been created.",
							  name.c_str());
					wattroff(message_win, A_BOLD);
					wgetch(message_win);
					return -1;
				}
				else
				{
					wbkgd(message_win, COLOR_PAIR(3));
					wattron(message_win, A_BOLD);
					mvwprintw(
						message_win, 0, 1,
						"ERROR: One or more fields left empty. Deck has not been "
						"created.");
					wattroff(message_win, A_BOLD);
					wgetch(message_win);
					return -3;
				}
			}
			else
			{
				return -3;
			}
		}
		else if (choice == 'd')
		{
			WINDOW *deleteWin = newwin(yMax - 3, xMax - 23, 1, 21);
			wbkgd(deleteWin, COLOR_PAIR(1));
			box(deleteWin, 0, 0);
			wattron(deleteWin, A_BOLD);
			mvwprintw(deleteWin, 0, 2, "Delete Deck");
			wattroff(deleteWin, A_BOLD);

			std::string delMessage = "Are you sure you would like to delete ";
			mvwprintw(deleteWin, 2, 2, "%s", delMessage.c_str());
			wattron(deleteWin, A_BOLD);
			mvwprintw(deleteWin, 2, delMessage.length() + 2, "%s",
					  deck_list[highlight].c_str());
			wattroff(deleteWin, A_BOLD);
			mvwprintw(deleteWin, 2,
					  delMessage.length() + deck_list[highlight].length() + 2, "?");
			wrefresh(deleteWin);

			int button_highlight = 1;
			int button_choice;
			std::vector<std::string> buttons = {"<Delete>", "<Cancel>"};

			while (1)
			{
				for (size_t i = 0, x = 0; i < buttons.size(); ++i)
				{
					wattron(deleteWin, A_BOLD);
					if (i == button_highlight)
						wattron(deleteWin, A_REVERSE);

					mvwprintw(deleteWin, 4, 2 + x, "%s", buttons[i].c_str());
					x += buttons[i].size() + 10;
					wattroff(deleteWin, A_BOLD);
					wattroff(deleteWin, A_REVERSE);
				}

				button_choice = wgetch(deleteWin);
				switch (button_choice)
				{
				case KEY_LEFT:
				case 'h':
					button_highlight--;
					if (button_highlight == -1)
						button_highlight = 0;
					break;
				case KEY_RIGHT:
				case 'l':
					button_highlight++;
					if (button_highlight == 2)
						button_highlight--;
					break;
				default:
					break;
				}

				if (button_choice == 10)
				{
					if (button_highlight == 0)
					{
						std::filesystem::remove(std::filesystem::path(
							"./decks/" + deck_list[highlight] + ".ncj"));
						wbkgd(message_win, COLOR_PAIR(1));
						wattron(message_win, A_BOLD);
						mvwprintw(message_win, 0, 1, "Deck '%s' has been deleted.",
								  deck_list[highlight].c_str());
						wattroff(message_win, A_BOLD);
						wgetch(message_win);
					}
					return -3;
				}
			}

			// options menu
		}
		else if (choice == 'o')
		{
			int option_highlight = 0;
			int option_choice;
			WINDOW *options_win = newwin(yMax - 3, xMax - 23, 1, 21);
			wbkgd(options_win, COLOR_PAIR(1));
			box(options_win, 0, 0);
			wattron(options_win, A_BOLD);
			mvwprintw(options_win, 0, 2, "Options");
			wattroff(options_win, A_BOLD);
			wrefresh(options_win);

			while (true)
			{
				for (size_t i = 0; i < options.size(); ++i)
				{
					if (i == option_highlight)
					{
						wattron(options_win, A_REVERSE);
					}

					std::string option_str = "";
					option_str += enabled_options[options[i].first] == "TRUE"
									  ? "[ x ]"
									  : "[  ]";

					mvwprintw(options_win, i + 2, 2, "%s", option_str.c_str());
					wattroff(options_win, A_REVERSE);
				}

				option_choice = wgetch(options_win);
				switch (option_choice)
				{
				case KEY_UP:
				case 'k':
					option_highlight--;
					if (option_highlight == -1)
					{
						option_highlight = 0;
					}
					break;
				case KEY_DOWN:
				case 'j':
					option_highlight++;
					if (option_highlight == options.size())
					{
						option_highlight--;
					}
					break;
				default:
					break;
				}

				if (option_choice == ' ')
				{
					if (option_highlight == 0 && options[0][1] == ' ')
					{
						options[0][1] = 'x';
						opt_shuffle = true;
					}
					else if (option_highlight == 0)
					{
						options[0][1] = ' ';
						opt_shuffle = false;
					}
				}
				else if (option_choice == 10)
				{
					wclear(options_win);
					wrefresh(options_win);
					break;
				}
			}
		}
	}

	return -1;
}

void Menu::add_option(std::string key, std::string option,
					  std::unique_ptr<int> &length, WINDOW *win)
{
	wattron(win, A_REVERSE);
	mvwprintw(win, 0, *length, "%s", key.c_str());
	*length += key.length() + 1;
	wattroff(win, A_REVERSE);
	mvwprintw(win, 0, *length, "%s", option.c_str());
	*length += option.length() + 1;
}

void Menu::new_deck(std::unique_ptr<Deck> &deck)
{
	deck->set_name(new_name);
	Deck deck2;
	db->save_deck(deck2);
}

bool Menu::get_shuffle()
{
	return opt_shuffle;
}

Menu::~Menu()
{
}
