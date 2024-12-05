#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>

#include "boardcell.h"
#include "grid.h"

using namespace std;

class GameBoard {
private:
    Grid<BoardCell*> board;
    size_t numRows;
    size_t numCols;
    ssize_t HeroRow;
    ssize_t HeroCol;
    int numMonsters;
    int numSuperMonsters;
    int numAbysses;
    int numBats;
    bool wonGame;

public:
    GameBoard() : board(15, 40), numRows(15), numCols(40), HeroRow(-1), HeroCol(-1),
                 numMonsters(4), numSuperMonsters(2), numAbysses(50), numBats(2), wonGame(false) {
        blankBoard();
    }

    GameBoard(size_t numRows, size_t numCols) : board(numRows, numCols), numRows(numRows), numCols(numCols),
                                               HeroRow(-1), HeroCol(-1),
                                               numMonsters(4), numSuperMonsters(2),
                                               numAbysses(50), numBats(2), wonGame(false) {
        blankBoard();
    }

    virtual ~GameBoard() {
        for (size_t row = 0; row < board.numrows(); row++) {
            for (size_t col = 0; col < board.numcols(row); col++) {
                delete board(row, col);
            }
        }
    }

    void blankBoard() {
        for (size_t row = 0; row < board.numrows(); row++) {
            for (size_t col = 0; col < board.numcols(row); col++) {
                board(row, col) = new Nothing(row, col);
            }
        }
    }

    char getCellDisplay(size_t r, size_t c) {
        return board(r, c)->display();
    }

    void setCell(BoardCell* myCell, size_t r, size_t c) {
        delete board(r, c);
        board(r, c) = myCell;
    }

    void freeCell(size_t r, size_t c) {
        delete board(r, c);
        board(r, c) = new Nothing(r, c);
    }

    void setupBoard(int seed) {
        srand(seed);
        size_t r, c;

        while (true) {
            r = rand() % numRows;
            c = rand() % 3;
            if (board(r, c)->isSpace()) {
                setCell(new Hero(r, c), r, c);
                HeroRow = r;
                HeroCol = c;
                break;
            }
        }

        while (true) {
            r = rand() % numRows;
            c = numCols - 1 - (rand() % 3);
            if (board(r, c)->isSpace()) {
                setCell(new EscapeLadder(r, c), r, c);
                break;
            }
        }

        int sizeMid = numCols - 6;

        size_t wallCols[3];
        for (int i = 0; i < 3; ++i) {
            while (true) {
                c = 3 + (rand() % sizeMid);
                bool conflict = false;
                for (int j = 0; j < i; ++j) {
                    if (abs((int)c - (int)wallCols[j]) <= 1) {
                        conflict = true;
                        break;
                    }
                }
                if (!conflict) {
                    wallCols[i] = c;
                    break;
                }
            }

            for (r = 0; r < numRows / 2; ++r) {
                setCell(new Wall(r, wallCols[i]), r, wallCols[i]);
            }
        }

        for (int i = 0; i < numAbysses; ++i) {
            while (true) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                if (board(r, c)->isSpace()) {
                    setCell(new Abyss(r, c), r, c);
                    break;
                }
            }
        }

