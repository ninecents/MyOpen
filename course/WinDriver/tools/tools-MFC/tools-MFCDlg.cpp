
// tools-MFCDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "tools-MFC.h"
#include "tools-MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
#include <BlackBone/Config.h>
#include <BlackBone/Process/Process.h>
#include <BlackBone/Process/MultPtr.hpp>
#include <BlackBone/Process/RPC/RemoteFunction.hpp>
#include <BlackBone/PE/PEImage.h>
#include <BlackBone/Misc/Utils.h>
#include <BlackBone/Misc/DynImport.h>
#include <BlackBone/Syscalls/Syscall.h>
#include <BlackBone/Patterns/PatternSearch.h>
#include <BlackBone/Asm/LDasm.h>
#include <BlackBone/localHook/VTableHook.hpp>
#include <BlackBone/DriverControl/DriverControl.h>

// /I"../../../../third/Blackbone/src"
#ifdef _DEBUG
#pragma comment(lib, "../../../../third/Blackbone/build/Win32/Debug(XP)/BlackBone.lib")
#else
#pragma comment(lib, "../../../../third/Blackbone/build/Win32/Release(XP)/BlackBone.lib")
#endif
//////////////////////////////////////////////////////////////////////////

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CtoolsMFCDlg 对话框



CtoolsMFCDlg::CtoolsMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TOOLSMFC_DIALOG, pParent)
	, m_mem_data(_T(""))
	, m_mem_length(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtoolsMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MEM_DATA, m_mem_data);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_mem_length);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_mem_address);
	DDX_Control(pDX, IDC_COMBO_PROCESS, m_combo_process);
}

BEGIN_MESSAGE_MAP(CtoolsMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_READ, &CtoolsMFCDlg::OnBnClickedButtonRead)
	ON_CBN_DROPDOWN(IDC_COMBO_PROCESS, &CtoolsMFCDlg::OnCbnDropdownComboProcess)
END_MESSAGE_MAP()


// CtoolsMFCDlg 消息处理程序

BOOL CtoolsMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	OnCbnDropdownComboProcess();
	m_combo_process.SetCurSel(0);
	m_mem_address.SetWindowText(_T("400000"));
	m_mem_length = 0x20;
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtoolsMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtoolsMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtoolsMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

std::string ToHex(PBYTE bytes, DWORD length)
{
	if (bytes == NULL || length <= 0)
	{
		return "";
	}

	std::string result;
	for (DWORD i = 0; i < length; ++i)
	{
		char hex_byte[4] = { 0 };
		sprintf_s(hex_byte, 4, "%02X ", bytes[i]);
		result += hex_byte;
	}

	return result;
}


std::string ToHexLines(PBYTE bytes, DWORD length)
{
	if (bytes == NULL || length <= 0)
	{
		return "";
	}
	
	DWORD line = length / 0x10;
	DWORD left = length % 0x10;

	std::string result;
	for (DWORD i = 0; i < line; ++i)
	{
		result += ToHex(bytes + 0x10 * i, 0x10);
		result += "\r\n";
	}

	result += ToHex(bytes + 0x10 * line, left);

	return result;
}

void CtoolsMFCDlg::OnBnClickedButtonRead()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_mem_data.SetString(_T(""));

	CString str_address;
	m_mem_address.GetWindowText(str_address);
	str_address = _T("0x") + str_address;
	LONGLONG ll_address = _tcstoull_l(str_address.GetBuffer(), NULL, 16, 0);

	int nIndex = m_combo_process.GetCurSel();
	DWORD pid = m_combo_process.GetItemData(nIndex);

	blackbone::Process process;
	process.Attach(pid);
	if (!process.valid())
	{
		AfxMessageBox(_T("打开进程失败。"));
		return;
	}

	PBYTE bytes = new BYTE[m_mem_length];
	if (NULL == bytes)
	{
		return;
	}

	// ReadProcessMemory方式
	if (false)
	{
		SIZE_T byte_read;
		BOOL result = ReadProcessMemory(process.core().handle(), (LPCVOID)ll_address, (LPVOID)bytes, (SIZE_T)m_mem_length, &byte_read);
		if (result == FALSE)
		{
			AfxMessageBox(_T("读取进程内存失败，请检查内存地址和大小。"));
			return;
		}
	}
	// blackbone方式
	else if (false)
	{
		NTSTATUS status = process.memory().Read(ll_address, m_mem_length, (PVOID)bytes);
		if (!NT_SUCCESS(status))
		{
			AfxMessageBox(_T("读取进程内存失败，请检查内存地址和大小。"));
			return;
		}
	}
	// 驱动方式
	else
	{
		NTSTATUS status = blackbone::Driver().EnsureLoaded();
		if (!NT_SUCCESS(status))
		{
			AfxMessageBox(_T("加载驱动失败。"));
			return;
		}
		status = blackbone::Driver().ReadMem(pid, ll_address, m_mem_length, (PVOID)bytes);
		if (!NT_SUCCESS(status))
		{
			AfxMessageBox(_T("读取进程内存失败，请检查内存地址和大小。"));
			return;
		}
	}
// 	m_mem_data.Format(_T("%02X %02X %02X %02X   %02X %02X %02X %02X "), bytes[0], bytes[1], bytes[2], bytes[3]
// 		, bytes[4], bytes[5], bytes[6], bytes[7]);
	std::string str_mem_data = ToHexLines(bytes, m_mem_length);
	m_mem_data = CStringA(str_mem_data.data());

	if (bytes)
	{
		delete(bytes);
		bytes = NULL;
	}
	UpdateData(FALSE);
}


void CtoolsMFCDlg::OnCbnDropdownComboProcess()
{
	// TODO: 在此添加控件通知处理程序代码
	m_combo_process.ResetContent();
	blackbone::Process process;

	std::vector<DWORD> vct_pids = blackbone::Process::EnumByName(L"");
	for (auto pid : vct_pids)
	{
		NTSTATUS status = process.Attach(pid);
		CString msg;

		if (NT_SUCCESS(status))
		{
			if (process.modules().GetMainModule())
			{
				CString str_64 = process.core().isWow64() ? _T("x86") : _T("x64");
				msg.Format(_T("[%05d][%s] %s"), process.pid(), str_64.GetBuffer(), process.modules().GetMainModule()->fullPath.data());
			}
			else
			{
				msg.Format(_T("[%d] %s"), process.pid(), _T("failed_get_path"));
			}
		}
		else
		{
			msg.Format(_T("[%d] %s"), pid, _T("failed_Attach"));
		}
		m_combo_process.AddString(msg);
		int nIndex = m_combo_process.GetCount() - 1;
		m_combo_process.SetItemData(nIndex, pid);
	}
}
