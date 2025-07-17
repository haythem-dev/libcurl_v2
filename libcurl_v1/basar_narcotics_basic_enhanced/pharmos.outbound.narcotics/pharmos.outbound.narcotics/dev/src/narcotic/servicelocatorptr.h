//--------------------------------------------------------------------------------------------//
/*! \file	servicelocatorptr.h
 *  \brief  Definition of servicelocatorptr
 *  \author Marco Köppendörfer
 *  \date   10.09.2012
 */
//--------------------------------------------------------------------------------------------//

#ifndef INFRASTRUCTURE_SERVICELOCATORPTR_H
#define INFRASTRUCTURE_SERVICELOCATORPTR_H

//--------------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>

//--------------------------------------------------------------------------------------------------//
// namespace section
//--------------------------------------------------------------------------------------------------//
namespace infrastructure
{
    class ServiceLocator;
    typedef boost::shared_ptr<ServiceLocator> ServiceLocatorPtr;

} //namespace infrastructure

#endif //INFRASTRUCTURE_SERVICELOCATORPTR_H