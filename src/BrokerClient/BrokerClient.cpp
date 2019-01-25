#include "zmq.hpp"
#include "zhelpers.hpp"
#include <iostream>
#include <string>


int main(int argc, char* arg[]) {
	if (argc < 2) {
		std::cout << "Usage: brokerclient <identity>" << std::endl;
		return -1;
	}

	std::string identity(arg[1]);  //get identity from command line argument

	zmq::context_t context(1);
	zmq::socket_t req_sock(context, ZMQ_REQ);
	zmq::socket_t sub_sock(context, ZMQ_SUB);

	sub_sock.setsockopt(ZMQ_SUBSCRIBE, "", 0);  //subscribe everything
	sub_sock.connect("tcp://localhost:5555");

	req_sock.connect("tcp://localhost:5556");

	s_sendmore(req_sock, identity);
	s_sendmore(req_sock, "");
	s_send(req_sock, "hello");  //send msg

	std::cout << "client(" << identity << ")" << "send: hello" << std::endl;
	//s_recv(req_sock);  //receive from server

	zmq::pollitem_t items[] = {
				{req_sock, 0, ZMQ_POLLIN, 0},
				{sub_sock, 0, ZMQ_POLLIN, 0}
	};

	while (1) {
		try {

			//std::cout << "\npress enter to send a msg" << std::flush;
			//std::cin.get();
			zmq_poll(items, 2, -1); //timeout is -1 means block infinitely if no event signaled

			if (items[0].revents & ZMQ_POLLIN) {
				std::string address = s_recv(sub_sock);
				std::string delimeter = s_recv(sub_sock);
				std::string msg = s_recv(sub_sock);  //the actual msg here

				if (address != identity
					&& delimeter.size() == 0) {
					//the right format
					//do process here
					std::cout << "client(" << identity << ")  recv: " << msg << std::endl;
				}
			}
			if (items[1].revents & ZMQ_POLLIN) {
				s_recv(req_sock);
			}


			

		}
		catch (std::exception& e) {
			std::cout << "app failed with " << zmq_errno() << ": " << zmq_strerror(zmq_errno()) << std::endl;
			break;
		}
	}

	return 0;

}