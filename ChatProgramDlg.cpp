
// ChatProgramDlg.cpp: 구현 파일
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32")

#include <winsock2.h>

#include "pch.h"
#include "framework.h"
#include "ChatProgram.h"
#include "ChatProgramDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <iostream>


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatProgramDlg 대화 상자



CChatProgramDlg::CChatProgramDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATPROGRAM_DIALOG, pParent)
	, m_nChatMode(0)
	, m_strMyIp(_T("")) //텍스트 초기화
	, m_strOtherIp(_T("")) //텍스트 초기화
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatProgramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, m_IPAddress);
	DDX_Control(pDX, IDC_LIST_CHAT, m_listChat);
	DDX_Radio(pDX, IDC_RADIO_SERVER, m_nChatMode);
	DDX_Control(pDX, IDC_LIST_CLIENTS, m_listClients);
}

BEGIN_MESSAGE_MAP(CChatProgramDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO_SERVER, &CChatProgramDlg::OnClickedRadioServer)
	ON_COMMAND(IDC_RADIO_CLIENT, &CChatProgramDlg::OnRadioClient)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CChatProgramDlg::OnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatProgramDlg::OnClickedButtonSend)
END_MESSAGE_MAP()


// CChatProgramDlg 메시지 처리기

BOOL CChatProgramDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	char name[255];
	PHOSTENT hostinfo;
	if (gethostname(name, sizeof(name)) == 0) {
		if ((hostinfo = gethostbyname(name)) != NULL) {
			m_strMyIp = GetIpAddress();
		}
	}
	m_IPAddress.SetWindowTextW(m_strMyIp);
	m_IPAddress.EnableWindow(false);
	SetDlgItemText(IDC_BUTTON_CONNECT, _T("Open"));

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


void CChatProgramDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CChatProgramDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CChatProgramDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatProgramDlg::OnClickedRadioServer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_IPAddress.SetWindowText(m_strMyIp);
	m_IPAddress.EnableWindow(FALSE);
	m_listClients.EnableWindow(TRUE);
	SetDlgItemText(IDC_BUTTON_CONNECT, _T("OPEN"));
}


void CChatProgramDlg::OnRadioClient()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_IPAddress.SetWindowText(_T(""));
	m_IPAddress.EnableWindow(TRUE);
	m_listClients.EnableWindow(FALSE);
	SetDlgItemText(IDC_BUTTON_CONNECT, _T("CONNECT"));
}


void CChatProgramDlg::OnClickedButtonConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(true);
	if (!m_nChatMode) { //서버
		((CChatProgramApp*)AfxGetApp())->InitServer();
		GetDlgItem(IDC_RADIO_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CLIENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
	}
	else { //클라이언트
		CString strIP;		
		GetDlgItemText(IDC_IPADDRESS_SERVER, strIP);
		if (strIP != _T("0.0.0.0")) {
			GetDlgItem(IDC_RADIO_SERVER)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_CLIENT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
			((CChatProgramApp*)AfxGetApp())->Connect(strIP);
			m_strOtherIp = strIP;
		}
		else {
			AfxMessageBox(_T("접속할 서버의 IP주소를 입력해주세요"));
		}
	}
}


void CChatProgramDlg::OnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strSend, strInsert;
	GetDlgItemText(IDC_EDIT_SEND, strSend);
	strInsert.Format(_T("[%s]:%s"), m_strMyIp, strSend);
	theApp.SendData(strSend);
	int sel = m_listChat.InsertString(-1, strInsert);
	m_listChat.SetCurSel(sel);
	SetDlgItemText(IDC_EDIT_SEND, _T(""));
}


void CChatProgramDlg::ReceiveData(CString strReceive)
{
	// TODO: 여기에 구현 코드 추가.
	CString strInsert;
	strInsert.Format(_T("%s"), strReceive);
	int sel = m_listChat.InsertString(-1, strInsert);
	m_listChat.SetCurSel(sel);
}


void CChatProgramDlg::Accept(CString strSock)
{
	// TODO: 여기에 구현 코드 추가.
	m_strOtherIp = strSock;
}


CString CChatProgramDlg::GetIpAddress()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	char name[255];
	PHOSTENT hostinfo;
	CString strIpAddress;

	wVersionRequested = MAKEWORD(2, 0);

	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		if (gethostname(name, sizeof(name)) == 0)
		{
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				strIpAddress = inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);
			}
		}

		WSACleanup();
	}


	return strIpAddress;
}

//void main() {
//std::cout << "디버깅 시작" << std::endl;
//}
