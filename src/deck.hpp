#ifndef DECK_HPP
#define DECK_HPP

#include <sqlite3.h>

#include <filesystem>
#include <string>
#include <vector>

class Deck
{
private:
	const std::filesystem::path db_path = "decks.sql";
	std::string file_name = "";
	std::string name = "";
	std::vector<std::pair<std::string, std::string>> notes =
		{}; // { front side, back side }

public:
	Deck();
	Deck(const std::string &name);

	void set_name(const std::string &name);
	std::string get_name();

	void set_notes(std::vector<std::pair<std::string, std::string>> notes);
	std::vector<std::pair<std::string, std::string>> get_notes();

	virtual ~Deck();
};

#endif
