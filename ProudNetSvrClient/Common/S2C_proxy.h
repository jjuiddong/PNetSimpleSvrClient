﻿
#pragma once

#include "S2C_common.h"


namespace S2C{
	class Proxy : public Proud::IRmiProxy
	{
	public:
virtual bool ConsoleString(Proud::HostID remote,Proud::RmiContext &rmiContext,const Proud::String &message)
 SEALED; 
	virtual bool ConsoleString(const Proud::HostID *remotes,int remoteCount,Proud::RmiContext &rmiContext,const Proud::String &message)
 SEALED;

// Rmi name 선언
// Rmi profiler 등 Rmi name을 가리키는 unique pointer이다. 이것이 있으면 strcmp 등을 쓰지 말고 바로 Rmi name pointer 값으로 name 매칭을 할 수 있다.
static LPCWSTR RmiName_ConsoleString;
static LPCWSTR RmiName_First;
    
		Proxy()
		{
			if(m_signature != 1)
				Proud::ShowUserMisuseError(Proud::ProxyBadSignatureErrorText);
		}
		virtual Proud::RmiID GetFirstRmiID() OVERRIDE { return (Proud::RmiID)Rmi_First; } 
		virtual Proud::RmiID GetLastRmiID() OVERRIDE { return (Proud::RmiID)Rmi_Last; }
	};
}

