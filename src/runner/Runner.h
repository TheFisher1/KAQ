//
// Created by Kaloyan Ribarov on 4.01.24.
//

#ifndef DSA_HW_RUNNER_H
#define DSA_HW_RUNNER_H
#include <cstddef>
#include <vector>

#include "../royal_family_tree/tree/Hierarchy.h"
#include "../royal_family_tree/exceptions/EmptyHierarchyException.h"

class Runner {
public:
    Runner();

    void run();

private:
    std::string parseMethod(const std::string&, std::size_t&) const;
    void addMemberCommand(const std::string& line);
    void printInfoCommand(const std::string&) const;
    void editReligionCommand(const std::string&);
    std::string getName(const std::string&, std::size_t&) const;
    std::vector<Heir> findSuccessorsCommand(const std::string&) const;
    void makeSuccessorMonarchCommand();
    void makeMonarchCommand(const std::string&);
    void declareDeceasedCommand(const std::string&);

    void printCommands() const;
private:
    Hierarchy hierarchy;

    static constexpr std::size_t INTERVALS_SPACES_AND_QUOTES_LGH = 8;
};


#endif //DSA_HW_RUNNER_H
