#include "dlg2.h"

#include <windows.h>

dlg2::dlg2(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
	pushButton->setToolTip("test");

	char str[128];
	sprintf(str, "dlg2: thread id %ld\n", GetCurrentThreadId());
	OutputDebugString(str);
}

dlg2::~dlg2()
{
}