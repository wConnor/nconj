#include "database.hpp"

Database::Database(const std::filesystem::path &db_path)
{
	this->db_path = db_path;
}

bool Database::init_db()
{
	int res = sqlite3_open(this->db_path.c_str(), &db);

	if (res != SQLITE_OK)
	{
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
		sqlite3_close(db);
		return false;
	}

	std::string sql_query = "CREATE TABLE NCONJ ("
							"id INTEGER PRIMARY KEY, "
							"name TEXT NOT NULL, "
							"type TEXT NOT NULL);";

	res = sqlite3_exec(db, sql_query.c_str(), nullptr, 0, nullptr);
	*log_file << std::time(nullptr) << "; "
			  << "Executed query " << sql_query << " res=" << res << ".\n";

	if (res != SQLITE_OK)
	{
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return false;
	}

	sql_query = "CREATE TABLE OPTIONS ("
				"id INTEGER PRIMARY KEY, "
				"option TEXT NOT NULL, "
				"description TEXT NOT NULL, "
				"value TEXT NOT NULL);";

	res = sqlite3_exec(db, sql_query.c_str(), nullptr, 0, nullptr);
	*log_file << std::time(nullptr) << "; "
			  << "Executed query " << sql_query << " res=" << res << ".\n";

	if (res != SQLITE_OK)
	{
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return false;
	}

	sql_query =
		"INSERT INTO OPTIONS (option, description, value) "
		"VALUES ('SHUFFLE', 'Randomise Order of Cards in a Session', 'TRUE');";

	res = sqlite3_exec(db, sql_query.c_str(), nullptr, 0, nullptr);
	*log_file << std::time(nullptr) << "; "
			  << "Executed query " << sql_query << " res=" << res << ".\n";

	if (res != SQLITE_OK)
	{
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return false;
	}

	sqlite3_close(db);

	*log_file << std::time(nullptr) << "; "
			  << "New database successfully initialised.\n";

	return true;
}

bool Database::delete_deck(const std::string &name)
{
	int res = sqlite3_open(db_path.c_str(), &db);
	std::string sql_query;

	if (res != SQLITE_OK)
	{
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return false;
	}

	sql_query = "DROP TABLE " + name + ";";
	res = sqlite3_exec(db, sql_query.c_str(), nullptr, 0, nullptr);
	*log_file << std::time(nullptr) << "; "
			  << "Executed query " << sql_query << " res=" << res << ".\n";

	if (res != SQLITE_OK)
	{
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
		sqlite3_close(db);
		return false;
	}

	sql_query = "DELETE FROM NCONJ WHERE name='" + name + "';";
	res = sqlite3_exec(db, sql_query.c_str(), nullptr, 0, nullptr);
	*log_file << std::time(nullptr) << "; "
			  << "Executed query " << sql_query << " res=" << res << ".\n";

	if (res != SQLITE_OK)
	{
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
		sqlite3_close(db);
		return false;
	}

	return true;
}

bool Database::save_deck(Deck &deck)
{
	int res = sqlite3_open(db_path.c_str(), &db);
	std::string sql_query;

	if (res != SQLITE_OK)
	{
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return false;
	}
	sql_query = "CREATE TABLE " + deck.get_name() +
				" ("
				"id INTEGER PRIMARY KEY, "
				"front TEXT NOT NULL, "
				"back TEXT NOT NULL);";

	res = sqlite3_exec(db, sql_query.c_str(), nullptr, 0, nullptr);
	*log_file << std::time(nullptr) << "; "
			  << "Executed query " << sql_query << " res=" << res << ".\n";

	if (res != SQLITE_OK)
	{
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
		sqlite3_close(db);
		return false;
	}

	for (auto &c : deck.get_notes())
	{
		sql_query = "INSERT INTO " + deck.get_name() +
					" (FRONT, BACK) "
					"VALUES ('" +
					c.first + "', '" + c.second + "');";

		res = sqlite3_exec(db, sql_query.c_str(), nullptr, 0, nullptr);
		*log_file << std::time(nullptr) << "; "
				  << "Executed query " << sql_query << " res=" << res << ".\n";

		if (res != SQLITE_OK)
		{
			*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
			sqlite3_close(db);
			return false;
		}
	}

	sql_query = "INSERT INTO NCONJ (NAME, TYPE) VALUES ('" + deck.get_name() +
				"', '" + deck.get_type_as_str() + "');";

	res = sqlite3_exec(db, sql_query.c_str(), nullptr, 0, nullptr);
	*log_file << std::time(nullptr) << "; "
			  << "Executed query " << sql_query << " res=" << res << ".\n";

	if (res != SQLITE_OK)
	{
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return false;
	}

	sqlite3_close(db);

	*log_file << std::time(nullptr) << "; "
			  << "Deck '" << deck.get_name()
			  << "' successfully saved to database.\n";

	return true;
}

