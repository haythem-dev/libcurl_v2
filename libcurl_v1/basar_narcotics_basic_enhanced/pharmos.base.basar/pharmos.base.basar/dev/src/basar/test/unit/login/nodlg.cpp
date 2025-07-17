#include <UnitTest++/UnitTest++.h>

#include "libbasarlogin.h"

#include "libbasarcmnutil_locale.h"

using basar::I18nString;
using basar::ConstString;
using basar::Int16;
using basar::cmnutil::Locale;
using basar::login::Manager;

#ifdef _WIN32
#pragma warning (push)
#pragma warning (disable: 4512 4127)
#include "libbasarqtwidget_i18n.h"
#include <QtCore/QLocale>
#include <QtCore/QTextCodec>
#pragma warning (pop)

using basar::gui::widget::I18n;
#endif

//------------------------------------------------------------------------------

SUITE(NoDlg)
{
	// ----------------------------------------------------------
	
	TEST (testlogin_basic_basar_function)
	{

		
		basar::VarString str("test");
		
		str.upper();
		
		CHECK_EQUAL("TEST", str.c_str());
		
		
	}
	
	TEST (testlogin_I18n_basar_function)
	{
		Locale::setLocale(Locale::GERMAN_DE);
		
		I18nString appl("DEPENT");
		
		appl.upper();
		
		CHECK_EQUAL("DEPENT", appl.c_str());
		
		I18nString user	= "t.hoerath";
		user.upper();
		
		CHECK_EQUAL("T.HOERATH", user.c_str());
	}
	
	
	TEST (testlogin_withoutdlg)
	{
		I18nString appl("DEPENT");
		I18nString user	= "t.hoerath";
		I18nString pw	= "rdzdSEQE";
		Int16 area		= 5;
		
		CHECK( Manager::getInstance().nonInteractiveLogin( appl, user, pw, area ) );
	}
	
	// ----------------------------------------------------------
	TEST (testlogin_userdata)
	{
		CHECK_EQUAL( 6  , Manager::getInstance().getApplID() );
		CHECK_EQUAL( 16 , Manager::getInstance().getUserID() );
		CHECK_EQUAL( "Hörath" , Manager::getInstance().getSurName().c_str() );
		
		// login was without language, so default language is used
		CHECK_EQUAL( "EN" , Manager::getInstance().getLanguageID() );
	}
	
	// ----------------------------------------------------------
	TEST (testlogin_legitimation)
	{
		I18nString right = "execute";
		CHECK( Manager::getInstance().isLegitimated(1,right) );

		Int16 ppe = 27;
		CHECK( !(Manager::getInstance().isLegitimated(1,right, ppe)) );
	}
	
	// ----------------------------------------------------------
	TEST (testlogin_parameter)
	{
		I18nString para1 = "5";
		I18nString parabez = "DB";
		I18nString paraVal;
		
		CHECK( Manager::getInstance().getParaValue(para1, parabez, paraVal) ); //(para1),(para2),parabez,paravalue
		CHECK_EQUAL( "ode21" , paraVal.c_str() );
	}

	// ----------------------------------------------------------
	TEST (testlogin_withoutdlg_language)
	{
		I18nString appl	= "DEPENT";
		I18nString user	= "t.hoerath";
		I18nString pw	= "rdzdSEQE";
		Int16 area		= 5;
		ConstString lang = "de";

		// login with explicit language --> language of former login is overwritten
		CHECK( Manager::getInstance().nonInteractiveLogin( appl, user, pw, area, lang ) );
		CHECK_EQUAL( "DE" , Manager::getInstance().getLanguageID().c_str() );
	
		// login without explicit language --> language of former login is used
		CHECK( Manager::getInstance().nonInteractiveLogin( appl, user, pw, area ) );
		CHECK_EQUAL( "DE" , Manager::getInstance().getLanguageID().c_str() );
	}

	// ----------------------------------------------------------
	TEST (testlogin_branchlist_for_userright)
	{
		I18nString right = "execute";
		
		basar::login::CollBranchesWithRight coll = Manager::getInstance().getBranchesWithRight( 1, right );

		CHECK_EQUAL( 20, static_cast<int>(coll.size()));

		basar::login::CollBranchesWithRight::iterator it = coll.begin();
		CHECK_EQUAL( 5, it->first ); //Fürth

		++it;
		CHECK_EQUAL( "Berlin", it->second.c_str() );

		// ------------
		basar::login::CollBranchesWithRightOrderedByName coll2 = Manager::getInstance().getBranchesWithRightOrderedByName( 1, right );

		CHECK_EQUAL( 20, static_cast<int>(coll2.size()));

		basar::login::CollBranchesWithRightOrderedByName::iterator it2 = coll2.begin();
		CHECK_EQUAL( 44, it2->second ); //Augsburg

		++it2;
		CHECK_EQUAL( "Bad Kreuznach", it2->first.c_str() );
	}

	// ----------------------------------------------------------
	TEST (testlogin_rightlist_for_modandarea)
	{
		Int16 mod  = 1;
		Int16 area = 5;
		
		basar::login::CollRightsForModAndArea coll = Manager::getInstance().getRightsByModuleAndArea( mod, area );

		CHECK_EQUAL( 1, static_cast<int>(coll.size()));

		basar::login::CollRightsForModAndArea::iterator it = coll.begin();
		CHECK_EQUAL( "EXECUTE", it->c_str() );

		Manager::clear();
	}

	// ----------------------------------------------------------
	TEST (testlogin_withoutdlg_language_2)
	{
		I18nString appl	 = "DEPENT";
		I18nString user	 = "t.hoerath";
		I18nString pw	 = "rdzdSEQE";
		Int16 area		 = 5;
		ConstString lang = "de";

		//new instance of login - no stored language from former login
		CHECK( Manager::getInstance().nonInteractiveLogin( appl, user, pw, area, lang ) ); 
		CHECK_EQUAL( "DE" , Manager::getInstance().getLanguageID().c_str() );

		Manager::clear();
	}

	// ----------------------------------------------------------
	TEST (testlogin_withoutdlg_param_de)
	{
		I18nString appl	= "DEPENT";
		I18nString user	= "t.hoerath";
		I18nString pw	= "rdzdSEQE";
		Int16 area      = 5;

		//new instance of login - no stored language from former login
		CHECK( Manager::getInstance().nonInteractiveLogin( appl, user, pw, area) ); 

		I18nString paramCond;
		paramCond.itos(area);
		I18nString paramVal;

        I18nString paramName = basar::login::PARAM_BASAR_LOCALE;
		CHECK( !Manager::getInstance().getParaValue(paramCond, paramName, paramVal) );

		paramName = basar::login::PARAM_BASAR_CODEPAGE;
		CHECK( !Manager::getInstance().getParaValue(paramCond, paramName, paramVal) );

		// basar locale, Qt settings
		CHECK_EQUAL(Locale::LOCALE_DEFAULT, Locale::getLocale  ());
		CHECK_EQUAL(Locale::CP_DEFAULT    , Locale::getCodepage());
#ifdef _WIN32
		CHECK_EQUAL(I18n::getQtCodecName(Locale::CP_DEFAULT), QTextCodec::codecForLocale()->name().constData());

		CHECK      (I18n::getQtLocale(Locale::LOCALE_DEFAULT) == QLocale());
#endif

		Manager::clear();
	}

	// ----------------------------------------------------------
	TEST (testlogin_withoutdlg_param_bg)
	{
		I18nString appl	= "DEPENT";
		I18nString user	= "t.hoerath";
		I18nString pw	= "rdzdSEQE";
		Int16 area      = 28;

		//new instance of login - no stored language from former login
		CHECK( Manager::getInstance().nonInteractiveLogin( appl, user, pw, area) ); 

		I18nString paramCond;
		paramCond.itos(area);
		I18nString paramName = basar::login::PARAM_BASAR_LOCALE;
		I18nString paramVal;
		
		CHECK( Manager::getInstance().getParaValue(paramCond, paramName, paramVal) );
		CHECK_EQUAL(Locale::getLocaleString(Locale::BULGARIAN_BG),
					paramVal.c_str() );

		// basar locale, Qt settings
		CHECK_EQUAL(Locale::BULGARIAN_BG, Locale::getLocale());
		CHECK_EQUAL(Locale::CP_CYRILLIC , Locale::getCodepage());
#ifdef _WIN32
		CHECK_EQUAL(I18n::getQtCodecName(Locale::CP_CYRILLIC), 
					QTextCodec::codecForLocale()->name().constData());
		CHECK      (I18n::getQtLocale(Locale::BULGARIAN_BG) == QLocale());
#endif

		Manager::clear();
	}

	// ----------------------------------------------------------
	TEST (testlogin_withoutdlg_param_two_sequential_logins)
	{
		I18nString appl	= "DEPENT";
		I18nString user	= "t.hoerath";
		I18nString pw	= "rdzdSEQE";

		I18nString paramVal;
		I18nString paramCond;
		I18nString paramName = basar::login::PARAM_BASAR_LOCALE;

		// 1st login
		Int16 area      = 5;
		CHECK( Manager::getInstance().nonInteractiveLogin( appl, user, pw, area) ); 

		paramCond.itos(area);
		CHECK( !Manager::getInstance().getParaValue(paramCond, paramName, paramVal) );

		// basar locale, Qt settings
		CHECK_EQUAL(Locale::LOCALE_DEFAULT, Locale::getLocale());
		CHECK_EQUAL(Locale::CP_DEFAULT    , Locale::getCodepage());
#ifdef _WIN32
		CHECK_EQUAL(I18n::getQtCodecName(Locale::CP_DEFAULT), 
					QTextCodec::codecForLocale()->name().constData());
		CHECK      (I18n::getQtLocale(Locale::LOCALE_DEFAULT) == QLocale());
#endif

		// 2nd login
		area      = 28;
		CHECK( Manager::getInstance().nonInteractiveLogin( appl, user, pw, area) ); 

		paramCond.itos(area);
		CHECK( Manager::getInstance().getParaValue(paramCond, paramName, paramVal) );
		CHECK_EQUAL(Locale::getLocaleString(Locale::BULGARIAN_BG), 
					paramVal.c_str() );

		// basar locale, Qt settings
		CHECK_EQUAL(Locale::BULGARIAN_BG, Locale::getLocale());
		CHECK_EQUAL(Locale::CP_CYRILLIC , Locale::getCodepage());
#ifdef _WIN32
		CHECK_EQUAL(I18n::getQtCodecName(Locale::CP_CYRILLIC), 
					QTextCodec::codecForLocale()->name().constData());
		CHECK      (I18n::getQtLocale(Locale::BULGARIAN_BG) == QLocale());
#endif

		Manager::clear();
	}

	// ----------------------------------------------------------
	TEST (testlogin_withoutdlg_param_hr_differing_codepage)
	{
		I18nString appl	= "DEPENT";
		I18nString user	= "t.hoerath";
		I18nString pw	= "rdzdSEQE";
		Int16 area      = 71;

		//new instance of login - no stored language from former login
		CHECK( Manager::getInstance().nonInteractiveLogin( appl, user, pw, area) ); 

		I18nString paramCond;
		paramCond.itos(area);
		I18nString paramName = basar::login::PARAM_BASAR_LOCALE;
		I18nString paramVal;
		
		CHECK( Manager::getInstance().getParaValue(paramCond, paramName, paramVal) );
		CHECK_EQUAL( Locale::getLocaleString(Locale::CROATIAN_HR), paramVal.c_str() );

		paramName = basar::login::PARAM_BASAR_CODEPAGE;
		CHECK( Manager::getInstance().getParaValue(paramCond, paramName, paramVal) );
		CHECK_EQUAL( Locale::getCodepageString(Locale::CP_EAST), paramVal.c_str() );

		// basar locale, Qt settings
		CHECK_EQUAL(Locale::CROATIAN_HR, Locale::getLocale());
		CHECK_EQUAL(Locale::CP_EAST    , Locale::getCodepage());
#ifdef _WIN32
		CHECK_EQUAL(I18n::getQtCodecName(Locale::CP_EAST), 
					QTextCodec::codecForLocale()->name().constData());
		CHECK      (I18n::getQtLocale(Locale::CROATIAN_HR) == QLocale());
#endif

		Manager::clear();
	}

	// ----------------------------------------------------------
	TEST (testlogin_withoutdlg_param_hr_differing_codepage_exception)
	{
		I18nString appl	= "DEPENT";
		I18nString user	= "t.hoerath";
		I18nString pw	= "rdzdSEQE";
		Int16 area      = 74;

		//new instance of login - no stored language from former login
		CHECK_THROW(Manager::getInstance().nonInteractiveLogin( appl, user, pw, area), 
					basar::InvalidParameterException); 

		Manager::clear();
	}

	// ----------------------------------------------------------
	TEST (testlogin_withoutdlg_param_hr)
	{
		I18nString appl	= "DEPENT";
		I18nString user	= "t.hoerath";
		I18nString pw	= "rdzdSEQE";
		Int16 area      = 72;

		//new instance of login - no stored language from former login
		CHECK( Manager::getInstance().nonInteractiveLogin( appl, user, pw, area) ); 

		I18nString paramCond;
		paramCond.itos(area);
		I18nString paramName = basar::login::PARAM_BASAR_LOCALE;
		I18nString paramVal;
		
		CHECK( Manager::getInstance().getParaValue(paramCond, paramName, paramVal) );
		CHECK_EQUAL( Locale::getLocaleString(Locale::CROATIAN_HR), paramVal.c_str() );

		paramName = basar::login::PARAM_BASAR_CODEPAGE;
		CHECK( !Manager::getInstance().getParaValue(paramCond, paramName, paramVal) );

		// basar locale, Qt settings
		CHECK_EQUAL(Locale::CROATIAN_HR, Locale::getLocale());
		CHECK_EQUAL(Locale::CP_WEST    , Locale::getCodepage());
#ifdef _WIN32
		CHECK_EQUAL(I18n::getQtCodecName(Locale::CP_WEST), 
					QTextCodec::codecForLocale()->name().constData());
		CHECK      (I18n::getQtLocale(Locale::CROATIAN_HR) == QLocale());
#endif

		Manager::clear();
	}

	// ----------------------------------------------------------
	TEST (testlogin_withoutdlg_param_hr_exception)
	{
		I18nString appl	= "DEPENT";
		I18nString user	= "t.hoerath";
		I18nString pw	= "rdzdSEQE";
		Int16 area      = 73;

		//new instance of login - no stored language from former login
		CHECK_THROW(Manager::getInstance().nonInteractiveLogin( appl, user, pw, area), 
			        basar::InvalidParameterException ); 

		Manager::clear();
	}

	// ----------------------------------------------------------
	TEST (testlogin_withoutdlg_invalid_appl)
	{
		I18nString appl	= "DEPENTXXX";
		I18nString user	= "t.hoerath";
		I18nString pw	= "rdzdSEQE";
		Int16 area      = 72;

		CHECK_THROW(Manager::getInstance().nonInteractiveLogin( appl, user, pw, area), 
					basar::login::InvalidApplNameException);

		Manager::clear();
	}
	// ----------------------------------------------------------
	TEST (testlogin_withoutdlg_invalid_area)
	{
		I18nString appl	= "DEPENT";
		I18nString user	= "t.hoerath";
		I18nString pw	= "rdzdSEQE";
		Int16 area      = -123;

		CHECK_THROW(Manager::getInstance().nonInteractiveLogin( appl, user, pw, area), 
					basar::login::InvalidAreaException);

		Manager::clear();
	}
	
	// ----------------------------------------------------------
	TEST (testlogin_withoutdlg_region_only)
	{
		I18nString appl	= "DEPENT";
		I18nString user	= "t.hoerath";
		I18nString pw	= "rdzdSEQE";
		Int16 area      = 5;

        Manager::getInstance().setLoginMode(basar::login::LOGIN_MODE_REGION_ONLY);
        
        Manager::getInstance().nonInteractiveLogin( appl, user, pw, area);


		I18nString right = "execute";
		
		basar::login::CollBranchesWithRight coll = Manager::getInstance().getBranchesWithRight( 1, right );
		
		CHECK_EQUAL(1, static_cast<int>(coll.size()) );
		
		Int16 mod  = 1;
		
		basar::login::CollRightsForModAndArea coll2 = Manager::getInstance().getRightsByModuleAndArea( mod, area );

		CHECK_EQUAL( 1, static_cast<int>(coll2.size()));

		basar::login::CollRightsForModAndArea::iterator it = coll2.begin();
		CHECK_EQUAL( "EXECUTE", it->c_str() );
		
		mod = 1;
		area = 42;
		
		coll2 = Manager::getInstance().getRightsByModuleAndArea( mod, area);
		
		CHECK_EQUAL(true, coll2.empty());
		
		Manager::clear();
	}	
	
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
