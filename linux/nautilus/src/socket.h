#ifndef __SOCKET_SERVER_H__
#define __SOCKET_SERVER_H__

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

struct ISocketCallback
{
	virtual void onStringReceived(int serverId, const std::string& text) = 0;
};

class SocketServer
{
public:
	SocketServer(int id, unsigned short port, ISocketCallback* callback);
	virtual ~SocketServer();

	void writeString(const std::string& data);
	bool readString(std::string& data);

	bool isConnected();
private:
	int id_;

	pthread_t acceptThread_;
	pthread_t readThread_;

	int serverSocket_;
	int clientSocket_;

	ISocketCallback* callback_;
	unsigned short port_;

	void startListening();

	static void* acceptHandler(void* param);
	void doAcceptLoop();

	static void* readHandler(void* param);
	void doReadLoop();
};

#endif
