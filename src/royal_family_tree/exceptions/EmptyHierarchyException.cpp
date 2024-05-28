//
// Created by Kaloyan Ribarov on 14.01.24.
//

#include "EmptyHierarchyException.h"

EmptyHierarchyException::EmptyHierarchyException() : message("There are no heirs in the hierarchy") { }

EmptyHierarchyException::EmptyHierarchyException(std::string msg) : message(std::move(msg)) { }

EmptyHierarchyException::EmptyHierarchyException(const char * msg) : message(msg) { }

const char *EmptyHierarchyException::what() const noexcept {
    return message.c_str();
}