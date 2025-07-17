//-------------------------------------------------------------------------------------------------//
//includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "contrvouchernoacknowledgement.h"
#pragma warning (push) // to avoid warnings from QT always put QT-includes between push - pop
#pragma warning(disable: 4127 4244 4251 4311 4312 4481 4512 4800)

#include "contributionvouchervc.h"
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

//-------------------------------------------------------------------------------------------------//
ContrVoucherNoAcknowledgement::ContrVoucherNoAcknowledgement( ContributionVoucherVC *parent)
: QObject( parent ), m_dialog( *parent )
{
}

//-------------------------------------------------------------------------------------------------//
ContrVoucherNoAcknowledgement::~ContrVoucherNoAcknowledgement( void )
{
}

//-------------------------------------------------------------------------------------------------//
bool ContrVoucherNoAcknowledgement::eventFilter( QObject *obj, QEvent *event )
{
	bool result = false;
			
	switch( event->type() )
	{
		case QEvent::KeyPress:
			result = keyPressEvent(obj, static_cast<QKeyEvent*>(event));
			break;
	}
	return(result);
}

//-------------------------------------------------------------------------------------------------//
bool ContrVoucherNoAcknowledgement::keyPressEvent( QObject *obj, QKeyEvent *keyEvent )
{
	bool result = false;
	obj;

	switch(keyEvent->key())
	{
		case Qt::Key_Enter:
		case Qt::Key_Return:
			
			result = true;
			break;
		default:			
			break;
	}
	return(result);
}

} //namespace viewConn
} //namespace narcotics