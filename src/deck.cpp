#include "deck.hpp"

Deck::Deck()
{
}

Deck::Deck(const std::string &deck_name)
{
	this->deck_name = deck_name;
}

bool Deck::create(const std::vector<std::pair<std::string, std::string>> &notes)
{
	sqlite3 *db;
	int res = sqlite3_open(db_path.c_str(), &db);
	std::string sql_query;

	if (res != SQLITE_OK)
	{
		// handle error
		return false;
	}
	else
	{
		sql_query = "CREATE TABLE " + this->deck_name +
					" ("
					"id INTEGER PRIMARY KEY, "
					"front TEXT NOT NULL, "
					"back TEXT NOT NULL);";

		res = sqlite3_exec(db, sql_query.c_str(), nullptr, 0, nullptr);

		if (res != SQLITE_OK)
		{
			// handle error
			sqlite3_close(db);
			return false;
		}

		for (auto &c : notes)
		{
			sql_query = "INSERT INTO " + this->deck_name +
						" (FRONT, BACK) "
						"VALUES ('" +
						c.first + "', " + c.second + ");";
			res = sqlite3_exec(db, sql_query.c_str(), nullptr, 0, nullptr);

			if (res != SQLITE_OK)
			{
				// handle error
				sqlite3_close(db);
				return false;
			}
		}

		sql_query = "INSERT INTO NCONJ (NAME) VALUES ('" + this->deck_name + "');";
		res = sqlite3_exec(db, sql_query.c_str(), nullptr, 0, nullptr);

		if (res != SQLITE_OK)
		{
			// handle error
			sqlite3_close(db);
			return false;
		}
	}
	sqlite3_close(db);

	return true;
}

void Deck::read_contents()
{
	sqlite3 *db;
	int res = sqlite3_open(db_path.c_str(), &db);

	if (res != SQLITE_OK)
	{
		// handle error
	}
	else
	{
		sqlite3_stmt *stmt;

		std::string sql_query = "SELECT * FROM " + this->deck_name + ";";
		res = sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt, nullptr);

		if (res != SQLITE_OK)
		{
			// handle error
		}
		else
		{
			while ((res = sqlite3_step(stmt)) == SQLITE_ROW)
			{
				deck.push_back(
					{reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)),
					 reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1))});
			}

			if (res != SQLITE_DONE)
			{
				// handle error
			}
		}
	}
}

void Deck::set_deck_name(const std::string &deck_name)
{
	this->deck_name = deck_name;
}

std::string Deck::get_deck_name()
{
	return this->deck_name;
}

std::vector<std::pair<std::string, std::string>> Deck::get_deck()
{
	return this->deck;
}

Deck::~Deck()
{
}
