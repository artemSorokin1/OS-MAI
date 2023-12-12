#pragma once
#include <iostream>
#include <zmq.hpp>
#include "../Tree/tree.h"

typedef struct MD {
    char cmd[128];
} MessageData;

typedef struct CD : public MessageData {
    int parentId;
    int newId;
} CreateData;

typedef struct PD : public MessageData {
    int newId;
} PidData;

typedef struct ExitD : public MessageData {} ExitData;

typedef struct ExecD : public MessageData {
    int execNodeId;
    char subcmd[16];
    Node* node;
} ExecData;

typedef struct KD : public MessageData {
    int id;
} KillData;

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

MessageData* receiveMessageData(zmq::socket_t & socket);

template <class Data>
void sendMessageData(zmq::socket_t &socket, MessageData* md) {
    zmq::message_t message(sizeof(Data));
    memcpy(message.data(), md, sizeof(Data));
    socket.send(message);
}

