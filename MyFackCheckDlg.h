
// MyFackCheckDlg.h: 头文件
//

#pragma once
#include "ButtonPNG.h"

// CMyFackCheckDlg 对话框
class CMyFackCheckDlg : public CDialogEx
{
// 构造
public:
	CMyFackCheckDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFACKCHECK_DIALOG };
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
	afx_msg void OnBnClickedButtonlogin();
	ButtonPNG m_btnlogin;
	CImage m_imgBK;
	ButtonPNG m_btnRegister;
	afx_msg void OnBnClickedButtonresigter();
};
