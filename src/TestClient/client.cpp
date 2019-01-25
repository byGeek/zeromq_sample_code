#include <zmq.hpp>
#include <iostream>
#include <string>

int main() {

	zmq::context_t context(1);
	zmq::socket_t recv_sock(context, ZMQ_SUB);
	zmq::socket_t send_sock(context, ZMQ_REQ);
	//zmq::socket_t send_sock(context, ZMQ_DEALER);

	recv_sock.setsockopt(ZMQ_SUBSCRIBE, "", 0);  //subscribe

	recv_sock.connect("tcp://localhost:5555");  //connect to pub
	send_sock.connect("tcp://localhost:5556");  //connect to rep server

	char data[10] = "hello";

	while (1) {


		std::cout << "press enter to send a message!" << std::endl;
		std::cin.get();


		zmq::message_t msg(10);
		memcpy(msg.data(), data, 10);

		//todo: add exception handling
		send_sock.send(msg);

		std::cout << "client send: " << data << std::endl;

		/*
		//msg.rebuild();
		zmq::message_t recv_data(10);

		recv_sock.recv(&recv_data);

		std::cout << "client recv: " << static_cast<char*>(recv_data.data()) << std::endl;
		*/
	}



	return 0;
}