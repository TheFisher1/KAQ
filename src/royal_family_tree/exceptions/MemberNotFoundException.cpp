//
// Created by Kaloyan Ribarov on 14.01.24.
//

#include "MemberNotFoundException.h"

MemberNotFoundException::MemberNotFoundException() : message("member could not be found") { }

MemberNotFoundException::MemberNotFoundException(const std::string& name) {
//    message = "Member with name: " + message + " could not be found";
        message = "Member with name: ";
        message += name;
        message += " could not be found";
}

const char *MemberNotFoundException::what() const noexcept {
    return message.c_str();
}