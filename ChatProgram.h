
// ChatProgram.h: PROJECT_NAME 애플리케이션에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.
#include "CServerSock.h"
#include "CClientSock.h"

// CChatProgramApp:
// 이 클래스의 구현에 대해서는 ChatProgram.cpp을(를) 참조하세요.
//

class CChatProgramApp : public CWinApp
{
public:
	CChatProgramApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.
	DECLARE_MESSAGE_MAP()

public:
	CServerSock* m_pServer;
	CClientSock* m_pClient;
	void InitServer();
//	void Accept();
//	void Connect();
//	void ReceiveData();
//	void Accept(int a);
	void Accept();
	void CleanUp();
//	void Connect();
	void ReceiveData(CClientSock* pSock);
	void Connect(CString strIP);
	void SendData(CString strData);
	void CloseChild(CClientSock* pClose);
	CObList m_ClientList;
};

extern CChatProgramApp theApp;
