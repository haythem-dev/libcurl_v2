/** $Id$                                          
*                                                  
* @file buttablepropertydescriptions.h                                  
* @author Anke Klink                      
* @date 2008                            
*/    


#ifndef  __GUARD_BASAR_TEST_UNIT_DBASPECT_TESTPROPERTYDESCRIPTIONS_H__ 
#define  __GUARD_BASAR_TEST_UNIT_DBASPECT_TESTPROPERTYDESCRIPTIONS_H__ 

#include "libbasarproperty_propertydescription.h"

    //! global Definition of Properties (with types)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLSER,   "colser",   basar::INT32)          
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLSMINT, "colsmint", basar::INT16)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLFL,    "colfl",    basar::FLOAT64)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLSMFL,  "colsmfl",  basar::FLOAT32)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLDEC,   "coldec",   basar::DECIMAL)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLCH1,   "colch1",   basar::STRING)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLVCH1,  "colvch1",  basar::STRING)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLVCH2,  "colvch2",  basar::STRING)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLVCH3,  "colvch3",  basar::STRING)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLVCH4,  "colvch4",  basar::STRING)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLLVCH,  "collvch",  basar::STRING)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLDATE,  "coldate",  basar::DATE)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLDT1,   "coldt1",   basar::DATETIME)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLTIME1, "coltime1", basar::TIME)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLTIME2, "coltime2", basar::DATETIME)

    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLNVL1,  "colnvl1"  , basar::STRING)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLNVL2,  "colnvl2"  , basar::STRING)

    //! test with types as strings :
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLINT,   "colint",   "int32")
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLMON,   "colmon",   " decimal")
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLCH2,   "colch2",   " string ")
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLDT2,   "coldt2",   "datetime ")

	//! stored procedure tests
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLRET0,  "ret0",   "int32")
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLRET1,  "ret1",   "int32")
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLRET2,  "ret2",   "int16")

	//! i18n
	CONST_PROPERTY_DESCRIPTION(PROPDEF_COLSERVERSTRING,  "serverstring", basar::STRING)
	CONST_PROPERTY_DESCRIPTION(PROPDEF_COLLOW         ,  "low"         , basar::STRING)

	//! TimeSpan
	CONST_PROPERTY_DESCRIPTION(PROPDEF_COLSPAN1, "colspan1", basar::TIMESPAN)
	CONST_PROPERTY_DESCRIPTION(PROPDEF_MYINTERVAL, "myInterval", basar::TIMESPAN)



#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