bool Database::save_options(
	std::vector<std::tuple<std::string, std::string, std::string>> options)
{
	int res = sqlite3_open(db_path.c_str(), &db);

	if (res != SQLITE_OK)
	{
		// handle error
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
		sqlite3_close(db);
	}

	for (auto &c : options)
	{
		std::string sql_query = "UPDATE OPTIONS "
								"SET value = '" +
								std::get<2>(c) +
								"' "
								"WHERE option = '" +
								std::get<0>(c) + "';";

		res = sqlite3_exec(db, sql_query.c_str(), nullptr, 0, nullptr);
		*log_file << std::time(nullptr) << "; "
				  << "Executed query " << sql_query << " res=" << res << ".\n";

		if (res != SQLITE_OK)
		{
			// handle error
			*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
			sqlite3_close(db);
		}
	}

	sqlite3_close(db);

	return true;
}

std::vector<std::string> Database::retrieve_deck_names()
{
	std::vector<std::string> decks;

	int res = sqlite3_open(db_path.c_str(), &db);

	if (res != SQLITE_OK)
	{
		// handle error
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
		sqlite3_close(db);
	}

	sqlite3_stmt *stmt;
	std::string sql_query = "SELECT * FROM NCONJ;";

	res = sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt, nullptr);
	*log_file << std::time(nullptr) << "; "
			  << "Executed query " << sql_query << " res=" << res << ".\n";

	if (res != SQLITE_OK)
	{
		// handle error
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
		sqlite3_close(db);
	}

	while ((res = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		decks.push_back(std::string(
			reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1))));
	}

	*log_file << std::time(nullptr) << "; "
			  << "Retrieved " << decks.size() << " decks.\n";

	if (res != SQLITE_DONE)
	{
		// handle error
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
		sqlite3_close(db);
	}

	return decks;
}

Deck Database::retrieve_deck(const std::string &name)
{
	Deck deck;
	std::vector<std::pair<std::string, std::string>> notes;

	int res = sqlite3_open(db_path.c_str(), &db);

	if (res != SQLITE_OK)
	{
		// handle error
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
		sqlite3_close(db);
	}

	sqlite3_stmt *stmt;
	std::string sql_query = "SELECT * FROM " + name + ";";
	res = sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt, nullptr);
	*log_file << std::time(nullptr) << "; "
			  << "Executed query " << sql_query << " res=" << res << ".\n";

	if (res != SQLITE_OK)
	{
		// handle error
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
		sqlite3_close(db);
	}

	while ((res = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		notes.push_back({std::string(reinterpret_cast<const char *>(
							 sqlite3_column_text(stmt, 1))),
						 std::string(reinterpret_cast<const char *>(
							 sqlite3_column_text(stmt, 2)))});
	}

	*log_file << std::time(nullptr) << "; "
			  << "Retrieved " << notes.size()
			  << " notes from deck '" + name + "'.\n";

	if (res != SQLITE_DONE)
	{
		// handle error
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
		sqlite3_close(db);
	}

	deck.set_name(name);
	deck.set_notes(notes);

	return deck;
}

std::vector<std::tuple<std::string, std::string, std::string>> Database::
	retrieve_options()
{
	std::vector<std::tuple<std::string, std::string, std::string>> options;

	int res = sqlite3_open(db_path.c_str(), &db);

	if (res != SQLITE_OK)
	{
		// handle error
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
		sqlite3_close(db);
	}

	sqlite3_stmt *stmt;
	std::string sql_query = "SELECT * FROM OPTIONS;";
	res = sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt, nullptr);
	*log_file << std::time(nullptr) << "; "
			  << "Executed query " << sql_query << " res=" << res << ".\n";

	if (res != SQLITE_OK)
	{
		// handle error
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
		sqlite3_close(db);
	}

	while ((res = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		options.push_back({std::string(reinterpret_cast<const char *>(
							   sqlite3_column_text(stmt, 1))),
						   std::string(reinterpret_cast<const char *>(
							   sqlite3_column_text(stmt, 2))),
						   std::string(reinterpret_cast<const char *>(
							   sqlite3_column_text(stmt, 3)))});
	}

	*log_file << std::time(nullptr) << "; "
			  << "Retrieved " << options.size() << " options.\n";

	if (res != SQLITE_DONE)
	{
		// handle error
		*log_file << std::time(nullptr) << "; " << sqlite3_errmsg(db) << '\n';
		sqlite3_close(db);
	}

	return options;
}

void Database::set_log_file(std::shared_ptr<std::fstream> &log_file)
{
	this->log_file = log_file;
}

Database::~Database()
{
}
