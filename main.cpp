#include "deck.hpp"
#include "menu.hpp"
#include "session.hpp"
#include <filesystem>

int main(int argc, char *argv[]) {
  /* checks to see whether or not the data directory
         exists and, if it does not, creates both it and
         the decks.ncj file with some example french
         conjugation.
  */
  if (!std::filesystem::exists("./decks/")) {
    std::filesystem::create_directory("./decks/");
    std::unique_ptr<Deck> exDeck1 = std::make_unique<Deck>("être");
    std::unique_ptr<Deck> exDeck2 = std::make_unique<Deck>("connaître");

    exDeck1->create("je,tu,il/elle,nous,vous,ils/elles",
                    "suis,es,est,sommes,êtes,sont");
    exDeck2->create(
        "je,tu,il/elle,nous,vous,ils/elles",
        "connais,connais,connaît,connaissons,connaissez,connaissent");
  }

  std::vector<std::string> availableDecks = {};

  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  start_color();

  Menu *menu = new Menu();
  int selectedDeck;
  while (1) {
    availableDecks.clear();
    cbreak();
    for (const auto &e : std::filesystem::directory_iterator("./decks/")) {
      std::string tempName = "";
      std::filesystem::path tempPath = e.path();
      tempName = tempPath.string();

      auto start = tempName.find_last_of('/');
      tempName.erase(0, start + 1);

      auto end = tempName.find_last_of('.');
      tempName.erase(end, tempName.size() - 1);

      availableDecks.push_back(tempName);
    }

    selectedDeck = menu->print(availableDecks);
    // create new deck
    if (selectedDeck == -1) {
      std::unique_ptr<Deck> tempDeck = std::make_unique<Deck>();
      menu->newDeck(tempDeck);
      // quit program
    } else if (selectedDeck == -2) {
      break;
      // start session with chosen deck
    } else if (selectedDeck >= 0) {
      std::unique_ptr<Deck> deck =
          std::make_unique<Deck>(availableDecks[selectedDeck]);
      std::unique_ptr<Session> session = std::make_unique<Session>();
      deck->readContents();
      session->begin(deck->getFront(), deck->getBack(), deck->getName());
    }
  }
  delete menu;
  endwin();

  return 0;
}
