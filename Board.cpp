//
//  Board.cpp
//  project3
//
//  Created by amanda  on 5/20/23.
//

#include "Board.h"
#include <iostream>

Board::Board(int nHoles, int nInitialBeansPerHole) {
    totalHoles = (nHoles > 0) ? nHoles : 1;
    
    nInitialBeansPerHole = (nInitialBeansPerHole > 0) ? nInitialBeansPerHole : 0;
   
    Hole* northPot = new Hole;
    northPot->side = NORTH;
    northSide.push_back(northPot);

    
    Hole* southPot = new Hole;
    southPot->side = SOUTH;
    southSide.push_back(southPot);
    
    for (int iN = 1; iN <= totalHoles; iN++) {
        Hole* inserted = new Hole;
        inserted->holeNum = iN;
        inserted->beans = nInitialBeansPerHole;
        inserted->side = NORTH;
        northSide.push_back(inserted);
        
    }
    for (int iS = totalHoles; iS > 0; iS--) {
        Hole* inserted = new Hole;
        inserted->holeNum = iS;
        inserted->beans = nInitialBeansPerHole;
        inserted->side = SOUTH;
        southSide.push_back(inserted);
    }
}

Board::Board(const Board& other) {
    totalHoles = other.holes();
    Hole* northPot = new Hole;
    northPot->side = NORTH;
    northSide.push_back(northPot);

    
    Hole* southPot = new Hole;
    southPot->side = SOUTH;
    southSide.push_back(southPot);
    
    for (int iN = 1; iN <= totalHoles; iN++) {
        int beansN = other.beans(NORTH, iN);
        Hole* inserted = new Hole;
        inserted->holeNum = iN;
        inserted->beans = beansN;
        inserted->side = NORTH;
        northSide.push_back(inserted);
    }
    for (int iS = totalHoles; iS > 0; iS--) {
        int beansS = other.beans(SOUTH, iS);
        Hole* inserted = new Hole;
        inserted->holeNum = iS;
        inserted->beans = beansS;
        inserted->side = SOUTH;
        southSide.push_back(inserted);
    }
}

Board& Board::operator=(const Board& other) {
    if (this != &other) {
        totalHoles = other.holes();
        for (std::vector<Hole*>::iterator pN = northSide.begin(); pN != northSide.end(); pN++) {
            delete *pN;
        }
        for (std::vector<Hole*>::iterator pS = southSide.begin(); pS != southSide.end(); pS++) {
            delete *pS;
        }
        northSide.clear();
        southSide.clear();
        
        Hole* northPot = new Hole;
        northPot->side = NORTH;
        northPot->beans = other.beans(NORTH, 0);
        northSide.push_back(northPot);

        
        Hole* southPot = new Hole;
        southPot->side = SOUTH;
        southPot->beans = other.beans(SOUTH, 0);
        southSide.push_back(southPot);
        
        for (int iN = 1; iN <= totalHoles; iN++) {
            int beansN = other.beans(NORTH, iN);
//            std::cout << beansN << std::endl;
            Hole* inserted = new Hole;
            inserted->holeNum = iN;
            inserted->beans = beansN;
            inserted->side = NORTH;
            northSide.push_back(inserted);
        }
        for (int iS = totalHoles; iS > 0; iS--) {
            int beansS = other.beans(SOUTH, iS);
//            std::cout << beansS << std::endl;
            Hole* inserted = new Hole;
            inserted->holeNum = iS;
            inserted->beans = beansS;
            inserted->side = SOUTH;
            southSide.push_back(inserted);
        }
    }
//    display();
    return *this;
}

Board::~Board() {
    // Deallocate the dynamically allocated Hole objects

    for (std::vector<Hole*>::iterator pN = northSide.begin(); pN != northSide.end(); pN++) {
        delete *pN;
    }
    for (std::vector<Hole*>::iterator pS = southSide.begin(); pS != southSide.end(); pS++) {
        delete *pS;
    }
    northSide.clear();
    southSide.clear();
}
int Board::holes() const {
    return totalHoles;
}

int Board::beans(Side s, int hole) const {
    if (hole == POT) {
        Hole* pot = (s == NORTH) ? northSide[0] : southSide[0];
        return pot->beans;
    }
    if (s == NORTH && hole <= totalHoles) {
        return northSide[hole]->beans;
    }
    else if (s == SOUTH && hole <= totalHoles) {
        int southHole = (totalHoles - hole) + 1;
        Hole* targetHole = southSide[southHole];
        return targetHole->beans;
    }
    return -1;
}

