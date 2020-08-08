#include "menu.hpp"
#include "session.hpp"
#include <filesystem>
#include <fstream>

int main(int argc, char *argv[]) {

  if (!std::filesystem::exists("./data/")) {
    std::filesystem::create_directory("./data/");
  }

  std::ifstream decksFile("./data/decks.ncj");
  std::string line;

  std::string infTemp = "";
  std::string frontTemp = "";
  std::string backTemp = "";

  std::vector<std::string> infinitives = {};
  std::vector<std::string> front = {};
  std::vector<std::string> back = {};
  std::vector<std::string> *fileContents = new std::vector<std::string>();

  // reads each line of decks.ncj into the fileContents vector.
  if (decksFile.good()) {
    while (std::getline(decksFile, line)) {
      if (line.size() > 0) {
        fileContents->push_back(line);
      }
    }
  }

  /* iterates through the data stored in fileContents,
	 extracting the contents between the starting delimiter
	 (%) and the end delimiter ($). these are then stored
	 in a temporary variable and then pushed back to
	 their respective vectors.
  */
  for (auto &c : *fileContents) {
    auto start = c.find('%');
    auto end = c.find('$');

    infTemp = c.substr(start + 1, end - (start + 1));
    c.erase(start, end + 1);
    frontTemp = c.substr(start + 1, end - (start + 1));
    c.erase(start, end + 1);
    backTemp = c.substr(start + 1, end - (start + 1));
    c.erase(start, end + 1);

    infinitives.push_back(infTemp);
    front.push_back(frontTemp);
    back.push_back(backTemp);
  }

  delete fileContents;
  decksFile.close();
  initscr();
  cbreak();
  start_color();

  Menu *menu = new Menu();
  int selectedDeck;
  while (1) {
    selectedDeck = menu->print(infinitives);
    if (selectedDeck >= 0) {
      Session *session = new Session();
      session->begin(front, back, infinitives[selectedDeck]);
      delete session;
    } else {
      break;
    }
  }
  delete menu;
  endwin();

  return 0;
}
