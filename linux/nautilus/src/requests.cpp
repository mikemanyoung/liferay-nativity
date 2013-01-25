#include "requests.h"
#include "logger.h"
#include <boost/algorithm/string.hpp>

RequestManager::RequestManager() :
	callbackSocket_(2, 33002, NULL),
	commandSocket_(1,33001,this)
{
}

RequestManager::~RequestManager()
{
}

RequestManager& RequestManager::instance()
{
	static RequestManager inst;
	return inst;
}

std::string RequestManager::queryMenuItems(const std::string& request)
{
	writeLog("Request menu items: %s\n", request.c_str());

	if (!callbackSocket_.isConnected())
	{
		writeLog("Client is not connected\n");
		return std::string();
	}

	callbackSocket_.writeString(request);

	std::string result;
	callbackSocket_.readString(result);

	return result;
}

void RequestManager::menuExecuted(const std::string& reply)
{
	if (callbackSocket_.isConnected())
		callbackSocket_.writeString(reply);
}

void RequestManager::onStringReceived(int serverId, const std::string& text)
{
	if (serverId == 1)
	{
		std::vector<std::string> itemsArray;
		boost::split(itemsArray, text, boost::is_any_of(":"));

		if (itemsArray.empty())
			return;

		std::string cmdId(itemsArray[0]);

		writeLog("Command received: %s\n", cmdId.c_str());

		if (cmdId == "setFileIcon")
			execSetFileIconCmd(itemsArray);
		else if (cmdId == "setFileIcons")
			execSetFileIconsCmd(itemsArray);
		else if (cmdId == "removeFileIcon")
			execRemoveFileIconCmd(itemsArray);
		else if (cmdId == "removeFileIcons")
			execRemoveFileIconsCmd(itemsArray);
		else if (cmdId == "enableOverlays")
			execEnableOverlaysCmd(itemsArray);
		else if (cmdId == "registerIcon")
			execRegisterIconCmd(itemsArray);
		else if (cmdId == "unregisterIcon")
			execUnregisterIconCmd(itemsArray);
		else if (cmdId == "setMenuTitle")
			execSetMenuTitleCmd(itemsArray);
	}
}

void RequestManager::execSetFileIconCmd(const std::vector<std::string>& cmdData)
{
	commandSocket_.writeString("1");
}

void RequestManager::execSetFileIconsCmd(const std::vector<std::string>& cmdData)
{
	commandSocket_.writeString("1");
}

void RequestManager::execRemoveFileIconCmd(const std::vector<std::string>& cmdData)
{
	commandSocket_.writeString("1");
}

void RequestManager::execRemoveFileIconsCmd(const std::vector<std::string>& cmdData)
{
	commandSocket_.writeString("1");
}

void RequestManager::execEnableOverlaysCmd(const std::vector<std::string>& cmdData)
{
	commandSocket_.writeString("1");
}

void RequestManager::execRegisterIconCmd(const std::vector<std::string>& cmdData)
{
	commandSocket_.writeString("1");
}

void RequestManager::execUnregisterIconCmd(const std::vector<std::string>& cmdData)
{
	commandSocket_.writeString("1");
}

void RequestManager::execSetMenuTitleCmd(const std::vector<std::string>& cmdData)
{
	commandSocket_.writeString("1");
}

