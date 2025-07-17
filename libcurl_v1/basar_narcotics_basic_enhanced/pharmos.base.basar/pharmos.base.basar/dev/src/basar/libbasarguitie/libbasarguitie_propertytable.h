//----------------------------------------------------------------------------
/*! \file
 *  \brief  extern interface guipropertytableref
 *  \author Roland Kiefert 
 *  \date   21.09.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARGUITIE_PROPERTYTABLE_H
#define GUARD_LIBBASARGUITIE_PROPERTYTABLE_H

//---------------------------------------------------------------------------------------//
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

//---------------------------------------------------------------------------------------//
#include <vector>
#include <boost/shared_ptr.hpp>
#include "libbasar_definitions.h"
#include "libbasarproperty_definitions.h"
#include "libbasarguitie_definitions.h"

//---------------------------------------------------------------------------------------//
// forward declaration
//---------------------------------------------------------------------------------------//
class QObject;

//---------------------------------------------------------------------------------------//
// namespaces
//---------------------------------------------------------------------------------------//
namespace basar {
	namespace property {
		class PropertyDescriptionListRef;
	}

namespace gui {
namespace tie {

//----------------------------------------------------------------------------
class  GuiPropertyTable_YIterator;
class  GuiPropertyTable_XIterator;
class  GuiPropertyTable;
class  Matcher;
class  ControlBehavour;

//----------------------------------------------------------------------------
/*! \brief  This class GuiPropertyTableRef is a special proxy (smart reference) 
			to the class GuiPropertyTable.

			Class GuiPropertyTableRef holds a smart (shared) pointer to its 
			implementation (class GuiPropertyTable). 
			So the reference-class can be copied as many times as necessary 
			without any doubts about memory allocation or leaks.
            Class throws all the exceptions the implementation throws. */
//---------------------------------------------------------------------------------------//
// class GuiPropertyTableRef
//---------------------------------------------------------------------------------------//
class GuiPropertyTableRef
{
public:
	/*! \brief return reference to PropertyDescriptionList
		\n no-throw */
	LIBBASARGUITIE_API property::PropertyDescriptionListRef	
													getPropertyDescriptionList	() const;

	/*! \brief return size of propertytable after resultset has been read completely
		\n GuiControlNotSupportedException */
	LIBBASARGUITIE_API UInt32						size						() const;
	
	/*! \brief check if there are no results
		\n no-throw */
	LIBBASARGUITIE_API bool							empty						() const;
		
	/*! \brief clear propertytable to 0
		\n GuiControlNotSupportedException*/
	LIBBASARGUITIE_API void							clear						();

	/*! \brief return iterator to first row
		\n no-throw */
	LIBBASARGUITIE_API GuiPropertyTable_YIterator	begin						() const;
	
	/*! \brief return iterator to row after last
		\n no-throw */
	LIBBASARGUITIE_API GuiPropertyTable_YIterator	end							() const;
    
	/*! \brief set new empty row to propertytable and return iterator to this row
		\n no-throw */
	LIBBASARGUITIE_API GuiPropertyTable_YIterator	insert						(); 
												
	/*! \brief set row from propertytable to state delete
		\n NoValidMemoryPolicyException, GuiControlNotSupportedException */
	LIBBASARGUITIE_API void							erase						( GuiPropertyTable_YIterator yIt);

	/*! \brief create the pointer of the class GuiPropertyTable. 
 		\n     no-throw */
	LIBBASARGUITIE_API								GuiPropertyTableRef			( boost::shared_ptr<GuiPropertyTable> pGuiPropertyTable);
	/*! \brief create an empty reference to GuiPropertyTable. 
 		\n     no-throw */
	LIBBASARGUITIE_API								GuiPropertyTableRef			();
	/*! \brief check if shared pointer to implementation class is null 
		\n no-throw */
	LIBBASARGUITIE_API bool							isNull						() const;
	/*! \brief set the sharedpointer to null.
		\n no-throw */
	LIBBASARGUITIE_API void							reset						();

	/*! \brief set an eventhandler for a special widget typ. 
		\n	   no-throw */
	LIBBASARGUITIE_API IdEventHandler				setEventHandler				(boost::shared_ptr<QObject> rEventHandler, 
																				 WidgetTypes                objectTyp = qUnset);
	/*! \brief delete an eventhandler. 
		\n	   no-throw */
	LIBBASARGUITIE_API void							deleteEventHandler			(IdEventHandler id);
	
	/*! \brief set the eventpolicy. 
		\n	   no-throw */
	LIBBASARGUITIE_API void							setEventPolicy				(UInt32 policy);

