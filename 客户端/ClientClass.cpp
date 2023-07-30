#include "ClientClass.h"
#pragma warning(disable:4996)
#include<windows.h>
using namespace std;

/*�д�����Ҫд�������ˣ�������*/
void ShowRoom(ClientClass& Client) {
	MsgType* msgtype;//��Ϣ����

	/*
		�����ˣ�������
		���ܣ����Ͳ鿴���з������Ϣ�������
	*/
	MsgShow s;
	//����Ϣ�����ָ��ָ��MsgShow���࣬���͸������
	msgtype = &s;
	send(Client.GetSocket(), (const char *)msgtype,msgtype->DataLen , NULL);
}

/*�д�����Ҫд�������ˣ�������*/
void CreateRoom(ClientClass &Client) {
	MsgType* msgtype;//��Ϣ����
	string roomid;//�����
	string name;//�ǳ�
	/*
		�����ˣ�������
		���ܣ����ʹ����������Ϣ�������
	*/
	//��ʾ�û����뷿��š��ǳ�string roomid ; string name
	cout << "���뷿���:";
	cin >> roomid;
	cout << "�����ǳ�:";
	cin >> name;

	//�����û�����ķ���ź��ǳƣ�������CLientClass���󷿼�ź��ǳ�����
	Client.SetRoomID(roomid);//��ȡ�ͻ������������
	Client.SetName(name);//���ÿͻ����ǳ�

	//����Ϣ�����ָ��ָ��MsgCreate���࣬���͸������
	MsgCreate msg(roomid, name);
	msgtype = &msg;
	send(Client.GetSocket(), (const char*)msgtype, msgtype->DataLen, NULL);
}

/*�д�����Ҫд�������ˣ����*/
void JoinRoom(ClientClass& Client) {
	/*
		�����ˣ�
		���ܣ����ͼ��뷿�����Ϣ�������
	*/

	//��ʾ�û����뷿��š��ǳ�string roomid ; string name

	//�����û�����ķ���ź��ǳƣ�������CLientClass���󷿼�ź��ǳ�����

	//����Ϣ�����ָ��ָ��MsgJoin���࣬���͸������

	MsgType* msgtype; // ��Ϣ�����ָ��

	// ��ʾ�û����뷿��ź��ǳ�
	string roomid, name;
	cout << "������Ҫ����ķ����: ";
	cin >> roomid;
	cout << "�������ǳ�: ";
	cin >> name;

	// ����ClientClass����ķ���ź��ǳ�����
	Client.SetRoomID(roomid);
	Client.SetName(name);

	// ����������MsgJoin��Ϣ��������
	msgtype = new MsgJoin(roomid, name);
	//Client.Send(msgtype);
	//delete msgtype;
	send(Client.GetSocket(), (const char*)msgtype, msgtype->DataLen, 0);//����Ϣ���������
}
bool IsLeave(ClientClass& Client, char* buff)
{
	//�ж��û��Ƿ�����leave,������MsgLeave����Ϣ��������	
	/*
	if (strcmp(buff, "leave") == 0)
		return True;
	else
		return False;	
	*/
	if (strcmp(buff, "//leave") == 0) {//�������//leave,���뿪����
		MsgType* msgtype;//��Ϣ����
		MsgLeave msgleave(Client.GetRoomID(), Client.GetName());
		msgtype = &msgleave;
		send(Client.GetSocket(), (const char*)msgtype, msgtype->DataLen, 0);//����Ϣ���������
		return TRUE;
	}
	return FALSE;
}
/*�д�����Ҫд�������ˣ����Ĵ�*/
void Chat(ClientClass& Client) {

	char buff[256];//�����Ҫ���͵���Ϣ

	/*
		�����ˣ�
		���ܣ�����������Ϣ�������
	*/

	//����������Ϣ�������
	//�ж��û��Ƿ�������leave,������MsgLeave����Ϣ��������
	//Ȼ���˳���ѭ����
	/*
	while (true) {
		// �ȴ��û�����������Ϣ
		cout << "������������Ϣ������'leave'�˳�����";
		cin.getline(buff, sizeof(buff));

		if (IsLeave(Client, buff))
		{
			MsgLeave leaveMsg;
			// ������Ϣ��������
			Client.SendData(reinterpret_cast<char*>(&leaveMsg), sizeof(leaveMsg));

			// �˳�ѭ������������
			break;
		}
		else
			Client.SendData(reinterpret_cast<char*>(&buff), sizeof(buff));
	}
	*/

	while (!IsLeave(Client, buff)) {

		memset(buff, 0, sizeof(buff));

		cout << "�����룺" << endl;
		cin >> buff;

		if (IsLeave(Client, buff)) {
			return;
		}
		send(Client.GetSocket(), buff, strlen(buff), NULL);//����Ϣ��������

	}
}

