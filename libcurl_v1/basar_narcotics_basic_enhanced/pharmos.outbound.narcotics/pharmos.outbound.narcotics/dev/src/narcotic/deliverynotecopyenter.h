#ifndef DELIVERYNOTECOPYENTER_H_GUARD
#define DELIVERYNOTECOPYENTER_H_GUARD

class QKeyEvent;
class QFocusEvent;

namespace narcotics 
{
namespace viewConn 
{

class ContributionVoucherVC;

class DeliverynoteCopyEnter : public QObject
{
public:
	DeliverynoteCopyEnter( ContributionVoucherVC *parent=0 );
	virtual ~DeliverynoteCopyEnter( void );
	
protected:
	/*!
		\brief The default key handler.
	*/
	bool eventFilter( QObject *obj, QEvent *event );

	virtual bool keyPressEvent( QObject *obj, QKeyEvent *keyEvent );

private:
	
	ContributionVoucherVC& m_dialog;
};

} //namespace viewConn
} //namespace narcotics

#endif //DELIVERYNOTECOPYENTER_H_GUARD