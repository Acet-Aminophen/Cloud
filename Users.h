#pragma once
#include "atlstr.h"

class Users
{
private:
	bool status_on = false;
	bool in_room = false;
	CString MAC_ADDRESS;
	int serial_number = -1;
	int enemy_serial_number = -1;
public:
	Users() {};
	~Users() {};

	int plate[16][16] = { 0, };
	// �ɷµǸ� �����Ҵ�����
	// ��ǥ 1���� ���� ���� ���� 1�߰�
	void setEnemy(int target) { enemy_serial_number = target; }
	int getEnemy() { return enemy_serial_number; }
	void setInRoom(bool target) { in_room = target; }
	bool getInRoom() { return in_room; }
	void setStatus(bool target) { status_on = target; }
	bool getStatus() { return status_on; }
	int getSer() { return serial_number; }
	void setSer(int target) { serial_number = target; }
	void setMac(CString target) { MAC_ADDRESS = target; };
};