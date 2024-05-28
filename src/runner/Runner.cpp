//
// Created by Kaloyan Ribarov on 4.01.24.
//

#include "Runner.h"
Runner::Runner() : hierarchy() { }

void Runner::run() {

    std::string line;
    std::cout << "=> ";
    std::getline(std::cin, line);

    while (line != "quit") {

        std::size_t i = 0;
        std::string command = parseMethod(line, i);


        if (command == "add-member") {
            addMemberCommand(line.substr(i));

        } else if (command == "print-info") {
            if (line.length() <= i) {
                std::cout << "No member was chosen. Please add a valid one.\n";

            } else {
                printInfoCommand(line.substr(i));
            }

        } else if (command == "make-monarch") {
            makeMonarchCommand(line.substr(i).substr(1, line.length() - 2));

        } else if (command == "edit-religion") {
            editReligionCommand(line.substr(i));

        } else if (command == "declare-deceased") {
            declareDeceasedCommand(line.substr(i));

        } else if (command == "find-successors") {

           std::vector<Heir> a = findSuccessorsCommand(line.substr(i));

           for (Heir h : a) {
                std::cout << h << '\n';
            }

        } else if (command == "make-successor-monarch") {
            makeSuccessorMonarchCommand();

        } else if (command == "load") {
            hierarchy.load(line.substr(i));

        } else if (command == "save") {
            hierarchy.save(line.substr(i));

        } else {
            std::cout << "=> Unknown command" << '\n';
        }

        std::cout << "=> ";
        std::getline(std::cin, line);
    }

    std::cout << "Process finished with exit code 0 :)";
}

std::string Runner::parseMethod(const std::string& line, std::size_t& n) const {
    std::size_t i = 0;
    std::string command;

    while (line[i] != ' ' && i < line.length()) {
        command.push_back(line[i]);
        ++i;
    }

    n = i + 1;
    return command;
}

void Runner::addMemberCommand(const std::string& line) {
    std::size_t i = 0;

    std::string parentName = getName(line, i);

    try {

        if (line.substr(i)[0] == '"') {
            Heir heir(line.substr(i));
            hierarchy.addMember(heir, parentName);

        } else {
            Heir heir(line);
            hierarchy.addMember(heir);
        }

    } catch (const MemberNotFoundException& e) {
        std::cout << "=> " << e.what() << '\n';
    } catch (const std::invalid_argument& e) {
        std::cout << "=> " << e.what() << '\n';
    } catch (const AlreadyExistingMemberException& e) {
        std::cout << "=> " << e.what() << '\n';
    }

}

void Runner::printInfoCommand(const std::string& line) const {

        std::string response = hierarchy.printInfo(line.substr(1, line.length() - 2));
        std::cout << "=> " << response << '\n';
}

void Runner::editReligionCommand(const std::string& line) {
    std::size_t i = 0;
    std::string name = getName(line, i);

    Religion religion;
    if (line.substr(i).length() > 0) {
        if (line.substr(i) == "protestant") {
            religion = Religion::PROTESTANTISM;
        } else {
            religion = Religion::CATHOLICISM;
        }
    } else {
        std::cout << "Please enter religion";
    }

    try {
        hierarchy.editReligion(name, religion);
    } catch (const EmptyHierarchyException& e) {
        std::cout << "=> " << e.what() << '\n';
    } catch (const std::invalid_argument& e) {
        std::cout << "=> " << e.what() << '\n';
    }
}

std::string Runner::getName(const std::string& line, std::size_t& n) const {
    std::string parentName;
    std::size_t count = 0;
    std::size_t i = 0;

    while (count < 2 && i < line.length()) {
        if (line[i] != '"') {
            parentName += line[i];
        } else {
            ++count;
        }

        ++i;
    }

    n = i + 1;
    return parentName;
}

std::vector<Heir> Runner::findSuccessorsCommand(const std::string& line) const {
    std::size_t n = std::stoi(line);
    std::vector<Heir> ans;
    try {
      ans = hierarchy.findSuccessors(n);
    } catch (const EmptyHierarchyException& e) {
        std::cout << e.what() << '\n';
    }

    return ans;
}

void Runner::makeSuccessorMonarchCommand() {
    try {
        hierarchy.makeSuccessorMonarch();
    } catch (const EmptyHierarchyException& e) {
        std::cout << e.what() << '\n';
    }

}

void Runner::makeMonarchCommand(const std::string & line) {
    try {
        hierarchy.makeMonarch(line);
    } catch (const EmptyHierarchyException& e) {
        std::cout << e.what() << '\n';
    }
}

void Runner::declareDeceasedCommand(const std::string & line) {
    try {
        hierarchy.declareDeceased(line.substr(1, line.length() - 2));
    } catch (const EmptyHierarchyException& e) {
        std::cout << e.what() << '\n';
    } catch (const MemberNotFoundException& e) {
        std::cout << e.what() << '\n';
    }

}