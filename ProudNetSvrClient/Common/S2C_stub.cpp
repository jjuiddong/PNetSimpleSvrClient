
#include "S2C_stub.h"

    const unsigned char szS2CRmi[] = { 0x00, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xff, 0x00, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xff,
                 0x6f,0xbe,0x1e,0x1f,0xb2,0x1a,0x1f,0xf6,0x1e,0x1f,0x6a,0x9e,0xf8,0xdc,0x5c,0x7f,0xda,0xd6,0x1f,0xde,0x12,0x4c,0x35,0x8d,0x2d,0xb9,0x4d,0xcd,0x3d,0xd1,0x35,0x15,0x45,0xf5,0x15,0x45,0xe5,0x85,0x35,0xe1,0x85, 0xaa, 0xbb, 0xcc, 0xdd, 0xff,0x99, 0xaa, 0xbb, 0xcc, 0x99, 0xaa, 0xdd, 0xff, 0x00 };   


namespace S2C{

	bool Stub::ProcessReceivedMessage(Proud::CReceivedMessage &pa, void* hostTag) 
	{
        szS2CRmi; 

		Proud::HostID remote=pa.GetRemoteHostID();
		if(remote==Proud::HostID_None)
		{
			ShowUnknownHostIDWarning(remote);
		}

		Proud::CMessage &___msg=pa.GetReadOnlyMessage();

		int orgReadOffset = ___msg.GetReadOffset();

		Proud::RmiID __rmiID;
		if(!___msg.Read(__rmiID))
			goto ___fail;
					
		switch(__rmiID)
		{
case Rmi_ConsoleString:
		{
			Proud::RmiContext ctx;
			ctx.m_sentFrom=pa.GetRemoteHostID();
			ctx.m_relayed=pa.IsRelayed();
			ctx.m_hostTag = hostTag;
			
Proud::String message; ___msg >> message;
m_core->PostCheckReadMessage(___msg,RmiName_ConsoleString);
if(m_enableNotifyCallFromStub && !m_internalUse)
			{
				Proud::String parameterString;
Proud::AppendTextOut(parameterString,message);	
	parameterString += L", ";	
parameterString = parameterString.Left(parameterString.GetLength() - 2); // 끝의 콤마를 제거한다.
NotifyCallFromStub((Proud::RmiID)Rmi_ConsoleString, RmiName_ConsoleString,parameterString);

				m_core->Viz_NotifyRecvToStub(remote, (Proud::RmiID)Rmi_ConsoleString, RmiName_ConsoleString, parameterString);
			}
			else if(!m_internalUse)
			{
				m_core->Viz_NotifyRecvToStub(remote, (Proud::RmiID)Rmi_ConsoleString, RmiName_ConsoleString, L"");
			}
			
			DWORD __t0 = 0;
			if(!m_internalUse && m_enableStubProfiling)
			{
				Proud::BeforeRmiSummary summary;
				summary.m_rmiID = (Proud::RmiID)Rmi_ConsoleString;
				summary.m_rmiName = RmiName_ConsoleString;
				summary.m_hostID = remote;
				summary.m_hostTag = hostTag;
				BeforeRmiInvocation(summary);
				__t0 = ::GetTickCount();
			}
			
			// 본 메서드를 호출한다.
			bool __ret=ConsoleString (remote,ctx ,message );
			
			if(__ret==false)
			{
				// 오류. 사용자가 구현하지 않은 RMI 함수를 호출했음.
				m_core->ShowNotImplementedRmiWarning(RmiName_ConsoleString);
			}
			
			if(!m_internalUse && m_enableStubProfiling)
			{
				Proud::AfterRmiSummary summary;
				summary.m_rmiID = (Proud::RmiID)Rmi_ConsoleString;
				summary.m_rmiName = RmiName_ConsoleString;
				summary.m_hostID = remote;
				summary.m_hostTag = hostTag;
				summary.m_elapsedTime = ::GetTickCount() - __t0;
				AfterRmiInvocation(summary);
			}
		}
		break;
default:
			goto ___fail;
		}		
		return true;
___fail:
		{
			___msg.SetReadOffset(orgReadOffset);
			return false;
		}
	}
// Rmi name 선언
// Rmi profiler 등 Rmi name을 가리키는 unique pointer이다. 이것이 있으면 strcmp 등을 쓰지 말고 바로 Rmi name pointer 값으로 name 매칭을 할 수 있다.
#ifndef HIDE_RMI_NAME_STRING
LPCWSTR Stub::RmiName_ConsoleString=L"ConsoleString";
#else
LPCWSTR Stub::RmiName_ConsoleString=L"";
#endif
LPCWSTR Stub::RmiName_First=RmiName_ConsoleString;
    }

