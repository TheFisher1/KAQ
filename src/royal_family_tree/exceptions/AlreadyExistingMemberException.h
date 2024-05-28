//
// Created by Kaloyan Ribarov on 14.01.24.
//

#ifndef UNTITLED4_ALREADYEXISTINGMEMBEREXCEPTION_H
#define UNTITLED4_ALREADYEXISTINGMEMBEREXCEPTION_H

#include <stdexcept>
#include "../heir/Heir.h"

class AlreadyExistingMemberException : public std::exception {
public:
    AlreadyExistingMemberException();
    AlreadyExistingMemberException(const Heir& heir);

    const char * what() const noexcept override;
private:
    std::string message;
};


#endif //UNTITLED4_ALREADYEXISTINGMEMBEREXCEPTION_H