	/*! \brief get the absolute index of the first shown row in the gui property table. 
		\n	   no-throw */
	LIBBASARGUITIE_API Int32						getAbsoluteIndex			();

	/*! \brief	get the absolute index of the current row or item in the gui property table. 
				\n no-throw */
	LIBBASARGUITIE_API Int32						getCurrentIndex				();

	/*! \brief	get the absolute indices of the selected rows or items in the gui property table. 
				\n no-throw */
	LIBBASARGUITIE_API std::vector<Int32>			getSelectedIndices			();

	/*! \brief	return if there any multi line controls exist in the property table. 
				\n no-throw */
	LIBBASARGUITIE_API bool							isMultiLinePropertyTable	();

	/*! \brief	set the insert strategy for the multi line ctrls. It defines 
				if new rows will be appended to the ctrl or will be overwriting 
				the existing hits so that the vertical scroll bar will never appear.
				\n throw NoValidMemoryPolicyException */
	LIBBASARGUITIE_API void							setMemoryPolicy				(MemoryPolicyEnum memoryPolicy );

	/*! \brief relational operator for GuiPropertyTableRef */
    LIBBASARGUITIE_API bool							operator ==					(const GuiPropertyTableRef & a) const;
  
    /*! \brief set the matcher. 
		\n	   no-throw */
	void											setMatcher					(boost::shared_ptr<Matcher> pMatcher);
	void											removeMatcher				();

private:
	/*! \brief return the pointer to a GuiPropertyTable. 
 		\n     NullReferenceException */
	const boost::shared_ptr<GuiPropertyTable> &		getImpl						() const;
	//!  \brief pointer to GuiPropertyTable
	boost::shared_ptr<GuiPropertyTable>				m_pGuiPropertyTable; 
};

//---------------------------------------------------------------------------------------//
// typedefs
//---------------------------------------------------------------------------------------//
typedef basar::VarString				ColumnName;		//!< name of a column as string
typedef basar::I18nString				SQLString;		//!< string type of a column
typedef basar::VarString				PropertyName;	//!< name of a property
typedef basar::property::ColumnIndex	ColumnSize;		//!< size of a column

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// class GuiPropertyTable_YIterator
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \brief  class YIterator holds a pointer to a GuiPropertyTable and a row-number.
            
		This is a final class.     
		class throws InvalidIteratorException, UnknownPropertyNameException, OutOfRangeIteratorException */
class GuiPropertyTable_YIterator
{
	friend class GuiPropertyTable;

public:
	/*!	\brief	constrution of GuiPropertyTable_YIterator; 
				pointer to GuiPropertyTable and row-number (default = -1) needed
				\n no-throw */
	LIBBASARGUITIE_API GuiPropertyTable_YIterator( boost::shared_ptr<GuiPropertyTable> pGuiPropertyTable, 
												   Int32 row = -1 );
	
	/*!	\brief	default constructor. \n no-throw */
	LIBBASARGUITIE_API GuiPropertyTable_YIterator();

	/*!	\brief	checks equality of two rows in GuiPropertyTable. \n throw DifferentContainersIteratorException */
	LIBBASARGUITIE_API bool								operator ==	( const GuiPropertyTable_YIterator& rhs ) const;
	
	/*!	\brief	checks unequality of two rows in GuiPropertyTable. \n no-throw */
	LIBBASARGUITIE_API bool								operator !=	( const GuiPropertyTable_YIterator& rhs ) const;
	
	/*!	\brief	returns GuiPropertyTable_XIterator pointing to the first gui ctrl of the read in ones \n no-throw */
	LIBBASARGUITIE_API GuiPropertyTable_XIterator		begin() const;
	
	/*!	\brief	returns GuiPropertyTable_XIterator pointing to column beyond the last one	\n no-throw */
	LIBBASARGUITIE_API GuiPropertyTable_XIterator		end() const;


	/*!	\brief	sets GuiPropertyTable_YIterator to next element (praecrement)	\n OutOfRangeIteratorException */
	LIBBASARGUITIE_API GuiPropertyTable_YIterator		operator ++ ();

	/*!	\brief sets GuiPropertyTable_YIterator to next element (postcrement)	\n OutOfRangeIteratorException */
	LIBBASARGUITIE_API GuiPropertyTable_YIterator		operator ++ ( int i );
	
	/*!	\brief	sets GuiPropertyTable_YIterator to previous row (praecrement). 
				\n OutOfRangeIteratorException */
	LIBBASARGUITIE_API GuiPropertyTable_YIterator		operator -- ();
    
