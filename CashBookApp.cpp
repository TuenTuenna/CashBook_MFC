//CashBookApp.cpp
#include "CashBookApp.h"
#include "UpdatingForm.h"

BOOL CashBookApp::InitInstance() {
	UpdatingForm updatingForm;
	this->m_pMainWnd = &updatingForm;
	updatingForm.DoModal();

	return TRUE;

}
CashBookApp cashBookApp;