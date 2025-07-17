#ifndef GUARD_LOGINUSERTYPE_H
#define GUARD_LOGINUSERTYPE_H

// namespace section
namespace basar
{
	namespace login
	{
		enum UserTypeEnum {
			DEVELOPERS					= 0,
    		NONDEVELOPERS				= 1,
			BRANCH_STANDARD_EMPLOYEES	= 2,
			MAIN_DEPARTMENT				= 3,
			CUSTOMER_CARE				= 4
		};
	}

} // namespace domMod
#endif // end GUARD_LOGINUSERTYPE_H