#include "zmq.hpp"
#include <iostream>
#include <string>
#include "zhelpers.hpp"


int main() {

	/*
	zeromq: dealer-router pattern
	*/


	zmq::context_t context(1);
	zmq::socket_t client_sock(context, ZMQ_DEALER);

	client_sock.setsockopt(ZMQ_IDENTITY, "PEER", 4);  //set identity
	client_sock.connect("tcp://localhost:5555");

	while (1) {
		try {

			std::cout << "\npress enter to send a message" << std::flush;
			std::cin.get();

			std::string s("hello");

			s_sendmore(client_sock, "");  //send empty frame to emulate request socket?
			s_send(client_sock, s);  //send actual message body

			std::cout << "client send: " << s << std::endl;

			//std::string identity = s_recv(client_sock);
			s_recv(client_sock);  //recv empty frame
			std::string reply = s_recv(client_sock);  //recv message body

			std::cout << "client recv: " << reply << std::endl;
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			break;
		}
	}

	std::cin.get();
	return 0;
}