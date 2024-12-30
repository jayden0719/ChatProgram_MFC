
// ChatProgram.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "ChatProgram.h"
#include "ChatProgramDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChatProgramApp

BEGIN_MESSAGE_MAP(CChatProgramApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CChatProgramApp 생성

CChatProgramApp::CChatProgramApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CChatProgramApp 개체입니다.

CChatProgramApp theApp;


// CChatProgramApp 초기화

BOOL CChatProgramApp::InitInstance()
{
	// Windows XP에서는 InitCommonControlsEx()를 필요로 합니다.
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// MFC 컨트롤의 테마를 사용하기 위해 "Windows 원형" 비주얼 관리자 활성화
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));

	CChatProgramDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 애플리케이션이 예기치 않게 종료됩니다.\n");
		TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	}

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고 응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}



void CChatProgramApp::InitServer(void)
{
	// TODO: 여기에 구현 코드 추가.
	m_pServer = new CServerSock;
	m_pServer->Create(7777);
	m_pServer->Listen();
}


void CChatProgramApp::Accept()
{
	CClientSock* pClient = new CClientSock;
	CChatProgramDlg* cDlg = (CChatProgramDlg*)AfxGetMainWnd();
	if (m_pServer->Accept(*pClient)) {
		m_ClientList.AddTail(pClient);
		CString head;
		head.Format(_T("Client (%d)"), (int)m_ClientList.Find(pClient));
		cDlg -> m_listClients.InsertString(-1, head);
		cDlg->m_listChat.InsertString(-1, head + _T("님이 입장하였습니다."));
	}
	// TODO: 여기에 구현 코드 추가.
	if (m_pClient == NULL) {
		m_pClient = new CClientSock;
		m_pServer->Accept(*m_pClient);
		CString strSock;
		UINT nPort;
		m_pClient->GetPeerName(strSock, nPort);
		((CChatProgramDlg*)m_pMainWnd)->Accept(strSock);
	}
}


void CChatProgramApp::CleanUp()
{
	// TODO: 여기에 구현 코드 추가.

	if (m_pServer) {
		delete m_pServer;
		m_pServer = NULL;

		if (m_ClientList.GetCount() > 0) {
			CClientSock* pClient;
			POSITION pos = m_ClientList.GetHeadPosition();
	
			while (pos != NULL) {
				pClient = (CClientSock*)m_ClientList.GetAt(pos);
				m_ClientList.RemoveAt(pos);
				delete pClient;
				m_ClientList.GetNext(pos);
			}
		}
	}
	if (m_pClient) {
		delete m_pClient;
	}
}


//void CChatProgramApp::Connect()
//{
	// TODO: 여기에 구현 코드 추가.
//}


void CChatProgramApp::ReceiveData(CClientSock* pClient)
{
	// TODO: 여기에 구현 코드 추가.
	//wchar_t temp[MAX_PATH];	 
	TCHAR strBuffer[1024];
	::ZeroMemory(strBuffer, sizeof(strBuffer));
	int nSize = pClient->Receive(strBuffer, sizeof(strBuffer));
	CString csize;
	csize.Format(_T("%d"), nSize);

	CString strOut;
	CString strIp;
	CString msg;
	UINT nPort;
	if (m_pServer != NULL) {
		int peer = pClient->GetPeerName(strIp, nPort);
		msg = strBuffer;
		strOut.Format(_T("[%s]:%s"), strIp, msg);
		((CChatProgramDlg*)m_pMainWnd)->ReceiveData(strOut);

		if (m_ClientList.GetCount() > 1) {
			CClientSock* pReceive;
			POSITION pos = m_ClientList.GetHeadPosition();
			while (pos != NULL) {
				pReceive = ((CClientSock*)m_ClientList.GetAt(pos));
				if (pClient != pReceive) {
					pReceive->Send((LPCTSTR)strOut, sizeof(TCHAR) * (strOut.GetLength() + 1));
				}
				m_ClientList.GetNext(pos);
			}
			int nCount = (int)m_ClientList.GetCount();
		}
	}else {
		int peer = pClient->GetPeerName(strIp, nPort);
		msg = strBuffer;
		strOut.Format(_T("[%s]:%s"), strIp, msg);
		((CChatProgramDlg*)m_pMainWnd)->ReceiveData(msg);
	}
	
}


void CChatProgramApp::Connect(CString strIP)
{
	// TODO: 여기에 구현 코드 추가.
	m_pClient = new CClientSock;
	m_pClient->Create();
	m_pClient->Connect(strIP, 7777);
}


void CChatProgramApp::SendData(CString strData)
{
	//AfxMessageBox(_T("1차 진입"));
	// TODO: 여기에 구현 코드 추가.
	if (m_pServer == NULL) {
		//AfxMessageBox(_T("클라이언트 진입"));
		m_pClient->Send((LPCTSTR)strData, sizeof(TCHAR) * (strData.GetLength() + 1));
	}
	else {
		//AfxMessageBox(_T("서버 진입"));
		//CClientSock* pClient;
		POSITION pos = m_ClientList.GetHeadPosition();
		CString strSend;
		CString cpos;
		cpos.Format(_T("%d"), pos);
		while (pos != NULL) {		
			//AfxMessageBox(cpos);
			strSend.Format(_T("[%s]:%s"), ((CChatProgramDlg*)m_pMainWnd)->m_strMyIp, strData);
			m_pServer = (CServerSock*)m_ClientList.GetAt(pos);
			m_pServer->Send((LPCTSTR)strSend, sizeof(TCHAR)* (strSend.GetLength()+1));
			m_ClientList.GetNext(pos);
		}
	}
}


void CChatProgramApp::CloseChild(CClientSock* pClose)
{
	// TODO: 여기에 구현 코드 추가.
	CChatProgramDlg* cDlg = (CChatProgramDlg*)AfxGetMainWnd();
	CString str1, str2;
	UINT indx = 0, posNum;
	if (m_pClient == pClose)
	{
		AfxMessageBox(_T("서버 강제 종료"));
		delete m_pClient;
		m_pClient = NULL;
	}
	else {		
		if (m_ClientList.GetCount() > 0) {
			CClientSock* pClient;
			POSITION pos = m_ClientList.Find(pClose);
			
			while (pos != NULL) {
				pClient = (CClientSock*)m_ClientList.GetAt(pos);
				posNum = (int)m_ClientList.Find(pClose);
				cDlg->m_listClients.GetText(indx, str1);
				str2.Format(_T("Client (%d)"), posNum);

				if (str1.Find(str2) != -1) {
					cDlg->m_listClients.DeleteString(indx);
					CString ExitMsg;
					ExitMsg.Format(_T("Client (%d)님이 퇴장하였습니다."), posNum);
					cDlg->m_listChat.InsertString(-1,ExitMsg);
					break;
				}

				indx++;

				if (pClient == pClose) {					
					m_ClientList.RemoveAt(pos);
					delete pClient;
					break;
				}

			}
		}
	}
}
