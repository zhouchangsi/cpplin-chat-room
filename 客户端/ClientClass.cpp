#include "ClientClass.h"
#pragma warning(disable:4996)
#include<windows.h>
using namespace std;

/*有代码需要写，负责人：陈育佳*/
void ShowRoom(ClientClass& Client) {
	MsgType* msgtype;//消息基类

	/*
		负责人：陈育佳
		功能：发送查看所有房间的消息给服务端
	*/
	
	//将消息基类的指针指向MsgShow子类，发送给服务端
}

/*有代码需要写，负责人：李亚伦*/
void CreateRoom(ClientClass &Client) {
	MsgType* msgtype;//消息基类
	string roomid;//房间号
	string name;//昵称
	/*
		负责人：李亚伦
		功能：发送创建房间的消息给服务端
	*/

	//提示用户输入房间号、昵称string roomid ; string name
	cout << "输入房间号:";
	cin >> roomid;
	cout << "输入昵称:";
	cin >> name;

	//接收用户输入的房间号和昵称，并设置CLientClass对象房间号和昵称属性
	Client.SetRoomID(roomid);//获取客户端所属房间号
	Client.SetName(name);//设置客户端昵称

	//将消息基类的指针指向MsgCreate子类，发送给服务端
	MsgCreate msg(roomid, name);
	msgtype = &msg;
	send(Client.GetSocket, (const char*)msgtype, strlen(msgtype), NULL);
}

/*有代码需要写，负责人：李冰*/
void JoinRoom(ClientClass& Client) {
	MsgType* msgtype;//消息基类
	string roomid;//房间号
	string name;//昵称
	/*
		负责人：
		功能：发送加入房间的消息给服务端
	*/

	//提示用户输入房间号、昵称string roomid ; string name

	//接收用户输入的房间号和昵称，并设置CLientClass对象房间号和昵称属性

	//将消息基类的指针指向MsgJoin子类，发送给服务端
}
bool IsLeave(ClientClass& Client, char* buff)
{
	//判断用户是否输入leave,有则发送MsgLeave类消息给服务器
	if (strcmp(buff, "leave") == 0)
		return True;
	else
		return False;
}
/*有代码需要写，负责人：单文聪*/
void Chat(ClientClass& Client) {

	char buff[256];//存放需要发送的消息

	/*
		负责人：
		功能：发送聊天消息给服务端
	*/

	//发送聊天消息给服务端
	//判断用户是否有输入leave,有则发送MsgLeave类消息给服务器
	//然后退出该循环；
	while (true) {
		// 等待用户输入聊天消息
		cout << "请输入聊天消息（输入'leave'退出）：";
		cin.getline(buff, sizeof(buff));

		if (IsLeave(Client, buff))
		{
			MsgLeave leaveMsg;
			// 发送消息给服务器
			Client.SendData(reinterpret_cast<char*>(&leaveMsg), sizeof(leaveMsg));

			// 退出循环，结束聊天
			break;
		}
		else
			Client.SendData(reinterpret_cast<char*>(&buff), sizeof(buff));
	}
}

void Send(LPVOID lp) {

	ClientClass Client = *(ClientClass*)lp;//客户端对象

	while (true) {
		

		cout << "+------------------------------------------------+" << endl;
		cout << "+        欢迎来到C加加林会议聊天室               +" << endl;
		cout << "+        1.输入show查看所有聊天室                +" << endl;
		cout << "+        2.输入join加入该聊天室                  +" << endl;
		cout << "+        3.输入create创建聊天室                  +" << endl;
		cout << "+        4.输入exit退出聊天                      +" << endl;
		cout << "+------------------------------------------------+" << endl;
		cout << endl;


		char buff[256];
		cout << "请输入指令：" << endl;
		cin >> buff;

		//创建房间
		if (strcmp(buff, "create") == 0) {

			CreateRoom(Client);
			
			Chat(Client);

		}
		
		//加入房间
		else if (strcmp(buff, "join") == 0) {

			JoinRoom(Client);
			
			Chat(Client);

		}

		//显示所有房间
		else if (strcmp(buff, "show") == 0) {
			ShowRoom(Client);
		}

		//退出程序
		else if (strcmp(buff, "exit") == 0) {
			Client.~ClientClass();
			exit(0);
		}
	}
	
	
}

void ClientClass::SendData() {
	//创建线程，负责发送消息
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Send, (LPVOID)this, 0, 0);

}

/*有代码需要写，负责人：祁子昂*/
void ClientClass::RecvData() {
	char buff[256];

	initgraph(300, 400, 1);//接收消息的窗口

	while (true) {

		/*
			负责人：
			功能：接收从服务端发来的消息
		*/


		//接收消息，将buff最后一位的下一位置为0，才能正确读取数据
		
		//将信息显示到图形界面窗口

		/*
			如果收到创建房间失败或者取昵称失败的消息
			要求重新输入房间号或昵称
		*/
	}

}

ClientClass::ClientClass() {
	//创建监听套接字
	ClntSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建TCP套接字
	if (ClntSock == SOCKET_ERROR) {
		cout << "监听套接字创建失败" << endl;
		return;
	}
	cout << "监听套接字创建成功" << endl;

}

ClientClass::~ClientClass() {
	closesocket(ClntSock);
}

void ClientClass::ConnectServer(const char* ip, unsigned short port) {//连接服务器

	sockaddr_in sock_in;
	sock_in.sin_family = AF_INET;
	sock_in.sin_port = htons(port);
	sock_in.sin_addr.S_un.S_addr = inet_addr(ip);

	if (connect(ClntSock, (const sockaddr*)&sock_in, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		cout << "连接服务器失败" << GetLastError() << endl;
		return;
	}
	cout << "与服务端建立连接" << endl;
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