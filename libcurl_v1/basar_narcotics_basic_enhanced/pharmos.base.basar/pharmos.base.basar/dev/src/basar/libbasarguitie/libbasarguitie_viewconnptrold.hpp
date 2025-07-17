//----------------------------------------------------------------------------
/*! \file 
 *  \brief  template for view connector pointers
 *  \author Thomas Hörath
 *  \date   22.08.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARGUITIE_VIEWCONNPTROLDERVERSION_H
#define GUARD_LIBBASARGUITIE_VIEWCONNPTROLDERVERSION_H

//----------------------------------------------------------------------------
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

//----------------------------------------------------------------------------
namespace basar {
namespace appl {
	class IEventSource;	//!< basis class of all view connectors, so that event handling is possible
}}

//----------------------------------------------------------------------------
namespace basar {
namespace gui	{
namespace tie	{

//----------------------------------------------------------------------------
/*! \brief Template to create a view connector, which is handled dynamically 
			(created at first access of operator '->') 
			and can be source of a gui event. 
	\throw no-throw*/
template<class T> class ViewConnPtrOlderVersion : public basar::appl::IEventSource
{
public:
    typedef T	element_type;	//!< typedef for VC
    typedef T	value_type;		//!< typedef for VC
    typedef T * pointer;		//!< typedef for ptr to VC

    /*! \brief Constructor of singleton instance. 
		\throw no-throw */
	ViewConnPtrOlderVersion(): parent(0), px()		{ }

	/*! \brief set parent of view connector if there should be one (default 0)
		\param parent Qt father to set for this
		\throw no-throw */
	void setParent(QWidget* parent)		{ this->parent = parent; }

    /*! \brief copy constructor
		\param r viewconnptr to set to this
		\throw no-throw */
	template<class Y>
		ViewConnPtrOlderVersion(ViewConnPtrOlderVersion<Y> const & r): parent(r.parent), px(r.px) { }


    /*! \brief constructor for sending QWidget
		\param sp shared ptr to set to this
		\throw no-throw */
	ViewConnPtrOlderVersion(boost::shared_ptr<T> sp): parent(static_cast<QWidget*>(sp->parent())), px(sp)   { }

	/*! \brief return pointer to QWidget out of ViewConnPtrOlderVersion (e.g. for setting as parent for next sub-dialog)
		\throw no-throw */
	QWidget* getWidgetPtr() const		{	return get().get();	}


#if !defined(BOOST_MSVC) || (BOOST_MSVC > 1200)

	/*! \brief operator = , only needed in special cases
		\param r viewconnptr to assign to this
		\throw no-throw */
	template<class Y>
		ViewConnPtrOlderVersion& operator=(ViewConnPtrOlderVersion<Y> const & r) 
    {
        px = r.px;
        pn = r.pn; 
		return *this;
    }

#endif

	/*! \brief operator == for checking equality between event source and this
		\param rhs event source to compare with
		\throw no-throw */
	virtual bool operator == (const basar::appl::IEventSource& rhs) const
	{
		// same class / type ?
		if ( (*(reinterpret_cast<const int *>(&(rhs)))) != *(reinterpret_cast<const int *>(this)) )		// compare vtable address
			return false;

		// same object ?
		return px == static_cast<const ViewConnPtrOlderVersion&>(rhs).px;
	}

	/*! \brief release all resources of view connector
		\throw no-throw */
	void reset						()			{ px.reset(); }

	/*! \brief get instance of this to call view connectors functions (show, init, shutdown, hide, ...)
		\throw no-throw */
	boost::shared_ptr<T> operator->	()			{ return getInstance(); }
    
    /*! \brief get instance without creating new instance if there was none.
		\throw no-throw */
	boost::shared_ptr<T> get		() const    { return px; }

    /*! \brief is viewconnptr-instance already available / created?
		\throw no-throw */
	operator bool					() const    { return static_cast <bool> (px); }

    /*! \brief check if shared ptr to instance is already created
		\throw no-throw */
	bool operator!					() const    { return px == 0; }	

    /*! \brief delegated to shared ptr´s unique()
		\throw no-throw */
	bool unique						() const    { return px.unique(); }	

    /*! \brief delegated to shared ptr´s use_count()
		\throw no-throw */
	basar::Long32 use_count			() const    { return px.use_count(); }	

private:
	/*! \brief create new instance (if there was none) and return shared ptr to view connector
		\throw no-throw */
	boost::shared_ptr<T> getInstance()
	{
		if (!px)
		{
			px = boost::shared_ptr<T>(new T(parent));
		}	
		return px;
	}

	QWidget* parent;			//!< parent of view connectors widget
	boost::shared_ptr<T> px;	//!< contained shared_ptr
};

//----------------------------------------------------------------------------
/*! \brief Template for checking equality of two ViewConnPtrOlderVersion´s
	\param a first ptr which is checked
	\param b second ptr which is checked
	\throw no-throw*/
template<class T, class U> inline bool operator==(ViewConnPtrOlderVersion<T> const & a, ViewConnPtrOlderVersion<U> const & b)
{
    return a.get() == b.get();
}

//----------------------------------------------------------------------------
/*! \brief Template for checking unequality of two ViewConnPtrOlderVersion´s
	\param a first ptr which is checked
	\param b second ptr which is checked
	\throw no-throw */
template<class T, class U> inline bool operator!=(ViewConnPtrOlderVersion<T> const & a, ViewConnPtrOlderVersion<U> const & b)
{
    return a.get() != b.get();
}

//----------------------------------------------------------------------------
}	// namespace tie
}	// namespace gui
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD
