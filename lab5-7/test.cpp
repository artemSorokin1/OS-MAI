#include "ZeroMQ_API/ZeroMQ_API.h"
#include "Tree/tree.h"

int endpoint(std::string &s) {
    std::string t = "";
    for (int i = s.size() - 1; i >= 0; --i) {
        if (s[i] == ':') {
            break;
        }
        t = s[i] + t;
    }
    return stoi(t);
}
int PORT = 1000;

int main() {

    zmq::context_t ctx;
    zmq::socket_t socket1(ctx, zmq::socket_type::req);
    ZMQ::API::bind(socket1, 101);
    zmq::socket_t socket1c(ctx, zmq::socket_type::rep);
    ZMQ::API::connect(socket1c, 101);
    zmq::socket_t socket2(ctx, zmq::socket_type::req);
    ZMQ::API::bind(socket2, 102);
    zmq::socket_t socket2c(ctx, zmq::socket_type::rep);
    ZMQ::API::connect(socket2c, 102);
    zmq::socket_t socket3(ctx, zmq::socket_type::req);
    ZMQ::API::bind(socket3, 103);
    zmq::socket_t socket3c(ctx, zmq::socket_type::rep);
    ZMQ::API::connect(socket3c, 103);
    auto msg = new MessageDataNew;
    msg->setCmd("OK");
    sendMessageData<MessageDataNew>(socket1, msg);
    msg = receiveMessageData(socket1c);
    sendMessageData<MessageDataNew>(socket2, msg);
    msg = receiveMessageData(socket2c);
    sendMessageData<MessageDataNew>(socket3, msg);
    msg = receiveMessageData(socket3c);
    msg->setCmd("NOK");

    sendMessageData<MessageDataNew>(socket3c, msg);
    receiveMessageData(socket3);
    sendMessageData<MessageDataNew>(socket2c, msg);
    receiveMessageData(socket2);
    sendMessageData<MessageDataNew>(socket1c, msg);
    receiveMessageData(socket1);

    std::cout << msg->cmd << '\n';

    return 0;
}
