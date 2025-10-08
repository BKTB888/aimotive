//
// Created by bktivadar on 10/7/25.
//

#ifndef AIMOTIVE_TILE_H
#define AIMOTIVE_TILE_H
#include <iostream>

enum TileType {
    EMPTY,
    T,
    X,
    C
};

class Tile {
    unsigned char value;

    static bool isHelperLetter(unsigned char c);
    static unsigned char CharToValue(unsigned char c);
    [[nodiscard]] unsigned char toChar() const;
public:
    Tile() = default;
    explicit Tile(char representation);

    friend std::ostream& operator<<(std::ostream& os, const Tile& t) {
        return os << t.toChar();
    }

    void setValue(unsigned char representation);
    [[nodiscard]] TileType getTileType() const;

    friend std::istream& operator>>(std::istream& is, Tile& t) {
        unsigned char representation;
        is >> representation;
        t.setValue(representation);
        return is;
    }
};

#endif //AIMOTIVE_TILE_H