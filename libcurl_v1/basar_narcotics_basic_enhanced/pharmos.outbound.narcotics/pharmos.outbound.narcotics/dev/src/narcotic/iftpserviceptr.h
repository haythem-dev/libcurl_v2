//--------------------------------------------------------------------------------------------//
/*! \file	iftpserviceptr.h
 *  \brief  Definition of IFtpSerivcePtr
 *  \author Marco Köppendörfer
 *  \date   10.09.2012
 */
//--------------------------------------------------------------------------------------------//

#ifndef INFRASTRUCTURE_IFTPSERIVCEPTR_H
#define INFRASTRUCTURE_IFTPSERIVCEPTR_H

//--------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------//
namespace infrastructure
{
    class  IFtpService;
    typedef boost::shared_ptr<IFtpService> IFtpServicePtr;

} //namespace infrastructure

#endif //INFRASTRUCTURE_IFTPSERIVCEPTR_H