    /*!	\brief ++operator to set iterator to bevor row (postcrement)
		\n OutOfRangeIteratorException */
	LIBBASARGUITIE_API GuiPropertyTable_YIterator		operator--( int i );

    /*! \brief returns the GuiPropertyTableRef the PropertyTable comes from
    */
    LIBBASARGUITIE_API GuiPropertyTableRef getPropertyTable() const;

	/*! \brief	checks if the passed column name is in GuiPropertyTable		\n no-throw */
	LIBBASARGUITIE_API bool								contains( const ColumnName& colName ) const;
	/*! \brief	checks if passed column name is in GuiPropertyTable AND has the passed type (PropertyType)	\n no-throw */
	LIBBASARGUITIE_API bool								contains( const ColumnName& colName, 
																  const property::PropertyType pt ) const;
	/*! \brief	checks if passed column name is in GuiPropertyTable AND has the passed type (PropertyType) in the form of a pair 
		\n no-throw */
	LIBBASARGUITIE_API bool								contains( const std::pair<ColumnName, property::PropertyType>& p) const;

	/*!	\brief	returns value representing the current line of the visible items of the concerned gui control.
				But this current line could be the line of the second page of visualized items 
				or beyond it. So the absolute position is the addition of the current line and
				the visible lines of the control.
				\n no-throw */
	LIBBASARGUITIE_API Int32							getAbsolutePosition() const;

	/*!	\brief	returns value representing the current line of the visible items of the concerned gui control.
				But this current line is the line of the actual page of visualized items 
				\n no-throw */
	LIBBASARGUITIE_API Int32							getPosition() const;

	/*!	\brief	returns const CacheController reference in the form of a shared pointer.	\n no-throw */
	LIBBASARGUITIE_API const GuiPropertyTableRef		getGuiPropertyTable() const;

	/*! \brief	gets the index of the passed parameter rColName (= ColumnName)	\n no-throw */
	LIBBASARGUITIE_API ColumnSize						getIndex	( const ColumnName& rColName ) const;

    /*! \brief	gets the name of the column at Index index
				\n InvalidIteratorException, UnknownPropertyNameException */
	LIBBASARGUITIE_API ColumnName						getName		( const ColumnSize index) const;
	
    /*! \brief	returns true if the value at this index exists	\n no-throw  */
	LIBBASARGUITIE_API bool								isValueExist( const ColumnSize index ) const;

	/*! \brief	returns true if the value for this property exists	\n no-throw  */
	LIBBASARGUITIE_API bool								isValueExist( const ColumnName& rColName ) const;

	/*!	\brief	gets the data type of the passed parameter rColName (ColumnName)
				\n InvalidIteratorException */
	LIBBASARGUITIE_API property::PropertyType			getType		( const ColumnName& rColName ) const;
		
	/*!	\brief	gets value of the passed parameter rColName (ColumnName)	\n no-throw */
	LIBBASARGUITIE_API I18nString						getString	( const ColumnName& rColName ) const;
	
	/*!	\brief	gets value as string of the column accessed via index	\n	no-throw */
	LIBBASARGUITIE_API I18nString						getString	( const ColumnSize index ) const;
	
	/*!	\brief	sets value (type string) to passed column	\n no-throw */
	LIBBASARGUITIE_API void								setString	( const ColumnName& rColName, 
																	  const I18nString& rValue );
	/*!	\brief	sets value as string to a column accessed via index	\n no-throw */
	LIBBASARGUITIE_API void								setString	( ColumnSize index, 
																	  const I18nString& rValue );
	/*!	\brief	gets value of the passed parameter rColName (ColumnName)	\n no-throw */
	LIBBASARGUITIE_API DateTime							getDateTime	( const ColumnName& rColName ) const;
	
	/*!	\brief	gets value as DateTime of the column accessed via index		\n	no-throw */
	LIBBASARGUITIE_API DateTime							getDateTime	( const ColumnSize index ) const;
	
	/*!	\brief	sets value (type DateTime) to passed column		\n no-throw */
	LIBBASARGUITIE_API void								setDateTime	( const ColumnName& rColName, 
																	  const DateTime& rValue );
	/*!	\brief	sets value (type DateTime) to a column accessed via index	\n no-throw */
	LIBBASARGUITIE_API void								setDateTime			( ColumnSize index, 
																			  const DateTime& rValue );
	/*!	\brief	gets value of the passed parameter rColName (ColumnName)	\n no-throw */
	LIBBASARGUITIE_API Date								getDate				( const ColumnName& rColName ) const;
	
	/*!	\brief	gets value as DateTime of the column accessed via index
				\n	InvalidIteratorException */
	LIBBASARGUITIE_API Date								getDate				( const ColumnSize index ) const;
	
