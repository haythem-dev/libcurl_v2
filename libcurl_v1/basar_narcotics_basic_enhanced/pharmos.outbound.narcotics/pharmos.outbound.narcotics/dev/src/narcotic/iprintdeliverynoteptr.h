//--------------------------------------------------------------------------------------------//
/*! \file	iprintdeliverynoteptr.h
 *  \brief  Definition of IPrintDeliveryNotePtr
 *  \author Marco Köppendörfer
 *  \date   10.09.2012
 */
//--------------------------------------------------------------------------------------------//

#ifndef INFRASTRUCTURE_IPRINTDELIVERYNOTEPTR_H
#define INFRASTRUCTURE_IPRINTDELIVERYNOTEPTR_H

//--------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------//
namespace infrastructure
{
    class  IPrintDeliveryNote;
    typedef boost::shared_ptr<IPrintDeliveryNote> IPrintDeliveryNotePtr;

} //namespace infrastructure

#endif //INFRASTRUCTURE_IPRINTDELIVERYNOTEPTR_H