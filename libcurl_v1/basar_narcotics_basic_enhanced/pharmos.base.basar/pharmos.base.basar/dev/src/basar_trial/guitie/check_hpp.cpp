// check for warnings including *.hpp

#include "libbasarguitie_spdeleterforqobjects.hpp"
#include "libbasarguitie_viewconnptr.hpp"



basar::gui::tie::SPDeleterForQObjects <std::string> obj(0);

void t ()
{
	basar::gui::tie::SPDeleterForQObjects <std::string> obj(0);

	std::string str;
	obj(&str);


	QObject qobj;
	basar::gui::tie::SPDeleterForQObjects <QWidget> obj1(&qobj);

	QWidget qobj3;
	basar::gui::tie::SPDeleterForQObjects <QWidget> obj2(&qobj3);


	boost::shared_ptr<QWidget> shstr;

	basar::gui::tie::ViewConnPtr < QWidget> vc1;
	basar::gui::tie::ViewConnPtr < QWidget> vc2(shstr);

	vc1->clearFocus();

	vc2.unique();

	bool t = (operator==(vc1, vc2));

	t = t;
}