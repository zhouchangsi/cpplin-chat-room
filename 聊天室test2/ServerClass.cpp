#include "ServerClass.h"

using namespace std;

HANDLE HMute = CreateMutex(NULL, FALSE, NULL);//互斥锁

const int MaxNum = 100;//客户端最大容量

int CountNum = 0;//统计当前的客户端数量

set<string> RoomIDs;//存放所有房间号

class ClntObject {//存放客户端的socket，房间号，昵称
private:

	SOCKET m_Socket;//客户端的socket

	string m_RoomID;//所属房间号

	string m_Name;//昵称

public:

	ClntObject() {

	}
	ClntObject(SOCKET Socket) {
		m_Socket = Socket;
	}

	~ClntObject() {

	}

	SOCKET GetSocket() {//获取socket
		return m_Socket;
	}

	void SetRoomID(string id) {//设置客户端所属房间号
		m_RoomID = id;
	}

	string GetRoomID() {//获取客户端所属房间号
		return m_RoomID;
	}

	void SetName(string name) {//设置客户端昵称
		m_Name = name;
	}

	string GetName() {//获取客户端昵称
		return m_Name;
	}
};

ClntObject Clnts[MaxNum];//缓冲区，存放客户端对象的数组

//获取当前时间，返回string类型
string TimeNow() {
	auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	//转为字符串
	stringstream ss;

	//ss << std::put_time(std::localtime(&t), "%Y年%m月%d日%H时%M分%S秒");
	ss << std::put_time(std::localtime(&t), "%Y年%m月%d日%H时%M分%S秒");

	string time = ss.str() + '\n';

	return time;
}

/*有代码需要写，负责人：陈育佳*/
void ShowRoom(ClntObject &Clnt, MsgType* msg) {
	string IDs;
	/*
		负责人：
		功能：接收到ShowRoom消息，给客户端发送所有房间号
	*/

	//将父类MsgType指针转为MsgShow子类指针

	//将所有房间的房间号作为消息发给该客户端
}

/*有代码需要写，负责人：李亚伦*/
void CreateRoom(ClntObject& Clnt, MsgType* msg) {

	char temp[256];//临时数组，存放该客户创建成功或失败的消息

	MsgCreate* create = (MsgCreate*)msg;//将父类MsgType指针转为子类指针

	/*
		负责人：李亚伦
		功能：接收到CreateRoom消息，发客户端发创建成功或失败的消息
		参数：create中有传过来的房间号和昵称属性
	*/

	//判断是否有该房间，若没有则新建，并且保存在RoomIDs里，记录昵称到Clnt的RoomID属性中，
	//若已存在房间，则发给该客户端新建房间错误的消息

	//如果在同一房间&&昵称无重复，则取昵称成功，并记录昵称到Clnt的Name属性中，
	//昵称重复则发给该客户端取名失败的消息
}

/*有代码需要写，负责人：李冰*/
void JoinRoom(ClntObject& Clnt, MsgType* msg) {

	char temp[256];//临时数组，存放该客户加入成功或失败的消息

	MsgJoin* join = (MsgJoin*)msg;//将父类MsgType指针转为子类指针

	/*
		负责人：
		功能：接收到JoinRoom消息，发客户端发加入成功或失败的消息
		参数：join中有传过来的房间号和昵称属性
	*/

	//将父类MsgType指针转为MsgCreate子类指针

	//判断是否有该房间，若有就加入，记录昵称到ClntObject的RoomID属性中，
	//若没有此房间，则发给该客户端新建房间错误的消息

	//如果在同一房间&&昵称无重复，则取昵称成功，并记录昵称到ClntObject的Name属性中，
	//昵称重复则发给该客户端取名失败的消息
}

/*有代码需要写，负责人：农乐诚*/
void LeaveRoom(ClntObject& Clnt, MsgType* msg) {

	char temp[256];//临时数组，存放该客户离开的消息

	MsgLeave* leave = (MsgLeave*)msg;//将父类MsgType指针转为子类指针

	/*
		负责人：
		功能：接收到leave消息，给同房间的其他客户端发该客户端离开的消息
	*/

	//将父类MsgType指针转为MsgLeave子类指针

	//将该客户端离开的消息发给该房间的所有成员

	//将该客户端的房间号和昵称置空
}

