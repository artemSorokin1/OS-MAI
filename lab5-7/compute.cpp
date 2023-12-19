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
    std::vector<std::pair<int, zmq::socket_t>> children;

    while (true) {
        MessageDataNew * messageData = receiveMessageData(parentSocket);
        if (compare(messageData->cmd, "pid")) {
            auto MDN = new MessageDataNew;
            MDN->setCmd("OK");
            sendMessageData<MessageDataNew>(parentSocket, MDN);
            delete MDN;
            MDN = nullptr;
            continue;
        } else if (compare(messageData->cmd, "create")) {
            if (messageData->path[messageData->index] == -1) {
                pid_t pid = fork();
                if (pid == -1) {
                    throw std::logic_error("fork problem");
                } else if (pid == 0) {
                    if (execl("./compute", "./compute", std::to_string(messageData->id).c_str(), nullptr) == -1) {
                        throw std::logic_error("exec problem");
                    }
                    return 0;
                } else {
                    zmq::socket_t newSocket(ctx, zmq::socket_type::req);
                    ZMQ::API::bind(newSocket, messageData->id);
                    newSocket.set(zmq::sockopt::sndtimeo, 5000);
                    children.emplace_back(messageData->id, std::move(newSocket));
                    auto msg = new MessageDataNew;
                    msg->setCmd("OK:" + std::to_string(getpid()));
                    sendMessageData<MessageDataNew>(parentSocket, msg);
                    delete msg;
                    msg = nullptr;
                    continue;
                }
            } else {
                int id = messageData->path[messageData->index];
                messageData->index++;
                for (auto & soc : children) {
                    if (id == soc.first) {
                        sendMessageData<MessageDataNew>(soc.second, messageData);
                        sendMessageData<MessageDataNew>(parentSocket, receiveMessageData(soc.second));
                        break;
                    }
                }
            }
        } else if (compare(messageData->cmd, "exec")) {
            if (messageData->path[messageData->index] == -1) {
                if (compare(messageData->subcmd, "time")) {
                    double time = messageData->node->timer.timeNow();
                    std::string stringTime = std::to_string(time);
                    auto msg = new MessageDataNew;
                    msg->setCmd("OK:" + std::to_string(messageData->id) + ":" + stringTime);
                    sendMessageData<MessageDataNew>(parentSocket, msg);
                    delete msg;
                    msg = nullptr;
                } else if (compare(messageData->subcmd, "start")) {
                    messageData->node->timer.timeStart();
                    auto msg = new MessageDataNew;
                    msg->setCmd("OK:" + std::to_string(messageData->id));
                    sendMessageData<MessageDataNew>(parentSocket, msg);
                    delete msg;
                    msg = nullptr;
                } else if (compare(messageData->subcmd, "stop")) {
                    messageData->node->timer.timeStop();
                    auto msg = new MessageDataNew;
                    msg->setCmd("OK:" + std::to_string(messageData->id));
                    sendMessageData<MessageDataNew>(parentSocket, msg);
                    delete msg;
                    msg = nullptr;
                }
            } else {
                int id = messageData->path[messageData->index];
                messageData->index++;
                for (auto & soc : children) {
                    if (id == soc.first) {
                        sendMessageData<MessageDataNew>(soc.second, messageData);
                        sendMessageData<MessageDataNew>(parentSocket, receiveMessageData(soc.second));
                        break;
                    }
                }
            }
        } else if (compare(messageData->cmd, "exit")) {
            // unbind -> kill
        } else if (compare(messageData->cmd, "kill")) {

        } else if (compare(messageData->cmd, "pingall")) {
            // обрабатывать все это в блоке try и если прилетит исключение - добавлять в массив ид ноды и всех ее детей
        }
    }

    return 0;
}
