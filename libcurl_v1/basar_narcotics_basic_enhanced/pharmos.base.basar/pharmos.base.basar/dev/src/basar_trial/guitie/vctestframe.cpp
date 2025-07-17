#include "vctestframe.h"
#include "test1vc.h"
#include "libbasarguitie.h"

VCTestFrame::VCTestFrame(QWidget *parent)
: QFrame(parent)
{
	ui.setupUi(this);
}

VCTestFrame::~VCTestFrame()
{

}

void VCTestFrame::on_pushButton_clicked()
{
	Policies p; 
	p.m_withTemporaryParent = m_withParent;
	p.m_store = (Qt::Checked == ui.checkBox->checkState ());
	p.m_showTwice = false;
	show(p);
}

void VCTestFrame::on_radioButton_toggled(bool)
{
	m_withParent = true;
}

void VCTestFrame::on_radioButton_2_toggled(bool)
{
	m_withParent = false;
}

void VCTestFrame::on_textEdit_textChanged()
{

}
void VCTestFrame::checkStoredVC()
{
	if (m_vcp)
	{
		ui.textEdit->append("stored ViewConnPtr exists ");
	}
	else
	{
		ui.textEdit->append("stored ViewConnPtr does not exists ");
	}
}

void VCTestFrame::show(const Policies & policies)
{
	bool exists = false;
	ui.textEdit->append("-----------------------------------------------------------");
	checkStoredVC();
	QPointer<QWidget> a;
	{
		basar::gui::tie::ViewConnPtr<Test1VC> w;
		{
			basar::gui::tie::ViewConnPtr<Test1VC> w2;
			w2.initialize();
			if (policies.m_withTemporaryParent)
			{
				w.setParent(w2.getWidgetPtr());
				ui.textEdit->append("set Parent to temporary widget ");
			}
			w->show();
			exists = true;
			if (policies.m_store)
			{
				ui.textEdit->append("store VC");
				this->m_vcp = w;
             	checkStoredVC();
			}
			a = w.getWidgetPtr();
		}
		if (0 == w.getWidgetPtr())
		{
			if (exists)
			{
				ui.textEdit->append("deleted with parent ");
				exists = false;
			}
			// what happes when we show again:
			if (policies.m_showTwice)
			{
				w->show();
				exists = true;
				if (0 == w.getWidgetPtr() )
				{
					ui.textEdit->append("deleted with parent");
					exists = false;
				}
				else
				{
					ui.textEdit->append("recreated");
				}
			}
		}
		else
		{
			ui.textEdit->append("not yet deleted  ");
		}
	}
	if (a.isNull())
	{
		if (exists)
		{
			ui.textEdit->append("deleted with VCPtr");
			exists = false;
		}
	}
	else
	{
		ui.textEdit->append("still there ");
	}
	checkStoredVC();
	ui.textEdit->append("-----------------------------------------------------------");
}

void VCTestFrame::on_pushButton_2_clicked()
{
	Policies p; 
	p.m_withTemporaryParent = m_withParent;
	p.m_store = (Qt::Checked == ui.checkBox->checkState ());
	p.m_showTwice = true;
	show(p);
}

void VCTestFrame::on_pushButton_3_clicked()
{
	ui.textEdit->append("-----------------------------------------------------------");
	checkStoredVC();
	ui.textEdit->append("reset stored ViewConnPtr ");
	m_vcp.reset();
	checkStoredVC();
	ui.textEdit->append("-----------------------------------------------------------");
}