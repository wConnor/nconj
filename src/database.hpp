#include <sqlite3.h>
#include <fstream>
#include <ctime>
#include <chrono>
#include "deck.hpp"

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

	void set_log_file(std::shared_ptr<std::fstream> &log_file);

	virtual ~Database();
};
