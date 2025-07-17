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

	CONST_PROPERTY_DESCRIPTION(PROPDEF_COLSPAN1, "colspan1", basar::TIMESPAN)
    
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLNVL1,  "colnvl1"  , basar::STRING)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLNVL2,  "colnvl2"  , basar::STRING)

    //! test with types as strings :
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLINT,   "colint",   "int32")
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLMON,   "colmon",   " decimal")
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLCH2,   "colch2",   " string ")
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLDT2,   "coldt2",   "datetime ")

    // for special tests
    CONST_PROPERTY_DESCRIPTION(PROPDEF_STRING1,   "colch1",    basar::STRING)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_STRING2,   "colch2",    basar::STRING)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_STRING3,   "string3",   basar::STRING)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_NULLDATE,  "nulldate",  basar::DATE)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_NULLDT1,   "nulldt",    basar::DATETIME)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_NULLTIME1, "nulltime1", basar::TIME)

    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLDT_DATE, "coldt_date", basar::DATETIME)
    CONST_PROPERTY_DESCRIPTION(PROPDEF_COLDT_TIME, "coldt_time", basar::DATETIME)

#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
