//
//  Game.cpp
//  project3
//
//  Created by amanda  on 5/20/23.
//

#include "Game.h"

Game::Game(const Board& b, Player* south, Player* north) : gameBoard(b), southPlayer(south), northPlayer(north), currStatus(true) {};

void Game::display() const {
    int holes = gameBoard.holes();

    int maxLength = (holes*3) + 2;
    int innerLength = maxLength - 3;
    int center = maxLength / 2;

    std::string northName = northPlayer->name();
    std::string southName = southPlayer->name();

    std::cout << std::string(center, ' ') << northName << std::endl; // NORTH player
    
    for (int i = 1; i <= holes; i++) {                                    // NORTH side
        std::cout << "  " << gameBoard.beans(NORTH, i) ;
    }
    
    std::cout << std::endl << gameBoard.beans(NORTH, 0);                 // NORTH pot
    
    for (int i = 0; i <= innerLength; i++) {
        std::cout << " ";
    }
    
    std::cout << gameBoard.beans(SOUTH, 0) << std::endl;                // SOUTH pot
    
    for (int i = 1; i <= holes; i++) {                                   // SOUTH side
        std::cout << "  " << gameBoard.beans(SOUTH, i);
    }
    
    std::cout << std::endl << std::string(center, ' ') << southName << std::endl; // SOUTH player
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const {
    if (currStatus == false) {
        over = true;
        if (gameBoard.beans(NORTH, 0) == gameBoard.beans(SOUTH, 0)) {
            hasWinner = false;
        }
        else {
            winner = (gameBoard.beans(NORTH, 0) > gameBoard.beans(SOUTH, 0)) ? NORTH : SOUTH;
            hasWinner = true;
        }
    }
    else {
        over = false;
    }
    //    If the game is over (i.e., the move member function has been called and returned false), set over to true; otherwise, set over to false and do not change anything else. If the game is over, set hasWinner to true if the game has a winner, or false if it resulted in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side.
}
bool Game::emptySide(Side s, int holes) {
    std::string southName = southPlayer->name();
    std::string northName = northPlayer->name();
    if (gameBoard.beansInPlay(s) == 0) {
        display();
        if (s == SOUTH) {
            std::cout << southName << " has no beans left to sow." << std::endl << "Sweeping remaining beans into " << northName << "'s pot." << std::endl;
            for (int i = holes; i > 0; i--) {
                gameBoard.moveToPot(SOUTH, i, SOUTH);
            }
            return true;
        }
        else {
            std::cout << northName << " has no beans left to sow." << std::endl << "Sweeping remaining beans into " << southName << "'s pot." << std::endl;
            for (int i = holes; i > 0; i--) {
                gameBoard.moveToPot(SOUTH, i, SOUTH);
            }
            return true;
        }
    }
    return false;
}
bool Game::move(Side s) {
    int holes = gameBoard.holes(); // # of holes on each side
    std::string southName = southPlayer->name();
    std::string northName = northPlayer->name();
    Side endSide;
    int endHole;
    bool shouldDisplay = true;
    if (s == NORTH) {
        int chosenHole = northPlayer->chooseMove(gameBoard, NORTH);
        if (chosenHole == -1) {
            currStatus = false;
            return false;
        }
        std::cout << northName << " chooses hole " << chosenHole << std::endl;
        gameBoard.sow(NORTH, chosenHole, endSide, endHole);
        if (endSide == NORTH && endHole == 0) { // ends in player's pot -> another turn
            if (emptySide(s, holes) == true) {
                currStatus = false;
                display();
                return false;
            }
            if (shouldDisplay) {
                display();
                std::cout << northName << " gets another turn." << std::endl;
            }
            shouldDisplay = false;
            bool anotherTurn = move(s);
            if (!anotherTurn) {
                return false;
            }
//            else {
//                return true;
//            }
//            return false;
        }
        if (endSide == NORTH && gameBoard.beans(NORTH, endHole) == 1 && gameBoard.beans(SOUTH, endHole) > 0) { // capture
            gameBoard.moveToPot(NORTH, endHole, NORTH); // last bean put into pot
            gameBoard.moveToPot(SOUTH, endHole, NORTH); // opponent's bean put into pot
            if (emptySide(s, holes) == true) {
                currStatus = false;
                display();
                return false;
            }
            if (shouldDisplay) {
                display();
                shouldDisplay = false;
            }
            //            return true;
        }
        if (emptySide(s, holes) == true) {
            currStatus = false;
            display();
            return false;
        }
        if (gameBoard.beansInPlay(s) == 0 || gameBoard.beansInPlay(SOUTH) == 0) {
            currStatus = false;
            return false;
        }
        if (shouldDisplay) {
            display();
            return true;
        }
        return true; // no capture, additional turns -> move still completed
    }
    
    else {
        int chosenHole = southPlayer->chooseMove(gameBoard, SOUTH);
        if (chosenHole == -1) {
            currStatus = false;
            return false;
        }
        std::cout << southName << " chooses hole " << chosenHole << std::endl;
        gameBoard.sow(SOUTH, chosenHole, endSide, endHole);
//        int oppositeHoleS = (holes + 1) - endHole;
        if (endSide == SOUTH && endHole == 0) { // ends in player's pot
            if (emptySide(s, holes) == true) {
                currStatus = false;
                display();
                return false;
            }
            if (shouldDisplay) {
                display();
                std::cout << southName << " gets another turn." << std::endl;
            }
            shouldDisplay = false;
            bool anotherTurn = move(s);
            if (!anotherTurn) {
                return false;
            }
//            return false;
        }
        if (endSide == SOUTH && gameBoard.beans(SOUTH, endHole) == 1 && gameBoard.beans(NORTH, endHole) > 0) { //capture
            gameBoard.moveToPot(SOUTH, endHole, SOUTH); //last bean
            gameBoard.moveToPot(NORTH, endHole, SOUTH); // opponent's beans
            if (emptySide(s, holes) == true) {
                currStatus = false;
                display();
                return false;
            }
            if (shouldDisplay) {
                display();
                shouldDisplay = false;
            }
//            return true;
        }
        if (emptySide(s, holes) == true) {
            currStatus = false;
            display();
            return false;
        }
        if (gameBoard.beansInPlay(s) == 0 || gameBoard.beansInPlay(NORTH) == 0) {
            currStatus = false;
            return false;
        }
        if (shouldDisplay) {
            display();
//            return true;
        }
        return true;
    }
}

void Game::play() {
    bool isOver = false;
    bool hasWinner = false;
    Side winningSide = NORTH;
    bool nonInteractive = (!southPlayer->isInteractive() && !northPlayer->isInteractive()) ? true : false;
    display();
    while (!isOver) {
        move(SOUTH);
        status(isOver, hasWinner, winningSide);
        if (hasWinner == true) {
            break;
        }
        if (nonInteractive == true) {
            std::cout << "Press ENTER to continue";
//            std::cin.ignore(9999, '\n');
            std::cin.ignore();
        }
        move(NORTH);
        status(isOver, hasWinner, winningSide);
        if (hasWinner == true) {
            break;
        }
        if (nonInteractive == true) {
            std::cout << "Press ENTER to continue";
//            std::cin.ignore(9999, '\n');
            std::cin.ignore();
        }
    }
    Player* winner = (winningSide == NORTH) ? northPlayer : southPlayer;
    std::cout << "The winner is " << winner->name() << "." << std::endl;
    //    Play the game. Display the progress of the game in a manner of your choosing, provided that someone looking at the screen can follow what's happening. If neither player is interactive, then to keep the display from quickly scrolling through the whole game, it would be reasonable periodically to prompt the viewer to press ENTER to continue and not proceed until ENTER is pressed. (The ignore function for input streams is useful here.) Announce the winner at the end of the game. You can apportion to your liking the responsibility for displaying the board between this function and the move function.
}

int Game::beans(Side s, int hole) const {
    return gameBoard.beans(s, hole);
    //    Return the number of beans in the indicated hole or pot of the game's board, or −1 if the hole number is invalid. This function exists so that we and you can more easily test your program.
}
