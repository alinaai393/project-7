#ifndef _BOARDCELL_H
#define _BOARDCELL_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

class BoardCell {
public:
    BoardCell() : myRow(0), myCol(0), moved(false) {}
    virtual ~BoardCell() {}

    virtual char display() = 0;

    virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
        newR = myRow;
        newC = myCol;
    }

    virtual bool isHero() { return false; }
    virtual bool isBaddie() { return false; }
    virtual bool isSpace() { return false; }
    virtual bool isBarrier() { return false; }
    virtual bool isHole() { return false; }
    virtual bool isExit() { return false; }
    virtual bool isStatic() { return true; }

    virtual void setPower(int num) {}
    virtual int getPower() { return 1; }
    virtual void setNextMove(char inChar) {}

    void setMoved(bool m) { moved = m; }
    bool getMoved() const { return moved; }
    void setRow(size_t r) { myRow = r; }
    size_t getRow() const { return myRow; }
    void setCol(size_t c) { myCol = c; }
    size_t getCol() const { return myCol; }
    void setPos(size_t r, size_t c) {
        setRow(r);
        setCol(c);
    }

private:
    size_t myRow;
    size_t myCol;
    bool moved;
};

class Hero : public BoardCell {
public:
    Hero(size_t r, size_t c) {
        setRow(r);
        setCol(c);
    }

    virtual bool isHero() { return true; }
    virtual bool isStatic() { return false; }
    virtual char display() { return 'H'; }

    virtual void setNextMove(char inChar) {
        nextMove = inChar;
    }

    virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) override {
        char move = tolower(nextMove);
        size_t currentRow = getRow();
        size_t currentCol = getCol();

        newR = currentRow;
        newC = currentCol;

        switch (move) {
            case 'q':
                if (currentRow > 0) newR -= 1;
                if (currentCol > 0) newC -= 1;
                break;
            case 'w':
                if (currentRow > 0) newR -= 1;
                break;
            case 'e':
                if (currentRow > 0) newR -= 1;
                newC += 1;
                break;
            case 'a':
                if (currentCol > 0) newC -= 1;
                break;
            case 's':
                break;
            case 'd':
                newC += 1;
                break;
            case 'z':
                newR += 1;
                if (currentCol > 0) newC -= 1;
                break;
            case 'x':
                newR += 1;
                break;
            case 'c':
                newR += 1;
                newC += 1;
                break;
            default:
                break;
        }
    }

private:
    char nextMove;
};

class Monster : public BoardCell {
public:
    Monster(size_t r, size_t c) {
        setRow(r);
        setCol(c);
        power = 1;
    }

    virtual bool isBaddie() { return true; }
    virtual bool isStatic() { return false; }

    virtual char display() {
        if (power == 2) {
            return 'M';
        } else {
            return 'm';
        }
    }

    virtual void setPower(int num) {
        power = num;
    }

    virtual int getPower() {
        return power;
    }

    virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) override {
        size_t currentRow = getRow();
        size_t currentCol = getCol();
        int stepSize = power;

        newR = currentRow;
        newC = currentCol;

        if (hRow < currentRow) {
            if (power == 1 && currentRow > 0) {
                newR -= 1;
            } else if (power == 2 && currentRow >= 2) {
                newR -= 2;
            }
        } else if (hRow > currentRow) {
            newR += stepSize;
        }

        if (hCol < currentCol) {
            if (power == 1 && currentCol > 0) {
                newC -= 1;
            } else if (power == 2 && currentCol >= 2) {
                newC -= 2;
            }
        } else if (hCol > currentCol) {
            newC += stepSize;
        }
    }

private:
    int power;
};

class Bat : public BoardCell {
public:
    Bat(size_t r, size_t c) {
        setRow(r);
        setCol(c);
    }

    virtual bool isBaddie() { return true; }
    virtual bool isStatic() { return false; }
    virtual char display() { return '~'; }

    virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) override {
        size_t currentRow = getRow();
        size_t currentCol = getCol();

        newR = currentRow;
        newC = hCol;
    }
};

class Abyss : public BoardCell {
public:
    Abyss(size_t r, size_t c) {
        setRow(r);
        setCol(c);
    }
    virtual char display() { return '#'; }
    virtual bool isHole() { return true; }
};

class Wall : public BoardCell {
public:
    Wall(size_t r, size_t c) {
        setRow(r);
        setCol(c);
    }
    virtual char display() { return '+'; }
    virtual bool isBarrier() { return true; }
};

class Nothing : public BoardCell {
public:
    Nothing(size_t r, size_t c) {
        setRow(r);
        setCol(c);
    }
    virtual char display() { return ' '; }
    virtual bool isSpace() { return true; }
};

class EscapeLadder : public BoardCell {
public:
    EscapeLadder(size_t r, size_t c) {
        setRow(r);
        setCol(c);
    }
    virtual char display() { return '*'; }
    virtual bool isExit() { return true; }
};

#endif
