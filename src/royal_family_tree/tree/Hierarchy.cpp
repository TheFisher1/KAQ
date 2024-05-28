//
// Created by Kaloyan Ribarov on 21.12.23.
//

#include "Hierarchy.h"


Hierarchy::Node::Node(const Heir& thisHeir) : thisHeir(thisHeir), children() { }

Hierarchy::Hierarchy() : root(nullptr) { }

Hierarchy::Hierarchy(Hierarchy&& other) noexcept : root(other.root) {
    other.root = nullptr;
}

Hierarchy& Hierarchy::operator=(Hierarchy&& other) noexcept {
    Hierarchy copy(std::move(other));
    std::swap(this->root, copy.root);
    return *this;
}

Hierarchy::~Hierarchy() {
    destroyTree();
}

void Hierarchy::addMember(const Heir& heir, const std::string& parentName) {
    if (root == nullptr) {
        root = new Node(heir);
    } else {
        if (parentName.empty()) {
            throw std::invalid_argument("no parentName has been added");
        } else if (!heir.isProtestant()) {
            throw std::invalid_argument("catholics cannot be successors to the throne");
        }

        if (!addMemberHelper(heir, parentName, root)) {
            throw MemberNotFoundException(parentName);
        }

    }
}

bool Hierarchy::addMemberHelper(const Heir &newHeir, const std::string& parentName, Hierarchy::Node *& node) {
    if (node->thisHeir.getName() == parentName) {

        if (node->children.empty()) {
            node->children.push_back(new Node(newHeir));

        } else {
            node->children.insert(node->children.begin() + findIndex(node->children, newHeir), new Node(newHeir));
        }

        return true;

    } else if (node->thisHeir == newHeir) {
        throw AlreadyExistingMemberException(newHeir);
    }

        for (Node* i: node->children) {
           bool flag = addMemberHelper(newHeir, parentName, i);
            if (flag) {
                return true;
            }
        }

    return false;
}

std::size_t Hierarchy::findIndex(std::vector<Node*>& ls, const Heir& newHeir) noexcept {
    std::size_t i = 0;
    std::vector<Node*>::iterator iter = ls.begin();

    if (ls.size() > 1) {
        while (iter != ls.end() && newHeir < (*iter)->thisHeir) {
            ++iter;
            ++i;
        }
        return i;
    } else {
        return 1;
    }
}

void Hierarchy::deserialise(std::ifstream& is) {
    if (this->root != nullptr) {
        destroyTree();
    }

    std::string line;

    std::stack<Node*> st;
    Node* curr = nullptr;

    while (std::getline(is, line) && (!is.eof() || !line.empty())) {

        std::size_t depth = 0;
        while (line[depth] == '\t') {
            ++depth;
        }

        Heir heir(line.substr(depth));

        Node* newNode = new Node(heir);
        if (depth == 0) {
            root = newNode;
            curr = newNode;
        } else {

            while (st.size() > depth) {
                st.pop();
            }

            Node* parent = st.top();
            parent->children.push_back(newNode);
            curr = newNode;
        }

        st.push(curr);
    }
}

void Hierarchy::serialise(std::ofstream & os) const {
    printHelper(root, 0, os);
}

void Hierarchy::makeMonarch(const std::string & name) {
    if (root != nullptr && name == root->thisHeir.getName()) {
        return;
    } else if (root == nullptr) {
        throw EmptyHierarchyException();
    } else {
        makeMonarchHelper(root, name);
    }
}

void Hierarchy::destroyTreeHelper(Hierarchy::Node*& node) noexcept {
    if (node->children.empty()) {
        return;
    }

    for (Node* n : node->children) {
        destroyTreeHelper(n);
    }

    for (Node* n : node->children) {
        delete n;
    }

    node->children.clear();
}

void Hierarchy::save(const std::string& filename) const noexcept{
    std::ofstream file(filename);
    serialise(file);
    file.close();
}

