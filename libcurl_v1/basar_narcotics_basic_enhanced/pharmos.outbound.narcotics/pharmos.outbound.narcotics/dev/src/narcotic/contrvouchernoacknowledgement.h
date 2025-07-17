#ifndef CONTRVOUCHERNOACKNOWLEDGEMENT_H_GUARD
#define CONTRVOUCHERNOACKNOWLEDGEMENT_H_GUARD

class QKeyEvent;
class QFocusEvent;

namespace narcotics {
namespace viewConn {

class ContributionVoucherVC;

class ContrVoucherNoAcknowledgement : public QObject
{
public:
	ContrVoucherNoAcknowledgement( ContributionVoucherVC *parent=0 );
	virtual ~ContrVoucherNoAcknowledgement( void );
	
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

#endif //CONTRVOUCHERNOACKNOWLEDGEMENT_H_GUARD