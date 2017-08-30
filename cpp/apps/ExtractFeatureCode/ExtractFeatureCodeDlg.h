
// ExtractFeatureCodeDlg.h : 头文件
//

#pragma once


// CExtractFeatureCodeDlg 对话框
class CExtractFeatureCodeDlg : public CDialog
{
// 构造
public:
	CExtractFeatureCodeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EXTRACTFEATURECODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtExtract();
	afx_msg void OnBtCopy();
	DECLARE_MESSAGE_MAP()

private:
	CEdit			edit_asm_input_;
	CEdit			edit_result_;
};
