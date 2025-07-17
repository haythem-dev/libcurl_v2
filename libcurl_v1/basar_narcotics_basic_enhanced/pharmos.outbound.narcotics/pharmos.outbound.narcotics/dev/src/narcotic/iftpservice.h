//--------------------------------------------------------------------------------------------//
/*! \file	iftpservice.h
 *  \brief  base class for iftpservice
 *  \author Marco Köppendörfer
 *  \date   10.09.2012
 */
//--------------------------------------------------------------------------------------------//

#ifndef INFRASTRUCTURE_IFTPSERVICE_H
#define INFRASTRUCTURE_IFTPSERVICE_H

//--------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------//
#include "iservice.h"

//--------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------//
namespace infrastructure
{
	//-------------------------------------------------------------------
    class IFtpService : public IService
    {
        public:
            virtual ~IFtpService(){};
            virtual bool doesFileExist( basar::Int32 contributionVoucherNo ) = 0;
            virtual bool storeFile( basar::cmnutil::IPrintDataProvider& printData, basar::Int32 contributionVoucherNo ) = 0;
    };

} //namespace infrastructure

#endif //INFRASTRUCTURE_IFTPSERVICE_H