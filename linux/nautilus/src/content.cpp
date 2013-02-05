#include "content.h"

ContentManager::ContentManager() :
	lastIconId_(0),
	overlaysEnabled_(false),
	menuTitle_("Liferay")
{
}

ContentManager::~ContentManager()
{
}

ContentManager& ContentManager::instance()
{
	static ContentManager inst;
	return inst;
}

std::string ContentManager::getFileIconName(const std::string& fileName) const
{
	std::map<std::string, int>::const_iterator itIcon = iconsForFiles_.find(fileName);
	if (itIcon == iconsForFiles_.end())
		return "";

	std::map<int, std::string>::const_iterator itName = icons_.find(itIcon->second);
	if (itName == icons_.end())
		return "";

	return itName->second;
}

void ContentManager::setIconForFile(const std::string& fileName, int icon)
{
	iconsForFiles_[fileName] = icon;
}

void ContentManager::removeFileIcon(const std::string& fileName)
{
	iconsForFiles_.erase(fileName);
}

int ContentManager::registerIcon(const std::string& fileName)
{
	lastIconId_++;
	icons_[lastIconId_] = fileName;
}

void ContentManager::unregisterIcon(int iconId)
{
	icons_.erase(iconId);
}

void ContentManager::enableOverlays(bool enable)
{
	overlaysEnabled_ = enable;
}

bool ContentManager::isOverlaysEnabled()
{
	return overlaysEnabled_;
}

void ContentManager::setMenuTitle(const std::string& title)
{
	menuTitle_ = title;
}

const std::string& ContentManager::getMenuTitle() const
{
	return menuTitle_;
}

