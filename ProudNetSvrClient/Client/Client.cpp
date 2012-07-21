// Client.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <conio.h>
#include "ProudNet.h"
#include <iostream>
#include "../Common/S2C_stub.h"
#include "../Common/S2C_stub.cpp"
#include "../Common/C2S_proxy.h"
#include "../common/C2S_proxy.cpp"


#ifdef _DEBUG
#pragma comment(lib, "ProudNet_2008_Debug.lib" )
#else
#pragma comment(lib, "ProudNet_2008_Release.lib" )
#endif


class S2CStub : public S2C::Stub
{
	DECRMI_S2C_ConsoleString
	{
		printf( "Recv ConsoleString %s \n",  (LPCSTR)CW2A(message) );		
		return true;
	}
};
S2CStub g_s2cStub;
C2S::Proxy g_c2sProxy;


bool g_isConnectWaiting = true;
bool g_isConnected = false;

bool IsConnectWaiting()
{
	return g_isConnectWaiting;
}
Proud::HostID g_memberHostID = Proud::HostID_None;

class CClientEventSink: public Proud::INetClientEvent
{
	virtual void OnJoinServerComplete(Proud::ErrorInfo *info,
		const Proud::ByteArray &replyFromServer) OVERRIDE
	{
		g_isConnectWaiting = false;
		if (info->m_errorType == Proud::ErrorType_Ok)
		{
//			printf("Succeed to connect server. Allocated hostID=%d\n", g_cli->GetLocalHostID());
			g_isConnected = true;
		}		
		else
			printf("Failed to connect server.\n");
	}

	virtual void OnLeaveServer(Proud::ErrorInfo *errorInfo) OVERRIDE
	{
		g_isConnected = false;
	}
	virtual void OnP2PMemberJoin(Proud::HostID memberHostID, Proud::HostID groupHostID,
		int memberCount, const Proud::ByteArray &customField) OVERRIDE
	{
// 		printf( "[Client] P2P member %d joined group %d.\n", memberHostID, groupHostID);
// 		if (memberHostID != g_cli->GetLocalHostID())
// 		{
// 			g_memberHostID = memberHostID;
// 			g_C2CProxy.P2PChat(memberHostID, RmiContext::ReliableSend, L"Welcome!!", 1, 1.0f);
// 		}
	}
	virtual void OnP2PMemberLeave(Proud::HostID memberHostID, Proud::HostID groupHostID,
		int memberCount) OVERRIDE
	{
//		printf("[Client] P2P member %d left group %d.\n", memberHostID, groupHostID);
	}

	virtual void OnChangeServerUdpState(Proud::ErrorType reason)
	{
		printf("OnChangeServerUdpState : %d\n", (int)reason);
	}

	virtual void OnChangeP2PRelayState(Proud::HostID remoteHostID, Proud::ErrorType reason) OVERRIDE 
	{
		printf("OnChangeP2PRelayState : %d\n", (int)reason);
	}
	virtual void OnSynchronizeServerTime() OVERRIDE {}

	virtual void OnError(Proud::ErrorInfo *errorInfo) OVERRIDE{ printf("OnError : %s",(LPCSTR)CW2A(errorInfo->ToString())); }
	virtual void OnWarning(Proud::ErrorInfo *errorInfo) OVERRIDE{ printf("OnWarning : %s",(LPCSTR)CW2A(errorInfo->ToString())); }
	virtual void OnInformation(Proud::ErrorInfo *errorInfo) OVERRIDE{ printf("OnInformation : %s",(LPCSTR)CW2A(errorInfo->ToString())); }

	virtual void OnException(Proud::Exception &e) OVERRIDE{ printf("OnInformation : %s",e.what()); }

	virtual void OnNoRmiProcessed(Proud::RmiID rmiID) OVERRIDE {}

};

CClientEventSink g_eventSinkClient;


int _tmain(int argc, _TCHAR* argv[])
{
	Proud::CNetClient* client = Proud::CNetClient::Create();
	client->SetEventSink(&g_eventSinkClient);
	client->AttachProxy(&g_c2sProxy);
	client->AttachStub(&g_s2cStub);

	Proud::CNetConnectionParam cp;
	cp.m_protocolVersion = Proud::Guid(); 
	//cp.m_serverIP = L"192.168.77.144";	// Address of server
	cp.m_serverIP = L"127.0.0.1";	// Address of server
	cp.m_serverPort = 33334;	// Port of server

	client->Connect(cp);

	while (IsConnectWaiting())
	{
		Sleep(10);
		client->FrameMove();
	}

	while (g_isConnected)
	{
		Sleep(10);
		client->FrameMove();
		if (_kbhit())
		{
			int temp = _getch();
			if (temp == 27)
			{
				goto out;
			}
			else if(temp == 97)
			{
				static int a = 0;
				g_c2sProxy.Message( Proud::HostID_Server, Proud::RmiContext::ReliableSend, ++a );
			}
		}
	}

out:
	delete client;

	return 0;
}
