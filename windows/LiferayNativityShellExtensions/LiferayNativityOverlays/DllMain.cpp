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

#include "NativityOverlayRegistrationHandler.h"
#include "OKOverlayFactory.h"
#include "Constants.h"

#include <Guiddef.h>
#include <windows.h>

// {0DD5B4B0-25AF-4e09-A46B-9F274F3D700B}
const CLSID CLSID_LiferaySyncOKOverlay = 
{ 
	0xdd5b4b0, 0x25af, 0x4e09, 
		{0xa4, 0x6b, 0x9f, 0x27, 0x4f, 0x3d, 0x70, 0xb}
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

    if (!IsEqualCLSID(CLSID_LiferaySyncOKOverlay, rclsid))
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

    OKOverlayFactory* oKOverlayFactory = new OKOverlayFactory(szModule);

    if (oKOverlayFactory)
    {
		hResult = oKOverlayFactory->QueryInterface(riid, ppv);
		oKOverlayFactory->Release();
	}
    return hResult;
}

STDAPI DllCanUnloadNow(void)
{
    //return dllReferenceCount > 0 ? S_FALSE : S_OK;
	
	return S_FALSE;
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

	hResult = SyncOverlayRegistrationHandler::RegisterCOMObject(
		szModule, CLSID_LiferaySyncOKOverlay);

	if(!SUCCEEDED(hResult))
	{
		return hResult;
	}

	hResult = SyncOverlayRegistrationHandler::MakeRegistryEntries(
		CLSID_LiferaySyncOKOverlay, DL_OK_OVERLAY_DLL);

	if(!SUCCEEDED(hResult))
	{
		return hResult;
	}

    return hResult;
}

STDAPI DllUnregisterServer(void)
{
    HRESULT hResult = S_OK;

    wchar_t szModule[MAX_PATH];
    
	if (GetModuleFileNameW(instanceHandle, szModule, ARRAYSIZE(szModule)) == 0)
    {
        hResult = HRESULT_FROM_WIN32(GetLastError());
        return hResult;
    }

	hResult =SyncOverlayRegistrationHandler::UnregisterCOMObject(
		CLSID_LiferaySyncOKOverlay);

	if(!SUCCEEDED(hResult))
	{
		return hResult;
	}

	hResult = SyncOverlayRegistrationHandler::RemoveRegistryEntries(
		DL_OK_OVERLAY_DLL);
	
	if (!SUCCEEDED(hResult))
    {
		return hResult;
	}

    return hResult;
}
