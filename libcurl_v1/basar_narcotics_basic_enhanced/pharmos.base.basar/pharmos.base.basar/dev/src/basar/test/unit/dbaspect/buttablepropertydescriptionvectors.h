 /** $Id$                                          
  *                                                  
  * @file buttablepropertydescriptionvectors.h                                  
  * @author Anke Klink                      
  * @date 2008                            
  */    

#ifndef  __BASAR_TEST_UNIT_DBASPECT_TESTPROPERTYDESCRIPTIONVECTORS_H__ 
#define  __BASAR_TEST_UNIT_DBASPECT_TESTPROPERTYDESCRIPTIONVECTORS_H__ 

#include "buttablepropertydescriptions.h"

	// ----------------------------------------------------------
    const basar::property::PropertyDescriptionArray propertyDescriptionArrayButTableDescriptionAll =  
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

    const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAll(18, propertyDescriptionArrayButTableDescriptionAll);
    
    const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllDates(4, &(propertyDescriptionArrayButTableDescriptionAll[14])); 
    const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllFloats(2, &(propertyDescriptionArrayButTableDescriptionAll[3])); 
    const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllDecs(2, &(propertyDescriptionArrayButTableDescriptionAll[5]));   
    const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllStrings(5, &(propertyDescriptionArrayButTableDescriptionAll[7])); 
    const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllInts(3, propertyDescriptionArrayButTableDescriptionAll); 
    
    const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllSettableInts(2, &(propertyDescriptionArrayButTableDescriptionAll[1])); 
    const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllStartSMINT(16, &(propertyDescriptionArrayButTableDescriptionAll[2]));
    
    const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllNoColser(17, &(propertyDescriptionArrayButTableDescriptionAll[1]));
    const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionAllNoDates(14, propertyDescriptionArrayButTableDescriptionAll);
   
    const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionColserOnly(1, &(propertyDescriptionArrayButTableDescriptionAll[0]));
    
    const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionIntValues = propertyDescriptionVectorButTableDescriptionAllInts;

	// ----------------------------------------------------------

    const basar::property::PropertyDescriptionArray propertyDescriptionArrayButTableFirstDate = 
    {
        PROPDEF_COLDATE, 
        PROPDEF_COLDT1, 
        PROPDEF_COLTIME1 
    };
    const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableFirstDate(3, propertyDescriptionArrayButTableFirstDate);

    // ----------------------------------------------------------

	const basar::property::PropertyDescriptionArray propertyDescriptionArrayButTableSuccessiveStatements = 
	{
		PROPDEF_COLSMINT,
		PROPDEF_COLCH2,
		PROPDEF_COLVCH2
	};

	const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableSuccessiveStatements(3, propertyDescriptionArrayButTableSuccessiveStatements);
	
    // ----------------------------------------------------------

	const basar::property::PropertyDescriptionArray propDescArrayButTableStmtTypes = 
	{
		PROPDEF_COLSMINT,
		PROPDEF_COLCH2,
		PROPDEF_COLVCH2
	};

	const basar::property::PropertyDescriptionVector propDescVecButTableStmtTypes(3, propDescArrayButTableStmtTypes);
	
    // ----------------------------------------------------------
											
    const basar::property::PropertyDescriptionArray propertyDescriptionArrayButTableDescriptionSomeValues =  
    {
		PROPDEF_COLSER, 
		PROPDEF_COLINT,
        PROPDEF_COLCH1 ,
        PROPDEF_COLSMINT
        
    };
    const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionSomeValues(4, propertyDescriptionArrayButTableDescriptionSomeValues);

    // ----------------------------------------------------------
											
    const basar::property::PropertyDescriptionArray propertyDescriptionArrayButTableDescriptionFixedStrings =  
    {
		PROPDEF_COLNVL1,
		PROPDEF_COLNVL2
    };
    const basar::property::PropertyDescriptionVector propertyDescriptionVectorButTableDescriptionFixedStrings(2, propertyDescriptionArrayButTableDescriptionFixedStrings);

  	// ----------------------------------------------------------

	const basar::property::PropertyDescriptionArray propDescArrayButProcsStoredProc = 
	{
		PROPDEF_COLRET0,
		PROPDEF_COLRET1,
		PROPDEF_COLRET2
	};

	const basar::property::PropertyDescriptionVector propDescVecButProcsStoredProc(3, propDescArrayButProcsStoredProc);

  	// ----------------------------------------------------------

	const basar::property::PropertyDescriptionArray propDescArrayButTableI18n = 
	{
		PROPDEF_COLINT,
		PROPDEF_COLVCH3,
		PROPDEF_COLLOW,
		PROPDEF_COLSERVERSTRING,
		PROPDEF_COLFL,
		PROPDEF_COLSMFL,
		PROPDEF_COLDEC,
		PROPDEF_COLMON,
		PROPDEF_COLDATE,
		PROPDEF_COLDT1,
		PROPDEF_COLDT2,
		PROPDEF_COLTIME1,
		PROPDEF_COLTIME2
	};

	const basar::property::PropertyDescriptionVector propDescVecButTableI18n(13, propDescArrayButTableI18n);


#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
