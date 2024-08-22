// WinReg.cpp: 实现文件
//

#include "pch.h"
#include "MyFackCheck.h"
#include "WinReg.h"
#include "afxdialogex.h"
#include<opencv2/opencv.hpp>
using namespace cv;
#include "tools.h"

#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include "faceTool.h"
#include "WinWelcome.h"
extern MHandle faceModel;

static VideoCapture cap;

// WinReg 对话框

IMPLEMENT_DYNAMIC(WinReg, CDialogEx)

WinReg::WinReg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYFACKCHECK_reg, pParent)
{

}

WinReg::~WinReg()
{
}

void WinReg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1_REG, m_btnMouse);
	DDX_Control(pDX, IDC_EDIT1, m_name);
	DDX_Control(pDX, IDC_SNOW, m_snowBG);
	DDX_Control(pDX, IDC_PHOTO_REAL, m_photoReal);
}

BOOL WinReg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadPicture(m_faceBg, IDB_PNG4);
	m_btnMouse.Init(IDB_PNG5, 4, BTN_TYPE_NORMAL);

	CString name;
	for (int i = 0; i < 16; i++)
	{
		name.Format(L"res/snow/snow_%d.bmp", i);
		m_snows[i] = (HBITMAP)LoadImage(0, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}
	SetTimer(1, 30, NULL);

	CFont font;
	font.CreatePointFont(200, L"花文行楷", NULL);
	m_name.SetFont(&font);

	::MoveWindow(m_snowBG.m_hWnd, 440, 0, 640, 609, 1);	
	::MoveWindow(m_snowBG.m_hWnd, 440, 64, 640, 480, 1);
	srand(time(NULL));

	return 0;
}


BEGIN_MESSAGE_MAP(WinReg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1_REG, &WinReg::OnBnClickedButton1Reg)
	ON_BN_CLICKED(IDC_BUTTON_login, &WinReg::OnBnClickedButtonlogin)
	ON_BN_CLICKED(IDC_BUTTON_REG, &WinReg::OnBnClickedButtonReg)
END_MESSAGE_MAP()


// WinReg 消息处理程序


void WinReg::OnPaint()
{
	drawPicOnPait(&m_faceBg, this, 0, 0);
}


void WinReg::OnTimer(UINT_PTR nIDEvent)
{
	static int snowIndex = 0;
	static int count = 0;
	if (nIDEvent == 1)
	{
		m_snowBG.SetBitmap(m_snows[snowIndex]);
		snowIndex = (snowIndex + 1) % 16;
	}
	else if (nIDEvent == 2)
	{
		//使用摄像头捕获
		refreshCamera(&cap, m_hWnd, IDC_PHOTO_REAL);
		count++;
		if (count == 1)
		{
			mciSendString(L"play res/zhuche.mp3", 0, 0, 0);
		}
		else if (count == 30)
		{
			paiZhao(&cap, "tmp.jpg");
			count = 0;
			KillTimer(2);
			cap.release();
			IplImage* img1 = cvLoadImage("tmp.jpg");
			IplImage* img2 = cvLoadImage("res/leijun.jpg");
			float xishu = faceCompare(faceModel, img1, img2);
			int yanzhi = xishu * 1000 + 80 + rand() % 15;
			if (yanzhi <= 0) yanzhi = 60;
			else if (yanzhi >= 100) yanzhi = 99;

			const char* jobs[] = { "精神科主任", "精神科副主任", "精神科实习生" };

			char cmd[521];

			CString editName;
			m_name.GetWindowText(editName);

			USES_CONVERSION;
			char* editName2 = T2A(editName);

			sprintf_s(cmd, sizeof(cmd), "move tmp.jpg users\\%s-%s-%d.jpg", jobs[rand() % 3], editName2, yanzhi);
			system(cmd);

			WinWelcome win;
			win.DoModal();
			CDialogEx::OnOK();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void WinReg::OnBnClickedButton1Reg()
{

	if (m_name.GetWindowTextLength() == 0)
	{
		MessageBox(L"请输入用户名");
		return;
	}
	KillTimer(1);

	HBITMAP pic = (HBITMAP)LoadImage(0, L"res/wall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_snowBG.SetBitmap(pic);

	//打开opencv处理摄像头
	cap.open(0);
	SetTimer(2, 100, NULL);
}
