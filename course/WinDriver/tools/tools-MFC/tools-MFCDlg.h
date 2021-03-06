﻿
// tools-MFCDlg.h: 头文件
//

#pragma once


// CtoolsMFCDlg 对话框
class CtoolsMFCDlg : public CDialogEx
{
// 构造
public:
	CtoolsMFCDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLSMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_mem_data;
	DWORD m_mem_length;
	CEdit m_mem_address;
	CComboBox m_combo_process;
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnCbnDropdownComboProcess();
	afx_msg void OnCbnSelchangeComboProcess();
	CComboBox m_combo_read_type;
};
