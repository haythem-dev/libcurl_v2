//--------------------------------------------------------------------------------------------//
/*! \file	iprintdeliverynote.h
 *  \brief  base class for printdeliverynote
 *  \author Marco Köppendörfer
 *  \date   10.09.2012
 */
//--------------------------------------------------------------------------------------------//

#ifndef INFRASTRUCTURE_IPRINTDELIVERYNOTE_H
#define INFRASTRUCTURE_IPRINTDELIVERYNOTE_H

//--------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------//
#include "iservice.h"

//--------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------//
namespace infrastructure
{
    class IPrintDeliveryNote : public IService
    {
        public:
            virtual ~IPrintDeliveryNote(){};
            virtual void reprintDeliveryNote( basar::db::aspect::AccessorPropertyTable_YIterator noacknowledgementIt ) = 0;
    };

} //namespace infrastructure

#endif //INFRASTRUCTURE_IPRINTDELIVERYNOTE_H