#ifndef GUARD_LIBBASARGUITIE_MATCHER_H
#define GUARD_LIBBASARGUITIE_MATCHER_H
//------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  external interface matcherref
 *  \author Roland Kiefert 
 *  \date   20.09.2005
 */
//------------------------------------------------------------------------------------------------------------//

#ifndef LIBBASARGUITIE_API
	#ifdef _WIN32
		#ifdef LIBBASARGUITIE_EXPORTS
			#define LIBBASARGUITIE_API __declspec(dllexport)        //!< dll exported
		#else
			#define LIBBASARGUITIE_API __declspec(dllimport)        //!< dll imported
		#endif
	#else
		#define LIBBASARGUITIE_API
	#endif
#endif	// LIBBASARGUITIE_API

//------------------------------------------------------------------------------------------------------------//
#include <list>
#include <boost/shared_ptr.hpp>
#include "libbasar_definitions.h"

//------------------------------------------------------------------------------------------------------------//
// namespaces
//------------------------------------------------------------------------------------------------------------//
namespace basar {
	namespace db {
		namespace aspect {
			class AccessorPropertyTable_YIterator;
			class AccessorPropertyTableRef;
		}
	}
	
	namespace gui {
		namespace tie {

//------------------------------------------------------------------------------------------------------------//
typedef UInt32 TransformerIndex;		//!< index of transformer in the list

//------------------------------------------------------------------------------------------------------------//
// forward declaration
//------------------------------------------------------------------------------------------------------------//
class GuiPropertyTableRef;
class GuiPropertyTable_YIterator;
class Transformer;
class Matcher;

//------------------------------------------------------------------------------------------------------------//
// class declaration
//------------------------------------------------------------------------------------------------------------//
/*! \brief  class MatcherRef holds a guipropertytable and an accessorpropertytable.
            class matches between guipropertytable and accessorpropertytable, so that properties from guipropertytable are set to 
			accessorpropertytable and the other way.
            
			This is a final class.     
			Class throws the follwing exceptions: NullReferenceException. */
class MatcherRef
{
	friend class ManagerImpl;

public:

	//--------------------------------------------------------------------------------------//
	// static section
	//--------------------------------------------------------------------------------------//
	/*! \brief     move the iterator of accessorpropertytable to the iterator of guipropertytable
		\n         throw UnknownPropertyTypeException, UnknownColoumnException */
	LIBBASARGUITIE_API static void						RightToLeft( const GuiPropertyTable_YIterator                  guiIt, 
																	 const db::aspect::AccessorPropertyTable_YIterator accIt, 
																	 const Int32                                       policy );
	/*! \brief     move the iterator of guipropertytable to the iterator of accessorpropertytable
		\n         throw UnknownPropertyTypeException, UnknownColoumnException */
	LIBBASARGUITIE_API static void						LeftToRight( const GuiPropertyTable_YIterator                  guiIt, 
																	 const db::aspect::AccessorPropertyTable_YIterator accIt, 
																	 const Int32                                       policy );

	/*! \brief construct reference object and initialize shared pointer to implementation object to null
 		\n     no-throw */
	LIBBASARGUITIE_API									MatcherRef();


	/*! \brief return reference to AccessorPropertyTable. \n no-throw */
	LIBBASARGUITIE_API 
	db::aspect::AccessorPropertyTableRef				getRight() const;
	/*! \brief return reference to GuiPropertyTable. \n no-throw */
	LIBBASARGUITIE_API GuiPropertyTableRef				getLeft() const;

	/*! \brief set the matching policy. \n no-throw */
	LIBBASARGUITIE_API void								setMatchingPolicy(const Int32 policy);

