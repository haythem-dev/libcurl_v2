//--------------------------------------------------------------------------------------------//
/*! \file	iservice.h
 *  \brief  base class for all service interfaces 
 *  \author Marco Köppendörfer
 *  \date   10.09.2012
 */
//--------------------------------------------------------------------------------------------//

#ifndef INFRASTRUCTURE_ISERVICE_H
#define INFRASTRUCTURE_ISERVICE_H

//--------------------------------------------------------------------------------------------//
// namespace
//--------------------------------------------------------------------------------------------//
namespace infrastructure
{
    class IService
    {
        public:
            virtual ~IService(){};
            virtual void shutdown() = 0;
    };

} //namespace infrastructure

#endif //INFRASTRUCTURE_ISERVICE_H