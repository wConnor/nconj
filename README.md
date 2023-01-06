# nconj
## Introduction
nconj is a terminal-based Linux program used to strengthen short-term memory of any facts that the user wishes to. It acts similar to flashcard applications like Anki - there are front side and back sides of cards, and once shown the front side, the user must correctly recall the back side by typing in the answer.

## Compilation
nconj requires a C++ compiler with support for C++17, ncurses development libraries, and CMake. Once the dependencies have been acquired, the program can be compiled as follows:
```
$ mkdir build && cd build
$ cmake ..
$ make -j$(nproc)
```

This should result in the `nconj` binary inside of the `./build/src/` directory.

## Usage
Once compiled, the software is simply ran using `./build/src/nconj` from the repository's root directory. The user should then be prompted with the terminal interface.

## Screenshots
![Main Screen](/images/main-screen.png)

Main home screen that the user is returned to between sessions.

![Deck Creation](/images/creation.png)

Screen where users create their decks with their desired cards.

![Session Screen](/images/session-screen.png)

Session screen where the user is prompted to input their response to seeing the front side of the card.

![Completion Screen](/images/complete.png)

Completion screen for when the user has finished reviewing the deck's cards.

