#include "menu.hpp"
#include "session.hpp"
#include <filesystem>
#include <sqlite3.h>

const std::filesystem::path db_path = "./decks.sql";

int main(int argc, char *argv[])
{
	// checks to see whether or not the SQL database already exists. if not, then it
	// must be the user's first time running the program.
	if (!std::filesystem::exists(db_path))
	{
		sqlite3 *db;
		int res = sqlite3_open(db_path.c_str(), &db);
		std::string sql_query;

		if (res != SQLITE_OK)
		{
			// handle error
		}
		else
		{
			sql_query = "CREATE TABLE NCONJ ("
						"id INTEGER PRIMARY KEY, "
						"name TEXT NOT NULL);";

			res = sqlite3_exec(db, sql_query.c_str(), nullptr, 0, nullptr);

			if (res != SQLITE_OK)
			{
				// handle error
			}
			else
			{
				// success
			}
		}
		sqlite3_close(db);

		std::unique_ptr<Deck> example1 = std::make_unique<Deck>("être");

		example1->create({{"je", "suis"},
						  {"tu", "es"},
						  {"il/elle", "est"},
						  {"nous", "sommes"},
						  {"vous", "êtes"},
						  {"ils/elles", "sont"}});
	}

	std::vector<std::string> availableDecks = {};

	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	start_color();

	std::unique_ptr<Menu> menu = std::make_unique<Menu>();
	int selectedDeck;
	while (1)
	{
		availableDecks.clear();
		cbreak();
		for (const auto &e : std::filesystem::directory_iterator("./decks/"))
		{
			std::string tempName = "";
			std::filesystem::path tempPath = e.path();
			tempName = tempPath.string();

			auto start = tempName.find_last_of('/');
			tempName.erase(0, start + 1);

			auto end = tempName.find_last_of('.');
			tempName.erase(end, tempName.size() - 1);

			availableDecks.push_back(tempName);
		}

		selectedDeck = menu->print(availableDecks);
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
				std::make_unique<Deck>(availableDecks[selectedDeck]);
			std::unique_ptr<Session> session = std::make_unique<Session>();
			deck->read_contents();
			session->begin(deck->get_deck(), deck->get_deck_name(),
						   menu->get_shuffle());
		}
	}

	endwin();

	return 0;
}
