#ifndef DATEINFO_GUARD
#define DATEINFO_GUARD

//------------------------------------------------------------------------------
//namespaces
//------------------------------------------------------------------------------
namespace narcotics
{
namespace datecalc
{

//------------------------------------------------------------------------------
class DateInfo
{
public:
	DateInfo(void);
	virtual ~DateInfo(void);

	void setStartDate(basar::Int32 startDate);
	basar::Int32 getStartDate();

	void setEndDate(basar::Int32 endDate);
	basar::Int32 getEndDate();

	void addDaysToStartDate(basar::Int32 nDays);

private:
	basar::Int32 m_startDate;
	basar::Int32 m_endDate;
};

} //namespace datecalc
} //namespace narcotics

#endif //DATEINFO_GUARD