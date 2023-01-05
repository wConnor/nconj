#include "deck.hpp"

Deck::Deck()
{
}

Deck::Deck(const std::string &name)
{
	deck_name = name;
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

	std::string tempName = "";
	std::string tempFront = "";
	std::string tempBack = "";

	while (file.good())
	{
		std::getline(file, tempName, '\n');
		std::getline(file, tempFront, '\n');
		std::getline(file, tempBack, '\n');
	}

	file.close();

	std::stringstream frontStream(tempFront);
	std::stringstream backStream(tempBack);

	while (frontStream.good() && backStream.good())
	{
		std::string frontSub, backSub;
		std::getline(frontStream, frontSub, ',');
		std::getline(backStream, backSub, ',');
		front.push_back(frontSub);
		back.push_back(backSub);
	}
}

void Deck::set_name(const std::string &name)
{
	deck_name = name;
}

std::string Deck::get_name()
{
	return deck_name;
}

std::vector<std::string> Deck::get_front()
{
	return front;
}

std::vector<std::string> Deck::get_back()
{
	return back;
}

Deck::~Deck()
{
}
