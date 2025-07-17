#ifndef GUARD_LIBBASARPROPERTY_DEFINITIONS_H
#define GUARD_LIBBASARPROPERTY_DEFINITIONS_H
//-----------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  public library interface: definitions
 *  \author Bjoern Bischof
 *  \date   20.11.2005
 */
//-----------------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------------//
// include of public definitions (stl, boost) the component depends on
//-----------------------------------------------------------------------------------------------------//
#include <vector>
#include <list>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

//-----------------------------------------------------------------------------------------------------//
// include of general definitions used of all components of the lib
//-----------------------------------------------------------------------------------------------------//
#include "libbasar_definitions.h"

//-----------------------------------------------------------------------------------------------------//
// includes of other components depending on
//-----------------------------------------------------------------------------------------------------//
#include "libbasarcmnutil_definitions.h"
#include "libbasarcmnutil_logging.h"
#include "libbasarcmnutil_exceptions.h"
#include "libbasarcmnutil_pattern.h"
#include "libbasarcmnutil_decimal.h"
#include "libbasarcmnutil_datetime.h"
#include "libbasarcmnutil_timespan.h"
#include "libbasarcmnutil_date.h"
#include "libbasarcmnutil_i18nstring.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_parameterlist.h"

//-----------------------------------------------------------------------------------------------------//
// namespaces
//-----------------------------------------------------------------------------------------------------//
namespace basar    {
namespace property {

//-----------------------------------------------------------------------------------------------------//
// forward declaration section
//-----------------------------------------------------------------------------------------------------//
class PropertyType;
class PropertyDescriptionList;
class PropertyInternal;
class PropertyTable;
class PropertyTableSnapshot;
class PropertyTableYIteratorSnapshot;

//-----------------------------------------------------------------------------------------------------//
// typedef section
//-----------------------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// propertydescriptionlist specific typedefs
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \brief	should clarify that the name of a column is string based. 
//			It is typedefed here, because it is used in several files.
typedef basar::VarString										ColumnName;
//! \brief	type of sql-string-values
typedef basar::I18nString										SQLString;

//! \brief	pair of columnnames and propertytypes
typedef std::pair<ColumnName, PropertyType>						ColNamePropTypePair;
/*! \brief	vector with an assigned type (here: pair of columnnames and propertytypes) */
typedef std::vector<ColNamePropTypePair >						ColNamePropTypeColl;
//! \brief	iterator of vector containing pairs of columnnames and propertytypes
typedef ColNamePropTypeColl::iterator							ColNamePropTypeCollIter;
/*! \brief	representing the number of columns (elements of the vector) contained in the vector 
			of pairs of columnnames and propertytypes. It is also used for a certain 
			position within the vector. It is typedefed here, because it is used in several files. */
typedef ColNamePropTypeColl::size_type							ColumnIndex;
/*! \brief	representing the number of rows contained in the vector of pairs of 
			columnnames and propertytypes. */
typedef ColNamePropTypeColl::size_type							NumberColumns;
//! \brief	shared reference to implementation object of class PropertyDescriptionList.
typedef boost::shared_ptr<PropertyDescriptionList>				PdlSharedPtr;
//! \brief	weak reference to implementation object of class PropertyDescriptionList 
typedef boost::weak_ptr<PropertyDescriptionList>				PdlWeakPtr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// propertytable specific typedefs
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \brief collection of internal properties
typedef std::vector< PropertyInternal >                         CollPropertyInternalColumn;
//! \brief pair, where insert type and belonging column are mapped
typedef std::pair<InsertTypeEnum, CollPropertyInternalColumn >	PairInsertTypeCollPropertyInternalColumn;
//! \brief collection of rows (= pairs of insert type + column)
typedef std::list< PairInsertTypeCollPropertyInternalColumn >	CollPropertyInternalRow;
//! \brief number of internal property rows in collection
typedef CollPropertyInternalRow::size_type						NumberRows;
//! \brief maximum number of internal property rows in collection
typedef CollPropertyInternalRow::size_type						MaxSizeRows;
//! \brief shared pointer to property table
typedef boost::shared_ptr< PropertyTable >                      PropertyTableSharedPtr;
//! \brief iterator to collection of internal properties
typedef CollPropertyInternalColumn::iterator                    CollPropertyInternalColumnIter;                                                               

// for YIterator
//! \brief iterator to collection of rows (pairs)
typedef CollPropertyInternalRow::iterator                       CollPropertyInternalRowIter;
//! \brief const iterator to collection of rows (pairs)
typedef CollPropertyInternalRow::const_iterator                 CollPropertyInternalRowConstIter;
//! \brief weak reference to implementation object of class PropertyTable
typedef boost::weak_ptr< PropertyTable >                        PropertyTableWeakPtr;

// for snapshot
//! \brief shared pointer to implementation object of class PropertyTableSnapshot
typedef boost::shared_ptr< PropertyTableSnapshot >				PropertyTableSnapshotSharedPtr;
//! \brief weak reference to implementation object of class PropertyTableSnapshot
typedef boost::weak_ptr<   PropertyTableSnapshot >				PropertyTableSnapshotWeakPtr;
//! \brief shared pointer to implementation object of class PropertyTableYIteratorSnapshot
typedef boost::shared_ptr< PropertyTableYIteratorSnapshot >		PropertyTableYIteratorSnapshotSharedPtr;
//! \brief weak reference to implementation object of class PropertyTableYIteratorSnapshot
typedef boost::weak_ptr<   PropertyTableYIteratorSnapshot >		PropertyTableYIteratorSnapshotWeakPtr;

//-------------------------------------------------------------------------------------------------------//
// define section
//-------------------------------------------------------------------------------------------------------//
#define SQL_VALUE_DELIM		"'"		//!< delimiter at first and end of sql-value-string

//----------------------------------------------------------------------------
/*! macro define a const PropertyDescription, with name, propertyname and Propertytype
 * f.ex. CONST_PROPERTY_DESCRIPTION(PROPDEF_COLDATE,  "coldate",  basar::DATE);
 * defines the varable PROPDEF_COLDATE, with propertyname "coldate" and type DATE
 */
#define CONST_PROPERTY_DESCRIPTION(DescriptionName, DescriptionValue, DescriptionType) \
    const basar::property::PropertyDescription DescriptionName = basar::property::PropertyDescription(DescriptionValue, DescriptionType); \


//-----------------------------------------------------------------------------------------------------//
}	// namespace property
}	// namespace basar


//-----------------------------------------------------------------------------------------------------//
#endif	// GUARD_LIBBASARPROPERTY_DEFINITIONS_H
