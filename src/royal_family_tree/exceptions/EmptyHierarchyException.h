//
// Created by Kaloyan Ribarov on 14.01.24.
//

#ifndef UNTITLED4_EMPTYHIERARCHYEXCEPTION_H
#define UNTITLED4_EMPTYHIERARCHYEXCEPTION_H

#include <exception>
#include <string>

class EmptyHierarchyException : public std::exception {
public:
    EmptyHierarchyException();
    EmptyHierarchyException(std::string);
    EmptyHierarchyException(const char *);

    const char * what() const noexcept override;
private:
    std::string message;
};


#endif //UNTITLED4_EMPTYHIERARCHYEXCEPTION_H
