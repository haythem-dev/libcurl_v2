#include "commonheader.h"
#include "narcoticsalertingapp.h"
#ifndef WIN32
#include "gitversioninfo.h"
#endif

extern "C"
{
	extern const char versioninfo[];
	extern const char BASAR_versioninfo[];
}

//---------------------------------------------------------------------------------------------------------//
int main(int argc, char** argv)
{
    try 
	{
		basar::VarString	contributionvoucherdir;
		for (int i = 1; i < argc; i++)
		{
#ifndef WIN32
			if (!strcmp(argv[i], "-version"))
			{
				std::cout << versioninfo << std::endl;
				std::cout << BASAR_versioninfo << std::endl;
				std::cout << GitVersionInfo::instance() << std::endl;
				return 0;
			}
#endif
		}
		if (argc > 2)
		{
			basar::Int32 startDate = atoi(argv[1]);
			basar::Int32 endDate = atoi(argv[2]);
			narcotics::NarcoticsAlertingAppl::getInstance().init(contributionvoucherdir, startDate, endDate);
		}
		else
		{
			narcotics::NarcoticsAlertingAppl::getInstance().init(contributionvoucherdir);
		}
		narcotics::NarcoticsAlertingAppl::getInstance().run();
		narcotics::NarcoticsAlertingAppl::getInstance().shutdown();
		narcotics::NarcoticsAlertingAppl::clear();	
		return 0;
    }
	catch( ... )
	{
		return 1;
	}
}
