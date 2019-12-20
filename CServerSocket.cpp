// CServerSocket.cpp: 구현 파일
//

#include "pch.h"
#include "exam_20182653_leedongseok_server.h"
#include "CServerSocket.h"


// CServerSocket

CServerSocket::CServerSocket()
{
}

CServerSocket::~CServerSocket()
{
}

void CServerSocket::SetWnd(HWND hwnd)
{
	m_hwnd = hwnd;
}

void CServerSocket::OnAccept(int nErrorCode)
{
	CClientSocket* pClient = new CClientSocket; \
		CString strIPAddress = _T("");
	UINT uPortNumber = 0;

	GetPeerName(strIPAddress, uPortNumber);
	if (Accept(*pClient))
	{
		pClient->GetPeerName(strIPAddress, uPortNumber);
		pClient->SetWnd(m_hwnd);
	}
	else
	{
		delete pClient;
		AfxMessageBox(_T("ERROR : Failed can't accept new Client!"));
	}

	SendMessage(m_hwnd, WM_ACCEPT_SOCKET, (LPARAM)((LPCTSTR)strIPAddress), (LPARAM)pClient);
	CSocket::OnAccept(nErrorCode);
}