int Board::beansInPlay(Side s) const {
    //    Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.
    int total = 0;
    if (s == NORTH) {
        for (std::vector<Hole*>::const_iterator pN = northSide.begin()+1; pN != northSide.end(); pN++) {
            total += (*pN)->beans;
        }
    }
    if (s == SOUTH) {
        for (std::vector<Hole*>::const_iterator pS = southSide.begin()+1; pS != southSide.end(); pS++) {
            total += (*pS)->beans;
        }
    }
    return total;
}

int Board::totalBeans() const {
    int total = 0;
    for (std::vector<Hole*>::const_iterator pN = northSide.begin(); pN != northSide.end(); pN++) {
        total += (*pN)->beans;
    }
    for (std::vector<Hole*>::const_iterator pS = southSide.begin(); pS != southSide.end(); pS++) {
        total += (*pS)->beans;
    }
    //    Return the total number of beans in the game, including any in the pots.

    return total;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole) {
    if (hole == 0 || hole > totalHoles) {
        return false;
    }
    int southHole = (totalHoles - hole) + 1; //convert hole # to correct index for southSide
    
    Hole* targetHole = (s == NORTH) ? northSide[hole] : southSide[southHole];
    if (targetHole->beans == 0) {
        return false;
    }
    int removedBeans = targetHole->beans;
    targetHole->beans = 0;
    
    int iN, iS;
    
    Hole* nextHole;
    if (s == NORTH) {
        iN = hole - 1; //start at hole after target
        iS = totalHoles; //start at hole 1, closest to north pot
    }
    else {
        iN = totalHoles; //start at last hole, closest to south pot
        iS = southHole - 1; //start at hole after target
    }
    while (removedBeans > 0) {
        if (s == NORTH) {
            if (iN >= 0) { //while on north side
                nextHole = northSide[iN];
                iN--;
            }
            else { //switch to south side
                if (iS > 0) { //skip south pot
                    nextHole = southSide[iS];
                    iS--;
                }
                else { //continue on north side
                    nextHole = northSide[totalHoles];
                    iN = totalHoles;
                }
            }
            nextHole->beans += 1;
            removedBeans--;
        }
        else {
            if (iS >= 0) { //on south side
                nextHole = southSide[iS];
                iS--;
            }
            else { //on northside
                if (iN > 0) { //before reaching north Pot
                    nextHole = northSide[iN];
                    iN--;
                }
                else { //skip north pot, continue on south side
                    iS = totalHoles;
                    nextHole = southSide[iS];
                    iS--;
                }
            }
            nextHole->beans += 1;
            removedBeans--;
        }
    }
    endSide = nextHole->side;
    endHole = nextHole->holeNum;
    return true;
        
        
    
        //    If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything. Otherwise, it will return true after sowing the beans: the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. The function sets the parameters endSide and endHole to the side and hole where the last bean was placed. (This function does not make captures or multiple turns; different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.)
}

bool Board::moveToPot(Side s, int hole, Side potOwner) {
    //    If the indicated hole is invalid or a pot, return false without changing anything. Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true.
    if (hole == 0 || hole > totalHoles) {
        return false;
    }
    int southHole = (totalHoles - hole) + 1;
    Hole* targetHole = (s == NORTH) ? northSide[hole] : southSide[southHole];
    Hole* pot = (potOwner == NORTH) ? northSide[0] : southSide[0];
    int currBeans = targetHole->beans;
    targetHole->beans = 0;
    pot->beans += currBeans;
    return true;
}

bool Board::setBeans(Side s, int hole, int beans) {
    if (hole > totalHoles || beans < 0) {
        return false;
    }
    int southHole = (totalHoles - hole) + 1;
    Hole* targetHole = (s == NORTH) ? northSide[hole] : southSide[southHole];
    targetHole->beans = beans;
    //    If the indicated hole is invalid or beans is negative, this function returns false without changing anything. Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. (This could change what beansInPlay and totalBeans return if they are called later.) This function exists solely so that we and you can more easily test your program: None of your code that implements the member functions of any class is allowed to call this function directly or indirectly. (We'll show an example of its use below.)
    return true;
    
}

void Board::display() const{

    int maxLength = (totalHoles*3) + 2;
    int innerLength = maxLength - 3;
    
    for (int i = 1; i <= totalHoles; i++) {                                    // NORTH side
        std::cout << "  " << beans(NORTH, i) ;
    }
    
    std::cout << std::endl << beans(NORTH, 0);                 // NORTH pot
    
    for (int i = 0; i <= innerLength; i++) {
        std::cout << " ";
    }
    
    std::cout << beans(SOUTH, 0) << std::endl;                // SOUTH pot
    
    for (int i = 1; i <= totalHoles; i++) {                                   // SOUTH side
        std::cout << "  " << beans(SOUTH, i);
    }
    
}
