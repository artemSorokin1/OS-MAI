#include "ZeroMQ_API.h"

std::string ip = "tcp://127.0.0.1:";

void ZMQ::API::sendMessage(zmq::socket_t &socket,
                           const std::string &msg) {
    zmq::message_t message(msg.size());
    memcpy(message.data(), msg.c_str(), msg.size());
    socket.send(message);
}

std::string ZMQ::API::receiveMessage(zmq::socket_t & socket) {
    zmq::message_t message;
    bool request;
    try {
        request = socket.recv(&message);
    } catch (...) {
        request = false;
    }
    if (!request)
        throw std::logic_error("Problem with receive message");
    std::string result(static_cast<char*>(message.data()), message.size());
    return result;
}

void ZMQ::API::connect(zmq::socket_t &socket,
                       uint16_t id) {
    std::string address = ip + std::to_string(8000 + id);
    socket.connect(address);
}

void ZMQ::API::disconnect(zmq::socket_t & socket,
                       uint16_t id) {
    std::string address = ip + std::to_string(8000 + id);
    socket.disconnect(address);
}

void ZMQ::API::bind(zmq::socket_t & socket,
                    uint16_t id) {
    std::string address = ip + std::to_string(8000 + id);
    socket.bind(address);
}

void ZMQ::API::unbind(zmq::socket_t & socket,
                    uint16_t id) {
    std::string address = ip + std::to_string(8000 + id);
    socket.unbind(address);
}

MessageData* receiveMessageData(zmq::socket_t & socket) {
    zmq::message_t message;
    bool request;
    try {
        request = socket.recv(&message);
    } catch (...) {
        request = false;
    }
    if (!request)
        throw std::logic_error("Problem with receive message");
    auto md = (MessageData*)message.data();
    return md;
}

