#ifndef TEST1VC_H
#define TEST1VC_H

#pragma warning (push)
#pragma warning(disable: 4127 4244 4251 4311 4312 4512 4800)
#include "usings.h"
#include <QT/QDialog.h> 
#include "ui_test1vc.h"
#pragma warning (pop)

 
#include <boost/enable_shared_from_this.hpp>
#include "libbasarguitie_iviewconnector.h"

class Test1VC : public QDialog ,
					public boost::enable_shared_from_this<Test1VC>,
					public basar::gui::tie::IViewConnector

{
	Q_OBJECT

public:
	Test1VC(QWidget *parent = 0);
	~Test1VC();

	/*! \brief show vc´s dialog
					\n no-throw */
					virtual basar::gui::tie::WidgetReturnEnum	show		();

					/*! \brief hide vc´s dialog
					\n no-throw */
					virtual void								hide		();

					/*! \brief close vc´s dialog
					\n no-throw */
					virtual bool								shutdown	() ;
					void							        	use	() ;		

private:
	Ui::Test1VCClass ui;
};

#endif // TEST1VC_H
