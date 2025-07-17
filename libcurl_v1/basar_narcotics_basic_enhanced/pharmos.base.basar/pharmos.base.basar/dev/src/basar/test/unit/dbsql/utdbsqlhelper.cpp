/*
* @file utdbsqlhelper.h                                                   
* @brief ...                                       
* @author Anke Klink                      
* @date 2007                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasardbsql.h"

#include "libbasarcmnutil.h"
#include "utdbsqlhelper.h"

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace dbsql
			{
                const basar::Float64  Defines::csm_defaultDecimalCloseTolerance = 0.00001;
                const basar::Int32    Defines::csm_int32DefaultNullValue = 0;
                const basar::Int16    Defines::csm_int16DefaultNullValue = 0;
                const basar::Float64  Defines::csm_float64DefaultNullValue = 0.0;		
                const basar::Float32  Defines::csm_float32DefaultNullValue = 0.0;

                const basar::Decimal      Defines::csm_decimalDefaultNullValue;
                const basar::ConstString  Defines::csm_stringDefaultNullValue = "";

                const basar::Date      Defines::csm_dateDefaultNullValue = getDateDefaultNullValue();
                //const basar::DateTime  Defines::csm_dateTimeDefaultNullValue = getDateTimeDefaultNullValue();


                const basar::Date Defines::getDateDefaultNullValue()
                {
                    basar::Date  retval ;
                    retval.setInvalid();
                    return retval;
                }

                //const basar::DateTime Defines::getDateTimeDefaultNullValue()
                //{
                //    basar::DateTime  retval ;
                //    retval.setInvalid();
                //    return retval;
                //}

			}
		}
	}
}


// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
