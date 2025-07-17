 /** $Id$                                          
  *                                                  
  * @file                                            
  * @author Anke Klink                      
  * @date 2008                            
  */    

#include "selects.h"
#include "usings.h"
#include "buttablepropertydescriptionvectors.h"
#include <QtGui/QMessageBox>

VarString selectString = " SELECT  first 10 " +
propertyDescriptionVectorButTableDescriptionAllStrings.toCommaSeparatedNames() 
+ " from but_master ";

BUILDER_DEFINITION(StringSelect)
	DONT_CHECK_PROPERTIES(StringSelect)
	SQL_PATTERN(StringSelect,selectString.c_str())

VarString selectStringCustom1 = 
	"SELECT DISTINCT "
	"'abcdefrwretew' AS " + PROPDEF_COLVCH1.getName() + ", "
	"'ghijklmnopqrstuvwxyzasdfasdfasdfasdfasdfasdsdf' AS " + PROPDEF_COLVCH2.getName() + ", "
	"'' AS " + PROPDEF_COLVCH3.getName() + ", "
	"4711::INT AS " + PROPDEF_COLSER.getName() + ", "
	"69::SMALLINT AS " + PROPDEF_COLSMINT.getName() + ", "
	"1.23::DECIMAL AS " + PROPDEF_COLDEC.getName() + ", "
	"DATE('11/11/2011') AS " + PROPDEF_COLDATE.getName() + ", "
	"TO_DATE( '2012-12-31 10:54:23.12345', '%Y-%m-%d %H:%M:%02S%03F5' ) AS " + PROPDEF_COLDT1.getName() + ", "
	"TO_DATE( '10:54:23.12345', '%H:%M:%02S%03F5' ) AS " + PROPDEF_COLTIME1.getName() + " "
	"FROM but_master ";

VarString selectStringCustom2 = 
	"SELECT DISTINCT "
	"'ghijklmnopqrstuvwxyzasdfasdfasdfasdfasdfasdsdf' AS " + PROPDEF_COLVCH1.getName() + ", "
	"'abcdefrwretew' AS " + PROPDEF_COLVCH2.getName() + ", "
	"'' AS " + PROPDEF_COLVCH3.getName() + ", "
	"4711::INT AS " + PROPDEF_COLSER.getName() + ", "
	"69::SMALLINT AS " + PROPDEF_COLSMINT.getName() + ", "
	"1.23::DECIMAL AS " + PROPDEF_COLDEC.getName() + ", "
	"DATE('11/11/2011') AS " + PROPDEF_COLDATE.getName() + ", "
	"TO_DATE( '2012-12-31 10:54:23.12345', '%Y-%m-%d %H:%M:%02S%03F5' ) AS " + PROPDEF_COLDT1.getName() + ", "
	"TO_DATE( '10:54:23.12345', '%H:%M:%02S%03F5' ) AS " + PROPDEF_COLTIME1.getName() + " "
	"FROM but_master ";

VarString selectStringCustom3 = 
	"SELECT DISTINCT "
	"'' AS " + PROPDEF_COLVCH1.getName() + ", "
	"'ghijkl mnopqrstu vwxyzasd fasdf' AS " + PROPDEF_COLVCH2.getName() + ", "
	"'abcdefrwretew' AS " + PROPDEF_COLVCH3.getName() + ", "
	"4711::INT AS " + PROPDEF_COLSER.getName() + ", "
	"69::SMALLINT AS " + PROPDEF_COLSMINT.getName() + ", "
	"1.23::DECIMAL AS " + PROPDEF_COLDEC.getName() + ", "
	"DATE('11/11/2011') AS " + PROPDEF_COLDATE.getName() + ", "
	"TO_DATE( '2012-12-31 10:54:23.12345', '%Y-%m-%d %H:%M:%02S%03F5' ) AS " + PROPDEF_COLDT1.getName() + ", "
	"TO_DATE( '10:54:23.12345', '%H:%M:%02S%03F5' ) AS " + PROPDEF_COLTIME1.getName() + " "
	"FROM but_master ";


BUILDER_DEFINITION( StringCustom1 )
	DONT_CHECK_PROPERTIES( StringCustom1 )
	SQL_PATTERN( StringCustom1, selectStringCustom1.c_str() )

BUILDER_DEFINITION( StringCustom2 )
	DONT_CHECK_PROPERTIES( StringCustom2 )
	SQL_PATTERN( StringCustom2, selectStringCustom2.c_str() )

BUILDER_DEFINITION( StringCustom3 )
	DONT_CHECK_PROPERTIES( StringCustom3 )
	SQL_PATTERN( StringCustom3, selectStringCustom3.c_str() )

namespace accessor_selectString
{
    BEGIN_ACCESSOR_DEFINITION("Acc_StringSelect")
        PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableDescriptionAllStrings)
        ACCESS_METHOD("AccMethod_StringSelect")
        SQL_BUILDER_CREATE(StringSelect)
        SQL_BUILDER_PUSH_BACK(StringSelect)  
        END_ACCESSOR_DEFINITION
}


namespace accessor_selectString2
{
    BEGIN_ACCESSOR_DEFINITION("Acc_StringSelectReverse")
        PROPERTY_DESCRIPTION_LIST(propertyDescriptionVectorButTableDescriptionAllStringsReverse)
        ACCESS_METHOD("AccMethod_StringSelect")
        SQL_BUILDER_CREATE(StringSelect)
        SQL_BUILDER_PUSH_BACK(StringSelect)  
        END_ACCESSOR_DEFINITION
}


namespace accessor_selectStringCustom
{
    BEGIN_ACCESSOR_DEFINITION( "Acc_StringCustom" )
        PROPERTY_DESCRIPTION_LIST( propertyDescriptionVectorButTableDescriptionCustom )

		ACCESS_METHOD( "AccMethod_StringCustom" )
			SQL_BUILDER_CREATE( StringCustom1 )
			SQL_BUILDER_PUSH_BACK( StringCustom1 )  
			SQL_BUILDER_CREATE( StringCustom2 )
			SQL_BUILDER_PUSH_BACK( StringCustom2 )  
			SQL_BUILDER_CREATE( StringCustom3 )
			SQL_BUILDER_PUSH_BACK( StringCustom3 )  

			SQL_BUILDER_PUSH_BACK( StringCustom1 )  
			SQL_BUILDER_PUSH_BACK( StringCustom1 )  
			SQL_BUILDER_PUSH_BACK( StringCustom1 )  

			SQL_BUILDER_PUSH_BACK( StringCustom2 )  
			SQL_BUILDER_PUSH_BACK( StringCustom2 )  
			SQL_BUILDER_PUSH_BACK( StringCustom2 )  

			SQL_BUILDER_PUSH_BACK( StringCustom3 )  
			SQL_BUILDER_PUSH_BACK( StringCustom3 )  
			SQL_BUILDER_PUSH_BACK( StringCustom3 )  

			SQL_BUILDER_PUSH_BACK( StringCustom1 )  
			SQL_BUILDER_PUSH_BACK( StringCustom1 )  
			SQL_BUILDER_PUSH_BACK( StringCustom3 )  

			SQL_BUILDER_PUSH_BACK( StringCustom1 )  
			SQL_BUILDER_PUSH_BACK( StringCustom2 )  
			SQL_BUILDER_PUSH_BACK( StringCustom2 )  

			SQL_BUILDER_PUSH_BACK( StringCustom3 )  
			SQL_BUILDER_PUSH_BACK( StringCustom2 )  
			SQL_BUILDER_PUSH_BACK( StringCustom2 )  

		END_ACCESSOR_DEFINITION
}

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
