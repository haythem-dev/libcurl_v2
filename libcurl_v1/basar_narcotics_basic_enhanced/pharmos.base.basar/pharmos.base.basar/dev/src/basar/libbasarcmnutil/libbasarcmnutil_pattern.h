//----------------------------------------------------------------------------
/*! \file
 *  \brief  pattern class definitions
 *  \author Thomas Hörath
 *  \date   15.07.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_PATTERN_H
#define GUARD_LIBBASARCMNUTIL_PATTERN_H

#ifdef _WIN32
#pragma warning(disable: 4661)
#endif

//----------------------------------------------------------------------------
namespace basar   {
namespace cmnutil {

/*! \brief Template to create a singleton class. */
template <class T> class Singleton
{
private:
	//------------------------------------------------------------------------
	/*! \brief Class to work with the singleton instance.*/
	class InstancePtr
    {
    public:
				
		/*! \brief Constructor of singleton instance. */
		InstancePtr() : m_ptr(0)	{}

		/*! \brief Destructor of singleton instance. */
		~InstancePtr()	{ delPtr();	}
		
		/*! \brief  returning the singleton instance. 
			\return singleton instance */
		T * getPtr()	{ return m_ptr; }

		/*! \brief registrating the singleton instance. 
			\param pT points to object wrapped by singleton */
		void setPtr(T * pT)
		{ 
			if (pT != 0)
			{	  
				delete m_ptr;
				m_ptr = pT;
			}
		}

		/*! \brief clearing the singleton instance. */
		void delPtr()
		{
			delete m_ptr;
			m_ptr = 0;
		}

	private:
        T *	m_ptr;	//!< points to object wrapped by singleton.
    };

	//------------------------------------------------------------------------
    /*! \brief construction-on-first-use
        \verbatim
		method must be specialised/implemented in cpp file:

		namespace basar
		{
			namespace cmnutil 
			{
				using $namespace::myclass$;
				
				template <>
				typename Singleton< $myclass$ >::InstancePtr & 
					Singleton< $myclass$ >::inst()
				{
					// the one and only instance
					static InstancePtr s_Inst;  

					return s_Inst;
				}
			}
		}
		\endverbatim
	*/
	//! \return shared_ptr pointing to instance
    static InstancePtr & inst();

public:   
	/*! \brief returning the singleton instance of template class. 
		\return singleton instance */
	static T & getInstance()	
    {
        if (inst().getPtr() == 0)
            inst().setPtr(new T());

        return *(inst().getPtr());
    }

	/*! \brief clearing the singleton instance of template class. */
	static void clear()	
    {
        inst().delPtr();
    }
};

//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

#endif	// GUARD
