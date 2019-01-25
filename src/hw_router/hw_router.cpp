#include <iostream>
#include <string>

#include "zmq.hpp"
#include "zhelpers.hpp"


int main() {
	/*
	zeromq: dealer-router pattern
	*/


	zmq::context_t context(1);
	zmq::socket_t server_sock(context, ZMQ_ROUTER);

	server_sock.bind("tcp://*:5555");
	std::cout << "server listening on port 5555" << std::endl;

	while (1) {
		try {

			std::string identity = s_recv(server_sock);  //identity
			s_recv(server_sock);  //empty frame
			std::string message = s_recv(server_sock);

			std::cout << "\nserver recv: " << message << std::endl;

			/*
			router socket send three frame
			1. identity frame
			2. empty envelop delimeter
			3. message body
			*/
			s_sendmore(server_sock, identity);// send identity
			s_sendmore(server_sock, "");

			std::string reply("world");
			s_send(server_sock, "world");
			
			std::cout << "server send: " << reply << std::endl;
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
			break;
		}
	}

	std::cin.get();
	return 0;
}