#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "deck.hpp"

#include <chrono>
#include <ctime>
#include <fstream>
#include <map>
#include <sqlite3.h>

class Database
{
private:
	std::shared_ptr<std::fstream> log_file;
	std::filesystem::path db_path = "";
	sqlite3 *db;

public:
	Database(const std::filesystem::path &db_path);
	bool init_db();
	bool save_deck(Deck &deck);
	std::vector<std::string> retrieve_deck_names();
	std::map<std::string, std::string> retrieve_options();
	Deck retrieve_deck(const std::string &name);

	void set_log_file(std::shared_ptr<std::fstream> &log_file);

	virtual ~Database();
};

#endif
