#ifndef __CONTENT_H__
#define __CONTENT_H__

#include <map>
#include <set>
#include <string>

class ContentManager
{
public:
	static ContentManager& instance();

	std::string getFileIconName(const std::string& fileName) const;

	void setIconForFile(const std::string& fileName, int icon);
	void removeFileIcon(const std::string& fileName);
	int registerIcon(const std::string& fileName);
	void unregisterIcon(int iconId);

	void enableOverlays(bool enable);
	bool isOverlaysEnabled();

	void setMenuTitle(const std::string& title);
	const std::string& getMenuTitle() const;
private:
	std::map<std::string, int> iconsForFiles_;
	int lastIconId_;
	std::map<int, std::string> icons_;

	bool overlaysEnabled_;
	std::string menuTitle_;
	mutable std::set<std::string> registeredFolders_;
	
	ContentManager();
	~ContentManager();
};

#endif
