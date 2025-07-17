#ifndef GUARD_STRINGBUILDERTYPEINDEXES_H
#define GUARD_STRINGBUILDERTYPEINDEXES_H
//--------------------------------------------------------------------------------------------------------//
/*! \file
*  \brief  intern interface for building SQL - statements
*  \author Marco Köppendörfer
*  \date   02.04.2014
*/
//--------------------------------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------------------//
#include "libbasar_definitions.h"
#include "libbasarproperty_propertytype.h"



//--------------------------------------------------------------------------------------------------------//
// namespaces
//--------------------------------------------------------------------------------------------------------//
namespace basar  {
namespace db     {
namespace aspect {

struct StringbuilderTypeIndexes
{
    StringbuilderTypeIndexes(basar::Int32 indexPrepStmt, basar::Int32 indexYit, property::PropertyType propertyType) : m_IndexPreparedStatement(indexPrepStmt), m_IndexYit(indexYit), m_Type(propertyType)
    {
    }
    
    basar::Int32 m_IndexPreparedStatement;
    basar::Int32 m_IndexYit;
    property::PropertyType m_Type;
};

}
}
}

#endif //GUARD_STRINGBUILDERTYPEINDEXES_H

