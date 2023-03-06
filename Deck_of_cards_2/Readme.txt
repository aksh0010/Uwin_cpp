Follow the below CMake Steps. Also Cmake file has been added.


    1) $ mkdir build
    2)$ cd build
    3)$ cmake ..
[ 16%] Building CXX object CMakeFiles/a5-gen-input.dir/a5-gen-input-provided.cpp.o
[ 33%] Building CXX object CMakeFiles/a5-gen-input.dir/a4-provided.cpp.o
[ 50%] Linking CXX executable a5-gen-input
[ 50%] Built target a5-gen-input
[ 66%] Building CXX object CMakeFiles/a5.dir/a5.cpp.o
[ 83%] Building CXX object CMakeFiles/a5.dir/a4-provided.cpp.o
[100%] Linking CXX executable a5
[100%] Built target a5
    4)$ make
[ 16%] Building CXX object CMakeFiles/a5-gen-input.dir/a5-gen-input-provided.cpp.o
[ 33%] Building CXX object CMakeFiles/a5-gen-input.dir/a4-provided.cpp.o
[ 50%] Linking CXX executable a5-gen-input
[ 50%] Built target a5-gen-input
[ 66%] Building CXX object CMakeFiles/a5.dir/a5.cpp.o
[ 83%] Building CXX object CMakeFiles/a5.dir/a4-provided.cpp.o
[100%] Linking CXX executable a5
[100%] Built target a5
  5)$ ./a5-gen-input input
DEBUG: Company "Bicycle Cards" will have 10 decks
DEBUG: Company "KEM" will have 10 decks
DEBUG: Company "Hoyle" will have 7 decks
DEBUG: Company "Bee Playing Cards" will have 7 decks
DEBUG: Company "Tally-Ho Playing Cards" will have 4 decks
DEBUG: Company "Aviator" will have 4 decks
DEBUG: Company "Copag" will have 10 decks
DEBUG: Company "Modiano" will have 1 decks

    6)$ ./a5 input
Total number of cards read in: 3036

Total number of companies: 8
  "Aviator"
  "Bee Playing Cards"
  "Bicycle Cards"
  "Copag"
  "Hoyle"
  "KEM"
  "Modiano"
  "Tally-Ho Playing Cards"

"Aviator" card stats:
  Total number of cards: 226
  Total number of decks needed: 4
  Deck 1 is not missing any cards.
  Deck 2 is not missing any cards.
  Deck 3 is not missing any cards.
  Deck 4 is missing: 2c6c7d10dQhR

"Bee Playing Cards" card stats:
  Total number of cards: 403
  Total number of decks needed: 7
  Deck 1 is not missing any cards.
  Deck 2 is not missing any cards.
  Deck 3 is not missing any cards.
  Deck 4 is not missing any cards.
  Deck 5 is not missing any cards.
  Deck 6 is not missing any cards.
  Deck 7 is missing: 5sKdW

"Bicycle Cards" card stats:
  Total number of cards: 575
  Total number of decks needed: 10
  Deck 1 is not missing any cards.
  Deck 2 is not missing any cards.
  Deck 3 is not missing any cards.
  Deck 4 is not missing any cards.
  Deck 5 is not missing any cards.
  Deck 6 is not missing any cards.
  Deck 7 is not missing any cards.
  Deck 8 is not missing any cards.
  Deck 9 is missing: Js
  Deck 10 is missing: JsJhJdQh

"Copag" card stats:
  Total number of cards: 576
  Total number of decks needed: 10
  Deck 1 is not missing any cards.
  Deck 2 is not missing any cards.
  Deck 3 is not missing any cards.
  Deck 4 is not missing any cards.
  Deck 5 is not missing any cards.
  Deck 6 is not missing any cards.
  Deck 7 is not missing any cards.
  Deck 8 is not missing any cards.
  Deck 9 is not missing any cards.
  Deck 10 is missing: Ac4s5dKs

"Hoyle" card stats:
  Total number of cards: 400
  Total number of decks needed: 7
  Deck 1 is not missing any cards.
  Deck 2 is not missing any cards.
  Deck 3 is not missing any cards.
  Deck 4 is not missing any cards.
  Deck 5 is not missing any cards.
  Deck 6 is missing: Ks
  Deck 7 is missing: AcAs5sKcKs

"KEM" card stats:
  Total number of cards: 570
  Total number of decks needed: 10
  Deck 1 is not missing any cards.
  Deck 2 is not missing any cards.
  Deck 3 is not missing any cards.
  Deck 4 is not missing any cards.
  Deck 5 is not missing any cards.
  Deck 6 is not missing any cards.
  Deck 7 is not missing any cards.
  Deck 8 is not missing any cards.
  Deck 9 is not missing any cards.
  Deck 10 is missing: 2c2d3h5s6c6d8sQhCcCd

"Modiano" card stats:
  Total number of cards: 57
  Total number of decks needed: 1
  Deck 1 is missing: 8c

"Tally-Ho Playing Cards" card stats:
  Total number of cards: 229
  Total number of decks needed: 4
  Deck 1 is not missing any cards.
  Deck 2 is not missing any cards.
  Deck 3 is not missing any cards.
  Deck 4 is missing: Ad7sKd
