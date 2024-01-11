//
//  Game.hpp
//  project3
//
//  Created by amanda  on 5/20/23.
//

#ifndef Game_h
#define Game_h

#include <stdio.h>
#include "Side.h"
#include "Board.h"
#include "Player.h"
#include <iostream>
#include <string>
//
//class Board;
//class Player;

class Game {
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;

    bool move(Side s);

    void play();

    int beans(Side s, int hole) const;

private:
//    int totalHoles;
//    int beansInEachHole;
    bool currStatus;
    Board gameBoard;
    Player* southPlayer;
    Player* northPlayer;
    bool emptySide(Side s, int holes);


};
#endif /* Game_h */
