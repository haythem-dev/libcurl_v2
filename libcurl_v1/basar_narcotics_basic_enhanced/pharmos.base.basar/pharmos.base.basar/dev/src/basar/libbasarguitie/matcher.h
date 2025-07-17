#ifndef GUARD_MATCHER_H
#define GUARD_MATCHER_H
//--------------------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  internal interface matcher guitie
 *  \author Roland Kiefert
 *  \date   20.09.2005
 */
//--------------------------------------------------------------------------------------------------------------//

#include <list>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "libbasar_definitions.h"
#include "libbasardbaspect_accessorpropertytable.h"
#include "libbasarguitie_definitions.h"
#include "libbasarguitie_propertytable.h"
#include "libbasarguitie_transformer.h"

//--------------------------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------------------------//
namespace basar {

namespace db
{
	namespace aspect	
	{
		class AccessorPropertyTable_YIterator;
	}
}

//----------------------------------------------------------------------------
namespace gui {
namespace tie {

//--------------------------------------------------------------------------------------------------------------//
class Transformer;

//--------------------------------------------------------------------------------------------------------------//
// typedef
//--------------------------------------------------------------------------------------------------------------//
typedef UInt32 TransformerIndex;		//!< index of transformer in the list

//--------------------------------------------------------------------------------------------------------------//
// forward declaration
//--------------------------------------------------------------------------------------------------------------//
class  GuiPropertyTableRef;
class  GuiPropertyTable_YIterator;
struct IndexStruct;

//--------------------------------------------------------------------------------------------------------------//
// class declaration
//--------------------------------------------------------------------------------------------------------------//
/*! \brief  Matcher class of guitie.

  This is a final class. */
class Matcher
{
public:
	typedef boost::shared_ptr< Matcher >				MatcherSharedPtr;		//!< shared pointer to Matcher
	typedef db::aspect::AccessorPropertyTable_YIterator AccPropTblYIterator;	//!< abbreviation
	typedef db::aspect::AccessorPropertyTableRef        AccPropTblRef;			//!< abbreviation
    
	//! d'tor	\n no-throw
	virtual				   ~Matcher				();

	/*! \brief return reference to AccessorPropertyTableRef		\n no-throw */
	AccPropTblRef			getRight			();
	/*! \brief return reference to GuiPropertyTableRef		\n no-throw */
	GuiPropertyTableRef		getLeft				();

	/*! \brief set matching policy	\n no-throw */
	void					setMatchingPolicy	(const MatcherPolicyType policy);

	/*! \brief move from right propertytable to left propertytable	\n no-throw */
	Int32					RightToLeft			();
	/*! \brief move from left propertytable to right propertytable		\n no-throw */
	Int32					LeftToRight			();
	/*! \brief move specified number of rows from right propertytable to left propertytable, 
	           starting at index-position
		\n UnknownIndexException */
	Int32					RightToLeft			(const Int32 index, 
												 const Int32 count);
	/*! \brief move specified number of rows from left propertytable to right propertytable, 
	           starting at index-position
		\n	UnknownIndexException */
	Int32					LeftToRight			(const Int32 index, 
												 const Int32 count);
	/*! \brief move the iterator of accessorpropertytable to iterator of guipropertytable
		\n UnknownPropertyTypeException, UnknownColoumnException */
	void					RightToLeft			(GuiPropertyTable_YIterator guiIt, 
												 const AccPropTblYIterator  accIt);

	/*! \brief move iterator of guipropertytable to iterator of accessorpropertytable
		\n UnknownPropertyTypeException, UnknownColoumnException */
	void					LeftToRight			(const GuiPropertyTable_YIterator guiIt,
												 const AccPropTblYIterator        accIt);

	/*! \brief get the gui property table y-iterator to the current line		\n no-throw */
	GuiPropertyTable_YIterator					
							getCurrentLeft		();
	/*! \brief get the accessor property table y-iterator to the current line		\n no-throw */
	AccPropTblYIterator	
							getCurrentRight		();
	/*! \brief get the gui property table y-iterator to the first selected line		\n no-throw */
	GuiPropertyTable_YIterator			
							getSelectedLeft		();
	/*! \brief get the gui property table y-iterator to the next selected line		\n Uno-throw  */
	GuiPropertyTable_YIterator					
							getNextSelectedLeft	();
	/*! \brief get the accessor property table y-iterator to the first selected line		\n no-throw  */
	AccPropTblYIterator	
							getSelectedRight	();
	/*! \brief get the accessor property table y-iterator to the next selected line \n no-throw  */
	AccPropTblYIterator	
							getNextSelectedRight();

