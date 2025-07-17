//----------------------------------------------------------------------------
/*! \file
 *  \brief  acknowledgement status transformer for NA17
 *  \author Marco Köppendörfer
 *  \date   16.01.2013
 */
//----------------------------------------------------------------------------
#ifndef GUARD_ACKNOWLEDGEMENTSTATUSTRANSFORMER_H
#define GUARD_ACKNOWLEDGEMENTSTATUSTRANSFORMER_H

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{
    namespace viewConn
    {

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//

        class AcknowledgementStatusTransformer : public basar::gui::tie::Transformer
        {
            public:
	        //! destructor										\n no-throw
					        ~AcknowledgementStatusTransformer				();

	        //! constructor	for property name transformer		\n no-throw
					        AcknowledgementStatusTransformer					();

	        //! transform values from right to left iterator	\n UnknownPropertyTypeException
	        virtual void	RightToLeft							(basar::gui::tie::GuiPropertyTable_YIterator	         guiIt,	//!< iterator to GuiPropertyTable row
														        const basar::db::aspect::AccessorPropertyTable_YIterator accIt	//!< iterator to property table/cache controller row
														        );
	        //! transform values from left to right iterator	\n TypecastFailedException
	        virtual void	LeftToRight							(const basar::gui::tie::GuiPropertyTable_YIterator        guiIt,	//!< iterator to GuiPropertyTable row
														        basar::db::aspect::AccessorPropertyTable_YIterator       accIt	//!< iterator to property table/cache controller row
														        );

	        //! creates cloned instance							\n no-throw
	        virtual boost::shared_ptr < basar::gui::tie::Transformer > 	create	();
        };
    }
}
#endif//GUARD_ACKNOWLEDGEMENTSTATUSTRANSFORMER_H