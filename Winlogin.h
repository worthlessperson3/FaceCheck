#pragma once
#include "ButtonPNG.h"

// Winlogin 对话框

class Winlogin : public CDialogEx
{
	DECLARE_DYNAMIC(Winlogin)

public:
	Winlogin(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Winlogin();
	BOOL OnInitDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFACKCHECK_login };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	ButtonPNG m_btnMouse;
	CImage m_imgBG;
	afx_msg void OnPaint();
	HBITMAP m_snows[16];
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_snowBG;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonlogin();
	CStatic M_photo;

	char m_job[64];
	char m_userName[64];
	int m_yanzhi;
	bool m_logined;
};
