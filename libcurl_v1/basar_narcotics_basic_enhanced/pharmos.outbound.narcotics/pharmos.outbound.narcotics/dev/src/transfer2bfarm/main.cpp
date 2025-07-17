#include "commonheader.h"
#include "transferbfarmappl.h"
#ifndef WIN32
#include "gitversioninfo.h"
#endif

extern "C"
{
	extern const char versioninfo[];
	extern const char BASAR_versioninfo[];
}

//---------------------------------------------------------------------------------------------------------//
int main(int argc, char* argv[])
{
    try 
	{
		bool				crosscheck = false;
		bool				transfer = false;
		bool				archive = false;
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
			if (!strcmp(argv[i], "-crosscheck"))
				crosscheck = true;
			if (!strcmp(argv[i], "-transfer"))
				transfer = true;
			if (!strcmp(argv[i], "-archive"))
				archive = true;
			if (!strcmp(argv[i], "-contributionvoucherdir") && argc >= i+1)
			{
				i++;
				contributionvoucherdir = argv[i];
			}
		}

		narcotics::TransferBfarmAppl::getInstance().init(crosscheck, transfer, archive, contributionvoucherdir);
		narcotics::TransferBfarmAppl::getInstance().run();
		narcotics::TransferBfarmAppl::getInstance().shutdown();
		narcotics::TransferBfarmAppl::clear();	
		return 0;
    }
	catch( ... )
	{
		return 1;
	}
}
