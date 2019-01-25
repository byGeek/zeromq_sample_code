#include <iostream>
#include <string>

#include "zmq.hpp"


int main() {

	zmq::context_t context(1);
	//zmq::socket_t server_sock(context, ZMQ_ROUTER);
	zmq::socket_t server_sock(context, ZMQ_REP);

	server_sock.bind("tcp://*:5555");
	std::cout << "server listening on port 5555" << std::endl;

	while (1) {
		try {

			zmq::message_t message;

			server_sock.recv(&message);
			std::cout << "\nserver recv: " << std::string(static_cast<char*>(message.data()), message.size()) << std::endl;

			zmq::message_t reply(5);
			std::string data("world");

			memcpy(reply.data(), data.c_str(), data.size());
			server_sock.send(reply);
			std::cout << "server send: " << data << std::endl;
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			break;
		}
	}

	std::cin.get();
	return 0;
}