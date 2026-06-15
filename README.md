# Battleship.cpp 🚢

A console-based Battleship game built in C++ as a programming fundamentals 
project. Supports Player vs Player and Player vs Computer modes, with 
color-coded boards and a persistent leaderboard.

## Features
-  Player vs Player (pass-and-play) and Player vs Computer modes
-  Color-coded ships and hit/miss indicators
-  Persistent leaderboard saved between sessions
-  Manual or random ship placement
-  Multi-attack turns based on remaining ships

## How to Play
- Each player places 5 ships on a 10x10 grid
- Take turns attacking your opponent's grid
- Hit = +10 points | Miss = -1 point
- Red X = Hit | Pink X = Sunk | Grey O = Miss
- First to sink all enemy ships wins!

## Ships
| Ship             | Size |
|------------------|------|
| Aircraft Carrier | 5    |
| Battleship       | 4    |
| Destroyer        | 3    |
| Submarine        | 3    |
| Patrol Boat      | 2    |

## Built With
- C++ (console, Windows)
- Windows.h for colors
- File I/O for leaderboard persistence

## Authors
- [meowryam](https://github.com/meowryam)
- [Rayyan](#) ← 
