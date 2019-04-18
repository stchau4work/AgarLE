# AgarAI

Reinforcement Learning environment for Agar.io

# todo
- Get basic game mechanics
  - target in game world
  - max speed as a function of mass
  - Handle player keyboard inputs (split/feed)
  - Entity interactions (several of these)
- Refactoring
  - restructure class structure to avoid Diamond problem (worth it?)
  - better solution to NumberWrapper (is there one?)
- Client-Server connection
  - GameState serialization
  - Write server I/O
  - Write client I/O
- Bind engine to AgarLE with PyBind
  - Write C++ environment
  - Get images without rendering them
  - Write basic opponent algorithms
  - Write non-visual (i.e. RAM) state space

