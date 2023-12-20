#pragma once
#include <iostream>
#include <zmq.hpp>
#include "../Tree/tree.h"

typedef struct MessageDataNew {
    char cmd[128];
    char subcmd[128];
    int path[20];
    int parentId = 0;
    int id = 0;
    Node* node = nullptr;
//    std::deque<int> *path;
    int index = 0;

    MessageDataNew();

    void setCmd(std::string && s) {
        memcpy(cmd, (s + '\0').c_str(), strlen((s + '\0').c_str()) + 1);
    }
    void setCmd(const std::string & s) {
        memcpy(cmd, (s + '\0').c_str(), strlen((s + '\0').c_str()) + 1);
    }
    void setSubCmd(std::string && s) {
        memcpy(subcmd, (s + '\0').c_str(), strlen((s + '\0').c_str()) + 1);
    }
    void setSubCmd(const std::string & s) {
        memcpy(subcmd, (s + '\0').c_str(), strlen((s + '\0').c_str()) + 1);
    }

} MessageDataNew;

namespace ZMQ {
    class API {
    public:
        static void sendMessage(zmq::socket_t & socket, const std::string & msg);
        static std::string receiveMessage(zmq::socket_t & socket);
        static void connect(zmq::socket_t & socket, uint16_t id);
        static void disconnect(zmq::socket_t & socket, uint16_t id);
        static void bind(zmq::socket_t & socket, uint16_t id);
        static void unbind(zmq::socket_t & socket, uint16_t id);

    };

}

MessageDataNew* receiveMessageData(zmq::socket_t & socket);

template <class Data>
void sendMessageData(zmq::socket_t &socket, MessageDataNew* md) {
    zmq::message_t message(sizeof(MessageDataNew));
    memcpy(message.data(), md, sizeof(MessageDataNew));
    try {
        socket.send(message, zmq::send_flags::none);
    } catch (const zmq::error_t& e) {
        std::cerr << "Error sending message: " << e.what() << std::endl;
        throw;
    }
}

bool compare(const char arr[], const char * t);




