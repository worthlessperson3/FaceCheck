// Winlogin.cpp: 实现文件
//
#include "pch.h"
#include "MyFackCheck.h"
#include "Winlogin.h"
#include "afxdialogex.h"
#include<opencv2/opencv.hpp>
using namespace cv;
#include "tools.h"

#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include "faceTool.h"
#include "WinWelcome.h"
#include<vector>
using namespace std;
extern MHandle faceModel;

static VideoCapture cap;

// Winlogin 对话框

IMPLEMENT_DYNAMIC(Winlogin, CDialogEx)

Winlogin::Winlogin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYFACKCHECK_login, pParent)
{

}

Winlogin::~Winlogin()
{
}

BOOL Winlogin::OnInitDialog()
{
	//父类初始化
	CDialogEx::OnInitDialog();
	//
	LoadPicture(m_imgBG, IDB_PNG4);
	m_btnMouse.Init(IDB_PNG5,4,BTN_TYPE_NORMAL);

	CString name;

	for (int i = 0; i < 16; i++)
	{
		name.Format(L"res/snow/snow_%d.bmp", i);
		m_snows[i] = (HBITMAP)LoadImage(0, name, IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
	}
	//启动定时器
	SetTimer(1, 30, NULL);
	::MoveWindow(m_snowBG.m_hWnd, 440, 0, 640, 609, 1);
	//::MoveWindow(m_photoReal.m_hWnd, 440, 64, 640, 480, 1);

	return 0;
}

void Winlogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_login, m_btnMouse);
	DDX_Control(pDX, IDC_SNOW, m_snowBG);
	DDX_Control(pDX, IDC_PHOTO_REAL, M_photo);
}


BEGIN_MESSAGE_MAP(Winlogin, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &Winlogin::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_login, &Winlogin::OnBnClickedButtonlogin)
END_MESSAGE_MAP()


// Winlogin 消息处理程序


void Winlogin::OnPaint()
{
	drawPicOnPait(&m_imgBG, this, 0, 0);
}
bool faceCheck(const char*filename,char*userName)
{
	vector<CString>vctPath;
	WIN32_FIND_DATA fileData;
	HANDLE file = FindFirstFile(L"users/*.jpg", &fileData);
	if (file != INVALID_HANDLE_VALUE)
	{
		vctPath.push_back(fileData.cFileName);
		while (FindNextFile(file, &fileData))
		{
			vctPath.push_back(fileData.cFileName);
		}
	}

	IplImage* img1 = cvLoadImage(filename);

	for (int i = 0; i < vctPath.size(); i++)
	{
		USES_CONVERSION;
		char *othername = T2A(vctPath[i]);
		char name[256];
		sprintf_s(name, sizeof(name), "users/%s", othername);
		IplImage* img2 = cvLoadImage(name);
		float ret = faceCompare(faceModel,img1,img2);

		if (ret >= 0.92)
		{
			string tmpStr = othername;
			string nameStr = tmpStr.substr(0, tmpStr.length() - 4);
			strcpy(userName, nameStr.c_str());
			return true;
		}
	}

	return false;
}

void Winlogin::OnTimer(UINT_PTR nIDEvent)
{
	static int snowIndex = 0;

	static int count = 0;

	static char userName[256] = "";

	static bool success = false;

	if (nIDEvent == 1)
	{
		m_snowBG.SetBitmap(m_snows[snowIndex]);
		snowIndex = (snowIndex + 1) % 16;
	}
	else if (nIDEvent == 2)
	{
		refreshCamera(&cap, m_hWnd, IDC_PHOTO_REAL);
		count++;
		if (count == 10)
		{
			const char* filename = "tmp.jpg";
			paiZhao(&cap, filename);
			if (faceCheck(filename, userName))
			{
				mciSendString(L"play res/login.mp3", 0, 0, 0);
				success = true;
			}
		}
		else if (count == 20)
		{
			count = 0;
			KillTimer(2);
			cap.release();
			if (success)
			{
				strcpy(m_job, strtok(userName, "-"));
				strcpy(m_userName, strtok(userName, "-"));
				m_yanzhi = atoi(strtok(userName, "-"));
				m_logined = true;
				CDialogEx::OnOK();
			}
			else
			{
				MessageBox(L"登录失败!");
				m_logined = false;
				SetTimer(1, 100, NULL);
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}



void Winlogin::OnBnClickedButtonlogin()
{
	KillTimer(1);
	cap.open(0);
	HBITMAP pic = (HBITMAP)LoadImage(0, L"res/wall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_snowBG.SetBitmap(pic);

	::MoveWindow(M_photo.m_hWnd, 440, 64, 640, 480, 1);
	SetTimer(2, 100, NULL);
}
