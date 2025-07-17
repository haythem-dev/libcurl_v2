/** $Id$                                          
*                                                  
* @file                                  
* @author Anke Klink                      
* @date 2008                            
*/
#ifndef INTEGERSIGNALEMITTER_H
#define INTEGERSIGNALEMITTER_H

//--------------------------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------------------------//
#include <QtDesigner/QDesignerExportWidget>

#pragma warning (push)
#pragma warning(disable: 4127)
#include <QtCore/QObject>
#pragma warning (pop)

/*!
* The IntegerSignalEmitter
* Object emits the signal clicked(int) with the internally
* stored number, whenever the slot click is called. this is a
* Helperclass that translates an empty signal to a signal 
* holding a stored Integer Value
*/
class QDESIGNER_WIDGET_EXPORT IntegerSignalEmitter : public QObject
{
    Q_OBJECT

public:

    //! \brief Constructor that holds the number, that is used in the signal clicked
    IntegerSignalEmitter(
        int number,     //!< the number, that is used in the signal clicked
        QObject *parent //!< parent object
        );

    //! Destructor
    virtual ~IntegerSignalEmitter();

    //! returns the internal Number
    int getNumber();

signals:
    /*! signal, that is emitted, when the slot clicked is called, 
    *
    * the value is the member m_number
    */
    void clicked(
        int number //!< the number given in the constructor
        );

public slots:

    //! calls the signal clicked
    void click();

    //! changes the internal number
    void setNumber(int);

private:

    //! the number that is used in the signal clicked
    int m_number;

};

#endif // INTEGERSIGNALEMITTER_H
