//----------------------------------------------------------------------------
/*! \file 
*  \brief  template for view connector pointers
*  \author Thomas Hörath
*  \date   22.08.2006
*/
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARGUITIE_VIEWCONNPTR_H
#define GUARD_LIBBASARGUITIE_VIEWCONNPTR_H

//----------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>

#include "libbasarappl_systemeventmanager.h"
#include "libbasarqtwidget_widgetpointer.h"
#include "libbasarguitie_spdeleterforqobjects.hpp"

//----------------------------------------------------------------------------
namespace basar {
	namespace gui	{
		namespace tie	{

			/*! 
			* \brief 
			* Template class to create and store a view connector, 
			* the creation is handled dynamically 
			* (created at first access of operator '->', or with function initialize()) 
			* and can be source of a gui event. 
			* The ViewConnector is stored as a shared_ptr, if also a parent is given, 
			* the ViewConnector is deleted in the QObject Sequence. The "->" should therefore
			* be used with care.
			* \throw no-throw
			*/
			template<class T> class ViewConnPtr : public basar::appl::IEventSource,
												  public basar::gui::widget::WidgetPointer
			{
			public:
				typedef T	element_type;	//!< typedef for VC
				typedef T	value_type;		//!< typedef for VC
				typedef T * pointer;		//!< typedef for ptr to VC

				/*! \brief Constructor of singleton instance. 
				\throw no-throw */
				ViewConnPtr()		
				{
					
				}

				~ViewConnPtr()
				{
					cleanup();
				}

				/*! \brief copy constructor
				\param r viewconnptr to set to this
				\throw no-throw */
				template<class Y>
					ViewConnPtr(ViewConnPtr<Y> const & r): m_ptr(r.m_ptr)
				{
					

				}

				/*! \brief constructor for sending QWidget
				\param sp shared ptr to set to this
				\throw no-throw */
				ViewConnPtr(boost::shared_ptr<T> sp) 
				{
					//do cleanup
					cleanup();

					if (sp.get())
					{
						setEntryAndReadEntryParent(sp.get());
					}
					m_ptr = sp;
					
				}

				/*! \brief return pointer to QWidget out of ViewConnPtr (e.g. for setting as parent for next sub-dialog)
				\return internal QWidget pointer
				\throw no-throw */
				QWidget* getWidgetPtr() const		
				{	
					return getEntry();	
				}

				/*! initializes entries */
				void initialize()
				{
					init();
				}


#if !defined(BOOST_MSVC) || (BOOST_MSVC > 1200)

				/*! \brief operator = , only needed in special cases
				\param r viewconnptr to assign to this
				\return assigned instance
				\throw no-throw */
				template<class Y>
					ViewConnPtr& operator=(ViewConnPtr<Y> const & r) 
				{
					QObject w = r.getEntry();
					m_ptr = r.m_ptr;
					setEntryAndReadEntryParent(sp.getWidgetPtr());

					 
					return this;
				}

#endif

				// ?!? todo
				/*! \brief operator == for checking equality between event source and this
				\param rhs event source to compare with
				\return true in case of equality, false otherwise
				\throw no-throw */
				virtual bool operator == (const basar::appl::IEventSource& rhs) const
				{
					// same class / type ?
					if ( (*(reinterpret_cast<const int *>(&(rhs)))) != *(reinterpret_cast<const int *>(this)) )		// compare vtable address
						return false;

					// same object ?
					return m_ptr == static_cast<const ViewConnPtr&>(rhs).m_ptr;
				}

				/*! \brief release all resources of view connector
				\throw no-throw */
				void reset						()			
				{
					m_ptr.reset(); 
					setEntry(0);
				}

				/*! \brief get instance of this to call view connectors functions (show, init, shutdown, hide, ...)
				\return shared_ptr to this
				\throw no-throw */
				boost::shared_ptr<T> operator->	()			
				{ 
					return getInstance(); 
				}



				/*! \brief get instance without creating new instance if there was none.
				\return internal shared_ptr (w/o creating new instance if there is none)
				\throw no-throw */
				boost::shared_ptr<T> get		() const    
				{ 
					resetSPifEntryNull();
					return m_ptr; 
				}

				/*! \brief is viewconnptr-instance already available / created?
				\return true if instance exists, false otherwise
				\throw no-throw */
				operator bool					() const    
				{ 
					resetSPifEntryNull();
					return (m_ptr.get() != 0); 
				}

				/*! \brief check if shared ptr to instance is already created
				\return false if instance exists, true otherwise
				\throw no-throw */
				bool operator!					() const    
				{ 
					resetSPifEntryNull();
					return (m_ptr.get() == 0); 
				}	

				/*! \brief delegated to shared ptr's unique()
				\return true if internal shared_ptr is unique, false otherwise
				\throw no-throw */
				bool unique						() const    
				{
					resetSPifEntryNull();
					return m_ptr.unique(); 
				}	

				/*! \brief delegated to shared ptr's use_count()
				\return Int32 representing internal shared_ptr's use_count()
				\throw no-throw */
				basar::Long32 use_count			() const    
				{
					resetSPifEntryNull();
					return m_ptr.use_count(); 
				}	

			private:

				/*!
				* initializes
				*/
				void init()
				{
					prepairForNewEntry();
					T * w = new T(getParent()) ;
					SPDeleterForQObjects<QWidget> a(w );
					m_ptr   = boost::shared_ptr<T >( w , a); 
					setEntry(m_ptr.get());	  
				}

				// const is required to be called by const methods
				// resets shared_ptr is object pointed to has already
				// been deleted. can't reset ptr during delete of object
				// anymore as this would lead to a double delete
				inline void resetSPifEntryNull() const
				{
					if (getEntry().isNull())
					{
						m_ptr.reset();
					}
				}

				// remove entry by resetting shared_ptr
				// in case entry hasn't been deleted yet
				void cleanup()
				{
					if (0 != m_ptr.get())
					{
						if (!getEntry().isNull())
						{
							m_ptr.reset();
						}
					}
				}

				/*!
				* called when the parent is destroyed
				*/
				virtual void onDestroyParent()
				{
					WidgetPointer::onDestroyParent();
				}

				/*!
				* called when the object is destroyed,
				* the shared_ptr is not reset as this would lead 
				* to a double-delete (and then a nullptr access)
				* thus the shared_ptr still thinks it's pointing to valid memory 
				*/
				virtual void onDestroyEntry()
				{
					WidgetPointer::onDestroyEntry();
				}

				/*!
				* called when the objects older sibling
				* is destroyed. the parentage is changed
				* and the object is set to hidden 
				*/
				virtual void onDestroySibling()
				{
					if (0 != getEntry())
					{
						getEntry()->hide();
						setParent(0); 
					}
					WidgetPointer::onDestroySibling();
				}

				/*! \brief create new instance (if there was none) and return shared ptr to view connector
				\return shared_ptr to this
				\throw no-throw */
				boost::shared_ptr<T> getInstance()
				{
					resetSPifEntryNull();
					if (!m_ptr)
					{
						init();
					}	
					return m_ptr;
				}
				mutable boost::shared_ptr<T> m_ptr;	//!< contained shared_ptr
			};

			//----------------------------------------------------------------------------
			/*! \brief Template for checking equality of two ViewConnPtr's
			\param a first ptr which is checked
			\param b second ptr which is checked
			\return true in case of equality, false otherwise
			\throw no-throw*/
			template<class T, class U> inline bool operator==(ViewConnPtr<T> const & a, ViewConnPtr<U> const & b)
			{
				return a.get() == b.get();
			}

			//----------------------------------------------------------------------------
			/*! \brief Template for checking unequality of two ViewConnPtr's
			\param a first ptr which is checked
			\param b second ptr which is checked
			\return false in case of inequality, true otherwise
			\throw no-throw */
			template<class T, class U> inline bool operator!=(ViewConnPtr<T> const & a, ViewConnPtr<U> const & b)
			{
				return a.get() != b.get();
			}

			//----------------------------------------------------------------------------
		}	// namespace tie
	}	// namespace gui
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD
