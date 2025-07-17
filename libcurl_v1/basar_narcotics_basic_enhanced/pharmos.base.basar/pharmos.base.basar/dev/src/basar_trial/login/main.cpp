
#include <libbasar_definitions.h>
#include <libbasarcmnutil.h>

#include <libbasarlogin.h>
#include <libbasardbsql.h>
#include <libbasarproperty.h>
#include <libbasardbaspect.h>

#include <stdio.h>
#include <iostream>

#ifdef WIN32
	#pragma warning (push)
	#pragma warning(disable : 4127 4311 4312 4512 4800 4244)
	#include <Qt/qapplication.h>
	#pragma warning (pop)
#endif


//---------------------------------------------------------------------------
#ifdef SINGLETON_TEST

static const bool define()
{
	basar::db::aspect::AccessorDefinitionRef adr=basar::db::aspect::Manager::getInstance().createAccessorDefinition("main");
	return true;
}
static bool dummy = define();

#endif


//------------------------------------------------------------------------------
void test_version()
{
	printf("\n");
	printf("--- test_version ---------------------------\n");
	printf("\n");

//	printf("pkg           : %s\n", basar::login::getPkgVersion ());
	printf("ver           : %s\n", basar::login::getVersion    ());

	basar::VarString ver1 = basar::login::getFullVersion();
	basar::VarString ver2 = ver1.right("IT ");
	printf("full          : %s\n", ver1.left (" version").c_str());
	printf("                %s\n", ver2.left (" built:" ).c_str());

#ifdef _DEBUG
	printf("                %s\n", ver2.right("dbg "    ).c_str());
#else
	printf("                %s\n", ver2.right("rel "    ).c_str());
#endif

	printf("\n");
}

	
//------------------------------------------------------------------------------
void withdlg()
{
	//basar::I18nString appl = "TESTEXE";
	basar::I18nString appl = "ACTIVEBI";
	basar::I18nString title = "TestApp";

	bool loginSuccess = basar::login::Manager::getInstance().showDlg(appl,title,2);

	if (!loginSuccess) // Anwendung, Fenstertitel, Login-Versuche
	{
		printf("\nLOGIN with dlg FAILED\n");
	}
	else
	{
		//1. Zugriff auf Login-Daten
		printf("\nLOGIN OK\n");
		
		printf("\napplicationid = %d\n", basar::login::Manager::getInstance().getApplID());
		printf("applicationname = %s\n", basar::login::Manager::getInstance().getApplName().c_str());
		printf("userid = %d\n", basar::login::Manager::getInstance().getUserID());
		printf("username = %s\n", basar::login::Manager::getInstance().getUserName().c_str());
		printf("languageid = %s\n", basar::login::Manager::getInstance().getLanguageID().c_str());
		printf("languagename = %s\n", basar::login::Manager::getInstance().getLanguageName().c_str());
		printf("areaid = %d\n", basar::login::Manager::getInstance().getAreaID());
		printf("areaname = %s\n", basar::login::Manager::getInstance().getAreaName().c_str());
		printf("surname = %s\n", basar::login::Manager::getInstance().getSurName().c_str());
		printf("forename = %s\n", basar::login::Manager::getInstance().getForeName().c_str());
		printf("description = %s\n", basar::login::Manager::getInstance().getDescription().c_str());
		printf("password = %s\n", basar::login::Manager::getInstance().getCryptPW().c_str());
		printf("ntuser = %s\n", basar::login::Manager::getInstance().getNtUser().c_str());
		printf("email = %s\n", basar::login::Manager::getInstance().getEmail().c_str());
		printf("mail = %s\n", basar::login::Manager::getInstance().getMail().c_str());
	
		//2. Berechtigung für ein Modul erfragen:
		basar::I18nString right = "execute";
		bool legitimated = basar::login::Manager::getInstance().isLegitimated(1,right);
		
		if ( legitimated )
			printf("\nright <%s> set for this user\n", right.c_str());
		else
			printf("\nright <%s> NOT set for this user\n", right.c_str());
		
		
		//3. Berechtigung für ein Modul in anderer als angemeldeter Region erfragen:
		basar::Int16 areaID = 7;
		legitimated = basar::login::Manager::getInstance().isLegitimated(1,right,areaID);//moduleid, rightname, branchregionid (optional)

		if ( legitimated )
			printf("\nright <%s> set for this user in area %d\n", right.c_str(), areaID);
		else
			printf("\nright <%s> NOT set for this user in area %d\n", right.c_str(), areaID);
		
		
		//4. Parameter auslesen:
		basar::I18nString parabez = "PORT";
		basar::I18nString paraVal;
		bool paraFound = basar::login::Manager::getInstance().getParaValue(parabez,paraVal);//(para1),(para2),parabez,paravalue
		
		if ( paraFound )
			printf("\nparameter <%s> = %s\n", parabez.c_str(), paraVal.c_str());
		else
			printf("\nparameter <%s> not found\n", parabez.c_str());

			
		//------------------------------------
		parabez = "DB";
		areaID = basar::login::Manager::getInstance().getAreaID();
		basar::I18nString para1;
		para1.format("%d",areaID);

		paraFound = basar::login::Manager::getInstance().getParaValue(para1,parabez,paraVal);//(para1),(para2),parabez,paravalue
		
		if ( paraFound )
			printf("\nparameter <%s> = %s for para1 <%d>\n", parabez.c_str(), paraVal.c_str(), areaID);
		else
			printf("\nparameter <%s> not found for para1 <%d>\n", parabez.c_str(), areaID);

		//------------------------------------
		parabez = "FAXNR";
		basar::Int32 userid = basar::login::Manager::getInstance().getUserID();
		basar::I18nString para2 = "test";
		para1 = "99";
	
		paraFound = basar::login::Manager::getInstance().getParaValue(para1,para2,parabez,paraVal);//(para1),(para2),parabez,paravalue
		
		if ( paraFound )
			printf("\nparameter <%s> = %s for para1 <%d>, para2 <%d>\n", parabez.c_str(), paraVal.c_str(), areaID, userid);
		else
			printf("\nparameter <%s> not found for para1 <%d>, para2 <%d>\n", parabez.c_str(), areaID, userid);	
	}
}

