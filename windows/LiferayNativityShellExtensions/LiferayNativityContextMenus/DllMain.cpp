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

#include "ContextMenuFactory.h"
#include "NativityContextMenuRegistrationHandler.h"
#include <Guiddef.h>
#include <windows.h>

// {882FAB3F-7F07-402a-A94A-39744D146C1F}
const CLSID CLSID_LiferayNativityContextMenus =  
{
	0x882fab3f, 0x7f07, 0x402a, 
	{ 0xa9, 0x4a, 0x39, 0x74, 0x4d, 0x14, 0x6c, 0x1f }
};


HINSTANCE instanceHandle = NULL;

long dllReferenceCount = 0;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
			instanceHandle = hModule;
			DisableThreadLibraryCalls(hModule);
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	HRESULT hResult = CLASS_E_CLASSNOTAVAILABLE;

	if (!IsEqualCLSID(CLSID_LiferayNativityContextMenus, rclsid))
	{
		return hResult;
	}

	hResult = E_OUTOFMEMORY;

	wchar_t szModule[MAX_PATH];

	if (GetModuleFileName(instanceHandle, szModule, ARRAYSIZE(szModule)) == 0)
	{	
		hResult = HRESULT_FROM_WIN32(GetLastError());

		return hResult;
	}

	ContextMenuFactory* contextMenuFactory = new ContextMenuFactory(szModule);
		
	if (contextMenuFactory)
	{
		hResult = contextMenuFactory->QueryInterface(riid, ppv);
		
		contextMenuFactory->Release();
	}

    return hResult;
}

STDAPI DllCanUnloadNow(void)
{
	return dllReferenceCount > 0 ? S_FALSE : S_OK;
}

HRESULT _stdcall DllRegisterServer(void)
{	
	HRESULT hResult = S_OK;

	wchar_t szModule[MAX_PATH];

	if (GetModuleFileName(instanceHandle, szModule, ARRAYSIZE(szModule)) == 0)
	{	
		hResult = HRESULT_FROM_WIN32(GetLastError());

		return hResult;
	}

	hResult = NativityContextMenuRegistrationHandler::RegisterCOMObject(
		szModule, CLSID_LiferayNativityContextMenus);

	if(!SUCCEEDED(hResult))
	{
		return hResult;
	}

	hResult = NativityContextMenuRegistrationHandler::MakeRegistryEntries(
		CLSID_LiferayNativityContextMenus);

	if(!SUCCEEDED(hResult))
	{
		return hResult;
	}

    return S_OK;
}

STDAPI DllUnregisterServer(void)
{
	HRESULT hr = S_OK;

    wchar_t szModule[MAX_PATH];

	if (GetModuleFileName(instanceHandle, szModule, ARRAYSIZE(szModule)) == 0)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());

		return hr;
    }

    hr = NativityContextMenuRegistrationHandler::UnregisterCOMObject(
		CLSID_LiferayNativityContextMenus);

	if (!SUCCEEDED(hr))
    {
		return hr;
	}

	hr = NativityContextMenuRegistrationHandler::RemoveRegistryEntries();
	
	if (!SUCCEEDED(hr))
    {
		return hr;
	}

    return S_OK;
}