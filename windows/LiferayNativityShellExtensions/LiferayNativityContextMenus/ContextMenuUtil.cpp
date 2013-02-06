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

#include "ContextMenuUtil.h"
#include "ContextMenuContants.h"

#include <fstream>

using namespace std;

ContextMenuUtil::ContextMenuUtil()
{
	_nativityJavaInterface = new NativityJavaInterface();
	_selectedFiles = new std::list<std::wstring>;
}

ContextMenuUtil::~ContextMenuUtil(void)
{
	_selectedFiles = 0;
}

int ContextMenuUtil::GetActionIndex(std::wstring command)
{
	list<wstring>::iterator commandIterator = _selectedFiles->begin();

	int index = 0;

	while(commandIterator != _selectedFiles->end()) 
	{
		wstring commandName = *commandIterator;

		if(commandName.compare(command) == 0)
		{
			return index;
		}

		commandIterator++;
		index++;
	}

	return -1;
}

wstring ContextMenuUtil::GetHelpText(int index)
{
	//Currently no help text, so use command text.

	return _GetCommandText(index);
}

list<std::wstring>* ContextMenuUtil::GetMenus()
{
}

wstring ContextMenuUtil::GetRootText()
{
	wstring rootMenu = _nativityJavaInterface->PerformQuery(GET_ROOT_TEXT);

	return rootMenu;
}

wstring ContextMenuUtil::GetVerbText(int index)
{
	//Currently no verb text, so just using command text.

	return _GetCommandText(index);
}

bool ContextMenuUtil::IsMenuNeeded(void)
{  
	if(_nativityJavaInterface->IsSyncFile(_selectedFiles))
	{
		return true;
	}
	
	return false;
}

bool ContextMenuUtil::PerformAction(int index)
{
	return _nativityJavaInterface->PerformAction(index, _selectedFiles);
}

wstring ContextMenuUtil::_GetCommandText(int index)
{
	if(index < _selectedFiles->size())
	{
		return L"";
	}

	list<wstring>::iterator commandIterator = _selectedFiles->begin();

    std::advance(commandIterator, index);

	wstring commandName = *commandIterator;

	return commandName;
}