//
//  Side.h
//  project3
//
//  Created by amanda  on 5/20/23.
//

#ifndef Side_h
#define Side_h
#include <vector>

class Board;
class Player;

enum Side { NORTH, SOUTH };

    const int NSIDES = 2;
    const int POT = 0;

    inline
    Side opponent(Side s)
    {
        return Side(NSIDES - 1 - s);
    }
    




#endif /* Side_h */
