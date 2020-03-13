# SFML based video game

Scholar project done during my last year at ENSEA's Graduate school in IS specialization
The game has :
  * its own game engne
  * states
  * network mode

The project uses:
  * C++ 
  * dia2code (to building code from a UML diagram)
  * SFML

The game is :
  * multiplayer in network
  * indivudual player against IA
    * 3 different AI are implemented: random, heuristic, deepAI (using graph traversal of our states to find the best move)
  * random map generation

Building:
For windows 64bits you need to :
- install MinGW and use Msys make (add msys to the path)
- install mingw64 (add it to the path)

To build with windows use make (msys make).

http://localhost:8080/version