void Send(LPVOID lp) {

	ClientClass Client = *(ClientClass*)lp;//�ͻ��˶���

	while (true) {
		

		cout << "+------------------------------------------------+" << endl;
		cout << "+        ��ӭ����C�Ӽ��ֻ���������               +" << endl;
		cout << "+        1.����show�鿴����������                +" << endl;
		cout << "+        2.����join�����������                  +" << endl;
		cout << "+        3.����create����������                  +" << endl;
		cout << "+        4.����exit�˳�����                      +" << endl;
		cout << "+------------------------------------------------+" << endl;
		cout << endl;


		char buff[256];
		cout << "������ָ�" << endl;
		cin >> buff;

		//��������
		if (strcmp(buff, "create") == 0) {

			CreateRoom(Client);
			
			Chat(Client);

		}
		
		//���뷿��
		else if (strcmp(buff, "join") == 0) {

			JoinRoom(Client);
			
			Chat(Client);

		}

		//��ʾ���з���
		else if (strcmp(buff, "show") == 0) {
			ShowRoom(Client);
		}

		//�˳�����
		else if (strcmp(buff, "exit") == 0) {
			
			exit(0);
		}
	}
	
	
}

void ClientClass::SendData() {
	//�����̣߳���������Ϣ
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Send, (LPVOID)this, 0, 0);

}

/*�д�����Ҫд�������ˣ����Ӱ�*/
void ClientClass::RecvData() {
	char buff[256];

	initgraph(300, 400, 1);//������Ϣ�Ĵ���

	while (true) {

		/*
			�����ˣ�
			���ܣ����մӷ���˷�������Ϣ
		*/


		//������Ϣ����buff���һλ����һλ��Ϊ0��������ȷ��ȡ����
		
		//����Ϣ��ʾ��ͼ�ν��洰��

		/*
			����յ���������ʧ�ܻ���ȡ�ǳ�ʧ�ܵ���Ϣ
			Ҫ���������뷿��Ż��ǳ�
		*/
	}

}

ClientClass::ClientClass() {
	//���������׽���
	ClntSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//����TCP�׽���
	if (ClntSock == SOCKET_ERROR) {
		cout << "�����׽��ִ���ʧ��" << endl;
		return;
	}
	cout << "�����׽��ִ����ɹ�" << endl;

}

ClientClass::~ClientClass() {
	closesocket(ClntSock);
}

void ClientClass::ConnectServer(const char* ip, unsigned short port) {//���ӷ�����

	sockaddr_in sock_in;
	sock_in.sin_family = AF_INET;
	sock_in.sin_port = htons(port);
	sock_in.sin_addr.S_un.S_addr = inet_addr(ip);

	if (connect(ClntSock, (const sockaddr*)&sock_in, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		cout << "���ӷ�����ʧ��" << GetLastError() << endl;
		return;
	}
	cout << "�����˽�������" << endl;
}

SOCKET ClientClass::GetSocket() {
	return ClntSock;
}

void ClientClass::SetRoomID(string id) {
	m_RoomID = id;
}

string ClientClass::GetRoomID() {
	return m_RoomID;
}

void ClientClass::SetName(string name) {
	m_Name = name;
}

string ClientClass::GetName() {
	return m_Name;
}