	/*! \brief move from right propertytable to left propertytable
		\n no-throw */
	LIBBASARGUITIE_API Int32						    RightToLeft( ) ;
	/*! \brief move from left propertytable to right propertytable
		\n no-throw */
	LIBBASARGUITIE_API Int32							LeftToRight( ) ;
	/*! \brief move specified number of rows from right propertytable to left propertytable, starting at index-position
		\n	UnknownIndexException */
	LIBBASARGUITIE_API Int32							RightToLeft( const Int32 index, const Int32 count = 0 );
	/*! \brief move specified number of rows from left propertytable to right propertytable, starting at index-position
		\n	UnknownIndexException */
	LIBBASARGUITIE_API Int32							LeftToRight( const Int32 index, const Int32 count = 0 );
	/*! \brief move the iterator of accessorpropertytable to the iterator of guipropertytable
		\n UnknownPropertyTypeException, UnknownColoumnException */
	LIBBASARGUITIE_API void								RightToLeft( const GuiPropertyTable_YIterator                  guiIt, 
																	 const db::aspect::AccessorPropertyTable_YIterator accIt ) const;
	/*! \brief move the iterator of guipropertytable to the iterator of accessorpropertytable
		\n UnknownPropertyTypeException, UnknownColoumnException */
	LIBBASARGUITIE_API void								LeftToRight( const GuiPropertyTable_YIterator                  guiIt, 
																	 const db::aspect::AccessorPropertyTable_YIterator accIt ) const;

	/*! \brief get the gui property table y-iterator to the current line. \n no-throw */
	LIBBASARGUITIE_API GuiPropertyTable_YIterator		getCurrentLeft();
	/*! \brief get the accessor property table y-iterator to the current line. \n no-throw */
	LIBBASARGUITIE_API 
	db::aspect::AccessorPropertyTable_YIterator			getCurrentRight();
	/*! \brief get the gui property table y-iterator to the first selected line.\n no-throw  */
	LIBBASARGUITIE_API GuiPropertyTable_YIterator		getSelectedLeft();
	/*! \brief get the gui property table y-iterator to the next selected line. \n no-throw  */
	LIBBASARGUITIE_API GuiPropertyTable_YIterator		getNextSelectedLeft();
	/*! \brief get the accessor property table y-iterator to the first selected line.\n no-throw  */
	LIBBASARGUITIE_API 
	db::aspect::AccessorPropertyTable_YIterator			getSelectedRight();
	/*! \brief get the accessor property table y-iterator to the next selected line. \n no-throw  */
	LIBBASARGUITIE_API 
	db::aspect::AccessorPropertyTable_YIterator			getNextSelectedRight();
	/*! \brief add a transformer to the matcher. \n no-throw */
	LIBBASARGUITIE_API TransformerIndex					push_back( Transformer & transformer );

	
	/*! \brief check if shared pointer to implementation class is null \n no-throw */
	LIBBASARGUITIE_API const bool						isNull() const;

	/*! \brief set the sharedpointer to null.\n no-throw */
	LIBBASARGUITIE_API void								reset		();

	/*! \brief connect all column header labels with the original attribute names.\n no-throw */
	LIBBASARGUITIE_API void								transformProperties( const std::list<VarString>& headerLabels,          //!< list of table header label texts as shown on gui
																			 const std::list<VarString>& originalAttributeNames //!< list of attribute names of property table
																		    );

	/*! \brief removes all inner values */
	LIBBASARGUITIE_API  void clear();

	/*! \brief removes the matcher fron GuiTable */
	LIBBASARGUITIE_API  void removeMatcherEvents();
private:
	//------------------//
	// private methods
	//------------------//
	/*! \brief create the pointer to the class Matcher. \n no-throw */
	MatcherRef( boost::shared_ptr<Matcher> pMatcher );

	/*! \brief return the pointer to a Matcher implementation. \n NullReferenceException */
	const boost::shared_ptr<Matcher>&					getImpl		() const;
		
	//------------------//
	// private members
	//------------------//
	boost::shared_ptr<Matcher>							m_pMatcher; //!< pointer to Matcher
};

} // tie
} // gui
} // basar

#endif
