#include "ZeroMQ_API/ZeroMQ_API.h"
#include <unistd.h>


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
        zmq::socket_t mainSocket(ctx, zmq::socket_type::pair);
        ZMQ::API::bind(mainSocket, mainId);
        mainSocket.set(zmq::sockopt::sndtimeo, 5000);

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
                    std::cout << "Node already exsist\n";
                    continue;
                }
                if (!tree.idAlreadyExist(parent)) {
                    std::cout << "No fathers node\n";
                    continue;
                }
                // нахожу путь до родительской ноды и отправляю его вместе с сообщением
                std::vector<std::pair<int, int>> routes(tree.size);
                int index = 1;
                findPathHelper(tree, tree._root, routes, index);
                std::deque<int> path = findPath(parent, routes);
                if (parent == -1) {
                    parent = 1;
                }
                auto MDN = new MessageDataNew;
                MDN->id = newId;
                MDN->parentId = parent;
                MDN->path = path;
                MDN->setCmd("create");
                sendMessageData<MessageDataNew>(mainSocket, MDN);
                const MessageDataNew* MDN_req = receiveMessageData(mainSocket);
                std::cout << MDN_req->cmd << std::endl;
                if (parent == 1) {
                    tree.insertNode(newId, -1);
                } else {
                    tree.insertNode(newId, parent);
                }
//                tree.print();
                delete MDN;
            } else if (inputCommand == "exit") {
//                auto killData = new KillData;
//                memcpy(killData->cmd, "kill\0", strlen("kill\0") + 1);
//                killData->id = mainId;
//                sendMessageData<KillData>(mainSocket, &*killData);
//                const MessageData * md = receiveMessageData(mainSocket);
//                bool equal = strcmp(md->cmd, "OK");
//                if (equal) {
//                    ZMQ::API::unbind(mainSocket, mainId);
//                } else {
//                    std::cout << md->cmd << std::endl;
//                }
//                delete killData;
//                exit(0);
            } else if (inputCommand == "kill") {
//                int id;
//                std::cin >> id;
//                if (!tree.idAlreadyExist(id)) {
//                    std::cout << "id already exist\n";
//                } else {
//                    auto kd = new KillData;
//                    memcpy(kd->cmd, "kill\0", strlen("kill\0") + 1);
//                    kd->id = id;
//                    sendMessageData<KillData>(mainSocket, &*kd);
//                    const MessageData * md = receiveMessageData(mainSocket);
//                    std::cout << md->cmd << std::endl;
//                    if (id == 1) {
//                        ZMQ::API::unbind(mainSocket, mainId);
//                    }
//                    delete kd;
//                }
            } else if (inputCommand == "exec") {
                int id;
                std::cin >> id;
                std::string subcmd;
                std::cin >> subcmd;
                Node* node = tree.findNode(id, tree._root);
                if (!node) {
                    std::cout << "Invalid argument" << std::endl;
                    continue;
                }
                auto MDN_EXEC = new MessageDataNew;
                MDN_EXEC->setCmd("exec");
                MDN_EXEC->setSubCmd(subcmd);
                MDN_EXEC->id = id;
                MDN_EXEC->node = node;
                sendMessageData<MessageDataNew>(mainSocket, MDN_EXEC);
                const MessageDataNew * msg = receiveMessageData(mainSocket);
                std::cout << msg->cmd << '\n';
                delete MDN_EXEC;
//                auto execData = new ExecData;
//                execData->node = node;
//                memcpy(execData->cmd, "exec\0", strlen("exec\0") + 1);
//                memcpy(execData->subcmd, (subcmd + '\0').c_str(), strlen((subcmd + '\0').c_str()) + 1);
//                execData->execNodeId = id;
//                zmq::socket_t socketId(ctx, zmq::socket_type::req);
//                ZMQ::API::bind(socketId, id);
//                socketId.set(zmq::sockopt::sndtimeo, 5000);
//                sendMessageData<ExecData>(socketId, &*execData); // OK TODO mainSocket -> socketId отдаю через mq определенному процессу данные, чтобы именно он выполнил программу
//                const MessageData * md = receiveMessageData(socketId);
//                std::cout << md->cmd << std::endl;
//                ZMQ::API::unbind(socketId, id);
            } else if (inputCommand == "pingall") {

            }
        }
    }
}