//
// Created by Kaloyan Ribarov on 21.12.23.
//

#include "Heir.h"
Heir::Heir() { }

Heir::Heir(const std::string& name, const std::string& birthdate,
           const Religion& religion, const bool gender) : name(name), birthdate(birthdate),
           religion(religion), gender(gender) { }

Heir::Heir(const std::string& line) {
   std::vector<std::string> tokens = tokenise(line);

    this->name = tokens[0];
    this->birthdate = tokens[1];
    this->gender = (tokens[2] == "male");
    this->religion = (tokens[3] == "protestant") ? Religion::PROTESTANTISM : Religion::CATHOLICISM;
}

bool Heir::operator<(const Heir& other) const noexcept {
    bool thisReligion = (religion == Religion::PROTESTANTISM);
    bool otherReligion = (other.religion == Religion::PROTESTANTISM);

    bool flag = this->gender && thisReligion;
    bool otherFlag = other.gender && otherReligion;

    if (!(flag ^ otherFlag)) {
        int *thisDate = parseDate(this->birthdate);
        int *otherDate = parseDate(other.birthdate);

        int diff[3];
        for (std::size_t i = 0; i < 3; ++i) {
            diff[i] = thisDate[i] - otherDate[i];
        }

        delete thisDate;
        delete otherDate;

        if (diff[2] != 0) {
            return diff[2] > 0;
        } else if (diff[1] != 0) {
            return diff[1] > 0;
        } else if (diff[0] != 0){
            return diff[0] > 0;
        } else {
            return false;
        }
    } else {
        return !this->gender;
    }
}

int* Heir::parseDate(const std::string& birthdate) const noexcept {
    std::size_t i = 0;
    std::size_t j = 0;
    std::string date[3];

    for (int k = 0; k < 3; ++k) {
        date[k] = "0";
    }

    while (i < birthdate.length()) {
        if (birthdate[i] != '.') {
            date[j] += birthdate[i];
        } else {
            ++j;
        }

        ++i;
    }

    return new int[3]{
            std::stoi(date[0]),
            std::stoi(date[1]),
            std::stoi(date[2])
    };
}

bool Heir::operator==(const Heir& other) const noexcept {
    return this->name == other.name;
}
std::string Heir::getName() const {
    return name;
}

std::string Heir::getBirthdate() const {
    return birthdate;
}

bool Heir::getGender() const {
    return gender;
}

Religion Heir::getReligion() const {
    return religion;
}

bool Heir::isProtestant() const {
    return religion == Religion::PROTESTANTISM;
}

std::ostream& operator<<(std::ostream& os, const Heir& heir) {
    os << '"' << heir.name << '"';
    os << heir.birthdate << " ";
    os << (heir.gender ? "male" : "female") << " ";
    os << heir.religion << " ";
    return os;
}

std::vector<std::string> Heir::tokenise(const std::string& line) const {

    bool flag = true;

    std::vector<std::string> tokens;
    std::string var;
    std::size_t i = 0;
    std::size_t tokensSize = 4;

    while (i < line.length() && tokens.size() < tokensSize) {
        if (line[i] == '"') {
            flag = !flag;
        }

        if (line[i] == ' ' && flag) {
            tokens.push_back(var);
            var.clear();

        } else if (i == line.length() - 1 && flag) {
            var += line.back();
            tokens.push_back(var);

        } else if (line[i] != '"'){
                var += line[i];
        }

        ++i;
    }
        return tokens;
}



