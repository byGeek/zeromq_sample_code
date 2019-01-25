#include "zmq.hpp"
#include <iostream>
#include <string>
#include <thread>


int main() {

	zmq::context_t context(1);
	zmq::socket_t client_sock(context, ZMQ_REQ);
	//zmq::socket_t client_sock(context, ZMQ_DEALER);

	client_sock.setsockopt(ZMQ_IDENTITY, "PEER", 4);
	client_sock.connect("tcp://localhost:5555");

	while (1) {
		try {

			std::cout << "\npress enter to send a message" << std::flush;
			std::cin.get();

			zmq::message_t message(5);
			std::string s("hello");

			memcpy(message.data(), s.c_str(), s.size());
			client_sock.send(message);
			std::cout << "client send: " << s << std::endl;

			zmq::message_t reply;  //when recv , no need to specify the message size
			client_sock.recv(&reply);
			std::cout << "client recv: " << std::string(static_cast<char*>(reply.data()), reply.size()) << std::endl;
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			break;
		}
	}

	std::cin.get();
	return 0;
}