	/*! \brief add a transformer to the matcher
		\n no-throw */
	const TransformerIndex	push_back			(Transformer & transformer);

	/*! \brief create a Matcher. 
 		\n     no-throw */
	static MatcherSharedPtr	create				(const GuiPropertyTableRef guiPropTable , 
												 const AccPropTblRef       accPropTable);

	/*! \brief return shared pointer to this by casting weak -> shared
		\n no-throw */
	const MatcherSharedPtr	getThis				() const;

	/*! \brief     move the iterator of accessorpropertytable to the iterator of guipropertytable
		\n         throw UnknownPropertyTypeException, UnknownColoumnException */
	static void				RightToLeft			(GuiPropertyTable_YIterator guiIt, 
												 const AccPropTblYIterator  accIt, 
												 const MatcherPolicyType    policy);
			
	/*! \brief     move the iterator of guipropertytable to the iterator of accessorpropertytable
		\n         throw UnknownPropertyTypeException, UnknownColoumnException */
	static void				LeftToRight			(const GuiPropertyTable_YIterator guiIt, 
												 const AccPropTblYIterator        accIt, 
												 const MatcherPolicyType          policy);

	/*! \brief connect all column header labels with the original attribute names
		\n no-throw */
	void					transformProperties	(const std::list<VarString> & headerLabels          , 
												 const std::list<VarString> & originalAttributeNames);

	  /*! removes all inner m_GuiPropTable,m_AccPropTable */
	void					clear				();

	  /*! removes the matcher fron GuiTable */
	void					removeMatcherEvents	();

private:

	/*! \brief construction of Matcher called by the static method ( create() [simple factory method]); 
	setting GuiPropertyTableRef and AccessorPropertyTableRef to member. \n no-throw */
							Matcher				(const GuiPropertyTableRef guiPropTable , 
												 const AccPropTblRef       accPropTable);

	/*! \brief "weak" reference to Matcher (this self) that's already managed 
	           by a shared_ptr (Matcher), not to see from extern; initialized at static create */
	typedef boost::weak_ptr<Matcher>	MatcherWeakPtr;

	/*! \brief set member weak pointer. \n no-throw */
	void					setWeakPtr			(const MatcherWeakPtr weakPtr);		

	/*! \brief fills a list of IndexStruct with the translation indizes between a GuiPropertyTable_YIterator
	           and a AccessorPropertyTable_YIterator taking into account the given MatcherPolicyType	*/
	static void				fillIndexStruct		(std::list<IndexStruct>   & indexList,	//!< resulting IndexStruct
												 GuiPropertyTable_YIterator guiIt    ,	//!< GuiPropertyTable_YIterator
												 const AccPropTblYIterator  accIt	 ,	//!< AccessorPropertyTable_YIterator   
												 const MatcherPolicyType    policy		//!< MatcherPolicyType   
												);

	/*! \brief writes one AccessorPropertyTable_YIterator value to the corresponding
	           GuiPropertyTable_YIterator (as defined in the IndexStruct)	*/
	static void				setGuiPropTblYIt	(IndexStruct              & index, //!< defines which index the value is written to, and which index the value is gotten from
												 GuiPropertyTable_YIterator guiIt, //!< the GuiPropertyTable_YIterator
												 const AccPropTblYIterator  accIt  //!< the AccessorPropertyTable_YIterator  
												);
	/*! \brief writes all AccessorPropertyTable_YIterator values to the corresponding
               GuiPropertyTable_YIterator (as defined in the IndexStruct)	*/
	static void				setGuiPropTblYIt	(std::list<IndexStruct>   & index,  //!< defines which indicees the values are written to, and which indicees the values are gotten from
												 GuiPropertyTable_YIterator guiIt,  //!< the GuiPropertyTable_YIterator  
												 const AccPropTblYIterator  accIt	//!< the AccessorPropertyTable_YIterator  
												);