        for (int i = 0; i < numMonsters; ++i) {
            while (true) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                if (board(r, c)->isSpace()) {
                    setCell(new Monster(r, c), r, c);
                    break;
                }
            }
        }

        for (int i = 0; i < numSuperMonsters; ++i) {
            while (true) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                if (board(r, c)->isSpace()) {
                    Monster* superMonster = new Monster(r, c);
                    superMonster->setPower(2);
                    setCell(superMonster, r, c);
                    break;
                }
            }
        }

        for (int i = 0; i < numBats; ++i) {
            while (true) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                if (board(r, c)->isSpace()) {
                    setCell(new Bat(r, c), r, c);
                    break;
                }
            }
        }
    }

    void display() {
        cout << '-';
        for (size_t col = 0; col < board.numcols(0); col++) {
            cout << '-';
        }
        cout << '-' << endl;

        for (size_t row = 0; row < board.numrows(); row++) {
            cout << '|';
            for (size_t col = 0; col < board.numcols(row); col++) {
                cout << board(row, col)->display();
            }
            cout << '|' << endl;
        }

        cout << '-';
        for (size_t col = 0; col < board.numcols(0); col++) {
            cout << '-';
        }
        cout << '-' << endl;
    }

    bool getWonGame() const {
        return wonGame;
    }

    void setNumMonsters(int num) {
        numSuperMonsters = num / 3;
        numMonsters = num - numSuperMonsters;
    }

    void setNumAbysses(int num) {
        numAbysses = num;
    }

    void setNumBats(int num) {
        numBats = num;
    }

    size_t getNumRows() const {
        return numRows;
    }

    size_t getNumCols() const {
        return numCols;
    }

    void getHeroPosition(size_t& row, size_t& col) const {
        row = (HeroRow >= 0 && HeroRow < (ssize_t)numRows) ? HeroRow : -1;
        col = (HeroCol >= 0 && HeroCol < (ssize_t)numCols) ? HeroCol : -1;
    }

    void setHeroPosition(size_t row, size_t col) {
        HeroRow = row;
        HeroCol = col;
    }

    void findHero() {
        bool found = false;
        for (size_t r = 0; r < numRows && !found; r++) {
            for (size_t c = 0; c < numCols && !found; c++) {
                if (board(r, c)->isHero()) {
                    HeroRow = r;
                    HeroCol = c;
                    found = true;
                }
            }
        }
        if (!found) {
            HeroRow = -1;
            HeroCol = -1;
        }
    }

    void resetMovedFlags() {
        for (size_t r = 0; r < numRows; r++) {
            for (size_t c = 0; c < numCols; c++) {
                if (!board(r, c)->isStatic()) {
                    board(r, c)->setMoved(false);
                }
            }
        }
    }

