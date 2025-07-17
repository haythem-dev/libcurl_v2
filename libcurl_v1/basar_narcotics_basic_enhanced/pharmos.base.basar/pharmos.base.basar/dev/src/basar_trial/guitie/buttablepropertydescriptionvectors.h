/** $Id$                                          
*                                                  
* @file buttablepropertydescriptionvectors.h                                  
* @author Anke Klink                      
* @date 2008                            
*/    

#include "buttablepropertydescriptions.h"

#ifndef  __BASAR_TEST_UNIT_DBASPECT_TESTPROPERTYDESCRIPTIONVECTORS_H__ 
#define  __BASAR_TEST_UNIT_DBASPECT_TESTPROPERTYDESCRIPTIONVECTORS_H__ 

using basar::property::PropertyDescriptionArray;
using basar::property::PropertyDescriptionVector;

// ----------------------------------------------------------
const PropertyDescriptionArray propertyDescriptionArrayButTableDescriptionAll =  
{
    PROPDEF_COLSER, 
        PROPDEF_COLINT, PROPDEF_COLSMINT,
        PROPDEF_COLFL,  PROPDEF_COLSMFL,
        PROPDEF_COLDEC, PROPDEF_COLMON,
        PROPDEF_COLCH1, PROPDEF_COLCH2,
        PROPDEF_COLVCH1, PROPDEF_COLVCH2, PROPDEF_COLVCH3, PROPDEF_COLVCH4,
        PROPDEF_COLLVCH,
        PROPDEF_COLDATE,
        PROPDEF_COLDT1,PROPDEF_COLDT2,
        PROPDEF_COLTIME1
};

// ----------------------------------------------------------
const PropertyDescriptionArray propertyDescriptionArrayButTableDescriptionAllReverse =  
{
    PROPDEF_COLTIME1,
        PROPDEF_COLDT2, PROPDEF_COLDT1,
        PROPDEF_COLDATE,
        PROPDEF_COLLVCH,
        PROPDEF_COLVCH4, PROPDEF_COLVCH3, PROPDEF_COLVCH2, PROPDEF_COLVCH1,
        PROPDEF_COLCH2, PROPDEF_COLCH1,
        PROPDEF_COLMON, PROPDEF_COLDEC,  
        PROPDEF_COLSMFL,  PROPDEF_COLFL, 
        PROPDEF_COLSMINT, PROPDEF_COLINT, 
        PROPDEF_COLSER
};

// ----------------------------------------------------------
const PropertyDescriptionArray propertyDescriptionArrayButTableDescriptionCustom =  
{
	PROPDEF_COLVCH1,
	PROPDEF_COLVCH2,
	PROPDEF_COLVCH3,
    PROPDEF_COLSER,
	PROPDEF_COLSMINT,
	PROPDEF_COLDEC,
	PROPDEF_COLDATE,
	PROPDEF_COLDT1,
	PROPDEF_COLTIME1
};


const PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAll(18, propertyDescriptionArrayButTableDescriptionAll);

const PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllDates(4, &(propertyDescriptionArrayButTableDescriptionAll[14])); 
const PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllFloats(2, &(propertyDescriptionArrayButTableDescriptionAll[3])); 
const PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllDecs(2, &(propertyDescriptionArrayButTableDescriptionAll[5]));   
const PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllStrings(7, &(propertyDescriptionArrayButTableDescriptionAll[7])); 
const PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllInts(3, propertyDescriptionArrayButTableDescriptionAll); 

const PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllSettableInts(2, &(propertyDescriptionArrayButTableDescriptionAll[1])); 
const PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllStartSMINT(16, &(propertyDescriptionArrayButTableDescriptionAll[2]));

const PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllNoColser(17, &(propertyDescriptionArrayButTableDescriptionAll[1]));
const PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllNoDates(14, propertyDescriptionArrayButTableDescriptionAll);

const PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionColserOnly(1, &(propertyDescriptionArrayButTableDescriptionAll[0]));

const PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionIntValues = propertyDescriptionVectorButTableDescriptionAllInts;


const PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllStringsReverse(7, &(propertyDescriptionArrayButTableDescriptionAllReverse[4])); 

const PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionCustom( 9, propertyDescriptionArrayButTableDescriptionCustom ); 

 
// ----------------------------------------------------------



#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
