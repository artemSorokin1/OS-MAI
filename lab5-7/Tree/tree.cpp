#include "tree.h"

Tree::Tree() {
    _root = new Node(-1);
    size = 1;
}

Node *Tree::findNode(const int id, Node *node) const {
    if (!node) {
        return nullptr;
    }
    if (node->id == id) {
        return node;
    }
    for (auto & n : node->son) {
        Node * res = findNode(id, n);
        if (res) return res;
    }
    return nullptr;
}

void Tree::treeDeleter(Node *node) {
    if (!node) {
        return;
    }
    Node * tempNode = node;
    for (auto & n : tempNode->son) {
        treeDeleter(n);
    }
    std::string arg = std::to_string(node->pid);
//    if (execl("./killer.sh", "./killer.sh", arg.c_str(), nullptr) == -1) {
//        throw std::logic_error("exec problem");
//    }
    delete node;
    size--;
}

void Tree::deleteNode(Node *node) {
    for (auto it = node->parent->son.begin(); it != node->parent->son.end(); ++it) {
        if (*it == node) {
            node->parent->son.erase(it);
            return;
        }
    }
    treeDeleter(node);
    size--;
}

Tree::~Tree() noexcept {
    treeDeleter(_root);
}

void Tree::printWithNode(Node *node) {
    if (!node) return;
    std::cout << node->id << std::endl;
    for (auto & n : node->son) {
        printWithNode(n);
    }
}

void Tree::print() {
    printWithNode(_root);
}

bool Tree::idAlreadyExist(int id) {
    return findNode(id, _root);
}

void Tree::insertNode(int id, int parent) {
    Node* father = findNode(parent, _root);
    if (!father || findNode(id, _root))
        return;
    Node* newNode = new Node(id, father);
    father->son.push_back(newNode);
    size++;
}

void Tree::treeToArrayHelper(Node *node, std::vector<int> &vec) {
    if (!node) {
        return;
    }
    vec.push_back(node->id);
    for (auto & child : node->son) {
        treeToArrayHelper(child, vec);
    }
}

std::vector<int> Tree::treeToArray() {
    std::vector<int> array;
    treeToArrayHelper(this->_root, array);
    return array;
}