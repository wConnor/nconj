#include "deck.hpp"

Deck::Deck()
{
}

Deck::Deck(const std::string &name)
{
	this->name = name;
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
