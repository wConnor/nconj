#include "deck.hpp"
#include "menu.hpp"
#include "session.hpp"
#include <filesystem>

int main(int argc, char *argv[]) {
  Deck *deck = new Deck();

  /* checks to see whether or not the data directory
         exists and, if it does not, creates both it and
         the decks.ncj file with some example french
         conjugation.
  */
  if (!std::filesystem::exists("./decks/")) {
    std::filesystem::create_directory("./decks/");

    deck->create("être",
                 std::vector<std::string>{"je,tu,il/elle,nous,vous,ils/elles"},
                 std::vector<std::string>{"suis,es,est,sommes,êtes,sont"});
  }

  std::vector<std::string> availableDecks = {};
  for (const auto &e : std::filesystem::directory_iterator("./decks/")) {
    availableDecks.push_back(e.path());
  }

  deck->setFile("être");
  deck->readContents();

  initscr();
  cbreak();
  start_color();

  Menu *menu = new Menu();
  int selectedDeck;
  while (1) {
    selectedDeck = menu->print(availableDecks);
    if (selectedDeck >= 0) {
      Session *session = new Session();
      session->begin(deck->getFront(), deck->getBack(), deck->getName());
      delete session;
    } else {
      break;
    }
  }
  delete menu;
  endwin();

  return 0;
}
