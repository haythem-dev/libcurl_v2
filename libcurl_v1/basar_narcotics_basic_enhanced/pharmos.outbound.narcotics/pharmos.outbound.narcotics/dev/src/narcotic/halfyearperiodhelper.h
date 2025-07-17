#ifndef GUARD_HALFYEARPERIODHELPER_H
#define GUARD_HALFYEARPERIODHELPER_H

class halfyearperiodhelper
{
public:
	enum HalfYear
	{
		H1 = 1,
		H2
	};

	static HalfYear getHalfYearOfDate(const basar::DateTime& date, basar::Date& year);

	static bool isDateInCurrentOrPreviousHalfYear(const basar::DateTime& date);
};

#endif //GUARD_HALFYEARPERIODHELPER_H
