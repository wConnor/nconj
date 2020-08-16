#include "deck.hpp"

Deck::Deck() {}

Deck::Deck(const std::string &name) { deckName = name; }

void Deck::create(const std::string &front, const std::string &back) {
  std::ofstream file("./decks/" + deckName + ".ncj");

  file << deckName << std::endl;
  file << front << std::endl;
  file << back << std::endl;

  file.close();
}

void Deck::readContents() {
  std::fstream file("./decks/" + deckName + ".ncj");

  std::string tempName = "";
  std::string tempFront = "";
  std::string tempBack = "";

  while (file.good()) {
    std::getline(file, tempName, '\n');
    std::getline(file, tempFront, '\n');
    std::getline(file, tempBack, '\n');
  }

  file.close();

  std::stringstream frontStream(tempFront);
  std::stringstream backStream(tempBack);

  while (frontStream.good() && backStream.good()) {
    std::string frontSub, backSub;
    std::getline(frontStream, frontSub, ',');
    std::getline(backStream, backSub, ',');
    front.push_back(frontSub);
    back.push_back(backSub);
  }
}

void Deck::setName(const std::string &name) { deckName = name; }

std::string Deck::getName() { return deckName; }

std::vector<std::string> Deck::getFront() { return front; }

std::vector<std::string> Deck::getBack() { return back; }

Deck::~Deck() {}
