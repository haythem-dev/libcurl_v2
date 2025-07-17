// not working as BASAR isn't able to handle Threads
//--------------------------------------------------------------------------------------------//
/*! \file	printthread.h
 *  \brief  Header for PrintThread class
 *  \author Benjamin Endlicher
 *  \date   20.09.2010
 */
//--------------------------------------------------------------------------------------------//
                                         
#ifndef  GUARD_PRINTTHREAD_H 
#define  GUARD_PRINTTHREAD_H 

//--------------------------------------------------------------------------------------------//
// include section
//--------------------------------------------------------------------------------------------//
#include <libbasardbaspect.h>
#include <libbasarappl.h>

#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include "connection.h"
#include "branchmodule.h"
#include "contributionvouchermodule.h"
#include "narcoticsmodule.h"
#include "numberassignmentdm/numberassignmentdm.h"
#include <QtCore/QThread>
#include <QtCore/QMutex>
#pragma warning (pop)

class QTimer;

// -------------------------------------------------------------------------
namespace narcotics
{
    //struct for initalization params
    struct PrintThreadInitParams
    {
		basar::VarString printerName;
		basar::VarString startDatetxt;
		basar::VarString directoryname;
		basar::VarString internalaccounts;
        basar::VarString hostname;
		basar::VarString user;
		basar::VarString pw;
		basar::VarString localTmpDirectory;
        basar::VarString narcoticsUserName;
        basar::VarString narcoticsUserForeName;
        basar::VarString narcoticsUserSurName;
        basar::Int16	 branchNo;
    };

    //-------------------------------------------------------------------------------------------------//
    // class declaration
    //-------------------------------------------------------------------------------------------------//
    class PrintThread : public QThread
     {      
         Q_OBJECT

	public: 
        /*!
			\brief Constructor.
            \param no param
		*/
		PrintThread();

        /*!
			\brief Destructor.
		*/
		~PrintThread();

		void init( PrintThreadInitParams& params );

        /*! \brief	virtual function which is called with thread->start()
		    \throw  no-throw */
         void run();

	signals:
		 void heartBeat();
         void userInteractionRequired();
         void errorsResolved();
		 void terminatePrintThread();
	public slots:
		void on_timer_timeout();
		void on_termination_requested();

	private: 
		
        void printContributionVoucher();
		
        int printContributionVoucherEx( basar::db::aspect::AccessorPropertyTableRef orderToPrint );
		
        void printForms( basar::db::aspect::AccessorPropertyTableRef insertedBookings,
            basar::db::aspect::AccessorPropertyTable_YIterator yitBranch, 
			basar::db::aspect::AccessorPropertyTable_YIterator yitCustomer, 
			basar::Int32 startPosition, 
			basar::Int32 endPosition );

		basar::VarString PrintThread::generateFileName( const basar::Int32 bgano, const basar::Int32 contributionvoucherno );

		void setPrinted( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking, basar::Int32 contributionVoucherNo );

        void setNotPrintable( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking );

		void setInPrint( basar::db::aspect::AccessorPropertyTable_YIterator currentBooking );

        bool userStillHasPrintLock();

		 Connection											m_Connection;			 //!< connection member for thread

         // Member variable
		 const basar::ULong32								TIMEOUT;				 //!< member declares waiting time for threads during print processes

        /*! \brief copy constructor \n no-throw */
	    PrintThread( const PrintThread &r );
	    /*! \brief assign operator \n no-throw */
	    PrintThread & operator = ( const PrintThread &r );

		domMod::BranchDM					m_BranchDM;				 //!< domain module class for branch
		
		domMod::ContributionVoucherDM		m_ContributionVoucherDM; //!< domain module class for contribution vouchers
		
		domMod::NumberAssignmentDM			m_NumberAssignmentDM;

		basar::db::aspect::AccessorPropertyTable_YIterator m_YitSignature;
		
		basar::db::aspect::AccessorPropertyTable_YIterator m_YitSearchSerialOutput;

		basar::Int16		m_BranchNo;

		basar::VarString    m_NarcoticsUserName;
		
		basar::Int32		m_InternalAccounts;
		
        basar::Int32        m_Today;

		basar::VarString	m_PrinterName;
		
		basar::VarString	m_ContributionVoucherDir;
		
		basar::VarString	m_StartDate;
		
		basar::VarString	m_LocalTmpDirectory;
		
		basar::VarString	m_FTPhost;
		
		basar::VarString	m_FTPUser;
		
		basar::VarString	m_FTPPW;
		
		QTimer*				m_Timer;
		
		QMutex				m_TerminationMutex;

		bool				m_TerminationRequested;

        bool                m_UserInteractionRequiredEmitted;
     }; 
}

#endif //GUARD_PRINTTHREAD_H