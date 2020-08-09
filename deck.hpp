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
  Deck();
  void create(const std::string &name,
              const std::vector<std::string> &userFront,
              const std::vector<std::string> &userBack);
  void setFile(const std::string &name);
  void readContents();
  std::string getName();
  std::vector<std::string> getFront();
  std::vector<std::string> getBack();
  virtual ~Deck();
};

#endif
