#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;

template<typename T>
class Grid {
private:
    struct ROW {
        T* Cols;
        size_t NumCols;
    };

    ROW* Rows;
    size_t NumRows;

public:
    Grid() {
        Rows = new ROW[4];
        NumRows = 4;

        for (size_t r = 0; r < NumRows; ++r) {
            Rows[r].Cols = new T[4];
            Rows[r].NumCols = 4;

            for (size_t c = 0; c < Rows[r].NumCols; ++c) {
                Rows[r].Cols[c] = T();
            }
        }
    }

    Grid(size_t R, size_t C) {
        if (R == 0 || C == 0) {
            throw invalid_argument("Grid dimensions must be greater than 0.");
        }

        Rows = new ROW[R];
        NumRows = R;

        for (size_t r = 0; r < NumRows; ++r) {
            Rows[r].Cols = new T[C];
            Rows[r].NumCols = C;

            for (size_t c = 0; c < Rows[r].NumCols; ++c) {
                Rows[r].Cols[c] = T();
            }
        }
    }

    virtual ~Grid() {
        for (size_t r = 0; r < NumRows; ++r) {
            delete[] Rows[r].Cols;
        }
        delete[] Rows;
    }

    Grid(const Grid<T>& other) {
        NumRows = other.NumRows;
        Rows = new ROW[NumRows];

        for (size_t r = 0; r < NumRows; ++r) {
            Rows[r].NumCols = other.Rows[r].NumCols;
            Rows[r].Cols = new T[Rows[r].NumCols];
            for (size_t c = 0; c < Rows[r].NumCols; ++c) {
                Rows[r].Cols[c] = other.Rows[r].Cols[c];
            }
        }
    }

    Grid& operator=(const Grid& other) {
        if (this == &other) {
            return *this;
        }

        for (size_t r = 0; r < NumRows; ++r) {
            delete[] Rows[r].Cols;
        }
        delete[] Rows;

        NumRows = other.NumRows;
        Rows = new ROW[NumRows];

        for (size_t r = 0; r < NumRows; ++r) {
            Rows[r].NumCols = other.Rows[r].NumCols;
            Rows[r].Cols = new T[Rows[r].NumCols];
            for (size_t c = 0; c < Rows[r].NumCols; ++c) {
                Rows[r].Cols[c] = other.Rows[r].Cols[c];
            }
        }

        return *this;
    }

    size_t numrows() const {
        return NumRows;
    }

    size_t numcols(size_t r) const {
        if (r >= NumRows) {
            throw out_of_range("Row index out of range.");
        }
        return Rows[r].NumCols;
    }

    size_t size() const {
        size_t total = 0;
        for (size_t r = 0; r < NumRows; ++r) {
            total += Rows[r].NumCols;
        }
        return total;
    }

    T& operator()(size_t r, size_t c) {
        if (r >= NumRows || c >= Rows[r].NumCols) {
            throw out_of_range("Grid indices out of range.");
        }
        return Rows[r].Cols[c];
    }

    void _output() const {
        for (size_t r = 0; r < NumRows; ++r) {
            for (size_t c = 0; c < Rows[r].NumCols; ++c) {
                cout << Rows[r].Cols[c] << ' ';
            }
            cout << endl;
        }
    }
};
