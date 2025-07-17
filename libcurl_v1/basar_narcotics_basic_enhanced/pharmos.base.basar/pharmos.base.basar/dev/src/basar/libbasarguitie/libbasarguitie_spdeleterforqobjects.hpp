/** $Id: libbasarguitie_defaultexceptionhandler.h 10257 2009-01-30 14:49:46Z anke.klink $                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2009                            
*/ 
#ifndef GUARD_LIBBASARGUITIE_SPDELETERFORQOBJECTS_H
#define GUARD_LIBBASARGUITIE_SPDELETERFORQOBJECTS_H

//----------------------------------------------------------------------------
#include <boost/checked_delete.hpp>

#pragma warning (push)
#pragma warning(disable: 4127)
#include <QtCore/QPointer>
#pragma warning (pop)

//----------------------------------------------------------------------------
namespace basar {
	namespace gui	{
		namespace tie	{

			/*!
			 * Deleter Class for QObject objects that are stored in 
			 * a boost::shared_ptr<T> object and might also be stored
			 * in a parantal-sequence of QObjects. Then those objects
			 * might be deleted twice. In this SPDeleterForQObjects object
			 * the QObject object is stored in a QPointer object. The 
			 * QPointer object is cleared at the destruction of the 
			 * internal QObject object. If later the deleter Function 
			 * operator()(T * t) is called by the shared_ptr no 
			 * deletion is done. If the QPointer Object is still valid,
			 * the interbal Object is deleted by boost::checked_delete(t)
			 * that is used by all boost::shared_ptr<T> objects that
			 * use the default Deleter.
			 *
			 * <b>Caution</b>: The internal object might still be deleted,
			 * before the last shared_ptr<T> object is removed, if
			 * the internal object is deleted at deletion of its 
			 * QObject parent. This deleter only removed the second 
			 * deletion, that would cause the application to terminate.
			 * The use of the internal object in the corresponding 
			 * shared_ptr is still undefined, after the value was 
			 * deleted.
			 */
			template <class T> 
			struct SPDeleterForQObjects
			{
				/**
				 * QPointer of the internal object, if this 
				 * value is Null, the value of the shared_ptr
				 * will not be deleted.
				 */
				QPointer<QObject> m_inner;

				/**
				 * constructor, sets the 'QObject *' within
				 * the m_inner value, this vaslue indicates 
				 * if operator()(T * t) deletes the object t. 
				 * @param p inner Object
				 */
				SPDeleterForQObjects(QObject * p): m_inner(p)
				{
				};

				/**
				 * Copy constructor
				 * @param l object to be copied
				 */
				SPDeleterForQObjects(const SPDeleterForQObjects & l):
				m_inner(l.m_inner)
				{

				}

				/**
				 * Copy operator
				 * @param l object to be copied
				 * @return const reference to assigned instance
				 */
				const SPDeleterForQObjects & operator=(const SPDeleterForQObjects & l) 
				{
					m_inner = l.m_inner;
					return *this;
				}

				/**
				 * deleter function, this function is used to delete the 
				 * inner pointer in a boost::shared_ptr<T>
				 * do not call this function, is is used by the 
				 * boost:: shared ptr implementation
				 * @param t object to be deleted 
				 */
				void operator()(T * t) 
				{
					/*log4cplus::LogLevel loglevel = log4cplus::INFO_LOG_LEVEL;
					if (LoggerPool::loggerViewConnPtr.isEnabledFor(loglevel))
					{
						VarString msg;
						msg.format("SPDeleterForQObjects::operator() called on  %d ", t);
						LoggerPool::loggerViewConnPtr.forcedLog(loglevel, msg, __FILE__, __LINE__);
					}*/
					if (!m_inner.isNull())
					{
						boost::checked_delete(t);
						/*if (LoggerPool::loggerViewConnPtr.isEnabledFor(loglevel))
						{
							VarString msg;
							msg.format("SPDeleterForQObjects::operator() removed  %d ", t);
							LoggerPool::loggerViewConnPtr.forcedLog(loglevel, msg, __FILE__, __LINE__);
						}*/
					}
					m_inner = 0;
				}
			};

		}	// namespace tie
	}	// namespace gui
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD
