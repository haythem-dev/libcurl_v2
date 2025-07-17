//--------------------------------------------------------------------------------------------//
/*! \file	customerandsuppliersearcher.h
 *  \brief  Help class for printcontributionvoucher and printdeliverynote
 *  \author Frank Attmannspacher
 *  \date   16.05.2013
 */
//--------------------------------------------------------------------------------------------//

#ifndef CUSTOMER_AND_SUPPLIER_SEARCHER_H
#define CUSTOMER_AND_SUPPLIER_SEARCHER_H

//--------------------------------------------------------------------------------------------//
// namespaces
//--------------------------------------------------------------------------------------------//
namespace infrastructure
{
	//-------------------------------------------------------------------
    class CustomerAndSupplierSearcher
    {
        public:
            CustomerAndSupplierSearcher			();
            ~CustomerAndSupplierSearcher		();

			void setContributionVoucherData		( const AccessorPropertyTable_YIterator noAcknowledgementIt,
												  const boost::shared_ptr<domMod::ContributionVoucherDM> contributionVoucherDM,
												  const boost::shared_ptr<domMod::BranchDM> branchDM,
												  const boost::shared_ptr<domMod::SearchDM> searchDM,
												  const boost::shared_ptr<domMod::SupplierDM> supplierDM, 
												  const boost::shared_ptr<domMod::CustomerDM> customerDM
												  );

			AccessorPropertyTableRef			getArticleData			() const;
			AccessorPropertyTable_YIterator		getYitBranch			() const;
			AccessorPropertyTable_YIterator		getYitCustomerOrSupplier() const;
			AccessorPropertyTable_YIterator		getYitSignature			() const;
			basar::UInt32						getRows					() const;

        private:
			void setArticleData					( const boost::shared_ptr<domMod::ContributionVoucherDM> contributionVoucherDM,
												  const AccessorPropertyTable_YIterator noacknowledgementIt );
			void setRow							();
			void setCurrentBooking				();
			void setYitBranch					( const boost::shared_ptr<domMod::BranchDM> branchDM );
			void setSearchIt					( const boost::shared_ptr<domMod::SearchDM> searchDM );
			void findCustomerOrSupplierByPattern( const boost::shared_ptr<domMod::SupplierDM> supplierDM, 
												  const boost::shared_ptr<domMod::CustomerDM> customerDM );
			void setYitSignature				();

			bool isSearchItEqualGoodsinOrSupplierReturns() const;

		//-------------------------------------------------------------------
		//Membervariablen
		//-------------------------------------------------------------------
			AccessorPropertyTableRef			m_ArticleData;
			basar::UInt32						m_Rows;
			AccessorPropertyTable_YIterator		m_CurrentBooking;
			AccessorPropertyTable_YIterator		m_YitBranch;
			AccessorPropertyTable_YIterator		m_SearchIt;
			AccessorPropertyTable_YIterator		m_YitCustomerOrSupplier;
			AccessorPropertyTable_YIterator		m_YitSignature;
    };

} //namespace infrastructure

#endif // CUSTOMER_AND_SUPPLIER_SEARCHER_H