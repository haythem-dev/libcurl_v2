#ifndef CONVERT_GUARD
#define CONVERT_GUARD

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace utils
{

//--------------------------------------------------------------------------------------------------//
class Convert
{
public:
	static basar::VarString toVarString(const basar::Int32 number);
    static basar::VarString toVarString(const basar::Int16 number);

	static basar::Int32 toBasarInt32(const basar::VarString& number);
	static basar::Int32 toBasarInt32(const QString& number);

	Convert(void);
	virtual ~Convert(void);
};

} //namespace utils

#endif //CONVERT_GUARD