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

#include "LiferayNativityOverlay.h"

#include <strsafe.h>
#include <Shlwapi.h>
#include <string>

#pragma comment(lib, "shlwapi.lib")

extern HINSTANCE instanceHandle;

#define IDM_DISPLAY 0  
#define IDB_OK 101

LiferayNativityOverlay::LiferayNativityOverlay(): 
	_referenceCount(1)
{
	_nativityJavaInterface = new NativityJavaInterface();
}

LiferayNativityOverlay::~LiferayNativityOverlay(void)
{
}

IFACEMETHODIMP_(ULONG) LiferayNativityOverlay::AddRef()
{
    return InterlockedIncrement(&_referenceCount);
}

IFACEMETHODIMP LiferayNativityOverlay::QueryInterface(REFIID riid, void **ppv)
{
    HRESULT hr = S_OK;

    if (IsEqualIID(IID_IUnknown, riid) || 
        IsEqualIID(IID_IShellIconOverlayIdentifier, riid))
    {
        *ppv = static_cast<IShellIconOverlayIdentifier *>(this);
    }
    else
    {
        hr = E_NOINTERFACE;
        *ppv = NULL;
    }

    if (*ppv)
    {
        AddRef();
    }
	
    return hr;
}

IFACEMETHODIMP_(ULONG) LiferayNativityOverlay::Release()
{
    ULONG cRef = InterlockedDecrement(&_referenceCount);
    if (0 == cRef)
    {
        delete this;
    }

    return cRef;
}

IFACEMETHODIMP LiferayNativityOverlay::GetPriority(int *pPriority)
{
	pPriority = 0;

	return S_OK;
}

 IFACEMETHODIMP LiferayNativityOverlay::IsMemberOf(PCWSTR pwszPath, DWORD dwAttrib)
{
	if(!_DoesFileNeedOverlay(pwszPath))
	{
		return MAKE_HRESULT(S_FALSE, 0, 0);
	}
	else
	{
	    return MAKE_HRESULT(S_OK, 0, 0);
	}
}

IFACEMETHODIMP LiferayNativityOverlay::GetOverlayInfo(
	PWSTR pwszIconFile, int cchMax, int *pIndex, DWORD *pdwFlags)
{
	*pIndex = 0;

	*pdwFlags = ISIOI_ICONFILE | ISIOI_ICONINDEX;

	if (GetModuleFileName(instanceHandle, pwszIconFile, cchMax) == 0)
	{	
		HRESULT hResult = HRESULT_FROM_WIN32(GetLastError());

		return hResult;
	}

	return S_OK;
}


 bool LiferayNativityOverlay::_DoesFileNeedOverlay(const wchar_t* filePath)
{	
	bool displayOverlay = _nativityJavaInterface->IsDisplayOverlay(filePath);

	return displayOverlay;
}
