#include "pch.h"
#include "exam_20182653_leedongseok_server.h"
#include "CClientSocket.h"


// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::SetWnd(HWND hwnd)
{
	m_hwnd = hwnd;
}

void CClientSocket::OnReceive(int nErrorCode)
{
	CString strTmp = _T("");
	CString strIP = _T("");
	UINT strPORT;
	TCHAR strBuffer[65536 * 2];
	ZeroMemory(strBuffer, sizeof(strBuffer));
	GetPeerName(strIP, strPORT);
	ip_address = strIP;

	if (Receive(strBuffer, sizeof(strBuffer)) > 0)
		// 전달된 데이터가 존재
	{
		strTmp.Format(_T("%s"), strBuffer);
	}
	SendMessage(m_hwnd, WM_CLIENT_MSG_RECV, (LPARAM)this, (LPARAM)((LPCTSTR)strTmp));
	CSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode)
{
	SendMessage(m_hwnd, WM_CLIENT_CLOSE, 0, (LPARAM)this);
	CSocket::OnClose(nErrorCode);
}