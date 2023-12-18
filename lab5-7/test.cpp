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
std::string addr = "tcp://127.0.0.1:5252";

int main() {

    zmq::context_t ctx;
    zmq::socket_t socket1(ctx, zmq::socket_type::pair);
    socket1.bind(addr);
    zmq::socket_t socket1c(ctx, zmq::socket_type::pair);
    socket1c.connect(addr);

    std::vector<std::pair<int, zmq::socket_t*>> vec;
    vec.push_back({1, &socket1});
    auto msg = new MessageDataNew;
    msg->setCmd("Hello");
    sendMessageData<MessageDataNew>(*vec[0].second, msg);
    auto reply = receiveMessageData(socket1c);
    std::cout << reply->cmd << std::endl;
    delete msg;

    socket1.unbind(addr);
    socket1c.disconnect(addr);
    return 0;
}
