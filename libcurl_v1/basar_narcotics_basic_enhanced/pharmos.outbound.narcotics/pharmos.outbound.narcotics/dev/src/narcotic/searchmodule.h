//-------------------------------------------------------------------------------------------------//
/*! \file 
 *  \brief  Domain module for handling searches based on an empty accessor
 *  \author Benjamin Endlicher
 *  \date   24.08.2010
 */
//-------------------------------------------------------------------------------------------------//
#ifndef GUARD_SEARCHMODULE_H
#define GUARD_SEARCHMODULE_H

//-------------------------------------------------------------------------------------------------//
// include section
//-------------------------------------------------------------------------------------------------//
#include "accessor_definitions.h"
#include "logmodule.h"

//-------------------------------------------------------------------------------------------------//
// namespace section
//-------------------------------------------------------------------------------------------------//
namespace domMod
{
  //-------------------------------------------------------------------------------------------------//
  // class declaration
  //-------------------------------------------------------------------------------------------------//
  /*!\brief Domain module for handling searches */
	class SearchDM
	{
	public:
		/*!\brief Default constructor.
		\n no-throw */
		SearchDM();

		/*!\brief Default destructor.
		\n no-throw */
		virtual ~SearchDM();

		//! \brief call this function to initialize accessor instances \n no-throw
		void init( const basar::db::aspect::ConnectionRef activeConn );

        //! \brief call this function to clear all accessor instances and the contents of property tables \n no-throw 
	    void shutdown();

		//! \brief call this function to get search property table \n no-throw 
		basar::db::aspect::AccessorPropertyTableRef getAccSearchPropTab();

		//! \brief call this function to clear the content of search property table \n no-throw 
	    void clearSearchPropTab();
	private:
		/*! \brief copy constructor
		\n no-throw */
		SearchDM( const SearchDM& SearchDM );
		
		/*! \brief assignment operator
		\n no-throw */
		SearchDM& operator= ( const SearchDM& SearchDM );

		basar::db::aspect::AccessorInstanceRef getSearchACC();

		basar::db::aspect::ConnectionRef m_ConnectionRef;

		basar::Int32					 m_Initialized;	//!< Flag/Counter for the number of initializations, only the counts the number of domain modules

		//-------------------------------------------------------------------------------------
		// accessor instances ( created in init() ) which needs an active connection
		//-------------------------------------------------------------------------------------
        CREATE_ACCESSOR_INSTANCE_DECL( Search )
	};

} //namespace domMod

#endif //GUARD_SEARCHMODULE_H