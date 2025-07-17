#ifndef UTDBSQL_SINGLETON_H
#define UTDBSQL_SINGLETON_H

class SingleBase
{
public:
					SingleBase	();
	void 			init		();					

	int				m_Int;
	
private:
					SingleBase	(const SingleBase &);	// forbidden
	SingleBase &	operator =	(const SingleBase &);	// forbidden
};

//------------------------------------------------------------------------------
class SingleDerived : public basar::cmnutil::Singleton<SingleBase>
{
private:
						SingleDerived   ();							//!< forbidden
						SingleDerived   (const SingleDerived &);	//!< forbidden
	SingleDerived &		operator =		(const SingleDerived &);	//!< forbidden
};

#endif

