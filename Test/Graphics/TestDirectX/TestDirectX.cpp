#include "stdafx.h"
#include "TestDirectXDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CTestDirectXApp, CWinApp)
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CTestDirectXApp::CTestDirectXApp() {
}

CTestDirectXApp theApp;

BOOL CTestDirectXApp::InitInstance() {

//DEBUGTRACE;
//  AfxEnableControlContainer();
//DEBUGTRACE;

DEBUGTRACE;
    CTestDirectXDlg dlg;
DEBUGTRACE;
    m_pMainWnd = &dlg;
DEBUGTRACE;
    INT_PTR nResponse = dlg.DoModal();
DEBUGTRACE;
    if (nResponse == IDOK) {
    } else if (nResponse == IDCANCEL) {
    }
    return FALSE;
}
