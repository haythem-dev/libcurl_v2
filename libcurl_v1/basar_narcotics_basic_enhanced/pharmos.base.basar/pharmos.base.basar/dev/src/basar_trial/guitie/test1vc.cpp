#include "test1vc.h"

Test1VC::Test1VC(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

Test1VC::~Test1VC()
{

}

				basar::gui::tie::WidgetReturnEnum	Test1VC::show ()
				{
					// do nothing
					this->exec();
					return basar::gui::tie::WidgetReturnEnum();
				}

				void Test1VC::hide ()
				{
					// do nothing
					this->hide();
				}

				bool Test1VC::shutdown	() 
				{
					// do nothing
					return true;
				}

				void Test1VC::use()
				{
					// do nothing
				}

			