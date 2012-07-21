// Server.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <conio.h>
#include "ProudNet.h"
#include "../Common/C2S_stub.h"
#include "../Common/C2S_stub.cpp"
#include "../Common/S2C_proxy.h"
#include "../common/S2C_proxy.cpp"


#ifdef _DEBUG
	#pragma comment(lib, "ProudNet_2008_Debug.lib" )
#else
	#pragma comment(lib, "ProudNet_2008_Release.lib" )
#endif


using namespace std;
using namespace Proud;



class C2SStub: public C2S::Stub
{
public:
	DECRMI_C2S_Message
	{
		printf( "Recv Message %d \n", msg);
		return true;
	}
};
C2SStub g_stub;
S2C::Proxy g_proxy;
Proud::CFastArray<Proud::HostID> g_clientList;


class CServerEventSink: public INetServerEvent
{
	virtual void OnClientJoin(CNetClientInfo *clientInfo) OVERRIDE
	{
		printf("Client %d connected.\n", clientInfo->m_HostID);
		g_clientList.Add(clientInfo->m_HostID);
	}

	virtual void OnClientLeave( CNetClientInfo *clientInfo, ErrorInfo *errorInfo, const ByteArray& comment ) OVERRIDE
	{
		printf("Client %d disconnected.\n", clientInfo->m_HostID);
	}

	virtual bool OnConnectionRequest(AddrPort clientAddr, ByteArray &userDataFromClient, ByteArray &reply) OVERRIDE
	{
		return true;
	}

	virtual void OnP2PGroupJoinMemberAckComplete(HostID groupHostID,HostID memberHostID,ErrorType result) OVERRIDE {}
	virtual void OnUserWorkerThreadBegin() OVERRIDE {}
	virtual void OnUserWorkerThreadEnd() OVERRIDE {}

	virtual void OnError(ErrorInfo *errorInfo) OVERRIDE{ printf("OnError : %s",(LPCSTR)CW2A(errorInfo->ToString())); }
	virtual void OnWarning(ErrorInfo *errorInfo) OVERRIDE{ printf("OnWarning : %s",(LPCSTR)CW2A(errorInfo->ToString())); }
	virtual void OnInformation(ErrorInfo *errorInfo) OVERRIDE{ printf("OnInformation : %s",(LPCSTR)CW2A(errorInfo->ToString())); }
	virtual void OnException(Exception &e) OVERRIDE{ printf("OnInformation : %s",e.what()); }

	/** RMI가 호출이 들어왔으나 Attach된 Stub 중에 대응하는 RMI가 전혀 없으면 이것이 콜백된다. */
	/** RMI has called but there are no opposited RMI from attached Stub then this will callback. */
	virtual void OnNoRmiProcessed(Proud::RmiID rmiID) OVERRIDE {}
};

CServerEventSink g_eventSink;


int _tmain(int argc, _TCHAR* argv[])
{
	CNetServer* srv = ProudNet::CreateServer();
	srv->SetEventSink(&g_eventSink);
	srv->AttachStub(&g_stub);
	srv->AttachProxy(&g_proxy);

	CStartServerParameter p1;
	p1.m_protocolVersion = Guid();
	p1.m_tcpPort = 33334;
	srv->Start(p1);

	while (1)
	{
		if (_kbhit())
		{
			int ch = _getch();
			switch (ch)
			{
			case 27: // ESC key
				goto out;
			case '1':
				break;
			case '2':
				{
					for(int i=0; i < g_clientList.GetCount(); ++i)
					{
						g_proxy.ConsoleString(g_clientList[ i], RmiContext::ReliableSend,
							L"Hello~~~!");
					}
				}
				break;
			}
		}
		// CPU 점유를 100% 차지하지 않게 하기 위함
		Sleep(10); 
	}

out:
	delete srv;

	return 0;
}

