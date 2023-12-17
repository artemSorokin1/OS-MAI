#pragma once
#include <iostream>
#include <vector>
#include "../Timer/TimerManager.h"
#include <unistd.h>
#include <deque>

struct Node {
    int id;
    std::vector<Node*> son;
    Node *parent;
    LocalTimer timer;
    pid_t pid;
    explicit Node(int id = 0, Node* parent = nullptr) : id(id), parent(parent), son(std::vector<Node*>(0)), timer() {}

};

class Tree {
public:
    Node *_root;
    int size;

    bool idAlreadyExist(int id);
    Node *findNode(const int id, Node * node) const;
    void treeDeleter(Node * node);
    void printWithNode(Node * node);
public:
    Tree();
    ~Tree() noexcept;
    Tree(const Tree & tree) = delete;
    Tree operator=(Tree & tree) = delete;
    void insertNode(int id, int parent = -1);
    void print();
    void deleteNode(Node* node);

};