void Hierarchy::destroyTree() noexcept {
    if (root == nullptr) {
        return;
    }

    destroyTreeHelper(root);

    if (root != nullptr) {
        delete root;
        root = nullptr;
    }

}

Heir Hierarchy::getMonarch() const {
    return this->root->thisHeir;
}

void Hierarchy::makeMonarchHelper(Hierarchy::Node*& node, const std::string& name) noexcept {
    if (node->children.empty()) {
        return;
    }

    Node* newRoot = nullptr;

    std::vector<Node*>::iterator iter = node->children.begin();

    bool flag = false;
    while (iter != node->children.end()) {
        if ((*iter)->thisHeir.getName() == name) {
            newRoot = *iter;
            iter = node->children.erase(iter);
            flag = true;

        } else {
            ++iter;
        }
    }

    if (!flag) {
        for (Node* n : node->children) {
            makeMonarchHelper(n, name);
        }
    } else {

        for (Node* n : root->children) {
            newRoot->children.push_back(n);
        }

        delete root;
        root = newRoot;
    }
}

void Hierarchy::printHelper(Hierarchy::Node *node, int depth, std::ostream& os) const noexcept {
    if (!node) {
        return;
    }

    std::string indentation;

    for (std::size_t i = 0; i < depth; ++i) {
        indentation += '\t';
    }

    os << indentation << node->thisHeir << '\n';

        for (auto n: node->children) {
            printHelper(n, depth + 1, os);
        }

}

void Hierarchy::printHierarchy() const noexcept {
   printHelper(root, 0, std::cout);
}

std::string Hierarchy::printInfo(const std::string& name) const {

    for (auto it = begin(); it != end(); ++it) {
        if (it->getName() == name) {
            Heir heir = *it;
            return R"({"name" : ")" + heir.getName() + R"(", "birthdate" : ")" + heir.getBirthdate() + R"(", "gender" : )" + (heir.getGender() ? "\"male\"" : "\"female\"")
                   + ", \"religion\" : " + (heir.isProtestant() ? "\"protestant\"" : "\"catholic\"") + '}';
        }
    }

    return "member with name: " + name + " could not be found";

}

Hierarchy::Iterator::Iterator(Hierarchy::Node* current, const Hierarchy* h) : current(current), hierarchyPtr(h) {
    stack.push(nullptr);
    stack.push(current);
}

Hierarchy::Iterator& Hierarchy::Iterator::operator++() noexcept {
    Node* tos = stack.top();
    stack.pop();

    windstack(tos);

    return *this;
}

bool Hierarchy::Iterator::operator!=(const Hierarchy::Iterator& other) {
    if (this->hierarchyPtr != other.hierarchyPtr) {
        return true;
    }

    return stack.top() != other.stack.top();
}

Heir Hierarchy::Iterator::operator*() {
    return stack.top()->thisHeir;
}

Heir* Hierarchy::Iterator::operator->() {
    return &stack.top()->thisHeir;
}

void Hierarchy::Iterator::windstack(Hierarchy::Node* node) {
    for (std::vector<Node*>::reverse_iterator it = node->children.rbegin(); it != node->children.rend(); ++it) {
        stack.push(*it);
    }
}

Hierarchy::Iterator Hierarchy::begin() const {
    return Iterator(root, this);
}

Hierarchy::Iterator Hierarchy::end() const {
    return Iterator(nullptr, this);
}

void Hierarchy::declareDeceased(const std::string& name) {
    if (root == nullptr) {
        throw EmptyHierarchyException();
    }

    if (root->thisHeir.getName() == name) {
        if (root->children.empty()) {
            destroyTree();
        } else {
            makeSuccessorMonarch();
        }
    } else {
        if (!declareDeceasedHelper(root, name)) {
            throw MemberNotFoundException(name);
        }
    }

}