/*有代码需要写，负责人：李友军*/
void Talk(ClntObject& Clnt, char* temp1) {

	char temp[256] = { 0 };//临时数组，存放发给其他客户端的信息

	memset(temp, 0, 256);//清空

	WaitForSingleObject(HMute, INFINITE);
	/*
		负责人：
		功能：将聊天的消息发送给同房间的其他客户端
		发送要求：昵称：消息   例如 张三：中午吃啥？
	*/

	ReleaseMutex(HMute);
	}

unsigned WINAPI HandleClnt(void* Clnt) {//传入的参数是客户端对象在客户端对象数组中的索引

	ClntObject* Client = (ClntObject*)Clnt;
	
	char buff[256] = { 0 };//接收来自客户端的消息

	char temp[256] = { 0 };//临时数组，用于存放buff中的信息

	while (true) {
		//接收来自该客户端的消息

		int Len = recv(Client->GetSocket(), buff, 255, 0);

			if (Len > 0) {

			buff[Len] = 0;//将最后一位的下一位置为0，才能正确读取数据

			memset(temp, 0, 256);//清空

			strcpy_s(temp, buff);//temp1用于存放buff中的信息

			MsgType* msg = (MsgType*)buff;//将接收到的消息转为MsgType类

			//根据发送的消息类型MsgType不同进行不同的处理
			switch (msg->MsgType) {

				case MSG_SHOW: {

					ShowRoom(*Client, msg);
				}
						 break;

				case MSG_CREATE: {
					CreateRoom(*Client, msg);
				}
						 break;

				case MSG_JOIN: {
					JoinRoom(*Client, msg);
				}
						 break;

				case MSG_LEAVE: {
					WaitForSingleObject(HMute, INFINITE);

					LeaveRoom(*Client, msg);

					ReleaseMutex(HMute);
				}
						 break;

				default://默认就说明不是指令，是发送的聊天信息
					Talk(*Client, temp);
					break;
				}

		}

	}

	return 0;
}

ServerClass::ServerClass() {
	//创建监听套接字
	ServSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建TCP套接字
	if (ServSock == SOCKET_ERROR) {
		cout << "监听套接字创建失败" << endl;
		return;
	}

}

ServerClass::~ServerClass() {
	closesocket(ServSock);
	delete[] Clnts;
}

void ServerClass::Bind(const char* ip, unsigned short port) {

	sockaddr_in sock_in;// sockaddr_in是保存IPv4地址信息的结构体
	sock_in.sin_family = AF_INET;
	sock_in.sin_port = htons(port);//端口
	sock_in.sin_addr.S_un.S_addr = inet_addr(ip);//ip,值为INADDR_ANY时设置的是服务器的IP地址

	int ret = bind(ServSock, (const sockaddr*)&sock_in, sizeof(sock_in));//绑定(ip 与 socket绑定)

	if (ret == SOCKET_ERROR) {
		cout << "绑定套接字失败" << endl;
		return;
	}
	cout << "绑定套接字成功" << endl;
}

void ServerClass::Listen() {

	if (listen(ServSock, 10) == SOCKET_ERROR) {//监听，此处第二个参数是说 内核为 此套接字维护的最大链接个数
		cout << "监听套接字失败" << endl;
		return;
	}
	cout << "监听套接字成功" << endl;
}

/*有代码需要写，负责人：李忠科*/
void ServerClass::Communication() {
	for (int i = 0; i < MaxNum; i++) {
		//接收客户端的连接
		sockaddr_in ClntAddr;
		int clen = sizeof(ClntAddr);

		//连接客户端
		SOCKET ClntSock = accept(ServSock, (sockaddr*)&ClntAddr, &clen);

		if (ClntSock == SOCKET_ERROR) {
			cout << "接收客户端失败" << GetLastError() << endl;
			closesocket(ClntSock);
			return;
		}
		cout << "连接客户端" << i << "成功" << endl;

		
		
		//生产者
		WaitForSingleObject(HMute, INFINITE);
		ClntObject* Clnt = new ClntObject(ClntSock);//生产者
		Clnts[i] = *Clnt;//缓冲区
		CountNum++;
		//连接客户端成功后，生成ClntObeject对象，将对象放入Clnts数组

		ReleaseMutex(HMute);

		HANDLE hTheard = (HANDLE)_beginthreadex(0, 0, (_beginthreadex_proc_type)HandleClnt, (void*)&Clnts[i], 0, 0);

	}

}

SOCKET ServerClass::GetSocket() {
	return ServSock;
}