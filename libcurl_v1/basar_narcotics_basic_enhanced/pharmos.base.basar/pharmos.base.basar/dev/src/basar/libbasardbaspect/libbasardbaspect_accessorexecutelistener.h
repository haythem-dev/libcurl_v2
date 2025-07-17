#ifndef GUARD_LIBBASARDBASPECT_ACCESSOREXECUTELISTENER_H
#define GUARD_LIBBASARDBASPECT_ACCESSOREXECUTELISTENER_H

//----------------------------------------------------------------------------
/*! \file
 *  \brief  hosting a AccessorExecuteListener
 *  \author Roland Kiefert
 *  \date   18.07.2005
 */
//----------------------------------------------------------------------------

//------------------------------------------------------------------------------//
// dll exports and imports
//------------------------------------------------------------------------------//
#ifndef LIBBASARDBASPECT_API
	#ifdef _WIN32
		#ifdef LIBBASARDBASPECT_EXPORTS
			#define LIBBASARDBASPECT_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARDBASPECT_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARDBASPECT_API
	#endif 
#endif 

//------------------------------------------------------------------------------//
#include <boost/shared_ptr.hpp>
#include "libbasardbaspect_accessorpropertytable_yiterator.h"

//------------------------------------------------------------------------------//
namespace basar  {
namespace db	 {
namespace aspect {

//------------------------------------------------------------------------------//
class AccessorExecuteListenerRef;

//////////////////////////////////////////////////////////////////////////////////
/*! \brief  Event that is filled at Accessor::execute Method, if
 *          a non select statement is called.
 */
//////////////////////////////////////////////////////////////////////////////////
class AccessorExecuteEvent
{
public:
    //! constructor
    AccessorExecuteEvent	(const AccessMethodName                & name, 
							 const AccessorPropertyTable_YIterator & yit);

    const AccessMethodName					m_methodName;               //!< name of the Access method
    const AccessorPropertyTable_YIterator	m_yIterator;				//!< y Iterator used by the Access method
    std::vector< ExecuteResultInfo >		m_separateBuilderResults;	//!< Builder Results
    ExecuteResultInfo						m_executeResult;            //!< Execute Result


private:
    //! this class must not be assigned, copy constructor is allowed
    AccessorExecuteEvent &	operator = (const AccessorExecuteEvent &);
};

//------------------------------------------------------------------------------//
/*! Listener class, for listening for a AccessorExecuteEvent of one Accessor, has to
 *  be overwritten (f.ex. by the macros:
 *  <pre>
 *     BEGIN_ACCESSOREXECUTELISTENER_DECLARATION(TestAccessorExecuteListenerStdOutLog)
 *     END_ACCESSOREXECUTELISTENER_DECLARATION
 * 
 *     ACCESSOREXECUTELISTENER_FUNCTION(test::unit::dbaspect::TestAccessorExecuteListenerStdOutLog)
 *     {
 *         // to do your function code, f.ex:
 *         // std::cout << "test TestAccessorExecuteListener1 - result  Errornumber = " << event.m_executeResult.m_error << std::endl;
 *     }    
 *  </pre>
 */ 
class AccessorExecuteListener 
{   
public:

    //! constructor
	AccessorExecuteListener			()	{};

    //! virtual destructor
	virtual ~AccessorExecuteListener()	{};

public: 
    /*! create method used to create a AccessorExecuteListenerRef in Accessor, 
     *  overwritten in the macro 
     *  \return AccessorExecuteListenerRef referencing created instance
     */
    virtual AccessorExecuteListenerRef create				() const = 0;
    
    //! event method, to be overwritten
    virtual void						executePerformed	(const AccessorExecuteEvent &) = 0;
}; 

//------------------------------------------------------------------------------//
/*! Reference class, hosting a AccessorExecuteListener */
class AccessorExecuteListenerRef 
{ 
public:

    //! constructor, used by the AccessorExecuteListener::create method
    LIBBASARDBASPECT_API  
	AccessorExecuteListenerRef( boost::shared_ptr<db::aspect::AccessorExecuteListener> );

    //! default constructor
    LIBBASARDBASPECT_API  AccessorExecuteListenerRef();

     //! virtual destructor
    LIBBASARDBASPECT_API  virtual ~AccessorExecuteListenerRef();

    //! event method, called by accessor::execute
    LIBBASARDBASPECT_API  void executePerformed(const AccessorExecuteEvent & event //!< event 
											   );

    //! is Null metod
    LIBBASARDBASPECT_API  bool isNull();

    //! resets the AccessorExecuteListener
    LIBBASARDBASPECT_API void reset();

private:

    //! returns the inner AccessorExecuteListener
    const boost::shared_ptr<AccessorExecuteListener>& getImpl() const;

    //! AccessorExecuteListener Implemementation
    boost::shared_ptr<AccessorExecuteListener> m_impl;
};

//----------------------------------------------------------------------------
} // aspect
} // db
} // basar

#endif
