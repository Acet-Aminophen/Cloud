#include "pch.h"
#include "framework.h"
#include "exam_20182653_leedongseok_server.h"
#include "exam_20182653_leedongseok_serverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PORT 8080
//포트 정의

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

Cexam20182653leedongseokserverDlg::Cexam20182653leedongseokserverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXAM_20182653_LEEDONGSEOK_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cexam20182653leedongseokserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLIENT, m_list_client);
	DDX_Control(pDX, IDC_LIST_MSG, m_list_msg);
	DDX_Control(pDX, IDC_EDIT_SEND_FORCE, m_edit_send_force);
}

BEGIN_MESSAGE_MAP(Cexam20182653leedongseokserverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_ACCEPT_SOCKET, &Cexam20182653leedongseokserverDlg::OnAcceptSocket)
	ON_MESSAGE(WM_CLIENT_MSG_RECV, &Cexam20182653leedongseokserverDlg::OnClientMsgRecv)
	ON_MESSAGE(WM_CLIENT_CLOSE, &Cexam20182653leedongseokserverDlg::OnClientClose)
END_MESSAGE_MAP()

BOOL Cexam20182653leedongseokserverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	for (int i = 0; i < 50; i++)
	{
		Rooms[i] = _T("###BLANK###");
	}

	m_pServerSocket = new CServerSocket;
	m_pServerSocket->SetWnd(this->m_hWnd);

	m_pServerSocket->Create(PORT);
	m_pServerSocket->Listen();

	srand(time(NULL));

	return TRUE;
}

void Cexam20182653leedongseokserverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void Cexam20182653leedongseokserverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR Cexam20182653leedongseokserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//클라이언트 접속
afx_msg LRESULT Cexam20182653leedongseokserverDlg::OnAcceptSocket(WPARAM wParam, LPARAM lParam)
{
	///클라이언트 정보 저장///////////////////////////////////////
	LPCTSTR temp_ip = (LPCTSTR)wParam;
	m_pClientSocket = (CClientSocket*)lParam;
	m_pClientSocket->ip_address = temp_ip;
	m_ptrClientSocketList.AddTail(m_pClientSocket);
	CString temp_str;
	temp_str.Format(_T("NONE : %s"), temp_ip);
	m_list_client.InsertString(-1, temp_str);
	m_pClientSocket = NULL;
	delete m_pClientSocket;
	/////////////////////////////////////////////////////////////
	return 0;
}

