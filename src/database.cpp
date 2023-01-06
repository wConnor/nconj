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
							"name TEXT NOT NULL);";

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

	sql_query = "INSERT INTO NCONJ (NAME) VALUES ('" + deck.get_name() + "');";
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

void Database::set_log_file(std::shared_ptr<std::fstream> &log_file)
{
	this->log_file = log_file;
}

Database::~Database()
{
}
