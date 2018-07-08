//UpdatingForm.h

#ifndef _UPDATINGFORM_H
#define _UPDATINGFORM_H
#include "resource.h"
#include <afxwin.h>

typedef signed long int Long;

class CashBook;
class UpdatingForm :public CDialog {
public:
	enum { IDD = IDD_UPDATINGFORM };
public:
	UpdatingForm(CWnd *parent = NULL);
	virtual BOOL OnInitDialog();
public:
	CashBook *cashBook;
protected:
	void Load();
	void Save();
	void Insert(Long index);
	void Update(Long index);
	CString GetCode();
protected:
	afx_msg void OnRecordButtonClicked();
	afx_msg void OnFindButtonClicked();
	afx_msg void OnCorrectButtonClicked();
	afx_msg void OnCalculateButtonClicked();
	afx_msg void OnAmountEditKillFocus();
	afx_msg void OnListViewItemDoubleClicked(NMHDR *pNotifyStruct, LRESULT *result);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};

#endif //_UPDATINGFORM_H