#include "menu.hpp"
#include "session.hpp"
#include <iostream>

auto log_file = std::make_shared<std::fstream>();
const std::filesystem::path db_path = "decks.sql";

int main(int argc, char *argv[])
{
	(*log_file).open("log.txt",
					 std::fstream::in | std::fstream::app); // will be changed.
	std::shared_ptr<Database> db = std::make_shared<Database>(db_path);
	db->set_log_file(log_file);

	// checks to see whether or not the SQL database already exists. if not, then it
	// must be the user's first time running the program.
	if (!std::filesystem::exists(db_path))
	{
		db->init_db();

		Deck example1("être");
		example1.set_notes({{"je", "suis"},
							{"tu", "es"},
							{"il/elle", "est"},
							{"nous", "sommes"},
							{"vous", "êtes"},
							{"ils/elles", "sont"}});
		db->save_deck(example1);
	}

	std::vector<std::string> available_decks = {};

	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	start_color();

	Menu menu(db);
	int selected_deck;
	while (true)
	{
		available_decks.clear();
		cbreak();
		available_decks = db->retrieve_deck_names();
		selected_deck = menu.print(available_decks);

		// create new deck
		if (selected_deck == -1)
		{
			std::unique_ptr<Deck> tempDeck = std::make_unique<Deck>();
			menu.new_deck(tempDeck);
		}
		// quit program
		else if (selected_deck == -2)
		{
			break;
		}
		// start session with chosen deck
		else if (selected_deck >= 0)
		{
			Deck deck = db->retrieve_deck(available_decks[selected_deck]);
			Session session;
			session.begin(deck, menu.get_shuffle());
		}
	}

	endwin();

	(*log_file).close();

	return 0;
}
