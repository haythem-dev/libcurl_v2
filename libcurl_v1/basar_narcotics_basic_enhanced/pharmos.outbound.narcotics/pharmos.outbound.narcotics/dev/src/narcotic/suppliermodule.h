//----------------------------------------------------------------------------
/*! \file
 *  \brief  main interface for domain class supplier
 *  \author Frank Attmannspacher
 *  \date   15.05.2013
 */
//----------------------------------------------------------------------------
#ifndef GUARD_SUPPLIERMODULE_H
#define GUARD_SUPPLIERMODULE_H

//---------------------------------------------------------------------------
//namespace section
//---------------------------------------------------------------------------
namespace domMod
{

//---------------------------------------------------------------------------
/*! \brief  domain class for supplier module.

	\n This is a final class.  
	\n throws no-throw */
class SupplierDM
{
public:
	/*!\brief Default constructor.
		\n no-throw */
	SupplierDM();
	/*!\brief Default destructor.
		\n no-throw */
	virtual ~SupplierDM();
	/*! \brief This method init the module customerModule
		\n no-throw */
	void												init( const basar::Int16 branchNo, const basar::db::aspect::ConnectionRef activeConn );

	void												shutdown();

	basar::db::aspect::AccessorPropertyTable_YIterator	findSupplierByPattern( basar::db::aspect::AccessorPropertyTable_YIterator yit //!< yiterator, containing where-statement
														);

private:
	basar::db::aspect::AccessorInstanceRef				m_accSupplier;			//!< instance of supplier-accessor
	bool												m_Initialized;
};

} //namespace domMod

#endif //GUARD_SUPPLIERMODULE_H