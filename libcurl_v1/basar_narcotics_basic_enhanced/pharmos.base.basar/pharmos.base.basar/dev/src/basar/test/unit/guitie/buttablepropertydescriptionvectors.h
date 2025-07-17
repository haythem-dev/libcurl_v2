/** $Id$                                          
*                                                  
* @file buttablepropertydescriptionvectors.h                                  
* @author Anke Klink                      
* @date 2008                            
*/    

#ifndef  __BASAR_TEST_UNIT_DBASPECT_TESTPROPERTYDESCRIPTIONVECTORS_H__ 
#define  __BASAR_TEST_UNIT_DBASPECT_TESTPROPERTYDESCRIPTIONVECTORS_H__ 

#include "buttablepropertydescriptions.h"

const int NUMBER_OF_LINES_IN_BUTMASTER = 10;

// ----------------------------------------------------------
const basar::property::PropertyDescriptionArray propDescArrayButTableDescAll =  
{
    PROPDEF_COLSER, 
    PROPDEF_COLINT,  PROPDEF_COLSMINT,
    PROPDEF_COLFL,   PROPDEF_COLSMFL,
    PROPDEF_COLDEC,  PROPDEF_COLMON,
    PROPDEF_COLCH1,  PROPDEF_COLCH2,
    PROPDEF_COLVCH1, PROPDEF_COLVCH2, PROPDEF_COLVCH3, PROPDEF_COLVCH4,
    PROPDEF_COLLVCH,
    PROPDEF_COLDATE,														// #14
    PROPDEF_COLDT1,  PROPDEF_COLDT2,
    PROPDEF_COLTIME1,
	PROPDEF_COLDT_DATE,
	PROPDEF_COLDT_TIME
};


// ----------------------------------------------------------
const basar::property::PropertyDescriptionArray propDescArrayButTableDescTimeSpan =  
{
	PROPDEF_COLSPAN1
};

// ----------------------------------------------------------

const basar::property::PropertyDescriptionVector propDescVecButTableDescAll            (18, propDescArrayButTableDescAll);

const basar::property::PropertyDescriptionVector propDescVecButTableDescAllDates       (6, &(propDescArrayButTableDescAll[14])); 
const basar::property::PropertyDescriptionVector propDescVecButTableDescAllFloats      (2, &(propDescArrayButTableDescAll[3])); 
const basar::property::PropertyDescriptionVector propDescVecButTableDescAllDecs        (2, &(propDescArrayButTableDescAll[5]));   
const basar::property::PropertyDescriptionVector propDescVecButTableDescAllStrings     (5, &(propDescArrayButTableDescAll[7])); 
const basar::property::PropertyDescriptionVector propDescVecButTableDescAllInts        (3, propDescArrayButTableDescAll); 

const basar::property::PropertyDescriptionVector propDescVecButTableDescAllSettableInts(2, &(propDescArrayButTableDescAll[1])); 
const basar::property::PropertyDescriptionVector propDescVecButTableDescAllStartSMINT  (16, &(propDescArrayButTableDescAll[2]));

const basar::property::PropertyDescriptionVector propDescVecButTableDescAllNoColser    (17, &(propDescArrayButTableDescAll[1]));
const basar::property::PropertyDescriptionVector propDescVecButTableDescAllNoDates     (14, propDescArrayButTableDescAll);

const basar::property::PropertyDescriptionVector propDescVecButTableDescColserOnly     (1, &(propDescArrayButTableDescAll[0]));

const basar::property::PropertyDescriptionVector propDescVecButTableDescIntValues = propDescVecButTableDescAllInts;


const basar::property::PropertyDescriptionVector propDescVecButTableDescTimeSpan       (1, &(propDescArrayButTableDescTimeSpan[0])); 
// ----------------------------------------------------------

const basar::property::PropertyDescriptionArray propDescArrayButTableDescAllReverse =  
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

const basar::property::PropertyDescriptionVector propDescVecButTableDescAllReverse(18, propDescArrayButTableDescAllReverse);
const basar::property::PropertyDescriptionVector propDescVecButTableDescAllStringsReverse( 7, &(propDescArrayButTableDescAllReverse[4])); 

// ----------------------------------------------------------

const basar::property::PropertyDescriptionArray propDescArrayButTableDescTwoStrings = 
{

    PROPDEF_STRING1, PROPDEF_STRING2 

};
const basar::property::PropertyDescriptionVector propDescVecButTableTwoStrings(2, propDescArrayButTableDescTwoStrings);


// ----------------------------------------------------------
const basar::property::PropertyDescriptionArray propDescArrayButTableDescTwoStringsReverse =  
{
    PROPDEF_STRING2, PROPDEF_STRING1 
};

const basar::property::PropertyDescriptionVector propDescVecButTableTwoStringsReverse(2, propDescArrayButTableDescTwoStringsReverse);

#endif 


// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