bool makeMoves(char HeroNextMove) {
    resetMovedFlags();

    size_t attemptedHR, attemptedHC;
    Hero* hero = nullptr;

    if (HeroRow != (ssize_t)-1 && HeroCol != (ssize_t)-1) {
        hero = dynamic_cast<Hero*>(board(HeroRow, HeroCol));
        if (hero != nullptr) {
            hero->setNextMove(HeroNextMove);
            hero->attemptMoveTo(attemptedHR, attemptedHC, HeroRow, HeroCol);
        }
    }

    if (hero != nullptr) {
        // Adjust for out-of-bounds movement by ignoring components that cause out-of-bounds
        if (attemptedHR >= numRows) {
            attemptedHR = HeroRow;
            std::cout << "Hero move out-of-bounds vertically." << std::endl;
        }
        if (attemptedHC >= numCols) {
            attemptedHC = HeroCol;
            std::cout << "Hero move out-of-bounds horizontally." << std::endl;
        }

        // Now check for barriers
        if (board(attemptedHR, attemptedHC)->isBarrier()) {
            bool isDiagonal = (attemptedHR != HeroRow) && (attemptedHC != HeroCol);

            if (isDiagonal) {
                // Try moving vertically only
                size_t tempR = attemptedHR;
                size_t tempC = HeroCol;
                if (tempR < numRows && !board(tempR, tempC)->isBarrier()) {
                    attemptedHR = tempR;
                    attemptedHC = tempC;
                } else {
                    // Try moving horizontally only
                    tempR = HeroRow;
                    tempC = attemptedHC;
                    if (tempC < numCols && !board(tempR, tempC)->isBarrier()) {
                        attemptedHR = tempR;
                        attemptedHC = tempC;
                    } else {
                        // Stay in place
                        attemptedHR = HeroRow;
                        attemptedHC = HeroCol;
                    }
                }
            } else {
                // Moving onto barrier, stay in place
                attemptedHR = HeroRow;
                attemptedHC = HeroCol;
            }
        }

        // Now proceed to move the Hero
        BoardCell* destination = board(attemptedHR, attemptedHC);
        if (destination->isExit()) {
            delete board(HeroRow, HeroCol);
            board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
            HeroRow = -1;
            HeroCol = -1;
            wonGame = true;
        } else if (destination->isHole() || destination->isBaddie()) {
            delete board(HeroRow, HeroCol);
            board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
            HeroRow = -1;
            HeroCol = -1;
        } else if (destination->isSpace()) {
            setCell(new Hero(attemptedHR, attemptedHC), attemptedHR, attemptedHC);
            delete board(HeroRow, HeroCol);
            board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
            HeroRow = attemptedHR;
            HeroCol = attemptedHC;
        }
    }

    // Move Baddies
    for (size_t r = 0; r < numRows; r++) {
        for (size_t c = 0; c < numCols; c++) {
            BoardCell* cell = board(r, c);
            if (cell->isBaddie() && !cell->getMoved()) {
                size_t attemptedBR, attemptedBC;
                cell->attemptMoveTo(attemptedBR, attemptedBC, HeroRow, HeroCol);

                // Adjust for out-of-bounds movement
                if (attemptedBR >= numRows) {
                    attemptedBR = r;
                    std::cout << "Baddie move out-of-bounds vertically." << std::endl;
                }
                if (attemptedBC >= numCols) {
                    attemptedBC = c;
                    std::cout << "Baddie move out-of-bounds horizontally." << std::endl;
                }

                // Check for barriers and exit (Baddies treat exit as a barrier)
                if (board(attemptedBR, attemptedBC)->isBarrier() || board(attemptedBR, attemptedBC)->isExit()) {
                    bool isDiagonal = (attemptedBR != r) && (attemptedBC != c);

                    if (isDiagonal) {
                        // Try moving vertically only
                        size_t tempR = attemptedBR;
                        size_t tempC = c;
                        if (tempR < numRows && !board(tempR, tempC)->isBarrier() && !board(tempR, tempC)->isExit()) {
                            attemptedBR = tempR;
                            attemptedBC = tempC;
                        } else {
                            // Try moving horizontally only
                            tempR = r;
                            tempC = attemptedBC;
                            if (tempC < numCols && !board(tempR, tempC)->isBarrier() && !board(tempR, tempC)->isExit()) {
                                attemptedBR = tempR;
                                attemptedBC = tempC;
                            } else {
                                // Stay in place
                                attemptedBR = r;
                                attemptedBC = c;
                            }
                        }
                    } else {
                        // Moving onto barrier, stay in place
                        attemptedBR = r;
                        attemptedBC = c;
                    }
                }

                // Now proceed to move the Baddie
                BoardCell* destination = board(attemptedBR, attemptedBC);
                if (destination->isHole()) {
                    delete board(r, c);
                    board(r, c) = new Nothing(r, c);
                } else if (destination->isHero()) {
                    // Remove both Baddie and Hero
                    delete board(r, c);
                    board(r, c) = new Nothing(r, c);
                    delete board(HeroRow, HeroCol);
                    board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                    HeroRow = -1;
                    HeroCol = -1;
                } else if (destination->isBaddie()) {
                    // Do not move, stay in place
                    continue;
                } else if (destination->isSpace()) {
                    // Move Baddie to new location
                    BoardCell* newBaddie = nullptr;
                    if (cell->display() == 'M') {
                        newBaddie = new Monster(attemptedBR, attemptedBC);
                        newBaddie->setPower(2);
                    } else if (cell->display() == 'm') {
                        newBaddie = new Monster(attemptedBR, attemptedBC);
                    } else if (cell->display() == '~') {
                        newBaddie = new Bat(attemptedBR, attemptedBC);
                    }
                    setCell(newBaddie, attemptedBR, attemptedBC);
                    newBaddie->setMoved(true);

                    delete board(r, c);
                    board(r, c) = new Nothing(r, c);
                }
                board(attemptedBR, attemptedBC)->setMoved(true);
            }
        }
    }

    findHero();

    if (HeroRow != (ssize_t)-1 && HeroCol != (ssize_t)-1) {
        return true;
    } else {
        return false;
    }
}

};

#endif
