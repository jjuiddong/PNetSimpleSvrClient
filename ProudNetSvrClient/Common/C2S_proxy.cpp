
#include "C2S_proxy.h"


namespace C2S{
bool Proxy::Message(Proud::HostID remote,Proud::RmiContext &rmiContext,const int &msg)
{
// 여기서 더이상 main lock을 하지 않음.
    // Proud::CriticalSectionLock __lock(m_core->GetCritSec(),true);
	Proud::CMessage __msg;		// CStackAllocMessage 대신 CMessage를 써도 성능상 차이가 적다고 검증됨.
	__msg.UseInternalBuffer();
	
	Proud::RmiID __msgid=(Proud::RmiID)Rmi_Message;
	__msg.Write(__msgid); 
	
__msg << msg;

		return RmiSend(&remote,1,rmiContext,__msg,
			RmiName_Message, (Proud::RmiID)Rmi_Message);
	}

	bool Proxy::Message(const Proud::HostID *remotes,int remoteCount,Proud::RmiContext &rmiContext,const int &msg)
{
// 여기서 더이상 main lock을 하지 않음.
    // Proud::CriticalSectionLock __lock(m_core->GetCritSec(),true);
	Proud::CMessage __msg;		// CStackAllocMessage 대신 CMessage를 써도 성능상 차이가 적다고 검증됨.
	__msg.UseInternalBuffer();
	
	Proud::RmiID __msgid=(Proud::RmiID)Rmi_Message;
	__msg.Write(__msgid); 
	
__msg << msg;

		return RmiSend(remotes,remoteCount,rmiContext,__msg,
			RmiName_Message, (Proud::RmiID)Rmi_Message);
	}


// Rmi name 선언
// Rmi profiler 등 Rmi name을 가리키는 unique pointer이다. 이것이 있으면 strcmp 등을 쓰지 말고 바로 Rmi name pointer 값으로 name 매칭을 할 수 있다.
#ifndef HIDE_RMI_NAME_STRING
LPCWSTR Proxy::RmiName_Message=L"Message";
#else
LPCWSTR Proxy::RmiName_Message=L"";
#endif
LPCWSTR Proxy::RmiName_First=RmiName_Message;
    }

