#include <UnitTest++/UnitTest++.h>

#include "libbasarguiqt.h"

//------------------------------------------------------------------------------
using basar::Int32;
using basar::gui::qt::Manager;

//------------------------------------------------------------------------------
SUITE(Manager)
{

//------------------------------------------------------------------------------
TEST(InstantiateStructDef)
{
	basar::DefinitionStruct def, 
							def2;
	def.copyright          = "CR";
	def.programDescription = "desc";
	def.version            = "ver";

	Manager::getInstance().init(def);

	def2 = Manager::getInstance().getDef ();
	
	CHECK_EQUAL(def2.copyright         , def.copyright);
	CHECK_EQUAL(def2.programDescription, def.programDescription);
	CHECK_EQUAL(def2.version           , def.version);
}

//------------------------------------------------------------------------------
TEST(InstantiateStructInit)
{
	basar::InitStruct ini, 
					  ini2;

	ini.database = "db";
	ini.dokulink = "http";
	ini.host     = "server";

	Manager::getInstance().init(ini);

	ini2 = Manager::getInstance().getInit();

	CHECK_EQUAL(ini2.database, ini.database);
	CHECK_EQUAL(ini2.dokulink, ini.dokulink);
	CHECK_EQUAL(ini2.host    , ini.host    );	
}

//------------------------------------------------------------------------------
TEST(InstantiateStructDll)
{
	basar::DllStruct        dll[3], 
							dll2;

	for (Int32 i = 0; i < (sizeof(dll)/sizeof(dll[0])); ++i)
	{
		dll[i].name       = "nm";
		dll[i].pkgVersion = "pkgver";
	}

	for (Int32 i = 0; i < (sizeof(dll)/sizeof(dll[0])); ++i)
		Manager::getInstance().initDll(dll[i]);


	unsigned int cnt;
	bool         next;

	for (cnt = 0, next = Manager::getInstance().getFirstDll(dll2);
		 next;
		 ++cnt, next =  Manager::getInstance().getNextDll(dll2))
	{
		CHECK_EQUAL(dll2.name      , dll[cnt].name      );
		CHECK_EQUAL(dll2.pkgVersion, dll[cnt].pkgVersion);
	}

	CHECK_EQUAL(sizeof(dll)/sizeof(dll[0]), cnt);
}

//------------------------------------------------------------------------------
}	// SUITE
