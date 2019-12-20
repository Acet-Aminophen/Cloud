#pragma once
#include "CClientSocket.h"

#define WM_ACCEPT_SOCKET WM_USER+1

class CServerSocket : public CSocket
{
public:
	CServerSocket();
	virtual ~CServerSocket();

	HWND m_hwnd;
	void SetWnd(HWND hwnd);

	virtual void OnAccept(int nErrorCode);
};