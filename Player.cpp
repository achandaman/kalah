
#include "Player.h"


// Player implementations
Player::Player(std::string name) {
    mName = name;
}

std::string Player::name() const {
    return mName;
}

bool Player::isInteractive() const {
    return false;
}

int Player::chooseMove(const Board&b, Side s) const {
    return -1;
};

Player::~Player() {};

// HumanPlayer implementations
bool HumanPlayer::isInteractive() const {
    return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const{
    int hole = -1;
    bool validInput = false;
    while (validInput == false) {
        std::cout << "Select a hole, " << this->name() << ": ";
        std::cin >> hole;
        if (hole > b.holes() || hole < 1) { //invalid hole num
            std::cout << "The hole number must be from 1 to " << b.holes() << ". \n";
        }
        else if (b.beans(s, hole) <= 0) { //hole with no beans
            std::cout << "There are no beans in that hole." << std:: endl;
        }
        else {
            validInput = true;
            return hole;
        }
    }
    return hole;
}

HumanPlayer::~HumanPlayer() {
    ;
}

//BadPlayer implementations
bool BadPlayer::isInteractive() const {
    return false;
}

int BadPlayer::chooseMove(const Board& b, Side s) const{
    //choose hole nearest pot that still has beans
    if (s == NORTH) {
        if (b.beans(s, 1) > 0) {
            return 1;
        }
        for (int i = 2; i <= b.holes(); i++) {
            if (b.beans(s, i) > 0) {
                return i;
            }
        }
    }
    else if (s == SOUTH) {
        if (b.beans(s, b.holes()) > 0) {
            return b.holes();
        }
        for (int i = b.holes() - 1; i >= 1; i--) {
            if (b.beans(s, i) > 0) {
                return i;
            }
        }
    }
    return -1;
}

BadPlayer::~BadPlayer() {};

//SmartPlayer implementations
bool SmartPlayer::isInteractive() const {
    return false;
}

int SmartPlayer::evaluateMove(const Board& b, Side s, int endHole) const{ //evaluates how good a move will be, based on the board after the move is taken
    Side opponent = (s == NORTH) ? SOUTH : NORTH;
    int totalHoles = b.holes();
    int emptyHoles = 0;
    int value = 0;
    if (b.beansInPlay(opponent) == 0) { //opponent loses -> high value
        return INT_MAX;
    }
    if (b.beansInPlay(s) == 0) { //you lose -> low value
        return INT_MIN;
    }
    int diff = b.beans(s, 0) - b.beans(opponent, 0);
    value += diff * 4; //2 points awarded for each bean -> if you have more beans, move is better, else the move is considered worse

    for (int i = 1; i <= totalHoles; i++) {
        if (b.beans(s, i) == 0) {
            emptyHoles += 1;
        }
    }
    if (emptyHoles <= (totalHoles/2)) { //if less than half the total holes are empty, good
        value += emptyHoles * 3; //3 points for each empty hole -> more opportunity for captures
    }
    else {
        value += emptyHoles * -3; //too many holes empty is bad, -3 points for each empty hole
    }
    if (s == NORTH) {
        if (b.beans(s, 1) == 1) {
            value += 1; //easy point if one stone in hole closest to pot
        }
    }
    else {
        if (b.beans(s, totalHoles) == 1) {
            value += 1;
        }
    }
    int middle = totalHoles / 2;
    middle = (totalHoles % 2 != 0) ? middle + 1: middle;
    if (b.beans(s, middle) == middle) {
        value += 5; //correct amount of beans for last one to end in pot
    }
    else if (b.beans(s, middle) == middle + 1) { //if there are more beans in original middle,
        value += 5;
    }
    
    if (b.beans(s, endHole) == 1 && b.beans(opponent, endHole) > 0) { //capture is value
        value += 5;
    }
    return value;
}

void SmartPlayer::nextMove(Board& b, Board& copyBoard, Side s, int& endHole, int& bestHole, int& value, int level, bool isMax) const {
    int totalHoles = copyBoard.holes();
    Side opponent = (s == NORTH) ? SOUTH : NORTH;
    if (level == 5) { //terminating condition
        bestHole = -1;
        value = evaluateMove(copyBoard, s, endHole);
        return;
    }

    if (b.beansInPlay(NORTH) == 0 || b.beansInPlay(SOUTH) == 0) { //if game is over
        bestHole = -1;
        value = evaluateMove(copyBoard, s, endHole);
        return;
    }
    std::vector<int> possibleHoles; //determines which holes to explore
    for (int i = 1; i <= totalHoles; i++) {
        if (copyBoard.beans(s, i) > 0) {
            possibleHoles.push_back(i);
        }
    }

    if (isMax) { //maximizer called when player's turn
        value = INT_MIN;
        for (int i = 0; i < possibleHoles.size(); i++) {
            int currHole = possibleHoles[i];
            Board tempCopy(copyBoard); //all tested moves occur on tempCopy/copyBoard
            Side endSide;
            int tempBest = currHole; //stores currHole for comparison
            tempCopy.sow(s, currHole, endSide, endHole); // move "taken" on tempCopy
            if (endSide == s && endHole == 0) { //capture occurs
                nextMove(b, tempCopy, s, endHole, tempBest, value, level, true); //recursive call to "take" another move, recursive depth stays the same
                tempCopy = copyBoard;
                if (bestHole == -1 || tempBest == -1 || b.beans(s, tempBest) > 0  || copyBoard.beans(s, tempBest) > 0 ) { //bestHole updated if valid move on original board b, or the recursive depth was just reached
                    bestHole = currHole;
                }
            }
            else { //in all other cases, recursive call made to see possible moves taken by opponent
                int value2;
                nextMove(b, tempCopy, opponent, endHole, tempBest, value2, level+1, false);
                tempCopy = copyBoard;
                if (value2 > value) { //value2 greater if next moves are bad for opponent, updates value and best hole
                    value = value2;
                    if (bestHole == -1 || tempBest == -1 || b.beans(s, tempBest) > 0 || copyBoard.beans(s, tempBest) > 0) { //ensures bestHole is valid move on original board
                        bestHole = currHole;
                    }
                }
            }

        }
    }
    else { //minimizer called testing opponent's moves
        value = INT_MAX;
        for (int i = 0; i < possibleHoles.size(); i++) {
            int currHole = possibleHoles[i];
            Board tempCopy(copyBoard);
            Side endSide;
            int tempBest = currHole;
            tempCopy.sow(s, currHole, endSide, endHole);
            if (endSide == s && endHole == 0) {
                nextMove(b, tempCopy, s, endHole, tempBest, value, level, false);
                tempCopy = copyBoard;
                if (bestHole == -1 || tempBest == -1 || b.beans(s, tempBest) > 0 || copyBoard.beans(s, tempBest) > 0) {
                    bestHole = currHole;
                }
            }
            else {
                int value2;
                nextMove(b, tempCopy, opponent, endHole, tempBest, value2, level+1, true);
                tempCopy = copyBoard;
                if (value2 < value) {
                    value = value2;
                    if (bestHole == -1 || tempBest == -1 || b.beans(s, tempBest) > 0 || copyBoard.beans(s, tempBest) > 0) {
                        bestHole = currHole;
                    }

                }
            }
        }
    }
}


int SmartPlayer::chooseMove(const Board& b, Side s) const {
//    int endHole;
//    int bestHole;
//    int value;
//    if (b.beansInPlay(s) == 0) { //no valid moves
//        return -1;
//    }
//    Board copy(b);
//    nextMove(copy, copy, s, endHole, bestHole, value, 0, true); //calls nextMove to properly track recursive depth with added parameters
    if (s == NORTH) {
        if (b.beans(s, 1) > 0) {
            return 1;
        }
        for (int i = 2; i <= b.holes(); i++) {
            if (b.beans(s, i) > 0) {
                return i;
            }
        }
    }
    else if (s == SOUTH) {
        if (b.beans(s, b.holes()) > 0) {
            return b.holes();
        }
        for (int i = b.holes() - 1; i >= 1; i--) {
            if (b.beans(s, i) > 0) {
                return i;
            }
        }
    }
    return -1;
}

SmartPlayer::~SmartPlayer() {
    ;
}






