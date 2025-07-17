#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <string>

#include <locale>

#include "libbasar_printingol.h"
#include "libbasarlogin_manager.h"

namespace fs = boost::filesystem;

boost::uuids::random_generator generator;

std::string WStringToString(const std::wstring& wstr)
{
    std::string str;
    str.resize(wstr.length());
    wcstombs(&str[0], wstr.c_str(), wstr.size());
    return str;
}

std::wstring StringToWString(const std::string& str)
{
    std::wstring wstr;
    wstr.resize(str.length());
    mbstowcs(&wstr[0], str.c_str(), str.size());
    return wstr;
}

namespace basar {
	namespace printing
	{
		basar::VarString PrintUtils::GetNameExtended(const basar::VarString& srcName, const basar::VarString& ext)
		{
			boost::uuids::uuid uuid = generator();
#if 0
			std::wstring suuid = boost::lexical_cast<std::wstring>(uuid);
			fs::path name(StringToWString(srcName));
			name.replace_extension(suuid + ext.c_str());
			const std::string s = WStringToString(name.wstring());

			return s;
#else
			std::string suuid = boost::lexical_cast<std::string>(uuid);
			fs::path modName(srcName.c_str());
			modName.replace_extension(suuid + ext);

			return modName.string();
#endif
		}

		/**
		* Implicitly evaluate UADM Parameter with best match
		* 1) if areaId > 0 and regionId > 0 using areaId as para1 and regionId as para2
		* 2) if areaId > 0 or regionId > 0 using the parameter > 0 as para1
		* 3) use non of them returning the default parameter
		**/
		bool PrintUtils::getParaValue(const basar::I18nString& rParaName, basar::I18nString& rParaValue)
		{
			bool found = false;

			// implicitly determin areaId from logon
			basar::Int16 areaId = basar::login::Manager::getInstance().getAreaID();
			basar::Int32 areaIdAD = basar::login::Manager::getInstance().getAreaIDAD();
			basar::Int32 branchId = 0;
			basar::Int32 regionId = 0;
			if (areaId > 99)
			{
				regionId = areaId;
			}
			else
			{
				branchId = areaId;
			}
			if (areaIdAD > 99 && regionId == 0)
			{
				regionId = areaIdAD;
			}
			else if (branchId == 0)
			{
				branchId = areaIdAD;
			}

			basar::I18nString regionIdStr;
			basar::I18nString branchIdStr;

			if (regionId > 0 && branchId > 0)
			{
				regionIdStr.format("%ld", regionId);
				branchIdStr.format("%ld", branchId);
				found = basar::login::Manager::getInstance().getParaValue(regionIdStr, branchIdStr, rParaName, rParaValue);
			}
			if (!found && regionId > 0)
			{
				regionIdStr.format("%ld", regionId);
				found = basar::login::Manager::getInstance().getParaValue(regionIdStr, rParaName, rParaValue);
			}
			if (!found && branchId > 0)
			{
				branchIdStr.format("%ld", branchId);
				found = basar::login::Manager::getInstance().getParaValue(branchIdStr, rParaName, rParaValue);
			}
			if (!found)
			{
				found = basar::login::Manager::getInstance().getParaValue(rParaName, rParaValue);
			}
			return found;
		}
	}
}
