//
// Created by Kaloyan Ribarov on 14.01.24.
//

#ifndef UNTITLED4_MEMBERNOTFOUNDEXCEPTION_H
#define UNTITLED4_MEMBERNOTFOUNDEXCEPTION_H

#include <stdexcept>
#include <string>

class MemberNotFoundException : std::exception {
public:
    MemberNotFoundException();
    MemberNotFoundException(const std::string&);

    const char * what() const noexcept override;
private:
    std::string message;
};


#endif //UNTITLED4_MEMBERNOTFOUNDEXCEPTION_H
