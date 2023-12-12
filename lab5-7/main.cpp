#include "ZeroMQ_API/ZeroMQ_API.h"
#include <unistd.h>

int main(int argc, char* argv[]) {
    Tree tree;
    zmq::context_t ctx;
    zmq::socket_t mainSocket(ctx, zmq::socket_type::req);
    mainSocket.set(zmq::sockopt::sndtimeo, 5000);
    int mainId = 1;
    ZMQ::API::bind(mainSocket, mainId);

    pid_t pid = fork();
    if (pid == -1) {
        throw std::logic_error("fork problem");
    } else if (pid == 0) {
        if (execl("./compute", "./compute", std::to_string(mainId).c_str(), nullptr) == -1) {
            throw std::logic_error("exec problem");
        }
    } else {
        auto pd = new PidData;
        memcpy(pd->cmd, "pid\0", strlen("pid\0"));
        pd->newId = mainId;
        sendMessageData<PidData>(mainSocket, &*pd);
        const MessageData * msg = receiveMessageData(mainSocket);
        std::cout << msg->cmd << std::endl;
        delete pd;

        std::string inputCommand;
        while (std::cin >> inputCommand) {
            if (inputCommand == "create") {
                int newId;
                std::cin >> newId;
                if (tree.idAlreadyExist(newId)) {
                    std::cout << "Node already exsist\n";
                    continue;
                }
                auto cd = new CreateData;
                cd->newId = newId;
                cd->parentId = mainId;
                memcpy(cd->cmd, "create\0", strlen("create\0") + 1);
                sendMessageData<CreateData>(mainSocket, &*cd);
                const MessageData* md = receiveMessageData(mainSocket);
                std::cout << md->cmd << std::endl;
                tree.insertNode(newId, -1);
                tree.print();
                delete cd;
            } else if (inputCommand == "exit") {
                auto killData = new KillData;
                memcpy(killData->cmd, "kill\0", strlen("kill\0") + 1);
                killData->id = mainId;
                sendMessageData<KillData>(mainSocket, &*killData);
                const MessageData * md = receiveMessageData(mainSocket);
                bool equal = strcmp(md->cmd, "OK");
                if (equal) {
                    ZMQ::API::unbind(mainSocket, mainId);
                } else {
                    std::cout << md->cmd << std::endl;
                }
                delete killData;
                exit(0);
            } else if (inputCommand == "kill") {
                int id;
                std::cin >> id;
                if (!tree.idAlreadyExist(id)) {
                    std::cout << "id already exist\n";
                } else {
                    auto kd = new KillData;
                    memcpy(kd->cmd, "kill\0", strlen("kill\0") + 1);
                    kd->id = id;
                    sendMessageData<KillData>(mainSocket, &*kd);
                    const MessageData * md = receiveMessageData(mainSocket);
                    std::cout << md->cmd << std::endl;
                    if (id == 1) {
                        ZMQ::API::unbind(mainSocket, mainId);
                    }
                    delete kd;
                    exit(0);
                }
            } else if (inputCommand == "exec") {
                int id;
                std::cin >> id;
                std::string subcmd;
                std::cin >> subcmd;
                Node* node = tree.findNode(id, tree._root);
                if (!node) {
                    std::cout << "Invalid argument" << std::endl;
                }
                auto execData = new ExecData;
                execData->node = node;
                memcpy(execData->cmd, "exec\0", strlen("exec\0") + 1);
                memcpy(execData->subcmd, (subcmd + '\0').c_str(), strlen((subcmd + '\0').c_str()) + 1);
                execData->execNodeId = id;
                sendMessageData<ExecData>(mainSocket, &*execData);
                const MessageData * md = receiveMessageData(mainSocket);
                std::cout << md->cmd << std::endl;
            }
        }
    }
}