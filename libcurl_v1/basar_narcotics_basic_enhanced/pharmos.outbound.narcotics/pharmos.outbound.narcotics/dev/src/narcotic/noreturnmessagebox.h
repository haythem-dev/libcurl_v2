//-----------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  simple ui for scanning returned contribution vouchers
 *  \author Marco Köppendörfer
 *  \date   08.01.2013
 */
//-----------------------------------------------------------------------------------------------//
#ifndef GUARD_NARCOTICS_NORETURNMESSAGEBOX_H
#define GUARD_NARCOTICS_NORETURNMESSAGEBOX_H

//-----------------------------------------------------------------------------------------------//
// includes
//-----------------------------------------------------------------------------------------------//
#pragma warning (push)
#pragma warning(disable : 4127 4311 4312 4481 4512 4800 4244)
#include <QtWidgets/QMessageBox>
#include <QtCore/QTimer>
#pragma warning (pop)

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//

namespace narcotics
{
namespace viewConn
{

	class QNoReturnMessageBox : public QMessageBox
	{
		Q_OBJECT
	public:
		QNoReturnMessageBox(QWidget* parent = 0);
		QNoReturnMessageBox(Icon icon, const QString& title, const QString& text, StandardButtons buttons = NoButton, QWidget* parent = 0, Qt::WindowFlags f = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
		virtual ~QNoReturnMessageBox();

	public slots:
		virtual void setVisible(bool visible);
		void on_timer_timeout();

	protected:
		virtual bool eventFilter(QObject* /*obj*/, QEvent* event);
	private:
		QTimer	m_Timer;
		QColor	m_OriginalBackground;
	};

} //namespace viewConn
} //namespace narcotics

#endif //GUARD_NARCOTICS_NORETURNMESSAGEBOX_H