/** $Id$                                          
*                                                  
* @file propertydescriptionvector.cpp                                  
* @author Anke Klink                      
* @date 2008                            
*/    
//--------------------------------------------------------------------------------------------//

#include "libbasarproperty_propertydescription.h"

#include "libbasarcmnutil_i18nstring.h"

//--------------------------------------------------------------------------------------------//
namespace basar
{
    namespace property
    {
        //----------------------------------------------------------------------------------------------

        PropertyDescriptionVector::PropertyDescriptionVector()
        {

        }
        //----------------------------------------------------------------------------------------------
        PropertyDescriptionVector::PropertyDescriptionVector(
            int length, 
            const PropertyDescription * a) 
        {     
            if (length > 0)
            {
                m_vector.resize(length);
                std::copy( a, a+length,m_vector.begin());
            }
        }

        //----------------------------------------------------------------------------------------------

        PropertyDescriptionVector::operator cmnutil::ParameterList() const
        {
            // return cmnutil::ParameterList(toPropertyString());
            cmnutil::ParameterList retval;
            std::vector<PropertyDescription>::const_iterator iter;
            for (iter = m_vector.begin(); iter != m_vector.end(); ++iter)
            {
                retval.push_back(iter->getName());
            }
            return retval ; 
        }
        //----------------------------------------------------------------------------------------------

        VarString PropertyDescriptionVector::toDescriptionString() const
        {
            VarString retval;
            std::vector< PropertyDescription>::const_iterator iter;
            for (iter = m_vector.begin(); iter != m_vector.end(); ++iter)
            {
                retval += iter->toDescriptionString();
            }
            return retval;
        }

        //----------------------------------------------------------------------------------------------

        VarString PropertyDescriptionVector::toPropertyString() const
        {
            VarString retval;
            std::vector< PropertyDescription>::const_iterator iter;
            for (iter = m_vector.begin(); iter != m_vector.end(); ++iter)
            {
                retval += iter->toPropertyString();
            }
            return retval;
        }
        //----------------------------------------------------------------------------------------------

        VarString PropertyDescriptionVector::toCommaSeparatedNames() const
        {
            VarString retval;
            std::vector< PropertyDescription>::const_iterator iter = m_vector.begin();
            if (iter != m_vector.end())
            {
                retval += iter->getName();
                for (++iter; iter != m_vector.end(); ++iter)
                {
                    retval += ",";
                    retval += iter->getName();
                }
            }
            return retval;
        }

		//----------------------------------------------------------------------------------------------

		VarString PropertyDescriptionVector::toCommaSeparatedNames(ConstString tablename, bool useAs) const
        {
            VarString retval;
            std::vector< PropertyDescription>::const_iterator iter = m_vector.begin();
            if (iter != m_vector.end())
            {
                retval +=  iter->toStringWithTableName(tablename, useAs);
                for (++iter; iter != m_vector.end(); ++iter)
                {
                    retval += ","; 
                    retval += iter->toStringWithTableName(tablename, useAs);
                }
            }
            return retval;
        }

        //----------------------------------------------------------------------------------------------
        std::vector<VarString> PropertyDescriptionVector::toStringList() const
        {
            std::vector<VarString> retval;
            std::vector< PropertyDescription>::const_iterator iter = m_vector.begin();
            for ( ; iter != m_vector.end(); ++iter)
            {
                retval.push_back(iter->getName());
            }
            return retval;
        }
        //----------------------------------------------------------------------------------------------

    }
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
