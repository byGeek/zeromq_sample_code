#include "zmq.hpp"
#include "zhelpers.hpp"
#include <iostream>
#include <string>


/*
act as a proxy, recv msg from client and proxy msg to other clients

*/

int main() {
	zmq::context_t context(1);
	zmq::socket_t pub_sock(context, ZMQ_PUB);
	zmq::socket_t rep_sock(context, ZMQ_REP);//todo: replace socket type to ZMQ_ROUTER

	pub_sock.bind("tcp://*:5555");
	rep_sock.bind("tcp://*:5556");

	std::cout << "server listen on 5556" << std::endl;

	while (1) {
		try {


			std::string identity = s_recv(rep_sock);
			s_recv(rep_sock);  //empty delimeter
			std::string msg = s_recv(rep_sock);

			std::cout << "server recv: " << msg << std::endl;

			s_send(rep_sock, "");  //reply to client


			//pub a new msg to other client
			s_sendmore(pub_sock, identity);
			s_sendmore(pub_sock, "");
			s_send(pub_sock, msg);
		}
		catch (std::exception& e) {
			std::cout << "app failed with " << zmq_errno() << " : " << zmq_strerror(zmq_errno()) << std::endl;
			break;
		}
	}

	return 0;
}