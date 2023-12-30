#include "ZeroMQ_API/ZeroMQ_API.h"
#include <unistd.h>
#include <set>

void findPathHelper(const Tree & tree, Node* curNode, std::vector<std::pair<int, int>> & routes, int &index) {
    routes[0] = {-1, tree._root->id};
    if (!curNode) {
        return;
    }
    for (auto & n : curNode->son) {
        for (int i = 0; i < (int)routes.size(); ++i) {
            if (routes[i].second == n->parent->id) {
                routes[index] = std::make_pair(i, n->id);
                index++;
                break;
            }
        }
        findPathHelper(tree, n, routes, index);
    }
}

std::deque<int> findPath(int id, std::vector<std::pair<int, int>> & routes) {
    std::deque<int> path;
    std::pair<int, int> start = {0, 0};
    for (int i = 0; i < (int)routes.size(); ++i) {
        if (routes[i].second == id) {
            start = routes[i];
        }
    }
    while (start.first != -1) {
        path.push_front(start.second);
        start = routes[start.first];
    }
    return path;
}

int main(int argc, char* argv[]) {
    int mainId = 1;
    pid_t pid = fork();
    if (pid == -1) {
        throw std::logic_error("fork problem");
    } else if (pid == 0) {
        if (execl("./compute", "./compute", std::to_string(mainId).c_str(), nullptr) == -1) {
            throw std::logic_error("exec problem");
        }
    } else {
        Tree tree;
        zmq::context_t ctx;
        zmq::socket_t mainSocket(ctx, zmq::socket_type::req);
        ZMQ::API::bind(mainSocket, mainId);
        mainSocket.set(zmq::sockopt::sndtimeo, 2000);

        auto MDN_pid = new MessageDataNew;
        MDN_pid->setCmd("pid");
        MDN_pid->id = mainId;
        sendMessageData<MessageDataNew>(mainSocket, MDN_pid);
        const MessageDataNew * MDN_req_pid = receiveMessageData(mainSocket);
        std::cout << MDN_req_pid->cmd << std::endl;
        delete MDN_pid;

        std::string inputCommand;
        while (std::cin >> inputCommand) {
            if (inputCommand == "create") {
                int newId;
                std::cin >> newId;
                std::cout << "Введеите родителя, или -1\n";
                int parent;
                std::cin >> parent;
                if (tree.idAlreadyExist(newId)) {
                    std::cout << "Already exist\n";
                    continue;
                }
                if (!tree.idAlreadyExist(parent)) {
                    std::cout << "Parent not found\n";
                    continue;
                }
                std::vector<std::pair<int, int>> routes(tree.size);
                int index = 1;
                findPathHelper(tree, tree._root, routes, index);
                std::deque<int> p = findPath(parent, routes);
                if (parent == -1) {
                    parent = 1;
                }

                if (parent == 1) {
                    tree.insertNode(newId, -1);
                } else {
                    tree.insertNode(newId, parent);
                }
                Node* node = tree.findNode(newId, tree._root);

                auto MDN = new MessageDataNew;
                MDN->id = newId;
                MDN->node = node;
                MDN->parentId = parent;
                for (int i = 0; i < p.size(); ++i) {
                    MDN->path[i] = p[i];
                }
                MDN->setCmd("create");
                sendMessageData<MessageDataNew>(mainSocket, MDN);
                const MessageDataNew* MDN_req = receiveMessageData(mainSocket);
                std::cout << MDN_req->cmd << std::endl;
//                tree.print();
                delete MDN;
                MDN = nullptr;
            } else if (inputCommand == "exec") {
                int id;
                std::cin >> id;
                std::string subcmd{""};
                std::cin >> subcmd;
                Node* node = tree.findNode(id, tree._root);
                if (!node) {
                    std::cout << "Invalid argument" << std::endl;
                    continue;
                }
                std::vector<std::pair<int, int>> routes(tree.size);
                int index = 1;
                findPathHelper(tree, tree._root, routes, index);
                std::deque<int> p = findPath(id, routes);
                auto MDN_EXEC = new MessageDataNew;
                MDN_EXEC->setCmd("exec");
                MDN_EXEC->setSubCmd(subcmd);
                MDN_EXEC->id = id;
                MDN_EXEC->node = node;
                for (int i = 0; i < p.size(); ++i) {
                    MDN_EXEC->path[i] = p[i];
                }
                sendMessageData<MessageDataNew>(mainSocket, MDN_EXEC);
                const MessageDataNew * msg = receiveMessageData(mainSocket);
                std::cout << msg->cmd << '\n';
                delete MDN_EXEC;
            } else if (inputCommand == "pingall") {
                std::vector<int> treeArray;
                treeArray = tree.treeToArray();
                std::vector<int> result;
                for (auto & id : treeArray) {
                    auto MDN_ping = new MessageDataNew;
                    MDN_ping->setCmd("pingall");
                    std::vector<std::pair<int, int>> routes(tree.size);
                    int index = 1;
                    findPathHelper(tree, tree._root, routes, index);
                    std::deque<int> p = findPath(id, routes);
                    for (int i = 0; i < p.size(); ++i) {
                        MDN_ping->path[i] = p[i];
                    }
                    MDN_ping->id = id;
                    sendMessageData<MessageDataNew>(mainSocket, MDN_ping);
                    const MessageDataNew * msg = receiveMessageData(mainSocket);
                    if (!compare(msg->cmd, "Exist")) {
                        result.push_back(std::stoi(msg->cmd));
                    }
                    delete MDN_ping;
                }
                if (result.empty()) {
                    std::cout << -1;
                } else {
                    for (auto & elem : result) {
                        std::cout << elem << ' ';
                    }
                }
                std::cout << std::endl;
            }
        }
    }
}