	/*!	\brief	sets value (type DateTime) to passed column		\n no-throw */
	LIBBASARGUITIE_API void								setDate				( const ColumnName& rColName, 
																			  const Date& rValue );
	/*!	\brief	sets value (type DateTime) to a column accessed via index	\n no-throw */
	LIBBASARGUITIE_API void								setDate				( ColumnSize index, 
																			  const Date& rValue );
	/*!	\brief	gets value of the passed parameter rColName (ColumnName)	\n no-throw */
	LIBBASARGUITIE_API Time								getTime				( const ColumnName& rColName ) const;
	
	/*!	\brief	gets value as DateTime of the column accessed via index		\n no-throw */
	LIBBASARGUITIE_API Time								getTime				( const ColumnSize index ) const;
	
	/*!	\brief	sets value (type DateTime) to passed column		\n no-throw */
	LIBBASARGUITIE_API void								setTime				( const ColumnName& rColName, 
																			  const Time& rValue );
	/*!	\brief	sets value (type DateTime) to a column accessed via index	\n no-throw */
	LIBBASARGUITIE_API void								setTime				( ColumnSize index, 
																			  const Time& rValue );
	/*!	\brief	gets value of the passed parameter rColName (ColumnName)	\n no-throw */
	LIBBASARGUITIE_API I18nString						getCurrentString	( const ColumnName& rColName ) const;

	/*!	\brief	gets value as string of the column accessed via index		\n no-throw */
	LIBBASARGUITIE_API I18nString						getCurrentString	( const ColumnSize index ) const;
	
	/*!	\brief	sets value (type string) to passed column	\n no-throw */
	LIBBASARGUITIE_API void								setCurrentString	( const ColumnName& rColName, 
																			  const I18nString& rValue );
	/*!	\brief	sets value (type string) to a column accessed via index	\n no-throw */
	LIBBASARGUITIE_API void								setCurrentString	( ColumnSize index,
	                                                                          const I18nString& rValue );

	/*!	\brief	checks if iterator is set */	
	LIBBASARGUITIE_API bool isNull() const;

	/*!	\brief	sets value to NULL */	
    LIBBASARGUITIE_API void reset();

	/*! \brief checks if iterator points to begin() of the inner container */
	LIBBASARGUITIE_API bool isBegin() const;

	/*! \brief checks if iterator points to begin() of the inner container */
	LIBBASARGUITIE_API bool isEnd() const;

	/*! \brief	return the size returned by the method 'maxCountOfControls()'
	plus the member variable 'm_AbsoluteIndex' 
	\n GuiControlNotSupportedException */
	LIBBASARGUITIE_API UInt32 size() const;


    /*!	\brief	returns GuiPropertyTable_XIterator pointing to column with given index
				\n InvalidIteratorException */
	LIBBASARGUITIE_API GuiPropertyTable_XIterator		getXIterator(const ColumnSize index  ) const;

     /*!	\brief	returns GuiPropertyTable_XIterator pointing to column rColName 
				\n InvalidIteratorException */
	LIBBASARGUITIE_API GuiPropertyTable_XIterator		getXIterator(const ColumnName& rColName  ) const;

    /*! \brief	return the number of columns 
    */
	LIBBASARGUITIE_API ColumnSize                		getNumberOfColumns() const;

    /*! \brief notifies ControlBehaviour that matching is done\n member function intentionally not exported */
    void notifyMatchDone();

private:
	/*! \brief	checks if the iterator is valid 
				\n InvalidIteratorException */
	void												checkValid() const;

	/*!	\brief	Member: shared pointer to own GuiPropertyTable */
	boost::shared_ptr<GuiPropertyTable>					m_Pt;	
	
	/*!	\brief	Member: row-number representing the row in a multi line ctrl.*/
	Int32												m_CurrentRow;		
};

//-------------------------------------------------------------------------------------------------//
// class GuiPropertyTable_XIterator
//-------------------------------------------------------------------------------------------------//
/*! \brief  class XIterator holds a pointer to a GuiPropertyTable and row-/column-number.
			With this class, you can access each column of a row.
            
		This is a final class.     
		class throws	UnableToReadWriteInLineOfControlException, 
						NoValidMemoryPolicyException, 
						GuiControlNotSupportedException,
						InvalidIteratorException,
						OutOfRangeIteratorException,
						UnknownPropertyNameException */
class GuiPropertyTable_XIterator
{
public:

	/*! \brief	default constructor with no special action. 
				\n no-throw */
	LIBBASARGUITIE_API GuiPropertyTable_XIterator();

