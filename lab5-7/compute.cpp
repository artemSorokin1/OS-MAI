#include "ZeroMQ_API/ZeroMQ_API.h"
#include "Tree/tree.h"
#include <unistd.h>
#include <string>

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
                        auto reply = receiveMessageData(soc.second);
                        if (!reply) {
                            auto sendToParent = new MessageDataNew;
                            sendToParent->setCmd("Error");
                            sendMessageData<MessageDataNew>(parentSocket, sendToParent);
                            delete sendToParent;
                            break;
                        } else {
                            sendMessageData<MessageDataNew>(parentSocket, reply);
                            break;
                        }
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
                        auto reply = receiveMessageData(soc.second);
                        if (!reply) {
                            auto sendToParent = new MessageDataNew;
                            sendToParent->setCmd("Error");
                            sendMessageData<MessageDataNew>(parentSocket, sendToParent);
                            delete sendToParent;
                            break;
                        } else {
                            sendMessageData<MessageDataNew>(parentSocket, reply);
                            break;
                        }
                    }
                }
            }
        } else if (compare(messageData->cmd, "exit")) {
            // unbind -> kill
        } else if (compare(messageData->cmd, "kill")) {

        } else if (compare(messageData->cmd, "pingall")) {
            if (messageData->path[messageData->index] == -1) {
                auto msg = new MessageDataNew;
                msg->setCmd("Exist");
                sendMessageData<MessageDataNew>(parentSocket, msg);
                delete msg;
                msg = nullptr;
            } else {
                int id = messageData->path[messageData->index];
                messageData->index++;
                for (auto & soc : children) {
                    if (id == soc.first) {
                        sendMessageData<MessageDataNew>(soc.second, messageData);
                        auto reply = receiveMessageData(soc.second);
                        if (!reply) {
                            auto sendToParent = new MessageDataNew;
                            sendToParent->setCmd(std::to_string(messageData->id));
                            sendMessageData<MessageDataNew>(parentSocket, sendToParent);
                            delete sendToParent;
                            break;
                        } else {
                            sendMessageData<MessageDataNew>(parentSocket, reply);
                            break;
                        }
                    }
                }
            }
        }
    }

    return 0;
}
