#pragma once
#define WM_CLIENT_MSG_RECV WM_USER+2
#define WM_CLIENT_CLOSE WM_USER+3

class CClientSocket : public CSocket
{
public:
	CString ip_address;
	int native_serial_number = -1;
	HWND m_hwnd;
	CClientSocket();
	~CClientSocket();
	void SetWnd(HWND hwnd);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};