
// exam_20182653_leedongseok_serverDlg.h: 헤더 파일
//

#pragma once
#include "CServerSocket.h"
#include "Users.h"


// Cexam20182653leedongseokserverDlg 대화 상자
class Cexam20182653leedongseokserverDlg : public CDialogEx
{
private:
	CClientSocket* m_pClientSocket;
	CServerSocket* m_pServerSocket;
	Users User[100];
	CString Rooms[50];
	int Owner_Rooms[50] = { 0, };
// 생성입니다.
public:
	Cexam20182653leedongseokserverDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXAM_20182653_LEEDONGSEOK_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_list_client;
	CListBox m_list_msg;
	CPtrList m_ptrClientSocketList;
	CEdit m_edit_send_force;
protected:
	afx_msg LRESULT OnAcceptSocket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientMsgRecv(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientClose(WPARAM wParam, LPARAM lParam);
public:
	void giveRoomList(int user_index);
	int getUserIndex(int user_serial);
	int getUserEnemyIndex(int user_serial);
	int getRoomIndex(int owner_serial);
	void makeRoom(int owner_serial, int owner_index, CString title);
	void sendAll(CString str);
	void outRoom(int user_index);
	void breakRoom(int owner_serial);
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void sendNumOnline();
};
