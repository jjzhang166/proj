
// ExtractFeatureCodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ExtractFeatureCode.h"
#include "ExtractFeatureCodeDlg.h"
#include "afxdialogex.h"
#include <CmnMix/Cmn_Disasm.h>
#include <CmnMix/Cmn_UsefulFunc.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExtractFeatureCodeDlg 对话框



CExtractFeatureCodeDlg::CExtractFeatureCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExtractFeatureCodeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME);
}

void CExtractFeatureCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ASM_INPUT, edit_asm_input_);
	DDX_Control(pDX, IDC_EDIT_RESULT, edit_result_);
}

BEGIN_MESSAGE_MAP(CExtractFeatureCodeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BT_EXTRACT, &CExtractFeatureCodeDlg::OnBtExtract)
	ON_BN_CLICKED(IDC_BT_COPY, &CExtractFeatureCodeDlg::OnBtCopy)
END_MESSAGE_MAP()


// CExtractFeatureCodeDlg 消息处理程序

BOOL CExtractFeatureCodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CExtractFeatureCodeDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CExtractFeatureCodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CExtractFeatureCodeDlg::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg)
	{
		if ( WM_KEYDOWN == pMsg->message )
		{
			if ( VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam )
				return TRUE;
		}
	}
	return __super::PreTranslateMessage(pMsg);
}

void CExtractFeatureCodeDlg::OnBtExtract()
{
	CString str_code;
	edit_asm_input_.GetWindowText(str_code);
	CStringA str_code_a = CT2CA(str_code);

	Cmn_Disasm disasm;
	auto str_hex = Str2Hex(str_code_a, str_code_a.GetLength());
	disasm.SetInputBuffer((const unsigned char*)str_hex.c_str(), str_hex.size());

	const auto& str_res = disasm.GenFeatureCode();

	CString str_res_t = CA2CT(str_res.c_str());
	edit_result_.SetWindowText(str_res_t);
}

void CExtractFeatureCodeDlg::OnBtCopy()
{
	CString str_res;
	edit_result_.GetWindowText(str_res);
	CopyTextToClipboard(CT2CA(str_res), str_res.GetLength());
}
