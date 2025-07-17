//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "acknowledgementstatustransformer.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics
{   
    namespace viewConn
    {
        //----------------------------------------------------------------------------
        /*! \throw  no-throw */
        AcknowledgementStatusTransformer::~AcknowledgementStatusTransformer() 
        {
        }

        //----------------------------------------------------------------------------
        //! \throw  no-throw
        //! 
        AcknowledgementStatusTransformer::AcknowledgementStatusTransformer()
        {
        }

        //----------------------------------------------------------------------------
        //! \throw  no-throw
        //! 
        void AcknowledgementStatusTransformer::RightToLeft(basar::gui::tie::GuiPropertyTable_YIterator	guiIt,
									        const basar::db::aspect::AccessorPropertyTable_YIterator	accIt)
        {
	        const basar::VarString name = QApplication::translate("AcknowledgementStatusTransformer","AcknowledgementStatus").toLocal8Bit().data();
            

            switch(accIt.getInt16(constants::ACKNOWLEDGEMENTSTATUS))
            {
                case constants::PRINTED:
                    guiIt.setString(name, QApplication::translate("AcknowledgementStatusTransformer", "printed").toLocal8Bit().data());
                    break;
                case constants::ACKNOWLEDGEMENT_RECEIVED:
                    guiIt.setString(name, QApplication::translate("AcknowledgementStatusTransformer", "received").toLocal8Bit().data());
                    break;
                case constants::CHANGED_ACKNOWLEDGEMENT:
                    guiIt.setString(name, QApplication::translate("AcknowledgementStatusTransformer", "received").toLocal8Bit().data());
                    break;
                case constants::DELIVERY_NOTE_COPY_CREATED:
                    guiIt.setString(name, QApplication::translate("AcknowledgementStatusTransformer", "Delivery note copy created").toLocal8Bit().data());
                    break;
                default:
                    guiIt.setString(name, QString::number(accIt.getInt16(constants::ACKNOWLEDGEMENTSTATUS)).toLocal8Bit().data());
            }
        }

        //----------------------------------------------------------------------------
        //! \throw  no-throw
        //! 
        void AcknowledgementStatusTransformer::LeftToRight(const basar::gui::tie::GuiPropertyTable_YIterator		guiIt,
									                       basar::db::aspect::AccessorPropertyTable_YIterator	accIt)
        {
        }

        //----------------------------------------------------------------------------
        //! \throw  no-throw
        //! \return pointer to created transformer
	    boost::shared_ptr < basar::gui::tie::Transformer > AcknowledgementStatusTransformer::create()
        {
	        return boost::shared_ptr < basar::gui::tie::Transformer > (new AcknowledgementStatusTransformer(*this));
        }
    }
}