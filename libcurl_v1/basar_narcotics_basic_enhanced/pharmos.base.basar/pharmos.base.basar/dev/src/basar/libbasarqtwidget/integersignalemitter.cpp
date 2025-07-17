/*
* @file                                  
* @author Anke Klink                      
* @date 2008                            
*/

#ifdef _WIN32
#pragma warning (push)
#pragma warning(disable: 4512) // qobject_impl.h - C4512: 'QtPrivate::QSlotObjectBase' : assignment operator could not be generated
#endif
#include "libbasarqtwidget_integersignalemitter.h"
#ifdef _WIN32
#pragma warning (pop)
#endif

/*!
* Constructor of a IntegerSignalEmitter, the IntegerSignalEmitter
* Object emits the signal clicked(int) with the internally
* stored number, whenever the slot click is called
*/
IntegerSignalEmitter::IntegerSignalEmitter(
        int number,     //!< the number, that is used in the signal clicked
        QObject *parent //!< parent object
        )
: QObject(parent), m_number(number)
{
}

IntegerSignalEmitter::~IntegerSignalEmitter()
{
}

/*!
 *
 * sets the internal number
 */
void IntegerSignalEmitter::setNumber(int n)
{
    m_number = n;
}

/*!
 *
 * returns the internal number
 * \return internal number
 */
int IntegerSignalEmitter::getNumber()
{
    return m_number;
}

/*!
 *
 * slot, that calls the signal clicked
 */
void IntegerSignalEmitter::click()
{
    clicked(m_number);
}
