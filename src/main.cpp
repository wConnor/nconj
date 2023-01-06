#include "database.hpp"
#include "menu.hpp"
#include "session.hpp"
#include <iostream>
#include <sqlite3.h>

auto log_file = std::make_shared<std::fstream>();
const std::filesystem::path db_path = "decks.sql";

int main(int argc, char *argv[])
{
	(*log_file).open("log.txt",
					 std::fstream::in | std::fstream::app); // will be changed.
	std::unique_ptr<Database> db = std::make_unique<Database>(db_path);

	// checks to see whether or not the SQL database already exists. if not, then it
	// must be the user's first time running the program.
	if (!std::filesystem::exists(db_path))
	{
		db->set_log_file(log_file);
		db->init_db();

		Deck example1("être");
		example1.set_notes({{"je", "suis"},
							{"tu", "es"},
							{"il/elle", "est"},
							{"nous", "sommes"},
							{"vous", "êtes"},
							{"ils/elles", "sont"}});
		db->save_deck(example1);
		return 0;
	}

	std::vector<std::string> available_decks = {};

	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	start_color();

	std::unique_ptr<Menu> menu = std::make_unique<Menu>();
	int selectedDeck;
	while (true)
	{
		available_decks.clear();
		cbreak();
		for (const auto &e : std::filesystem::directory_iterator("decks"))
		{
			std::string tempName = "";
			std::filesystem::path tempPath = e.path();
			tempName = tempPath.string();

			auto start = tempName.find_last_of('/');
			tempName.erase(0, start + 1);

			auto end = tempName.find_last_of('.');
			tempName.erase(end, tempName.size() - 1);

			available_decks.push_back(tempName);
		}

		selectedDeck = menu->print(available_decks);
		// create new deck
		if (selectedDeck == -1)
		{
			std::unique_ptr<Deck> tempDeck = std::make_unique<Deck>();
			menu->new_deck(tempDeck);
			// quit program
		}
		else if (selectedDeck == -2)
		{
			break;
			// start session with chosen deck
		}
		else if (selectedDeck >= 0)
		{
			std::unique_ptr<Deck> deck =
				std::make_unique<Deck>(available_decks[selectedDeck]);
			std::unique_ptr<Session> session = std::make_unique<Session>();
			deck->read_contents();
			session->begin(deck->get_notes(), deck->get_name(), menu->get_shuffle());
		}
	}

	endwin();

	(*log_file).close();

	return 0;
}
