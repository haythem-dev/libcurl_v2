//---------------------------------------------------------------------------//

#ifndef GUARD_NARCTRANSFERAPPL_H
#define GUARD_NARCTRANSFERAPPL_H

//---------------------------------------------------------------------------------------------------------//
// include section
//---------------------------------------------------------------------------------------------------------//
#include "connection.h"
#include "transferuc.h"
#include "transferdm.h"

//---------------------------------------------------------------------------------------------------------//
// namespace section
//---------------------------------------------------------------------------------------------------------//
namespace narcotics {

// forward declaration
class NarcTransferAppl;

//---------------------------------------------------------------------------//
class NarcTransferApplBase
{
	friend class useCase::TransferUC;	//!< use case classes need private access to current YIterators / propertyTables
	friend class basar::cmnutil::Singleton<NarcTransferApplBase>; //!< singleton as friend for construction!

public:
	/*! \brief destructor \n no-throw */
	virtual ~NarcTransferApplBase();

	/*! \brief main entrance, called from main()
		\n no-throw */
	basar::appl::EventReturnStruct run(basar::appl::IEventSource& rSource,
									basar::db::aspect::AccessorPropertyTable_YIterator yitFromEvSrc);

	void handleSIGINT(int sig);

	void handleSIGTERM(int sig);

private:
	//!< std constructor, protected because of singleton
	NarcTransferApplBase();	
	//!< forbidden copy constructor
	NarcTransferApplBase( const NarcTransferApplBase& rCopy);
	//!< forbidden assignment operator
	NarcTransferApplBase& operator = (const NarcTransferApplBase& rCopy);

	/*! \brief initialize members \n \throw no-throw */
	virtual bool init();

	/*! \brief de-initialize members, so that not all value members have to be held the whole time! \n \throw no-throw */
	virtual void shutdown();

	// UseCases
	useCase::TransferUC				m_TransferUC;		//!< use case 
	// DomainModules
	domMod::TransferDM				m_TransferDM;
	// Connection 
	Connection						m_Connection;
	// Branch
	basar::Int16					m_Branchno;

	bool							m_ExitGracefull;

	basar::Int32					m_Interval;
};

//---------------------------------------------------------------------------------------//
/*! \brief  derived class from NarcTransferApplBase.

	This is a final class. Singleton  
 *  \note  class is necessary for dll exporting Singleton\<NarcTransferApplBase\>  */
class NarcTransferAppl : public basar::cmnutil::Singleton<NarcTransferApplBase>
{
	friend class dummy;	//!< prevents pedantic compiler warning

private:
	/*! \brief standard constructor \n \throw no-throw */
	NarcTransferAppl();
	/*! \brief copy constructor \n \throw no-throw */
	NarcTransferAppl(const NarcTransferAppl& r);
	/*! \brief assign operator \n \throw no-throw */
	NarcTransferAppl& operator = (const NarcTransferApplBase& r);
};


} //namespace narcotics

//---------------------------------------------------------------------------------------//
/*! \ingroup DIFF_WIN_UNIX */  
//#ifdef WIN32
//using basar::cmnutil::Singleton;
////! \brief explicit template instantiation for singleton
//Singleton<narcotics::NarcTransferApplBase>::InstancePtr Singleton<narcotics::NarcTransferApplBase>::m_Instance;
//#endif // WIN32

#endif //GUARD_NARCTRANSFERAPPL_H