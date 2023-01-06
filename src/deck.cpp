#include "deck.hpp"

Deck::Deck()
{
}

Deck::Deck(const std::string &name)
{
	this->name = name;
}

bool Deck::create(const std::vector<std::pair<std::string, std::string>> &notes)
{
	sqlite3 *db;

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

		std::string sql_query = "SELECT * FROM " + this->name + ";";
		res = sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt, nullptr);

		if (res != SQLITE_OK)
		{
			// handle error
		}
		else
		{
			while ((res = sqlite3_step(stmt)) == SQLITE_ROW)
			{
				notes.push_back(
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

void Deck::set_name(const std::string &name)
{
	this->name = name;
}

std::string Deck::get_name()
{
	return this->name;
}

void Deck::set_notes(std::vector<std::pair<std::string, std::string>> notes)
{
	this->notes = notes;
}

std::vector<std::pair<std::string, std::string>> Deck::get_notes()
{
	return this->notes;
}

Deck::~Deck()
{
}
