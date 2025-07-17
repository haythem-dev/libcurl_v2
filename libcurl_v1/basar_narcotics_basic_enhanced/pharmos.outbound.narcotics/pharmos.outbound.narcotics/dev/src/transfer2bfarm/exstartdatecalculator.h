#ifndef EXSTARTDATECALCULATOR_GUARD
#define EXSTARTDATECALCULATOR_GUARD

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace narcotics
{
namespace exceptions
{

//--------------------------------------------------------------------------------------------------//
class ExStartDateCalculator : public basar::cmnutil::BasarRuntimeException
{
public:
	ExStartDateCalculator(const basar::ExceptInfo& sInfo);
	virtual ~ExStartDateCalculator(void);
};

} //namespace exceptions
} //namespace narcotics

#endif //EXSTARTDATECALCULATOR_GUARD