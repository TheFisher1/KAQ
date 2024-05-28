//
// Created by Kaloyan Ribarov on 21.12.23.
//

#ifndef DSA_HW_HIERARCHY_H
#define DSA_HW_HIERARCHY_H

#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <string>

#include "../exceptions/AlreadyExistingMemberException.h"
#include "../exceptions/EmptyHierarchyException.h"
#include "../exceptions/MemberNotFoundException.h"

#include "../heir/Heir.h"


/*
 * Hierarchy class - implements the main tree
 */

class Hierarchy {
private:
    struct Node;
public:
    Hierarchy();

    Hierarchy(const Hierarchy&);
    Hierarchy& operator=(const Hierarchy&);

    Hierarchy(Hierarchy&&) noexcept ;
    Hierarchy& operator=(Hierarchy&&) noexcept;

    ~Hierarchy();

    void addMember(const Heir&, const std::string& parentName = "");
    std::string printInfo(const std::string&) const;
    void makeMonarch(const std::string&);

    void editReligion(const std::string&, const Religion& newReligion);
    void declareDeceased(const std::string&);
    std::vector<Heir> findSuccessors(const std::size_t) const;
    void makeSuccessorMonarch();

    void printHierarchy() const noexcept;

    void load(const std::string&);
    void save(const std::string&) const noexcept;

    Heir getMonarch() const; // test function
    bool contains(const std::string& name) const noexcept; // mainly test function

    class Iterator {
    public:
        Iterator(Node* current, const Hierarchy* h);

        Iterator& operator++() noexcept;
        bool operator!=(const Hierarchy::Iterator&);
        Heir operator*();
        Heir* operator->();
    private:
        void windstack(Node*);
    private:
        const Hierarchy * const hierarchyPtr;
        Node* current;
        std::stack<Node*> stack;
    };

    Iterator begin() const;
    Iterator end() const;

private:
    void copy(const Hierarchy&);
    void copyHelper(Node*&, Node* otherNode);
    bool addMemberHelper(const Heir& newHeir, const std::string& parent, Node*& node);
    void makeMonarchHelper(Node*& node, const std::string&) noexcept;
    void destroyTreeHelper(Node*& node) noexcept;
    void printHelper(Node* node, int depth, std::ostream&) const noexcept;
    bool declareDeceasedHelper(Node*& node, const std::string&) noexcept;
    bool editReligionHelper(Node*, const std::string&, const Religion&) noexcept;

    void serialise(std::ofstream&) const;
    void deserialise(std::ifstream&);
    void destroyTree() noexcept;


    Iterator find(const std::string&) const noexcept;
    std::size_t findIndex(std::vector<Node*>& ls, const Heir& newHeir) noexcept;
private:
    struct Node {
        Heir thisHeir;
        std::vector<Node*> children;

        Node(const Heir& thisHeir);
    };
private:
    Node* root;
};


#endif //DSA_HW_HIERARCHY_H