	/*! \brief internal function for RightToLeft*/
	inline void				innerRightToLeft	(GuiPropertyTable_YIterator guiIt, //!< GuiPropertyTable_YIterator  
												 const AccPropTblYIterator  accIt  //!< AccessorPropertyTable_YIterator  
												);
	/*!\brief internal finction for LeftToRight */
	inline void				innerLeftToRight	(const GuiPropertyTable_YIterator guiIt,	//!< Gui-Iterator
												 AccPropTblYIterator              accIt		//!< Accessor-Iterator
												);

	/*! \brief uses Transformer RightToLeft	*/
	void					transformRight2Left	(GuiPropertyTable_YIterator guiIt, 
												 const AccPropTblYIterator  accIt);
	/*! uses Transformer for LeftToRight */
	void					transformLeft2Right	(const	GuiPropertyTable_YIterator guiIt,   //!< Gui-Iterator
												 AccPropTblYIterator               accIt	//!< Accessor-Iterator
												);

	/*!\brief writes one GuiPropertyTable_YIterator value to the corresponding
	          AccessorPropertyTable_YIterator (as defined in the IndexStruct)*/
	static void				setAccPropTblYIt	(IndexStruct                    & index, 
												 const GuiPropertyTable_YIterator guiIt,
												 AccPropTblYIterator              accIt);
	/*! \brief writes GuiPropertyTable_YIterator values to the corresponding
	           AccessorPropertyTable_YIterator (as defined in the IndexStruct)	*/
	static void				setAccPropTblYIt	(std::list<IndexStruct>         & index, 
												 const GuiPropertyTable_YIterator guiIt,
												 AccPropTblYIterator              accIt);

	/*! \brief checks is one MatcherPolicyEnum Policy  (type) is part of the MatcherPolicyType policy */
	static bool				checkPolicy			(const MatcherPolicyType & policy,	//!< policy to be checked
												 const MatcherPolicyEnum & type		//!< policy value to be check for
												);
	/*! \brief checks is one MatcherPolicyEnum Policy  (type) is part of the used MatcherPolicyType  */
	bool					checkPolicy			(const MatcherPolicyEnum & type		//!< policy value to be check for
												);

	/*! \brief steps up to the index index */
	bool					step2Idx4Right2Left	(const Int32                  index, 
												 GuiPropertyTable_YIterator & guiIt, 
												 AccPropTblYIterator        & accIt);
	/*! \brief steps up to the index index	 */
	bool					step2Idx4Left2Right	(const Int32                  index, 
												 GuiPropertyTable_YIterator & guiIt, 
												 AccPropTblYIterator        & accIt
												);

	/*!\brief checks if the GuiPropertyTable_YIterator or AccessorPropertyTable_YIterator
	          is begin and throws an Exception if the value throws os true	 */
	bool					checkIsBegin		(GuiPropertyTable_YIterator & guiIt    , 
												 AccPropTblYIterator        & accIt    ,
												 bool                         throws   ,
												 ConstString                  errortext,
											 	 Int32                        index    ,
												 Int32                        count    );

	/*! \brief skips deleted Rows (forward (accIt++)) */
	bool					skipDeletedForward	(AccPropTblYIterator & accIt	//!< used YIterator to be increased
												);
	/*! \brief skips deleted Rows (backward (accIt--))  */
	bool					skipDeletedBackward	(AccPropTblYIterator & accIt	//!< used YIterator to be decreased
												);

    inline void RightToLeftInternal(GuiPropertyTable_YIterator guiIt, const AccPropTblYIterator  accIt);

private:

	MatcherWeakPtr								m_WeakPtr;			//!< weak pointer to this, needed for common reference group 

	MatcherPolicyType							m_Policy;			//!< policy of the matcher
	AccPropTblRef								m_AccPropTable;		//!< reference to an AccessorPropertyTable
	GuiPropertyTableRef							m_GuiPropTable;		//!< reference to a GuiPropertyTable
	std::list<Transformer::TransformSharedPtr>	m_TransformerList;	//!< list containing Transformers

	std::list<IndexStruct>						m_IndexTable;		//!< list containing index transfer
	std::vector<Int32>							m_LeftIndices;		//!< selected indices from the left side
	std::vector<Int32>::iterator				m_LeftIndex;		//!< aktual index from the vector m_LeftIndices
	std::vector<Int32>							m_RightIndices;		//!< selected indices from the right side
	std::vector<Int32>::iterator				m_RightIndex;		//!< aktual index from the vector m_RightIndices
};

} // tie
} // gui
} // basar

#endif
