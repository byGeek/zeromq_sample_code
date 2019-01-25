
#include "zmq.hpp"
#include <iostream>

int main() {
	zmq::context_t context(1);

	zmq::socket_t proxy_sock(context, ZMQ_PUB);
	//zmq::socket_t server_sock(context, ZMQ_ROUTER);
	zmq::socket_t server_sock(context, ZMQ_REP);

	proxy_sock.bind("tcp://*:5555");  //pub data to clients
	server_sock.bind("tcp://*:5556");  //receive data from client

	//server_sock.setsockopt(ZMQ_SUBSCRIBE, "", 0);  //subsribe on everything client send here

	//todo: add exception and handle error and interrupt signal


	//todo: in REQ-REP pattern, can not recv more data before reply
	while (1) {
		zmq::message_t message;
		try {
			server_sock.recv(&message);  //receive envelop delimitter, an empty frame
			server_sock.recv(&message);  //data from client
			std::cout << "server receive: " << static_cast<char*>(message.data()) << std::endl;

			//zmq::message_t pub_data(10);
			//memcpy(pub_data.data(), message.data(), 10);
			//proxy_sock.send(message);  //publish the received data
		}
		catch (zmq::error_t& e) {
			std::cout << "exception occured, errno: " << zmq_errno() << ", msg: " <<
				zmq_strerror(zmq_errno()) <<std::endl;
			break;
		}
		
	}

	std::cin.get();

	return 0;
}