	/*! \brief	overloaded constructor with the class that aggregates the container as shared pointer, 
				the insert type and the pure/real iterator of the container. 
				\n no-throw */
	LIBBASARGUITIE_API GuiPropertyTable_XIterator(  boost::shared_ptr<GuiPropertyTable>                         pt, 
													std::list< boost::shared_ptr< ControlBehavour > >::iterator it, 
													Int32                                                       currentRow );


	/*! \brief	checks the equality of this and the passed xiterator	\n DifferentContainersIteratorException */
	LIBBASARGUITIE_API bool									operator == ( const GuiPropertyTable_XIterator& rhs ) const;
	/*! \brief	checks the unequality of this and the passed xiterator	\n no-throw */
	LIBBASARGUITIE_API bool									operator != ( const GuiPropertyTable_XIterator& rhs ) const;

	/*! \brief	switches forward the iterator (preincrement)	\n OutOfRangeIteratorException */
	LIBBASARGUITIE_API GuiPropertyTable_XIterator			operator ++ ();

    /*! \brief	switches forward the iterator (postincrement) \n OutOfRangeIteratorException */
	LIBBASARGUITIE_API GuiPropertyTable_XIterator			operator++( int i );


	/*! \brief	returns the name of the column with the help of the belonging propertydescriptionlist	\n no-throw */
	LIBBASARGUITIE_API ColumnName							getName() const;
    
	/*! \brief	returns the datatype with the help of the belonging propertydescriptionlist	\n no-throw */
	LIBBASARGUITIE_API property::PropertyType				getType() const;

    /*! \brief	returns the position of the iterator. \n no-throw */
	LIBBASARGUITIE_API ColumnSize							getIndex() const;

	/*! \brief	returns true if the value for this index exists \n no-throw  */
	LIBBASARGUITIE_API bool									isValueExist( ) const;

	/*! \brief	returns the value the iterator points to as a STRING. \n no-throw */
    LIBBASARGUITIE_API I18nString							getString() const;
	
	/*! \brief	sets the value the iterator points to as a STRING. 
				\n	no-throw */
    LIBBASARGUITIE_API void									setString( const I18nString& value );

	/*! \brief	returns the value the iterator points to as a STRING. \n no-throw */
    LIBBASARGUITIE_API basar::I18nString					getCurrentString() const;

	/*! \brief	sets the value the iterator points to as a STRING. 
				\n no-throw */
    LIBBASARGUITIE_API void									setCurrentString( const I18nString& value );

	/*! \brief	returns the value the iterator points to as a DateTime. \n no-throw */
    LIBBASARGUITIE_API DateTime								getDateTime() const;

	/*! \brief	sets the value the iterator points to as a DateTime. 
				\n		 UnableToReadWriteInLineOfControlException, 
						 NoValidMemoryPolicyException, 
						 GuiControlNotSupportedException*/
    LIBBASARGUITIE_API void									setDateTime( const DateTime& value );

	/*! \brief	returns the value the iterator points to as a Date. \n no-throw */
    LIBBASARGUITIE_API Date									getDate() const;

	/*! \brief	sets the value the iterator points to as a Date. \n no-throw */
    LIBBASARGUITIE_API void									setDate( const Date& value );

	/*! \brief	returns the value the iterator points to as a Time. \n no-throw */
    LIBBASARGUITIE_API Time									getTime() const;

	/*! \brief	sets the value the iterator points to as a Time. \n no-throw */
    LIBBASARGUITIE_API void									setTime( const Time& value );

	
	/*!	\brief	checks if iterator is set */	
	LIBBASARGUITIE_API bool isNull() const;

	/*!	\brief	sets value to NULL */	
    LIBBASARGUITIE_API void reset();

	/*! \brief checks if iterator points to begin() of the inner container */
	LIBBASARGUITIE_API bool isBegin() const;

	/*! \brief checks if iterator points to begin() of the inner container */
	LIBBASARGUITIE_API bool isEnd() const;

    /*! \brief notifies ControlBehaviour that matching is done\n member function intentionally not exported */
    void notifyMatchDone();

private:
	/*! \brief checks if the iterator is valid \n throw InvalidIteratorException */
	void					checkValid() const;
	/*! \brief	stores the shared pointer to the implementation object.*/
    boost::shared_ptr<GuiPropertyTable>					m_Pt;

	std::list<boost::shared_ptr< ControlBehavour > >::iterator	m_it; //!< iterator to QWidget-list

	Int32												m_CurrentRow;		//!< current row
	Int32												m_CurrentColumn;	//!< current column
}; 

} // tie
} // gui
} // basar

#endif
