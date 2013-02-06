/**
 * Copyright (c) 2000-2012 Liferay, Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 */

#ifndef CONTEXTMENUUTIL_H
#define CONTEXTMENUUTIL_H

#pragma once

#include "NativityJavaInterface.h"

#include <list>
#include <string>
#include <windows.h>

class ContextMenuUtil
{
public:
	ContextMenuUtil();

	~ContextMenuUtil(void);

	int GetActionIndex(std::wstring);

	std::wstring GetHelpText(int);

	std::list<std::wstring>* GetMenus();

	std::wstring GetRootText();

	std::wstring GetVerbText(int);

	bool IsMenuNeeded(void);
	
	bool PerformAction(int);

private:

	std::wstring _GetCommandText(int);

	NativityJavaInterface* _nativityJavaInterface;
	
	std::list<std::wstring>* _selectedFiles;
};

#endif
