#pragma warning (push)
#pragma warning(disable: 4127 4311 4312 4512) 
#include <Qt/qapplication.h>
#include "libbasarguitie_qtmessagehandler.h"

#include "testguitie.h"
#include "comboboxes.h"

#include <windows.h>
#pragma warning (pop)

BEGIN_QUERY_BUILDER_DECLARATION(ArticleSearch)
//BEGIN_AGGREGATE_QUERY_BUILDER_DECLARATION(ArticleSearch)
END_BUILDER_DECLARATION
BEGIN_WRITE_BUILDER_DECLARATION(ArticleSave)
END_BUILDER_DECLARATION
BEGIN_WRITE_BUILDER_DECLARATION(FArticleSave)
END_BUILDER_DECLARATION

BUILDER_DEFINITION(ArticleSearch)
ENSURE_PROPERTIES_ARE_SET(ArticleSearch,"artikel_nr;")
SQL_PATTERN(ArticleSearch,
            //"select count(*) from zartikel where artikel_nr = #artikel_nr# ")
            "select z.artikel_nr as artikel_nr, artikel_name, filialnr, bestand, abc_artikel, internesteuerung, datum_aender, klasse_faktor "
            "from zartikel z, artikelf f where z.artikel_nr = f.artikel_nr and "
            //"z.artikel_name = #artikel_nr#   ")
            "z.artikel_nr = #artikel_nr#   ")
            BUILDER_DEFINITION(ArticleSave)
            ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(ArticleSave,"artikel_name;datum_aender;","UPDATE")
            SQL_PATTERN(ArticleSave,
            "update zartikel set artikel_name = #artikel_name#, datum_aender = #datum_aender# "
            "where artikel_nr = #artikel_nr# ")
            BUILDER_DEFINITION(FArticleSave)
            ENSURE_PROPERTIES_ARE_SET_AND_ONE_IN_STATE(FArticleSave,"bestand;klasse_faktor;abc_artikel;","UPDATE")
            SQL_PATTERN(FArticleSave,
            "update artikelf set bestand = #bestand#, klasse_faktor =#klasse_faktor#, abc_artikel=#abc_artikel# "
            "where artikel_nr = #artikel_nr# and filialnr = #filialnr# ")

            //enum SupportedTypeEnum { UNKNOWN=0, INT16, INT32, FLOAT32, FLOAT64, DECIMAL, STRING };  

            BEGIN_ACCESSOR_DEFINITION("AccessorArticle")
            PROPERTY_DESCRIPTION_LIST(	"int32 artikel_nr; string artikel_name; int16 filialnr; "
            "int32 bestand; string abc_artikel; int16 internesteuerung; "
            "int32 datum_aender; int32 klasse_faktor;")
            ACCESS_METHOD("ReadArticle")
            SQL_BUILDER_CREATE(ArticleSearch)
            SQL_BUILDER_PUSH_BACK(ArticleSearch)
            ACCESS_METHOD("SaveArticle")
            SQL_BUILDER_CREATE(ArticleSave)
            SQL_BUILDER_PUSH_BACK(ArticleSave)
            SQL_BUILDER_CREATE(FArticleSave)
            SQL_BUILDER_PUSH_BACK(FArticleSave)
            END_ACCESSOR_DEFINITION


            //------------------------------------------------------------------------------
            int tests()
{
    int ret = 0;

    //----------------------
    test_viewconnptr();
    //return 0;
    //----------------------

    char str[128];
    sprintf(str, "main: thread id %ld\n", GetCurrentThreadId());
    OutputDebugString(str);

    // database connect
    {
        basar::db::sql::DatabaseInfo  dbinfo;

        dbinfo.dbServer = "onl_sofumi1_tcp";
        //dbinfo.database = "pps1cg";
        dbinfo.database = "pos1g";
        dbinfo.user     = "devuser";
        dbinfo.passwd   = "phoenix";
        dbinfo.eConnectType = basar::INFORMIX;
        basar::db::aspect::ConnectionRef Connection;
        try 
        {
            Connection = basar::db::aspect::Manager::getInstance().createConnect(dbinfo);
        }
        catch (const basar::Exception& except)
        {
            except.what();
        }

        AccessorInstanceRef ai = basar::db::aspect::Manager::getInstance().createAccessorInstance("AccessorArticle_Inst","AccessorArticle", Connection, basar::db::aspect::ON_DEMAND_CACHING, true);
        ai.setDefaultParameter("artikel_nr=398008;");
        //ai.setParameterList("artikel_nr=xyz'abc';");
        ai.execute("ReadArticle");

        /*--- aggregate
        std::pair<bool, basar::Decimal> pair = ai.executeAggregate("ReadArticle");
        basar::Decimal d = pair.second;
        bool b = pair.first;
        basar::Float64 f = d.toFloat64();*/

        AccessorPropertyTable_YIterator yIt;
        yIt = ai.getPropertyTable().begin();

        //	basar::Int32 artno			= yIt.getInt32("artikel_nr");
        basar::VarString artname	= yIt.getString("artikel_name");

    }
    return ret;
}

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    int ret = 0;

    QApplication a(argc, argv);
    qInstallMsgHandler( basar::gui::tie::basarQTMessageHandler);
    {
       // ret = tests();      

        testguitie w;
        a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
        w.show();
        ret = a.exec();

        basar::gui::tie::Manager::clear();
    }
    basar::db::aspect::Manager::clear();

    return ret;
}