bool Hierarchy::declareDeceasedHelper(Hierarchy::Node*& node, const std::string& name) noexcept {
    std::vector<Node*>::iterator iter = node->children.begin();

    while (iter != node->children.end()) {
        if ((*iter)->thisHeir.getName() == name) {
            Node *save = *iter;

            iter = node->children.erase(iter);

            std::size_t index = iter - node->children.begin();
            for (Node *n: save->children) {
                node->children.insert(node->children.begin() + index++, n);
            }
            delete save;
            return true;
        } else {
            ++iter;
        }
    }
    if (!node->children.empty()) {
        for (Node *n: node->children) {
            bool flag = declareDeceasedHelper(n, name);
            if (flag) {
                return true;
            }
        }

    }
    return false;
}

void Hierarchy::editReligion(const std::string& name, const Religion& newReligion) {
    if (newReligion == Religion::PROTESTANTISM) {
        throw std::invalid_argument("new religion cannot be protestantism");
    } else if (root == nullptr) {
        throw EmptyHierarchyException();
    }

    if (root->thisHeir.getName() == name) {
        destroyTree();
    } else {
        if (!editReligionHelper(root, name, newReligion)) {
            throw MemberNotFoundException(name);
        }
    }
}

bool Hierarchy::editReligionHelper(Hierarchy::Node* node, const std::string & name, const Religion& newReligion) noexcept {

    std::vector<Node*>::iterator iter = node->children.begin();
    while (iter != node->children.end()) {
        if ((*iter)->thisHeir.getName() == name) {
            Node* save = *iter;
            iter = node->children.erase(iter);

            destroyTreeHelper(save);
            delete save;
            return true;
        } else {
            ++iter;
        }
    }
    if (!node->children.empty()) {
        for (Node *n: node->children) {
            bool flag = editReligionHelper(n, name, newReligion);
            if (flag) {
                return true;
            }
        }

    }

    return false;
}

void Hierarchy::makeSuccessorMonarch() {
    if (!root || root->children.empty()) {
        throw EmptyHierarchyException("there are no successors to the monarch");
    }

    makeMonarch((*root->children.begin())->thisHeir.getName());

}

std::vector<Heir> Hierarchy::findSuccessors(const std::size_t n) const {
    if (root == nullptr) {
        throw EmptyHierarchyException();
    }

    std::size_t i = 0;
    std::vector<Heir> ans;
    Hierarchy::Iterator it = begin();
    ++it;
    for (; it != end() && i < n; ++it) {
        Heir currHeir = (*it);
        ++i;
        ans.push_back(currHeir);
    }

    return ans;
}

void Hierarchy::load(const std::string& filename) {
    std::ifstream file(filename);
    if (file.good()) {
        deserialise(file);
        file.close();
    } else {
        std::cout << "Failed to open file with name: " << filename << '\n';
    }
}

bool Hierarchy::contains(const std::string& name) const noexcept {
    Iterator it = begin();

    while (it != end()) {
        if (it->getName() == name) {
            return true;
        }

        ++it;
    }

    return false;
}

Hierarchy::Hierarchy(const Hierarchy & other) : root(nullptr) {
    copy(other);
}

Hierarchy &Hierarchy::operator=(const Hierarchy& other) {

    if (this != &other) {
        destroyTree();
        copy(other);
    }

    return *this;
}

void Hierarchy::copy(const Hierarchy& other) {
    copyHelper(root, other.root);
}

void Hierarchy::copyHelper(Hierarchy::Node*& node, Hierarchy::Node* otherNode) {

    node = new Node(otherNode->thisHeir);

    if (!otherNode->children.empty()) {
        for (Node *n: otherNode->children) {
            Node* child = new Node(n->thisHeir);
            node->children.push_back(child);
        }
    }

    for (std::size_t i = 0; i < otherNode->children.size(); i++) {
        copyHelper(node->children[i], otherNode->children[i]);
    }

}

