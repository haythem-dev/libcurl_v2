//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "noreturnmessagebox.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace narcotics {
namespace viewConn {

//-------------------------------------------------------------------------------------------------//

QNoReturnMessageBox::QNoReturnMessageBox(QWidget* parent) : QMessageBox(parent)
{
	QPushButton* button = new QPushButton(tr("F1"), this);
	button->setShortcut(Qt::Key_F1);
	addButton(button, QMessageBox::AcceptRole);

	button->installEventFilter(this);
	m_Timer.setInterval(1000);
	connect(&m_Timer, SIGNAL(timeout()), this, SLOT(on_timer_timeout()));
	setAutoFillBackground(true);
}

//-------------------------------------------------------------------------------------------------//

QNoReturnMessageBox::QNoReturnMessageBox(Icon icon, const QString& title, const QString& text, StandardButtons buttons, QWidget* parent, Qt::WindowFlags f) :
	QMessageBox(icon, title, text, buttons, parent, f)
{
	QPushButton* button = new QPushButton(tr("F1"), this);
	button->setShortcut(Qt::Key_F1);
	addButton(button, QMessageBox::AcceptRole);

	button->installEventFilter(this);
	m_Timer.setInterval(1000);
	connect(&m_Timer, SIGNAL(timeout()), this, SLOT(on_timer_timeout()));
	setAutoFillBackground(true);
}

//-------------------------------------------------------------------------------------------------//

QNoReturnMessageBox::~QNoReturnMessageBox()
{
}

//-------------------------------------------------------------------------------------------------//

void QNoReturnMessageBox::setVisible(bool visible)
{
	if (visible == true)
	{
		QMessageBox::setVisible(visible);

		// if parent is not NULL, let the parent window flash:
		if (parentWidget() != NULL)
		{
			HWND hwnd = (HWND)parentWidget()->winId();
			FLASHWINFO flashStartParent = { sizeof(FLASHWINFO), hwnd, FLASHW_ALL | FLASHW_TIMER, 1000, 0 };
			FlashWindowEx(&flashStartParent);
		}

		QPalette palette = this->palette();
		m_OriginalBackground = palette.color(QPalette::Window);
		palette.setColor(QPalette::Window, Qt::red);
		setPalette(palette);
		m_Timer.start();
	}
	else
	{
		// if parent is not NULL, let the parent window flash:
		if (parentWidget() != NULL)
		{
			HWND hwnd = (HWND)parentWidget()->winId();
			FLASHWINFO flashStopParent = { sizeof(FLASHWINFO), hwnd, FLASHW_ALL | FLASHW_STOP, 0, 0 };
			FlashWindowEx(&flashStopParent);
		}

		m_Timer.stop();
		QMessageBox::setVisible(visible);
	}
}

//-------------------------------------------------------------------------------------------------//

void QNoReturnMessageBox::on_timer_timeout()
{
	QPalette palette = this->palette();
	if (palette.color(QPalette::Window) == m_OriginalBackground)
		palette.setColor(QPalette::Window, Qt::red);
	else
		palette.setColor(QPalette::Window, m_OriginalBackground);
	setPalette(palette);
}

//-------------------------------------------------------------------------------------------------//

bool QNoReturnMessageBox::eventFilter(QObject* /*obj*/, QEvent* event)
{
	if (event->type() != QEvent::KeyPress && event->type() != QEvent::KeyRelease)
		return false;

	QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

	switch (keyEvent->key())
	{
		case Qt::Key_Enter:
		case Qt::Key_Return:
		{
			if (event->type() != QEvent::KeyRelease)
			{
				//basar::Int32 frequency = 2000; // in Hertz
				basar::Int32 duration = 500; // in milliseconds
				Beep(1000, duration);
				Beep(3000, duration);
				Beep(1000, duration);
				Beep(3000, duration);
			}
			return true;
		}
	case Qt::Key_F1:
	case Qt::Key_Escape:
		accept();
		return true;
	default:
		break;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------//

} //namespace viewConn
} //namespace narcotics