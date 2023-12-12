#include "tree.h"


Tree::Tree() {
    _root = new Node(-1);
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
    for (auto & n : node->son) {
        treeDeleter(n);
    }
    delete node;
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
}

bool Tree::deleteNode(int id) {
    Node* find = findNode(id, _root);
    if (!find || id == -1) {
        return false;
    }
    std::vector<Node*> children = find->parent->son;
    for (auto it = children.begin(); it != children.end(); ++it) {
        if (*it == find) {
            children.erase(it);
        }
    }
    return true;
}
