#include "ZeroMQ_API/ZeroMQ_API.h"
#include "Tree/tree.h"
#include <unistd.h>
#include <string>

bool compare(const char arr[], const char * t) {
    int index = 0;
    while (arr[index] != '\0' && t[index] != '\0') {
        if (arr[index] != t[index]) {
            return false;
        }
        index++;
    }
    if (arr[index] != t[index]) {
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    int tempId = std::stoi(argv[1]);
    zmq::context_t ctx;
    zmq::socket_t parentSocket(ctx, zmq::socket_type::rep);
    ZMQ::API::connect(parentSocket, tempId);
    parentSocket.set(zmq::sockopt::sndtimeo, 5000);

    while (true) {
        MessageData * messageData = receiveMessageData(parentSocket);
        if (compare(messageData->cmd, "pid")) {
            auto msg = new MessageData;
            memcpy(msg->cmd, "OK\0", strlen("OK\0") + 1);
            sendMessageData<MessageData>(parentSocket, &*msg);
            delete msg;
        }
        if (compare(messageData->cmd, "create")) {
            auto createData = reinterpret_cast<CreateData*>(messageData);
            zmq::socket_t newSocket(ctx, zmq::socket_type::req);
            ZMQ::API::bind(newSocket, createData->newId);
            pid_t pid = fork();
            if (pid == -1) {
                throw std::logic_error("fork problem");
            } else if (pid == 0) {
                if (execl("./compute", "./compute", std::to_string(createData->newId).c_str(), nullptr) == -1) {
                    throw std::logic_error("exec problem");
                }
            } else {
                auto msg = new MessageData;
                memcpy(msg->cmd, ("OK:" + std::to_string(getpid()) + '\0').c_str(), strlen(("OK:" + std::to_string(getpid()) + '\0').c_str()) + 1);
                sendMessageData<MessageData>(parentSocket, &*msg);
                delete msg;
            }
        } else if (compare(messageData->cmd, "exec")) {
            auto execData = reinterpret_cast<ExecData*>(messageData);
            if (compare(execData->subcmd, "time")) {
                double time = execData->node->timer.timeNow();
                std::string stringTime = std::to_string(time);
                auto msg = new MessageData;
                memcpy(msg->cmd, ("OK:" + std::to_string(execData->execNodeId) + ':' + stringTime + '\0').c_str(),
                          strlen(("OK:" + std::to_string(execData->execNodeId) + ':' + stringTime + '\0').c_str()) + 1);
                sendMessageData<MessageData>(parentSocket, &*msg);
                delete msg;
            } else if (compare(execData->subcmd, "start")) {
                execData->node->timer.timeStart();
                auto msg = new MessageData;
                memcpy(msg->cmd, ("OK:" + std::to_string(execData->execNodeId) + '\0').c_str(),
                       strlen(("OK:" + std::to_string(execData->execNodeId) + '\0').c_str()) + 1);
                sendMessageData<MessageData>(parentSocket, &*msg);
                delete msg;
            } else if (compare(execData->subcmd, "stop")) {
                execData->node->timer.timeStop();
                auto msg = new MessageData;
                memcpy(msg->cmd, ("OK:" + std::to_string(execData->execNodeId) + '\0').c_str(),
                       strlen(("OK:" + std::to_string(execData->execNodeId) + '\0').c_str()) + 1);
                sendMessageData<MessageData>(parentSocket, &*msg);
                delete msg;
            }
        } else if (compare(messageData->cmd, "exit")) {

        } else if (compare(messageData->cmd, "kill")) {

        } else if (compare(messageData->cmd, "pingall")) {

        } else {
            std::cout << "Invalid argument" << std::endl;
        }
    }

    return 0;
}