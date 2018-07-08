//FindingForm.cpp
#include "FindingForm.h"
#include <afxcmn.h>
#include <AFXDTCTL.H>
#include "UpdatingForm.h"
#include "CashBook.h"
#include "Account.h"
#include "Income.h"
#include "Outgo.h"

BEGIN_MESSAGE_MAP(FindingForm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, OnCalculateButtonClicked)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ACCOUNTS, OnListViewItemDoubleClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

FindingForm::FindingForm(CWnd *parent)
	:CDialog(FindingForm::IDD, parent) {
	this->indexes = NULL;
	this->count = 0;
}

//1. 윈도우가 생성되었을 때
BOOL FindingForm::OnInitDialog() {
	CDialog::OnInitDialog();

	//1.1 리스트뷰의 헤더를 만든다.
	((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->InsertColumn(0, "일자", LVCFMT_CENTER, 100);
	((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->InsertColumn(1, "적요", LVCFMT_CENTER, 100);
	((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->InsertColumn(2, "금액", LVCFMT_CENTER, 100);
	((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->InsertColumn(3, "잔액", LVCFMT_CENTER, 100);
	((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->InsertColumn(4, "비고", LVCFMT_CENTER, 100);

	return FALSE;
}

//2. 찾기버튼을 클릭했을 때
void FindingForm::OnFindButtonClicked() {
	SYSTEMTIME time = { 0, };

	// 2.1 조건을 읽는다.(체크박스 확인)
	int dateCheck = ((CButton *)(this->GetDlgItem(IDC_CHECK_DATE)))->GetCheck();
	int contentCheck = ((CButton *)(this->GetDlgItem(IDC_CHECK_CONTENT)))->GetCheck();

	// 2.2 가계부 윈도우를 찾는다.
	UpdatingForm *updatingForm = (UpdatingForm *)CWnd::FindWindow("#32770", "가계부");

	// 기존 찾아진 데이터 삭제
	if (this->indexes != NULL) {
		delete[] indexes;
		this->indexes = NULL;
	}

	// 2.3 가계부 윈도우의 가계부에서 찾는다.
	CString content;
	CString date;
	// 적요 선택 일자 미선택
	if (contentCheck == BST_CHECKED && dateCheck == BST_UNCHECKED) {
		(this->GetDlgItem(IDC_EDIT_CONTENT))->GetWindowText(content);
		updatingForm->cashBook->Find(string((LPCTSTR)content), &(this->indexes), &(this->count));
	}
	// 적요 미선택 일자 선택
	else if (contentCheck == BST_UNCHECKED && dateCheck == BST_CHECKED) {
		((CDateTimeCtrl *)(this->GetDlgItem(IDC_DATETIMEPICKER_BEGIN)))->GetTime(&time);
		date.Format("%4d%02d%02d", time.wYear, time.wMonth, time.wDay);
		Date cDate((char*)((LPCTSTR)date));
		updatingForm->cashBook->Find(cDate, &(this->indexes), &(this->count));
	}
	// 두 조건 모두선택
	else if (contentCheck == BST_CHECKED && dateCheck == BST_CHECKED) {
		(this->GetDlgItem(IDC_EDIT_CONTENT))->GetWindowText(content);
		((CDateTimeCtrl *)(this->GetDlgItem(IDC_DATETIMEPICKER_BEGIN)))->GetTime(&time);
		date.Format("%4d%02d%02d", time.wYear, time.wMonth, time.wDay);
		Date cDate((char*)((LPCTSTR)date));
		updatingForm->cashBook->Find(cDate, string((LPCTSTR)content), &(this->indexes), &(this->count));
	}

	// 2.4 리스트뷰의 모든 항목들을 지운다.
	((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->DeleteAllItems();

	//2.5 리스트뷰의 항목을 새로 추가한다.
	Long i = 0;
	Account *account;
	Date tempDate;
	CString date_;
	CString amount;
	CString balance;
	while (i < this->count) {
		account = updatingForm->cashBook->GetAt(indexes[i]);
		tempDate = account->GetDate();
		date_.Format("%4d - %02d - %02d", tempDate.GetYear(), tempDate.GetMonth(), tempDate.GetDay());

		((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->InsertItem(i, date_);
		((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(i, 1, account->GetContent().c_str());

		amount.Format("%.0f", account->GetAmount());
		((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(i, 2, amount);

		balance.Format("%.0f", account->GetBalance());
		((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(i, 3, balance);
		((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(i, 4, account->GetComment().c_str());
		i++;
	}
}

void FindingForm::OnCalculateButtonClicked() {
	SYSTEMTIME time = { 0, };

	((CDateTimeCtrl*)(this->GetDlgItem(IDC_DATETIMEPICKER_BEGIN)))->GetTime(&time);
	CString _beginDate; _beginDate.Format("%4d%02d%02d", time.wYear, time.wMonth, time.wDay);
	Date beginDate((char*)((LPCTSTR)_beginDate));

	((CDateTimeCtrl*)(this->GetDlgItem(IDC_DATETIMEPICKER_END)))->GetTime(&time);
	CString _endDate; _endDate.Format("%4d%02d%02d", time.wYear, time.wMonth, time.wDay);
	Date endDate((char*)((LPCTSTR)_endDate));

	UpdatingForm *updatingForm = (UpdatingForm *)CWnd::FindWindow("#32770", "가계부");
	((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->DeleteAllItems();

	while (beginDate <= endDate) {
		if (this->indexes != NULL) {
			delete[] indexes;
		}
		updatingForm->cashBook->Find(beginDate, &indexes, &count);
		Long i = 0;
		while (i < count) {
			Account *account = updatingForm->cashBook->GetAt(indexes[i]);
			Date date = account->GetDate();
			CString date_; date_.Format("%4d - %02d - %02d", date.GetYear(), date.GetMonth(), date.GetDay());

			((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->InsertItem(i, date_);
			((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(i, 1, account->GetContent().c_str());

			CString amount;
			amount.Format("%.0f", account->GetAmount());
			((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(i, 2, amount);

			CString balance;
			balance.Format("%.0f", account->GetBalance());
			((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(i, 3, balance);

			((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(i, 4, account->GetComment().c_str());
			i++;
		}
		beginDate++;
	}
	beginDate--;
	Currency totalIncome;
	Currency totalOutgo;
	Currency difference;
	updatingForm->cashBook->Calculate(beginDate, endDate, &totalIncome, &totalOutgo, &difference);

	CString _totalIncome; _totalIncome.Format("%.0f", totalIncome);
	CString _totalOutgo; _totalOutgo.Format("%.0f", totalOutgo);
	CString _difference; _difference.Format("%.0f", difference);

	this->GetDlgItem(IDC_STATIC_SUMOFINCOME)->SetWindowText(_totalIncome);
	this->GetDlgItem(IDC_STATIC_SUMOFOUTGO)->SetWindowText(_totalOutgo);
	this->GetDlgItem(IDC_STATIC_SUMOFBALANCE)->SetWindowText(_difference);
}

//3. 리스트뷰 컨트롤의 아이템을 더블클릭 했을 때
void FindingForm::OnListViewItemDoubleClicked(NMHDR * pNotifyStruct, LRESULT * result) {
	//3.1 리스트뷰의 개인을 읽는다.
	Long index = (Long)((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->GetSelectionMark();
	CString date = ((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->GetItemText(index, 0);
	CString content = ((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->GetItemText(index, 1);
	CString amount = ((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->GetItemText(index, 2);
	CString balance = ((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->GetItemText(index, 3);
	CString comment = ((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->GetItemText(index, 4);

	UpdatingForm *updatingForm = (UpdatingForm*)CWnd::FindWindow("#32770", "가계부");

	Account *account = updatingForm->cashBook->GetAt(index);

	if (dynamic_cast<Income*>(account)) {
		((CButton*)(updatingForm->GetDlgItem(IDC_RADIO_INCOME)))->SetCheck(BST_CHECKED);
		((CButton*)(updatingForm->GetDlgItem(IDC_RADIO_OUTGO)))->SetCheck(BST_UNCHECKED);
	}
	else if (dynamic_cast<Outgo*>(account)) {
		((CButton*)(updatingForm->GetDlgItem(IDC_RADIO_INCOME)))->SetCheck(BST_UNCHECKED);
		((CButton*)(updatingForm->GetDlgItem(IDC_RADIO_OUTGO)))->SetCheck(BST_CHECKED);
	}

	Long year = _ttoi(date.Left(4));
	Long month = _ttoi(date.Mid(7, 2));
	Long day = _ttoi(date.Right(2));

	CTime setDate(year, month, day, 0, 0, 0);
	((CDateTimeCtrl*)(updatingForm->GetDlgItem(IDC_DATETIMEPICKER_DATE)))->SetTime(&setDate);
	(updatingForm->GetDlgItem(IDC_EDIT_CONTENT))->SetWindowText(content);
	(updatingForm->GetDlgItem(IDC_EDIT_AMOUNT))->SetWindowText(amount);
	(updatingForm->GetDlgItem(IDC_STATIC_BALANCE))->SetWindowText(balance);
	(updatingForm->GetDlgItem(IDC_EDIT_COMMENT))->SetWindowText(comment);

	if (this->indexes != NULL) {
		delete[] this->indexes;
	}

	this->EndDialog(0);
}

void FindingForm::OnClose() {
	if (this->indexes != NULL) {
		delete[] this->indexes;
	}
	this->EndDialog(0);
}