//----------------------------------------------------------------------------
/*! \file
 *  \brief  global accessor definitions for narcotics (at the moment only used in NarcoticsDM)
 *  \author Benjamin Endlicher
 *  \date   12.03.2010
 */
//----------------------------------------------------------------------------
#ifndef GUARD_ACCESSOR_DEFINITIONS_H
#define GUARD_ACCESSOR_DEFINITIONS_H

//----------------------------------------------------------------------------
//namespaces
//----------------------------------------------------------------------------
namespace lit
{
namespace narcotics
{
namespace domMod 
{
	//----------------------------------------------------------------------------------------------------//
	// help macros to handle accessors more easily
	//----------------------------------------------------------------------------------------------------//
	/*! \brief	Macro for creating and instantiating an Accessor Declaration.  */
	#define CREATE_ACCESSOR_INSTANCE_DECL( AccessorDeclarationName )  \
	basar::db::aspect::AccessorInstanceRef m_##AccessorDeclarationName##ACC;

// ToDo: the following accessor definition isn't working properly as the accessor name isn't corresponding to given declaration name
	#define CREATE_ACCESSOR_INSTANCE_DEF( DMName, AccessorDeclarationName, AccessorName, cachingPolicy )  \
	AccessorInstanceRef DMName::get##AccessorDeclarationName##ACC() \
	{\
		if( m_##AccessorDeclarationName##ACC.isNull() ){\
			m_##AccessorDeclarationName##ACC = basar::db::aspect::Manager::getInstance().createAccessorInstance( \
									"Acc_"#AccessorDeclarationName, #AccessorName, m_ConnectionRef, cachingPolicy );\
		}\
		return m_##AccessorDeclarationName##ACC;\
	}	

	/*!\brief Macro allows to clear and release a PropertyTable that contains some data \n
		If this is not done an ASSERT show up during termination of the program. */
	#define FREE_ACCESSOR( AccessorDeclarationName ) \
		if( false == AccessorDeclarationName.isNull() ) \
		{ \
			if( false == AccessorDeclarationName.getPropertyTable().isNull() ) \
			{ \
				AccessorDeclarationName.getPropertyTable().clear(); \
				AccessorDeclarationName.getPropertyTable().reset(); \
			} \
			AccessorDeclarationName.reset(); \
		}

} //namespace domMod
} //namespace narcotics
} //namespace lit

#endif //GUARD_ACCESSOR_DEFINITIONS_H