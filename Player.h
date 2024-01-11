
#ifndef Player_h
#define Player_h

#include <stdio.h>
#include <string>
#include <iostream>
#include <climits>
#include "Side.h"
#include "Board.h"
#include <vector>


class Board;
class Player {
public:
    Player(std::string name);
    std::string name() const;
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const = 0;
    virtual ~Player();
private:
    std::string mName;
};

class HumanPlayer : public Player {
public:
    HumanPlayer(std::string name) : Player(name) {};
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
    ~HumanPlayer();
    
};

class BadPlayer : public Player {
public:
    BadPlayer(std::string name) : Player(name) {};
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
    ~BadPlayer();
    
};

class SmartPlayer : public Player {
public:
    SmartPlayer(std::string name) : Player(name) {};
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
    ~SmartPlayer();
private:
    void nextMove(Board& b, Board& copyBoard, Side s, int& endHole, int& bestHole, int& value, int level, bool isMax) const;
//    void nextMove(Board& b, Side s, int& bestHole, int& value, int level, bool isMax) const;
    int evaluateMove(const Board& b, Side s, int endHole) const;
};

#endif /* Player_h */
