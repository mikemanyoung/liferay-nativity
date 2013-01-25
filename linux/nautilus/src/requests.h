#ifndef __REQUESTS_H__
#define __REQUESTS_H__

#include "socket.h"
#include <string>
#include <vector>

class RequestManager :
	public ISocketCallback
{
public:
	static RequestManager& instance();

	std::string queryMenuItems(const std::string& request);
	void menuExecuted(const std::string& reply);

protected:
	virtual void onStringReceived(int serverId, const std::string& text);
private:
	SocketServer callbackSocket_;
	SocketServer commandSocket_;

	RequestManager();
	~RequestManager();

	void execSetFileIconCmd(const std::vector<std::string>& cmdData);
	void execSetFileIconsCmd(const std::vector<std::string>& cmdData);
	void execRemoveFileIconCmd(const std::vector<std::string>& cmdData);
	void execRemoveFileIconsCmd(const std::vector<std::string>& cmdData);
	void execEnableOverlaysCmd(const std::vector<std::string>& cmdData);
	void execRegisterIconCmd(const std::vector<std::string>& cmdData);
	void execUnregisterIconCmd(const std::vector<std::string>& cmdData);
	void execSetMenuTitleCmd(const std::vector<std::string>& cmdData);
};

#endif