//------------------------------------------------------------------------------
void nodlg()
{
	basar::I18nString appl = "TRANSORD";
	basar::I18nString user = "r.kiefert";
	basar::I18nString pw = "O(~%CY3)";
	basar::Int16 area = 5;
	
	bool loginSuccess = basar::login::Manager::getInstance().withoutDlg(appl,user, pw, area);


	if (loginSuccess == false)
	{
		printf("\nLOGIN without dlg FAILED\n");
	}
	else
	{
		//1. Zugriff auf Login-Daten
		printf("\nLOGIN OK\n");
		
		printf("\napplicationid = %d\n", basar::login::Manager::getInstance().getApplID());
		printf("applicationname = %s\n", basar::login::Manager::getInstance().getApplName().c_str());
		printf("userid = %d\n", basar::login::Manager::getInstance().getUserID());
		printf("username = %s\n", basar::login::Manager::getInstance().getUserName().c_str());
		printf("languageid = %s\n", basar::login::Manager::getInstance().getLanguageID().c_str());
		printf("languagename = %s\n", basar::login::Manager::getInstance().getLanguageName().c_str());
		printf("areaid = %d\n", basar::login::Manager::getInstance().getAreaID());
		printf("areaname = %s\n", basar::login::Manager::getInstance().getAreaName().c_str());
		printf("surname = %s\n", basar::login::Manager::getInstance().getSurName().c_str());
		printf("forename = %s\n", basar::login::Manager::getInstance().getForeName().c_str());
		printf("description = %s\n", basar::login::Manager::getInstance().getDescription().c_str());
		printf("password = %s\n", basar::login::Manager::getInstance().getCryptPW().c_str());
		printf("ntuser = %s\n", basar::login::Manager::getInstance().getNtUser().c_str());
		printf("email = %s\n", basar::login::Manager::getInstance().getEmail().c_str());
		printf("mail = %s\n", basar::login::Manager::getInstance().getMail().c_str());
	
		//2. Berechtigung für ein Modul erfragen:
		basar::I18nString right = "execute";
		bool legitimated = basar::login::Manager::getInstance().isLegitimated(1,right);
		
		if ( legitimated )
			printf("\nright <%s> set for this user\n", right.c_str());
		else
			printf("\nright <%s> NOT set for this user\n", right.c_str());
		
		
		//3. Berechtigung für ein Modul in anderer als angemeldeter Region erfragen:
		basar::Int16 areaID = 7;
		legitimated = basar::login::Manager::getInstance().isLegitimated(1,right,areaID);//moduleid, rightname, branchregionid (optional)

		if ( legitimated )
			printf("\nright <%s> set for this user in area %d\n", right.c_str(), areaID);
		else
			printf("\nright <%s> NOT set for this user in area %d\n", right.c_str(), areaID);
		
		
		//4. Parameter auslesen:
		basar::I18nString parabez = "PORT";
		basar::I18nString paraVal;
		bool paraFound = basar::login::Manager::getInstance().getParaValue(parabez,paraVal);//(para1),(para2),parabez,paravalue
		
		if ( paraFound )
			printf("\nparameter <%s> = %s\n", parabez.c_str(), paraVal.c_str());
		else
			printf("\nparameter <%s> not found\n", parabez.c_str());

			
		//------------------------------------
		parabez = "DB";
		areaID = basar::login::Manager::getInstance().getAreaID();
		basar::I18nString para1;
		para1.format("%d",areaID);

		paraFound = basar::login::Manager::getInstance().getParaValue(para1,parabez,paraVal);//(para1),(para2),parabez,paravalue
		
		if ( paraFound )
			printf("\nparameter <%s> = %s for para1 <%d>\n", parabez.c_str(), paraVal.c_str(), areaID);
		else
			printf("\nparameter <%s> not found for para1 <%d>\n", parabez.c_str(), areaID);

		//------------------------------------
		parabez = "FAXNR";
		basar::Int32 userid = basar::login::Manager::getInstance().getUserID();
		basar::I18nString para2 = "test";
		para1 = "99";

		paraFound = basar::login::Manager::getInstance().getParaValue(para1,para2,parabez,paraVal);//(para1),(para2),parabez,paravalue
		
		if ( paraFound )
			printf("\nparameter <%s> = %s for para1 <%d>, para2 <%d>\n", parabez.c_str(), paraVal.c_str(), areaID, userid);
		else
			printf("\nparameter <%s> not found for para1 <%d>, para2 <%d>\n", parabez.c_str(), areaID, userid);	
	
	}
}


