#include "deck.hpp"

Deck::Deck() {}

void Deck::create(const std::string &name,
                  const std::vector<std::string> &userFront,
                  const std::vector<std::string> &userBack) {
  fileName = "./decks/" + name + ".ncj";

  std::ofstream file(fileName);
  file << name;

  file << std::endl;

  for (const auto &e : userFront)
    file << e;

  file << std::endl;

  for (const auto &v : userBack)
    file << v;

  file << std::endl;

  file.close();
}

void Deck::setFile(const std::string &name) {
  deckName = name;
  fileName = "./decks/" + deckName + ".ncj";
}

void Deck::readContents() {
  std::fstream file(fileName);

  std::string tempName = "";
  std::string tempFront = "";
  std::string tempBack = "";

  while (file.good()) {
    std::getline(file, tempName, '\n');
    std::getline(file, tempFront, '\n');
    std::getline(file, tempBack, '\n');
  }

  std::stringstream frontStream(tempFront);
  std::stringstream backStream(tempBack);

  while (frontStream.good() && backStream.good()) {
    std::string frontSub, backSub;
    std::getline(frontStream, frontSub, ',');
    std::getline(backStream, backSub, ',');
    front.push_back(frontSub);
    back.push_back(backSub);
  }
  unsigned int randSeed =
      std::chrono::system_clock::now().time_since_epoch().count();

  std::shuffle(front.begin(), front.end(),
               std::default_random_engine(randSeed));
  std::shuffle(back.begin(), back.end(), std::default_random_engine(randSeed));
}

std::string Deck::getName() { return deckName; }

std::vector<std::string> Deck::getFront() { return front; }

std::vector<std::string> Deck::getBack() { return back; }

Deck::~Deck() {}
