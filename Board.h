//
//  Board.hpp
//  project3
//
//  Created by amanda  on 5/20/23.
//

#ifndef Board_h
#define Board_h

#include <stdio.h>
#include <vector>
#include "Side.h"

struct Hole {
    int holeNum = POT;
    int beans = 0;
    Side side;
    
    Hole& operator=(const Hole& other) {
            if (this != &other) {
                holeNum = other.holeNum;
                beans = other.beans;
                side = other.side;
            }
            return *this;
        }
};

class Board {
public:
    Board(int nHoles, int nInitialBeansPerHole);
    
    Board(const Board& other);
    
    Board& operator=(const Board& other);
    
    ~Board();
    
    int holes() const;

    int beans(Side s, int hole) const;

    int beansInPlay(Side s) const;

    int totalBeans() const;
    
    bool sow(Side s, int hole, Side& endSide, int& endHole);
    
    bool moveToPot(Side s, int hole, Side potOwner);

    bool setBeans(Side s, int hole, int beans);
    
    void display() const;

private:
    std::vector<Hole*> northSide;
    std::vector<Hole*> southSide;
    int totalHoles;
};

#endif /* Board_h */
