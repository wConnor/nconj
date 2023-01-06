#include "deck.hpp"

Deck::Deck()
{
}

Deck::Deck(const std::string &deck_name)
{
	this->deck_name = deck_name;
}

void Deck::create(const std::string &front, const std::string &back)
{
	std::ofstream file("./decks/" + deck_name + ".ncj");

	file << deck_name << std::endl;
	file << front << std::endl;
	file << back << std::endl;

	file.close();
}

void Deck::read_contents()
{
	std::fstream file("./decks/" + deck_name + ".ncj");

	std::string temp_name = "", temp_front = "", temp_back = "";

	while (file.good())
	{
		std::getline(file, temp_name, '\n');
		std::getline(file, temp_front, '\n');
		std::getline(file, temp_back, '\n');
	}

	file.close();

	std::stringstream front_stream(temp_front);
	std::stringstream back_stream(temp_back);

	while (front_stream.good() && back_stream.good())
	{
		std::string frontSub, backSub;
		std::getline(front_stream, frontSub, ',');
		std::getline(back_stream, backSub, ',');
		deck.push_back({frontSub, backSub});
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
