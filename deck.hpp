#ifndef DECK_HPP
#define DECK_HPP

#include <algorithm>
#include <chrono>
#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

class Deck {
private:
  std::string fileName = "";
  std::string deckName;
  std::vector<std::string> front = {};
  std::vector<std::string> back = {};

public:
  Deck(const std::string &name);
  void create(const std::string &front,
              const std::string &back);
  void readContents();
  std::string getName();
  std::vector<std::string> getFront();
  std::vector<std::string> getBack();
  virtual ~Deck();
};

#endif
