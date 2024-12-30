#pragma once

// CClientSock 명령 대상

class CClientSock : public CAsyncSocket
{
public:
	CClientSock();
	virtual ~CClientSock();
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};


