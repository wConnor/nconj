#include "menu.hpp"
#include "session.hpp"
#include <filesystem>
#include <fstream>

int main(int argc, char *argv[]) {

  /* checks to see whether or not the data directory
         exists and, if it does not, creates both it and
         the decks.ncj file with some example french
         conjugation.
  */
  if (!std::filesystem::exists("./data/")) {
    std::filesystem::create_directory("./data/");
    std::ofstream *file = new std::ofstream("./data/decks.ncj");
    *file << "%être$%je,tu,il/elle,nous,vous,ils/"
             "elles$%suis,es,est,sommes,êtes,sont$"
          << std::endl;
    *file
        << "%avoir$%j',tu,il/elle,nous,vous,ils/elles$%ai,as,a,avons,avez,ont$"
        << std::endl;
    file->close();
    delete file;
  }

  std::ifstream decksFile("./data/decks.ncj");
  std::string line;

  std::string infTemp = "";
  std::string frontTemp = "";
  std::string backTemp = "";

  std::vector<std::string> infinitives = {};
  std::vector<std::vector<std::string>> front;
  std::vector<std::vector<std::string>> back;
  std::vector<std::string> *fileContents = new std::vector<std::string>();

  // reads each line of decks.ncj into the fileContents vector.
  if (decksFile.good()) {
    while (std::getline(decksFile, line)) {
      if (line.size() > 0) {
        fileContents->push_back(line);
      }
    }
  }

  /* iterates through the data stored in fileContents (line
         by line), extracting the contents between the starting
         delimiter (%) and the end delimiter ($). these are then stored
         in a temporary variable and then pushed back to
         their respective vectors.
  */
  std::vector<std::string> tempVF = {};
  std::vector<std::string> tempVB = {};
  int counter = 0;
  for (auto &c : *fileContents) {
	infTemp = "";
	frontTemp = "";
	backTemp = "";

    auto start = c.find('%');
    auto end = c.find('$');

    infTemp = c.substr(start + 1, end - (start + 1));
    c.erase(start, end + 1);
    start = c.find('%');
    end = c.find('$');

    frontTemp = c.substr(start + 1, end - (start + 1));
    std::stringstream frontStream(frontTemp);

    while (frontStream.good()) {
      std::string substr;
      std::getline(frontStream, substr, ',');
	  tempVF.push_back(substr);
    }
	front.push_back(tempVF);
	tempVF.clear();

    c.erase(start, end + 1);
    start = c.find('%');
    end = c.find('$');
    backTemp = c.substr(start + 1, end - (start + 1));

    std::stringstream backStream(backTemp);

    while (backStream.good()) {
      std::string substr;
      std::getline(backStream, substr, ',');
	  tempVB.push_back(substr);
    }
	back.push_back(tempVB);
	tempVB.clear();

    backTemp = c.substr(start + 1, end - (start + 1));
    c.erase(start, end + 1);

    infinitives.push_back(infTemp);
	++counter;
	c.erase(0,c.find('$'));
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
      session->begin(front[selectedDeck], back[selectedDeck], infinitives[selectedDeck]);
      delete session;
    } else {
      break;
    }
  }
  delete menu;
  endwin();

  return 0;
}
