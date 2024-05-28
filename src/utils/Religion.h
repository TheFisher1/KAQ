//
// Created by Kaloyan Ribarov on 14.01.24.
//

#ifndef UNTITLED4_RELIGION_H
#define UNTITLED4_RELIGION_H

#include <iostream>

/*
 * class for religion that allows for the wider use of different religions in the future
 */

enum class Religion {
    PROTESTANTISM, CATHOLICISM
};

inline std::ostream& operator<<(std::ostream& os, const Religion& religion) {
    if (religion == Religion::CATHOLICISM) {
        os << "catholic";
    } else if (religion == Religion::PROTESTANTISM) {
        os << "protestant";
    }
    return os;
}

inline std::istream& operator>>(std::istream& is, Religion& religion) {
    std::string in;
    is >> in;
    if (in == "catholic") {
        religion = Religion::CATHOLICISM;
    } else {
        religion = Religion::PROTESTANTISM;
    }

    return is;
}

#endif //UNTITLED4_RELIGION_H
