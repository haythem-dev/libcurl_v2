/*
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    
#include "libbasarqtwidget_widgetpointer.h"
#include "libbasarcmnutil_bstring.h"

#include <sstream>


//! output for tests
#define TESTOUTPUT(X)
//#define TESTOUTPUT(X) std::cout << X << std::endl;

//----------------------------------------------------------------------------
namespace basar {
	namespace gui   {
		namespace widget   {


		// ----------------------------------------------------------------

		WidgetPointer::WidgetPointer(bool tryUseOlderSiblingMechanism):
		m_tryUseOlderSiblingMechanism(tryUseOlderSiblingMechanism)
		{

		}
		// ----------------------------------------------------------------

		WidgetPointer::~WidgetPointer()
		{
		}
		// ----------------------------------------------------------------

		WidgetPointer::WidgetPointer(const WidgetPointer & w):
			m_inner(w.m_inner),
			m_tryUseOlderSiblingMechanism(w.m_tryUseOlderSiblingMechanism)
		{
			reconnect();
		}
		// ----------------------------------------------------------------

		WidgetPointer & WidgetPointer::operator=(const WidgetPointer &w )
		{
			disconnectDestroyed(PARENT);
			disconnectDestroyed(ENTRY);
			disconnectDestroyed(SIBLING);
			m_inner = w.m_inner;
			m_tryUseOlderSiblingMechanism = w.m_tryUseOlderSiblingMechanism;
			reconnect();
			return *this;
		}
		// ----------------------------------------------------------------
		void WidgetPointer::connectDestroyed(WidgetPointer::Which w)
		{
			if (PARENT == w)
			{
				TESTOUTPUT( "   connectDestroyed " << m_inner.m_parent  << " on" << toString().c_str() );
				connect(m_inner.m_parent, SIGNAL(destroyed ( QObject*)), this,  SLOT(onDestroyParentSlot(QObject *   ))); 
			}
			else if (ENTRY == w)
			{
				TESTOUTPUT( "   connectDestroyed " << m_inner.m_entry  << " on" << toString().c_str() );
				connect(m_inner.m_entry, SIGNAL(destroyed ( QObject*)), this,  SLOT(onDestroyEntrySlot(QObject *   ))); 

			}
			else if (SIBLING == w)
			{
				TESTOUTPUT( "   connectDestroyed " << m_inner.m_sibling  << " on" << toString().c_str() );
				connect(m_inner.m_sibling, SIGNAL(destroyed ( QObject*)), this,  SLOT(onDestroySiblingSlot(QObject *   ))); 
			}
		}
		// ----------------------------------------------------------------
		void WidgetPointer::disconnectDestroyed( WidgetPointer::Which w)
		{
			if (PARENT == w)
			{
				TESTOUTPUT( "disconnectDestroyed " << m_inner.m_parent  << " on" << toString().c_str() );
				disconnect(m_inner.m_parent, SIGNAL(destroyed ( QObject*)), this,  SLOT(onDestroyParentSlot(QObject *   ))); 
			}
			else if (ENTRY == w)
			{
				TESTOUTPUT( "disconnectDestroyed " << m_inner.m_entry  << " on" << toString().c_str() );
				disconnect(m_inner.m_entry, SIGNAL(destroyed ( QObject*)), this,  SLOT(onDestroyEntrySlot(QObject *   ))); 

			}
			else if (SIBLING == w)
			{
				TESTOUTPUT( "disconnectDestroyed " << m_inner.m_sibling  << " on" << toString().c_str() );
				disconnect(m_inner.m_sibling, SIGNAL(destroyed ( QObject*)), this,  SLOT(onDestroySiblingSlot(QObject *   ))); 
			}
		}
		// ----------------------------------------------------------------
		void WidgetPointer::reconnect()
		{
			if (m_inner.m_entry)
			{
			  connectDestroyed(ENTRY);
			}
			if (m_inner.m_parent)
			{
				connectDestroyed(PARENT);
			}
			if (m_inner.m_sibling)
			{
				connectDestroyed(SIBLING);
			}
		}
		// ----------------------------------------------------------------

		void WidgetPointer::setParentInner(QWidget * w)
		{
			TESTOUTPUT( "+setParentInner " << w  << " on" << toString().c_str() );
			if (!m_inner.m_parent.isNull()) 
			{
				disconnectDestroyed(PARENT); 
				if (m_tryUseOlderSiblingMechanism)
					disconnectDestroyed(SIBLING);
			}
			m_inner.m_parent = w;
			if (!m_inner.m_parent.isNull()) 
			{
				connectDestroyed(PARENT);
			}
			TESTOUTPUT( "-setParentInner " << w  << " on" << toString().c_str() );
		}
		// ----------------------------------------------------------------

		void WidgetPointer::setParentToEntryInner(bool connectEntry)
		{
			TESTOUTPUT( "+setParentToEntryInner ---- on" << toString().c_str() );
			if (!m_inner.m_entry.isNull())
			{
				if (!m_inner.m_parent.isNull() &&  (m_tryUseOlderSiblingMechanism))
				{
					m_inner.m_sibling = new QObject(m_inner.m_parent);
					connectDestroyed(SIBLING);
				}
				m_inner.m_entry->setParent(m_inner.m_parent, m_inner.m_entry->windowFlags());
				if (connectEntry)
				{
					connectDestroyed(ENTRY);
				}
			}
			TESTOUTPUT( "-setParentToEntryInner ---- on" << toString().c_str() );
		}

		// ----------------------------------------------------------------

		void WidgetPointer::setParent(QWidget * w)
		{
			TESTOUTPUT( "+setParent " << w  << " on" << toString().c_str() );
			setParentInner(w);
			setParentToEntryInner(false);
			TESTOUTPUT( "-setParent " << w  << " on" << toString().c_str() );
		}
		// ----------------------------------------------------------------
		void WidgetPointer::setEntry(QPointer<QWidget> w)
		{
			TESTOUTPUT( "+setEntry " << w  << " on" << toString().c_str() );
			if (!m_inner.m_entry.isNull()) 
			{
				disconnectDestroyed(ENTRY); 
				if (m_tryUseOlderSiblingMechanism)
					disconnectDestroyed(SIBLING); 
			}
			m_inner.m_entry = w;
			m_inner.m_sibling = 0;
			setParentToEntryInner(true);
			TESTOUTPUT( "-setEntry " << w  << " on" << toString().c_str() );
		}
		// ----------------------------------------------------------------

		void WidgetPointer::setEntryAndReadEntryParent(QWidget * w)
		{
			TESTOUTPUT( "+setEntryAndReadEntryParent " << w  << " on" << toString().c_str() );
			if (!m_inner.m_entry.isNull()) 
			{
				disconnectDestroyed(ENTRY); 
				if (m_tryUseOlderSiblingMechanism)
					disconnectDestroyed(SIBLING); 
			}
			m_inner.m_entry = w;
			if (m_inner.m_entry.isNull()) 
			{
				setParentInner(0);
			}
			else
			{ 
				setParentInner( w->parentWidget());
				// there can not be an older sibling
				connectDestroyed(ENTRY); 
				//setParentToEntryInner(true);
			}
			TESTOUTPUT( "-setEntryAndReadEntryParent " << w  << " on" << toString().c_str() );
		}

		// ----------------------------------------------------------------

		QPointer<QWidget> WidgetPointer::getParent() const
		{
			return m_inner.m_parent;
		}
		// ----------------------------------------------------------------
		QPointer<QWidget> WidgetPointer::getEntry() const
		{
			return m_inner.m_entry;
		}
		// ----------------------------------------------------------------
		void WidgetPointer::onDestroyParent()
		{
			TESTOUTPUT( "onDestroyParent "  << " on" << toString().c_str() );
			m_inner.m_parent = 0;
		}
		// ----------------------------------------------------------------
		void WidgetPointer::onDestroyEntry()
		{
			TESTOUTPUT( "onDestroyEntry "  << " on" << toString().c_str() );
			m_inner.m_entry = 0;
		}
		// ----------------------------------------------------------------

		void WidgetPointer::onDestroySibling()
		{
			TESTOUTPUT( "onDestroySibling "  << " on" << toString().c_str() );
			m_inner.m_sibling = 0;
		}
		// ----------------------------------------------------------------
		void WidgetPointer::onDestroyParentSlot( QObject *  /* w */)
		{
			TESTOUTPUT( "onDestroyParentSlot " << w  << " on" << toString().c_str() );
			onDestroyParent();
		}
		// ----------------------------------------------------------------
		void WidgetPointer::onDestroyEntrySlot( QObject *   /* w */  ) 
		{
			TESTOUTPUT( "onDestroyEntrySlot " << w  << " on" << toString().c_str() );
			onDestroyEntry();
		}
		// ----------------------------------------------------------------
		void WidgetPointer::onDestroySiblingSlot( QObject *  /* w */   )
		{
			TESTOUTPUT( "onDestroySiblingSlot " << w  << " on" << toString().c_str() );
			onDestroySibling();
		}

		// ----------------------------------------------------------------
		basar::VarString WidgetPointer::toString() const
		{
			std::stringstream str;

			str << "WidgetPointer( " << m_inner.m_parent << " , " << m_inner.m_entry;
			if (m_tryUseOlderSiblingMechanism)
				str << " , " << m_inner.m_sibling;
			str << ")";
			basar::VarString retval(str.str());
			return retval;
		}
		// ----------------------------------------------------------------
		void WidgetPointer::prepairForNewEntry()
		{
			setEntry(0);
			if ((!m_inner.m_parent.isNull()) && m_tryUseOlderSiblingMechanism)
			{
				m_inner.m_sibling = new QObject(m_inner.m_parent);
				connectDestroyed(SIBLING);
			}
		}
		// ----------------------------------------------------------------

}	// namespace
}	// namespace
}	// namespace
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