//클라이언트 수신
afx_msg LRESULT Cexam20182653leedongseokserverDlg::OnClientMsgRecv(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpOrder = (LPCTSTR)wParam;
	LPCTSTR lpszStr = (LPCTSTR)lParam;
	CString order_in = lpszStr;
	int user_serial = _ttoi(order_in.Mid(6, 4));
	int user_index = getUserIndex(_ttoi(order_in.Mid(6, 4)));
	int user_enemy_serial;
	int user_enemy_index;
	if (User[user_index].getEnemy() != -1)
	{
		user_enemy_serial = User[user_index].getEnemy();
		user_enemy_index = getUserEnemyIndex(user_serial);
	}
	m_list_msg.InsertString(-1, order_in);
	m_list_msg.SetCurSel(m_list_msg.GetCount() - 1);

	//방 만들기
	if (!order_in.Left(6).CompareNoCase(_T("OMKAMR")))
	{
		if (User[user_index].getInRoom() == false)
		{
			CString temp_cut = order_in.Mid(10);
			makeRoom(user_serial, user_index, temp_cut);
			giveRoomList(user_index);
		}
	}
	//고유번호 주기
	else if (!order_in.Left(6).CompareNoCase(_T("OMKWGS")))
	{
		int temp_serial = 0;
		while (!temp_serial)
		{
			temp_serial = rand() % 9000 + 1000;
			if (getUserIndex(temp_serial) != -1) temp_serial = 0;
		}
		for (int i = 0; i < 100; i++)
		{
			if (User[i].getStatus() == false)
			{
				User[i].setSer(temp_serial);
				User[i].setStatus(true);
				break;
			}
		}
		//접속 기록 확인하여 시리얼 부여
		CClientSocket* pClient = (CClientSocket*)wParam;
		UINT idx = 0;
		POSITION pos = m_ptrClientSocketList.Find(pClient);
		pClient->native_serial_number = temp_serial;

		CString str_sender;
		str_sender.Format(_T("OMKSSR%d"), temp_serial);
		sendAll(str_sender);

		//고유번호 확인한 텍스트로 재갱신
		CString str_list_box;
		str_list_box.Format(_T("NONE : %s"), pClient->ip_address);
		idx = m_list_client.SelectString(-1, (LPCTSTR)str_list_box);
		m_list_client.DeleteString(idx);
		str_list_box.Format(_T("%d : %s"), pClient->native_serial_number, pClient->ip_address);
		m_list_client.InsertString(-1, str_list_box);

		m_pClientSocket = NULL;
		delete m_pClientSocket;

		Sleep(100);
		sendNumOnline();
	}
	//방 리스트 주기
	else if (!order_in.Left(6).CompareNoCase(_T("OMKARL")))
	{
		giveRoomList(user_index);
	}
	//착수 된 경우
	else if (!order_in.Left(6).CompareNoCase(_T("OMKSIH")))
	{
		int x, y;
		x = _ttoi(order_in.Mid(10, 2)) - 10;
		y = _ttoi(order_in.Mid(12, 2)) - 10;

		//시간 초과로 인한 착수 포기
		if (x == 89 && y == 89);
		else
		{
			User[user_index].plate[x][y] = 1;
			User[user_enemy_index].plate[x][y] = 2;
		}
		sendAll(order_in);

		//적에게 착수 요구
		Sleep(100);
		CString str_sender;
		str_sender.Format(_T("OMKDMP%d"), User[user_enemy_index].getSer());
		sendAll(str_sender);
	}
	//방 연결 요구
	else if (!order_in.Left(6).CompareNoCase(_T("OMKATC")))
	{
		//아직 enemy_serial이 없을 때니 재갱신
		user_enemy_serial = _ttoi(order_in.Mid(10, 4));
		user_enemy_index = getUserIndex(user_enemy_serial);
		if (User[user_enemy_index].getEnemy() != -1 || User[user_enemy_index].getInRoom() == false) return 0;
		User[user_index].setInRoom(true);
		User[user_index].setEnemy(user_enemy_serial);
		User[user_enemy_index].setEnemy(user_serial);

		CString str_sender;
		str_sender.Format(_T("OMKCCF%d%d"), user_serial, user_enemy_serial);
		sendAll(str_sender);

		Sleep(100);
		str_sender.Format(_T("OMKCCF%d%d"), user_enemy_serial, user_serial);
		sendAll(str_sender);

		Sleep(100);
		str_sender.Format(_T("OMKDMP%d"), user_enemy_serial);
		sendAll(str_sender);
	}
	// 승리할 시
	else if (!order_in.Left(6).CompareNoCase(_T("OMKWTG")))
	{
		int x, y;
		x = _ttoi(order_in.Mid(10, 2)) - 10;
		y = _ttoi(order_in.Mid(12, 2)) - 10;
		if (x == 89 && y == 89);
		else
		{
			User[user_index].plate[x][y] = 1;
			User[user_enemy_index].plate[x][y] = 2;
		}
		sendAll(order_in);
		outRoom(user_index);
		outRoom(user_enemy_index);
	}
	// 방 나갈시
	else if (!order_in.Left(6).CompareNoCase(_T("OMKWTO")))
	{
		outRoom(user_index);
	}
	else
	{
		sendAll(order_in);
	}
	return 0;
}

afx_msg LRESULT Cexam20182653leedongseokserverDlg::OnClientClose(WPARAM wParam, LPARAM lParam)
{
	CClientSocket* pClient = (CClientSocket*)lParam;
	UINT idx = 0;
	POSITION pos = m_ptrClientSocketList.Find(pClient);

	int user_serial = pClient->native_serial_number;
	int user_index = getUserIndex(user_serial);
	if (pos != NULL)
	{
		CString str_sender;
		str_sender.Format(_T("OMKOUT%d"), user_serial);
		sendAll(str_sender);

		if (User[user_index].getEnemy() != -1) outRoom(getUserEnemyIndex(user_serial));
		if (User[user_index].getInRoom() == true) outRoom(user_index);
		User[user_index].setStatus(false);
		User[user_index].setEnemy(-1);
		User[user_index].setSer(-1);

		//m_list_client에서 해당 Client 삭제
		if (pClient->native_serial_number == -1)
		{
			str_sender.Format(_T("NONE : %s"), pClient->ip_address);
			idx = m_list_client.SelectString(-1, (LPCTSTR)str_sender);
			m_list_client.DeleteString(idx);
		}
		else
		{
			str_sender.Format(_T("%d : %s"), pClient->native_serial_number, pClient->ip_address);
			idx = m_list_client.SelectString(-1, (LPCTSTR)str_sender);
			m_list_client.DeleteString(idx);
		}

		//CPtrList에서도 해당 Client 삭제
		m_ptrClientSocketList.RemoveAt(pos);

		Sleep(100);
		sendNumOnline();
		return 0;
	}
	return 0;
}

void Cexam20182653leedongseokserverDlg::giveRoomList(int user_index)
{
	//방 리스트 줄 테니 리스트 비우라고 알리기
	CString str_sender;
	str_sender.Format(_T("OMKPTT%d"), User[user_index].getSer());
	sendAll(str_sender);

	//방 리스트 주기
	Sleep(100);
	for (int i = 0; i < 50; i++)
	{
		if (Rooms[i].CompareNoCase(_T("###BLANK###")))
		{
			int on_game = User[i].getEnemy() == -1 ? 0 : 1;
			str_sender.Format(_T("OMKGRL%d%d%d%s"), User[user_index].getSer(), Owner_Rooms[i], on_game, Rooms[i]);
			sendAll(str_sender);
			Sleep(100);
		}
	}
}

int Cexam20182653leedongseokserverDlg::getUserIndex(int user_serial)
{
	for (int i = 0; i < 100; i++)
	{
		if (User[i].getSer() == user_serial)
		{
			return i;
		}
	}
	return -1;
	// 못 찾으면 -1
}

int Cexam20182653leedongseokserverDlg::getUserEnemyIndex(int user_serial)
{
	for (int i = 0; i < 100; i++)
	{
		if (User[i].getSer() == User[getUserIndex(user_serial)].getEnemy())
		{
			return i;
		}
	}
	return -1;
}

int Cexam20182653leedongseokserverDlg::getRoomIndex(int owner_serial)
{
	for (int i = 0; i < 50; i++)
	{
		if (Owner_Rooms[i] == owner_serial)
		{
			return i;
		}
	}
	return -1;
}

void Cexam20182653leedongseokserverDlg::makeRoom(int owner_serial, int owner_index, CString title)
{
	User[owner_index].setInRoom(true);
	for (int i = 0; i < 50; i++)
	{
		if (!Rooms[i].CompareNoCase(_T("###BLANK###")))
		{
			Rooms[i] = title;
			Owner_Rooms[i] = owner_serial;
			break;
		}
	}
}

void Cexam20182653leedongseokserverDlg::sendAll(CString str)
{
	POSITION pos = m_ptrClientSocketList.GetHeadPosition();
	while (pos != NULL)
	{
		CClientSocket* pClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL)
		{
			pClient->Send(str, lstrlen(str) * 2);
		}
		m_list_msg.InsertString(-1, str);
		m_list_msg.SetCurSel(m_list_msg.GetCount() - 1);
	}
}

void Cexam20182653leedongseokserverDlg::outRoom(int user_index)
{
	breakRoom(User[user_index].getSer());
	for (int a = 0; a < 16; a++)
	{
		for (int b = 0; b < 16; b++)
		{
			User[user_index].plate[a][b] = 0;
		}
	}
	User[user_index].setEnemy(-1);
	User[user_index].setInRoom(false);
}

void Cexam20182653leedongseokserverDlg::breakRoom(int owner_serial)
{
	for (int i = 0; i < 50; i++)
	{
		if (Owner_Rooms[i] == owner_serial)
		{
			Owner_Rooms[i] = 0;
			Rooms[i] = _T("###BLANK###");
			return;
		}
	}
}

void Cexam20182653leedongseokserverDlg::OnOK()
{
	//CDialogEx::OnOK();
}

BOOL Cexam20182653leedongseokserverDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN && GetFocus() == GetDlgItem(IDC_EDIT_SEND_FORCE))
	{
		CString str;
		UpdateData(TRUE);
		m_edit_send_force.GetWindowTextW(str);
		sendAll(str);
		m_edit_send_force.SetWindowTextW(_T(""));
		m_list_msg.InsertString(-1, str);
		m_list_msg.SetCurSel(m_list_msg.GetCount() - 1);
		UpdateData(FALSE);
		return true;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void Cexam20182653leedongseokserverDlg::sendNumOnline()
{
	UpdateData();
	CString str;
	str.Format(_T("OMKNNO0000%d"), m_list_client.GetCount() + 1000);
	sendAll(str);
	UpdateData(false);
}