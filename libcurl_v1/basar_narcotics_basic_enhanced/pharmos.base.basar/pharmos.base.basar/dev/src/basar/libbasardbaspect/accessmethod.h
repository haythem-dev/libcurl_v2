#ifndef GUARD_ACCESSMETHOD_H
#define GUARD_ACCESSMETHOD_H
//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal interface access method
 *  \author Roland Kiefert
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#include "libbasardbaspect_definitions.h"

/*--------------------------------------------------------------------------------------------------*/
namespace basar  {
namespace db     {
namespace aspect {

/*--------------------------------------------------------------------------------------------------*/
class SQLStringBuilderRef;
class SQLStringBuilderListRef;
class SQLStringBuilder;

/*--------------------------------------------------------------------------------------------------*/
/*! \brief  A class for  acessmethods.

  This is a final class. */
class AccessMethod
{
public:
	typedef boost::shared_ptr< AccessMethod >    AccessMethodSharedPtr;	//!< shared pointer to AccessMethod
    
	/*! \brief The method creates a new SQLStringBuilder. \n no-throw */
	SQLStringBuilderIndex					    push_back( const SQLStringBuilder& builder);

	/*! \brief The method returns a reference to a SQLStringBuilder. \n OutOfRangeIndexException */
	SQLStringBuilderRef							getBuilder( const SQLStringBuilderListSize index ) const;

	/*! \brief The method returns a reference to a SQLStringBuilder. \n OutOfRangeIndexException */
	SQLStringBuilderRef	        				getBuilder( const SQLStringBuilderListSize index ) ;

private:
	/*! \brief returning shared pointer to this by casting weak -> shared
		\n no-throw */
	AccessMethodSharedPtr						getThis( );
	
public:
	/*! \brief The method returns a list of SQLStringBuilders. 
 		\n     no-throw */
	const SQLStringBuilderListRef				getBuilderList();

	/*! \brief The method returns an iterator to the first element of the SQLStringBuilder list. 
 		\n     no-throw */
	std::list<SQLStringBuilderRef>::iterator	begin();

	/*! \brief The method returns an iterator to the last element of the SQLStringBuilder list. 
 		\n     no-throw */
	std::list<SQLStringBuilderRef>::iterator	end();
	
	/*! \brief The method returns the size of the SQLStringBuilder list. 
 		\n     no-throw */
	SQLStringBuilderListSize				    size() const;
	
	/*! \brief The method checks if there are elements in the SQLStringBuilderList. 
 		\n     no-throw */
	bool										empty() const;


	/*! \brief The method creates an AccessMethod. 
 		\n     no-throw */
	static AccessMethodSharedPtr				create();
	/*! \brief The method creates an AccessMethod copied from itself 
	    \n     no-throw */
	const AccessMethodSharedPtr					createInstance() const;
	/*! \brief The method returns the name of the accessor method
		\n no-throw */
	const VarString&							getAccessorMethodName() const;

	/*! \brief The method sets the accessor method name
		\n no-throw */
	void										setAccessorMethodName( const VarString& name );

private:
	/*! \brief The method returns true if this object is a definition. \n no-throw */
	bool										isDefinition() const;

	/*! "weak" reference to object (itself) that's already managed by a shared_ptr (AccessMethod), 
		not visible from the outside, initialized at static create */
	typedef boost::weak_ptr<AccessMethod>		AccessMethodWeakPtr;	//!< weak pointer to AccessMethod

	/*! \brief The method sets the member weak pointer
		\n no-throw */
	void	setWeakPtr							( AccessMethodWeakPtr weakPtr );		

	AccessMethodWeakPtr	m_WeakPtr;			//!< weak pointer to this, needed for common reference group 

	CollSQLStringBuilderRef						m_BuilderList;			//!< list to SQLStringBuilder
	bool										m_Definition;			//!< is this accessmethod a definition?
	VarString									m_AccessorMethodName;	//!< name of the accessor method
};

/*--------------------------------------------------------------------------------------------------*/
} // aspect
} // db
} // basar

/*--------------------------------------------------------------------------------------------------*/
#endif