//------------------------------------------------------------------------------
void test_login(int argc, char * argv[])
{

#ifdef WIN32
	QApplication a(argc, argv);
	withdlg();
#endif

	nodlg();
//	withdlg();
//	nodlg();

	{
		basar::login::Manager::clear();
		basar::db::aspect::Manager::clear();
	}
}

//------------------------------------------------------------------------------
int main(int argc, char * argv[])
{
	// eliminate compiler warnings 
	argc = argc;
	argv = argv;

	printf("###########################################################################\n");
	printf("#                                                                         #\n");
	printf("# test login                                                              #\n");
	printf("#                                                                         #\n");
	printf("###########################################################################\n");
	printf("\n");

	try
	{
		test_version();
		test_login	(argc, argv );
	}
	catch (const basar::Exception & except)
	{
		printf("\n");
		printf("=== exception caught ===========================================================\n");
		printf("\n");
		printf("%s\n", except.what().c_str());
	}
	catch (...)
	{
		printf("=== unknown exception caught ===========================================================\n");	

		basar::login::Manager::clear();
		basar::db::aspect::Manager::clear();
	}

	printf("\npress return to continue\n");
	getchar();
	
	printf("\n");
	printf("################################################################################\n");

	printf("\npress return to continue\n");
	getchar();
	
	return 0;
}
