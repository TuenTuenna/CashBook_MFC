//UpdatingForm.cpp
#include "UpdatingForm.h"
#include "CashBook.h"
#include "Account.h"
#include "FindingForm.h"
#include <afxcmn.h>
#include <afxdb.h>
#include <afxdtctl.h>
#include <stdlib.h>
#include "Income.h"
#include "Outgo.h"

BEGIN_MESSAGE_MAP(UpdatingForm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnRecordButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnFindButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CORRECT, OnCorrectButtonClicked)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, OnCalculateButtonClicked)
	ON_EN_KILLFOCUS(IDC_EDIT_AMOUNT, OnAmountEditKillFocus)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ACCOUNTS, OnListViewItemDoubleClicked)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

UpdatingForm::UpdatingForm(CWnd *parent)
	:CDialog(UpdatingForm::IDD, parent) {
	this->cashBook = NULL;
}

//1. 윈도우가 생성되었을 때
BOOL UpdatingForm::OnInitDialog() {
	CDialog::OnInitDialog();

	this->cashBook = new CashBook;
	//1.1 ListView 헤더를 만든다.
	((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->InsertColumn(0, "일자", LVCFMT_CENTER, 100);
	((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->InsertColumn(1, "적요", LVCFMT_CENTER, 75);
	((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->InsertColumn(2, "금액", LVCFMT_CENTER, 100);
	((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->InsertColumn(3, "잔액", LVCFMT_CENTER, 100);
	((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->InsertColumn(4, "비고", LVCFMT_CENTER, 100);

	//1.2 지출라디오박스를 체크한다.
	((CButton*)(this->GetDlgItem(IDC_RADIO_OUTGO)))->SetCheck(BST_CHECKED);
	((CButton*)(this->GetDlgItem(IDC_RADIO_INCOME)))->SetCheck(BST_UNCHECKED);

	// DB에서 Load
	this->Load();

	Long i = 0;
	while( i < this->cashBook->GetLength()) {
		Account *account = this->cashBook->GetAt(i);

		Date date = account->GetDate();
		CString _date; _date.Format("%4d-%02d-%02d", date.GetYear(), date.GetMonth(), date.GetDay());
		((CListCtrl *)GetDlgItem(IDC_LIST_ACCOUNTS))->InsertItem(i, _date);
		((CListCtrl *)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 1, account->GetContent().c_str());

		CString amount; amount.Format("%.0f", account->GetAmount());
		((CListCtrl *)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 2, amount);

		CString balance; balance.Format("%.0f", account->GetBalance());
		((CListCtrl *)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 3, balance);

		((CListCtrl *)GetDlgItem(IDC_LIST_ACCOUNTS))->SetItemText(i, 4, account->GetComment().c_str());
		i++;
	}
	return FALSE;
}


//2. 기재하기 버튼을 클릭했을 때
void UpdatingForm::OnRecordButtonClicked() {
	// 2.1 조건을 읽는다.
	int OutgoCheck = ((CButton *)(this->GetDlgItem(IDC_RADIO_OUTGO)))->GetCheck();
	int IncomeCheck = ((CButton *)(this->GetDlgItem(IDC_RADIO_INCOME)))->GetCheck();

	//2.2 일자, 적요 금액, 비고를 읽는다.
	SYSTEMTIME time = { 0, };
	((CDateTimeCtrl *)(this->GetDlgItem(IDC_DATETIMEPICKER_DATE)))->GetTime(&time);
	CString date; date.Format("%4d-%02d-%02d", time.wYear, time.wMonth, time.wDay);
	CString content; (this->GetDlgItem(IDC_EDIT_CONTENT))->GetWindowText(content);
	CString amount; (this->GetDlgItem(IDC_EDIT_AMOUNT))->GetWindowText(amount);
	CString comment; (this->GetDlgItem(IDC_EDIT_COMMENT))->GetWindowText(comment);

	//2.3 가계부에 기재한다.
	date.Format("%4d%02d%02d", time.wYear, time.wMonth, time.wDay);
	Date cDate((char*)((LPCTSTR)date));
	//Date cDate(time.wYear, time.wMonth, time.wDay);
	//->자료형변환해줘야함->이코드 안됨
	Currency cAmount = atof((LPCTSTR)amount);
	Long index;
	// 지출의 Radio Button Click일 경우
	if (OutgoCheck == BST_CHECKED) {
		cAmount = -cAmount;
	}
	index = this->cashBook->Record(cDate, (LPCTSTR)content, cAmount, (LPCTSTR)comment);
	//DB에 Insert
	this->Insert(index);
	Account *account = this->cashBook->GetAt(index);
	date = account->GetDate();

	//2.4 리스트뷰에 항목을 추가한다.
	cDate = account->GetDate();
	date.Format("%4d-%02d-%02d", cDate.GetYear(), cDate.GetMonth(), cDate.GetDay());
	// ListView에 출력
	((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->InsertItem(index, date);
	((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(index, 1, (account->GetContent().c_str()));
	amount.Format("%.0f", account->GetAmount());
	((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(index, 2, amount);
	CString balance; balance.Format("%.0f", account->GetBalance());
	((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(index, 3, balance);
	((CListCtrl *)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(index, 4, account->GetComment().c_str());

	// EditBox 초기화
	GetDlgItem(IDC_EDIT_CONTENT)->SetWindowText("");
	GetDlgItem(IDC_EDIT_AMOUNT)->SetWindowText("");
	GetDlgItem(IDC_STATIC_BALANCE)->SetWindowText("");
	GetDlgItem(IDC_EDIT_COMMENT)->SetWindowText("");
}


//3. 찾기 버튼을 클릭했을 때
void UpdatingForm::OnFindButtonClicked() {
	FindingForm findingForm;

	//3.1 찾기 윈도우를 생성한다.
	findingForm.DoModal();
}

//4. 고치기 버튼을 클릭했을 때
void UpdatingForm::OnCorrectButtonClicked() {
	//4.1 금액, 비고를 읽는다.
	CString amount; (this->GetDlgItem(IDC_EDIT_AMOUNT))->GetWindowText(amount);
	CString comment; (this->GetDlgItem(IDC_EDIT_COMMENT))->GetWindowText(comment);

	//4.2 리스트뷰의 선택된 항목의 위치를 읽는다.
	Long index = (Long)(((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->GetSelectionMark());

	//4.3 가계부에서 고친다.
	Currency cAmount = atof((LPCTSTR)amount);
	index = this->cashBook->Correct(index, cAmount, (LPCTSTR)comment);
	//DB에 Update
	this->Update(index);
	Account *accountLink = this->cashBook->GetAt(index);

	//4.4 리스트뷰에서 항목을 고쳐적는다.
	cAmount = accountLink->GetAmount();
	amount.Format("%.0f", cAmount);
	((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(index, 2, amount);

	Currency cBalance = accountLink->GetBalance();
	CString balance; balance.Format("%.0f", cBalance);
	((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(index, 3, balance);

	comment = CString(accountLink->GetComment().c_str());
	((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(index, 4, comment);

	//4.5 리스트뷰에서 고쳐적은 항목 이후의 항목들을 고친다.
	Long length = this->cashBook->GetLength();

	index++;
	while (index < length) {
		accountLink = this->cashBook->GetAt(index);
		cBalance = accountLink->GetBalance();
		balance.Format("%.0f", cBalance);
		((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->SetItemText(index, 3, balance);
		index++;
	}
	this->OnAmountEditKillFocus();
}

//5. 정산하기 버튼을 클릭했을 때
void UpdatingForm::OnCalculateButtonClicked() {
	FindingForm findingForm;

	//5.1 찾기(정산하기)윈도우를 생성한다.
	findingForm.DoModal();
}


//6. 금액 에디트가 포커스를 잃었을 때
void UpdatingForm::OnAmountEditKillFocus() {
	//6.1 구분을 읽는다.
	int isIncome = ((CButton*)(this->GetDlgItem(IDC_RADIO_INCOME)))->GetCheck();
	int isOutgo = ((CButton*)(this->GetDlgItem(IDC_RADIO_OUTGO)))->GetCheck();
	//6.2 금액을 읽는다.
	CString amount; (this->GetDlgItem(IDC_EDIT_AMOUNT))->GetWindowText(amount);
	//6.3 가계부의 마지막항목의 잔액을 읽는다.
	Long length = this->cashBook->GetLength();
	Account *accountLink = NULL;
	Currency previousBalance = 0.0;
	if (length > 0) {
		accountLink = this->cashBook->GetAt(length - 1);
		previousBalance = accountLink->GetBalance();
	}
	Currency balance;
	CString setBalance;
	if (isOutgo == BST_CHECKED) {
		balance = previousBalance - (_ttof(amount));
		setBalance.Format("%.0f", balance);
		(this->GetDlgItem(IDC_STATIC_BALANCE))->SetWindowText(setBalance);
	}
	else if (isIncome == BST_CHECKED) {
		balance = previousBalance + (atof((LPCTSTR)amount));
		setBalance.Format("%.0f", balance);
		(this->GetDlgItem(IDC_STATIC_BALANCE))->SetWindowText(setBalance);
	}
}

//7. 리스트뷰 컨트롤의 항목을 더블클릭 했을 때
void UpdatingForm::OnListViewItemDoubleClicked(NMHDR *pNotifyStruct, LRESULT *result) {
	Long index = (Long)(((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->GetSelectionMark());
	CString date = ((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->GetItemText(index, 0);
	CString content = ((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->GetItemText(index, 1);
	CString amount = ((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->GetItemText(index, 2);
	CString balance = ((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->GetItemText(index, 3);
	CString comment = ((CListCtrl*)(this->GetDlgItem(IDC_LIST_ACCOUNTS)))->GetItemText(index, 4);

	Account *account = this->cashBook->GetAt(index);
	if (dynamic_cast<Income*>(account)) {
		((CButton*)(this->GetDlgItem(IDC_RADIO_INCOME)))->SetCheck(BST_CHECKED);
		((CButton*)(this->GetDlgItem(IDC_RADIO_OUTGO)))->SetCheck(BST_UNCHECKED);
	}
	else if (dynamic_cast<Outgo*>(account)) {
		((CButton*)(this->GetDlgItem(IDC_RADIO_INCOME)))->SetCheck(BST_UNCHECKED);
		((CButton*)(this->GetDlgItem(IDC_RADIO_OUTGO)))->SetCheck(BST_CHECKED);
	}

	int year = _ttoi(date.Left(4));
	int month = _ttoi(date.Mid(5, 2));
	int day = _ttoi(date.Right(2));

	CTime setDate(year, month, day, 0, 0, 0);
	((CDateTimeCtrl*)(this->GetDlgItem(IDC_DATETIMEPICKER_DATE)))->SetTime(&setDate);
	(this->GetDlgItem(IDC_EDIT_CONTENT))->SetWindowText(content);
	(this->GetDlgItem(IDC_EDIT_AMOUNT))->SetWindowText(amount);
	(this->GetDlgItem(IDC_STATIC_BALANCE))->SetWindowText(balance);
	(this->GetDlgItem(IDC_EDIT_COMMENT))->SetWindowText(comment);
}

//8. 닫기 버튼을 클릭했을 때
void UpdatingForm::OnClose() {

	if (this->cashBook != NULL) {
		//8.1 가계부를 저장한다.
		this->Save();
		//8.2 가계부를 버린다.
		delete this->cashBook;
	}

	//8.3 윈도우를 닫는다.
	this->EndDialog(0);
}


void UpdatingForm::Save() {

	CDatabase db;
	db.OpenEx("DSN=CashBook;UID=root;PWD=mysql");
	CRecordset rs(&db);

	CString code;
	CString sql = "SELECT Account.code FROM Account;";
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);


	sql = "DELETE FROM ACCOUNT;";
	db.ExecuteSQL(sql);

	Long i = 0;
	while( i < this->cashBook->GetLength()) {

		Account *account = this->cashBook->GetAt(i);
		rs.GetFieldValue((short)0, code);

		if (dynamic_cast<Income *>(account)) {
			sql.Format("INSERT INTO Account(code, date, content, amount, comment) "
				"VALUES('%s', '%4d-%02d-%02d', '%s', %f, '%s');",
				(LPCTSTR)code, account->GetDate().GetYear(), account->GetDate().GetMonth(),
				account->GetDate().GetDay(), account->GetContent().c_str(), account->GetAmount(),
				account->GetComment().c_str());
		}
		else if (dynamic_cast<Outgo *>(account)) {
			sql.Format("INSERT INTO Account(code, date, content, amount, comment) "
				"VALUES('%s', '%4d-%02d-%02d', '%s', %f, '%s');",
				(LPCTSTR)code, account->GetDate().GetYear(), account->GetDate().GetMonth(),
				account->GetDate().GetDay(), account->GetContent().c_str(), account->GetAmount(),
				account->GetComment().c_str());
		}
		db.ExecuteSQL(sql);
		rs.MoveNext();
		i++;
	}
	rs.Close();
	db.Close();
}

void UpdatingForm::Load() {
	CDatabase db;
	CRecordset rs(&db);
	CDBVariant DBdate;

	//DB Connection
	CString sql = "SELECT Account.date, Account.content, Account.amount, Account.comment FROM Account;";
	db.OpenEx("DSN=CashBook;UID=root;PWD=mysql");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	
	CString content;
	CString amount;
	CString comment;

	while (!rs.IsEOF()) {
		rs.GetFieldValue((short)0, DBdate);

		Date date((int)DBdate.m_pdate->year, (Month)DBdate.m_pdate->month, (int)DBdate.m_pdate->day);

		rs.GetFieldValue((short)1, content);
		rs.GetFieldValue((short)2, amount);
		rs.GetFieldValue((short)3, comment);

		this->cashBook->Record(date, (LPCTSTR)content, atof((LPCTSTR)amount), (LPCTSTR)comment);
		rs.MoveNext();
	}
	rs.Close();
	db.Close();
}

void UpdatingForm::Insert(Long index) {
	CDatabase db;
	CString sql;

	Account *account = this->cashBook->GetAt(index);
	sql.Format("INSERT INTO Account(code, date, content, amount, comment) "
		"VALUES('%s', '%4d-%02d-%02d', '%s', %f, '%s');",
		(LPCTSTR)this->GetCode(), account->GetDate().GetYear(), account->GetDate().GetMonth(),
		account->GetDate().GetDay(), account->GetContent().c_str(), account->GetAmount(),
		account->GetComment().c_str());
	db.OpenEx("DSN=CashBook;UID=root;PWD=mysql");
	db.ExecuteSQL(sql);
	db.Close();
}

void UpdatingForm::Update(Long index) {
	CDatabase db;
	CRecordset rs(&db);
	CString sql = "SELECT Account.code FROM Account;";
	db.OpenEx("DSN=CashBook;UID=root;PWD=mysql");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);

	CString code;
	Long number = 0;
	while (!rs.IsEOF() && number <= index) {
		rs.GetFieldValue((short)0, code);
		number++;
		rs.MoveNext();
	}
	Account *account = this->cashBook->GetAt(index);
	sql.Format("UPDATE = Account SET amount='%.0f',comment='%s', WHERE code='%s';", account->GetAmount(), account->GetComment(), (LPCTSTR)code);
	db.ExecuteSQL(sql);
	rs.Close();
	db.Close();
}

CString UpdatingForm::GetCode() {
	CDatabase db;
	CRecordset rs(&db);

	// 내림차순으로 코드를 정렬
	CString sql = "SELECT Account.code FROM Account ORDER BY code DESC;";
	
	// DB connection
	db.OpenEx("DSN=CashBook;UID=root;PWD=mysql");
	rs.Open(AFX_DB_USE_DEFAULT_TYPE, sql);
		
	CString code;
	SYSTEMTIME time{ 0, };
	((CDateTimeCtrl *)(this->GetDlgItem(IDC_DATETIMEPICKER_DATE)))->GetTime(&time);
	
	//수입인지 지출인지 확인한다.
	int outgoCheck = ((CButton*)(this->GetDlgItem(IDC_RADIO_OUTGO)))->GetCheck();
	CString temp("O");
	if (outgoCheck = BST_UNCHECKED) {
		temp.Format("%s", "I");
	}
	code.Format("%4d%02d001%s", time.wYear, time.wMonth,(LPCTSTR)temp);
	
	int year; int month; int tail;
	if (!rs.IsEOF()) {
		rs.GetFieldValue((short)0, code);
		year = _ttoi(code.Left(4));
		month = _ttoi(code.Mid(4, 2));
		tail = _ttoi(code.Mid(6, 3));
		tail++;
		code.Format("%4d%02d%d%s", year,month,tail, (LPCTSTR)temp);
	}
	rs.Close();
	db.Close();

	return code;
}
