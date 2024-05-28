//
// Created by Kaloyan Ribarov on 14.01.24.
//

#include "AlreadyExistingMemberException.h"

AlreadyExistingMemberException::AlreadyExistingMemberException() : message("the hierarchy already has an heir with the same name") { }

AlreadyExistingMemberException::AlreadyExistingMemberException(const Heir& heir) : message("heir with name \"" + heir.getName() + "\" already exists.") { }

const char * AlreadyExistingMemberException::what() const noexcept {
    return message.c_str();
}