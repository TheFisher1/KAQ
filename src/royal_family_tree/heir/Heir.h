//
// Created by Kaloyan Ribarov on 21.12.23.
//

#ifndef DSA_HW_HEIR_H
#define DSA_HW_HEIR_H

#include <string>
#include <iostream>
#include <sstream>
#include "../../utils/Religion.h"

class Heir {
public:
    Heir();
    Heir(const std::string&, const std::string&, const Religion&, bool);
    Heir(const std::string&);

    bool operator<(const Heir&) const noexcept;
    bool operator==(const Heir&) const noexcept;

    std::string getName() const;
    std::string getBirthdate() const;
    Religion getReligion() const;
    bool getGender() const;

    bool isProtestant() const;

    friend std::ostream& operator<<(std::ostream&, const Heir&);
//    friend std::istream& operator>>(std::istream&, Heir&);
private:
    int* parseDate(const std::string&) const noexcept;
    std::vector<std::string> tokenise(const std::string&) const;
private:
    std::string name;
    std::string birthdate;
    Religion religion;
    bool gender; // 1 -> male, 0 -> female
};


#endif //DSA_HW_HEIR_H
