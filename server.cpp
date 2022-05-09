#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read

// struct sockaddr_in {
//    sa_family_t    sin_family; /* address family: AF_INET */
//    in_port_t      sin_port;   /* port in network byte order */
//    struct in_addr sin_addr;   /* internet address */
// };

int main(int argc, char **argv) {

	if (argc != 2) {
		std::cout << "Error\nExecutable run as follows:\n\"./webserv [configuration file]\"" << std::endl;
		exit(EXIT_FAILURE);
	}
	(void)argv;
	// Create a socket (IPv4, TCP)
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		std::cout << "Failed to create socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	// Listen to port 9999 on any address
	int port = 9999;
	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_port = htons(port); // convert a number to network byte order
	if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
		std::cout << "Failed to bind to port " << port << ". errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	// 10 is the maximum number of connections that will be queued before connections start being refused
	if (listen(sockfd, 10) < 0) {
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	// Grab a connection from the queue
	socklen_t addrlen = sizeof(sockaddr); // not sure if int or socklen_t
	int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
	if (connection < 0) {
		std::cout << "Failed to connect. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	// Read from the connection
	char buffer[100] = {0};
	read(connection, buffer, 100);
	std::cout << "Request: " << buffer;

	// Send a message to the connection
	std::string response = "Server response: fuck off\n";
	send(connection, response.c_str(), response.size(), 0);

	// Close the connections
	close(connection);
	close(sockfd);
}