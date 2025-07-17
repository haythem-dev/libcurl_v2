/*
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    

#ifndef  GUARD_BASAR_LIBBASARQTWIDGET_WIDGETPOINTER_H__ 
#define  GUARD_BASAR_LIBBASARQTWIDGET_WIDGETPOINTER_H__ 

//------------------------------------------------------------------------------------------------------------//
#ifndef LIBBASARQTWIDGET_API
	#ifdef _WIN32
		#ifdef LIBBASARQTWIDGET_EXPORTS
			#define LIBBASARQTWIDGET_API __declspec(dllexport)	//!< dll exported
		#else
			#define LIBBASARQTWIDGET_API __declspec(dllimport)	//!< dll imported
		#endif 
	#else 
		#define LIBBASARQTWIDGET_API
	#endif
#endif

//----------------------------------------------------------------------------

#pragma warning (push)
#pragma warning(disable: 4127 4231 4512 4800)
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtWidgets/QWidget>
#pragma warning (pop)

#include "libbasar_definitions.h"

//----------------------------------------------------------------------------
namespace basar {
	namespace gui   {
		namespace widget   {

			/*!
			* This class summerises a widget with its parent 
			* and a helper object that functions as an older
			* sibling. The class is used to make it possible to
			* execute 'last orders' when the objects are deleted.
			* As the signal destroyed(QObject *) on a QWidget
			* is called after its children are destroyed, no (!)
			* rescue mission on the Entry(i.e. remove from Parent, in 
			* oreder to keep the entry) can be done after signal
			* destroyed is called on the parent. If the Helper
			* Object remains an older sibling, this object will be
			* destroyed before the entry is destroyed 
			* and a rescue missions on the entry
			* can be done. Caution: The sequence of cildren can be 
			* changed by the functions QWidget::raise(), QWidget::lower().
			* If those functions are used, the sibling object might be
			* deleted after the entry object, therefor one schould not
			* rely solely on this 'older siblings mechanismn'.
			* The usage of an older sibling can be probited, 
			* by setting tryUseOlderSiblingMechanism to false. This is
			* the standard, use the tryUseOlderSiblingMechanism, if you
			* want to try to remove the entry from the parent, before 
			* automatic destruction by the parent.
			*
			* There are different function to set 
			* the entry and the parent values:
			* <ul> 
			* <li> The most usefull variant ist this sequence:
			*      <ul>
			*      <li> setParent(widget1);  sets the parent </li>
			*      <li> prepairForNewEntry(); creates the older sibling, if the 
			*           policy m_tryUseOlderSiblingMechanism is on </li> 
			*      <li> widget2 = new ...; creation of the entry widget </li>
			*      <li> setEntry(widget2); sets the Widget </li>
			*      </ul>
			*      Then the sibling is created before the entry is set to the 
			*      parent, and the older siblings mechanism can be used. </li>
			* <li> If the widget has already the parent value as Parent, the 
			*      older sibling mechanism can't be used, and the function
			*      setEntryAndReadEntryParent() sets both parent and entry 
			*      values </li>
			* </ul>
			*
			* The class has three virtual functions, that are called when one
			* of the inner objects is deleted. All QPointer objects are 
			* cleaned before.
			* <ul> 
			* <li> virtual void onDestroyParent() - is called within the
			*      destructor of the parent widget, all children are
			*      already gone </li>
			* <li> virtual void onDestroyEntry() - is called within the
			*      destructor of the entry widget </li>
			* <li> virtual void onDestroySibling() - is called within the
			*      destructor of the sibling object. If the Helper
			*      Object remains an older sibling, this object should be
			*      destroyed before the entry and rescue missions (s.a)
			*      can be done. Caution: The sequence of cildren can be 
			*      changed by the functions QWidget::raise(), QWidget::lower() 
			*      </li>
			* </ul>
			*/
			class  LIBBASARQTWIDGET_API WidgetPointer : public QObject
			{
				Q_OBJECT
			private:

				/*! 
				* Internally used enum to identify the objects
				*/
				enum Which
				{
					PARENT,
					ENTRY,
					SIBLING
				};

				/*!
				* This internal class/struct is necessary for the
				* dll Export, that would throw a warning if
				* the template member are added directly.
				*/
				struct InnerWidgetPointer
				{
					QPointer<QWidget> m_parent;    //!< pointer to the parent value
					QPointer<QWidget> m_entry;     //!< pointer to the entry value
					QPointer<QObject> m_sibling;   //!< pointer to the special created older sibling
				};

				/*!
				* The pointer to parent, entry and older sibling
				*/
				InnerWidgetPointer m_inner; 

				/*!
				* The older sibling mechanism can be prohibited
				*/
				bool m_tryUseOlderSiblingMechanism;

				/*!
				* Inner function to set the parent value 
				* @param w Parent object
				*/
				void setParentInner(QWidget * w);

				/*!
				* Inner function to set the parent value
				* on the entry value
				* @param connectEntry - needs to be connected
				*/
				void setParentToEntryInner(bool connectEntry);

				/*!
				* connects the destroyed(QObject *) signal
				* to the appropriate slots 
				* @param w which Object is destroyed
				*/
				void connectDestroyed(Which w);

				/*!
				* disconnects the destroyed(QObject *) signal
				* to the appropriate slots 
				* @param w which Object is destroyed
				*/
				void disconnectDestroyed(Which w);

				/*!
				* copies all relevantr connections
				*/
				void reconnect( );

			public:

				/*!
				* constructor sets the tryUseOlderSiblingMechanism Policy
				* @param tryUseOlderSiblingMechanism usind older siblings Mechanism
				*/
				WidgetPointer(bool tryUseOlderSiblingMechanism = false);


				/*!
				* copyconstructor, necessary for dll export
				*/
				WidgetPointer(const WidgetPointer &);

				/*!
				* virtual destructor
				*/
				virtual ~WidgetPointer();

				/*!
				* copyoperator, necessary for dll export
				* \return WidgetPointer which was assigned
				*/
				WidgetPointer & operator=(const WidgetPointer &);

				/*! 
				* sets the parent value:
				* most usefull used in this sequence:
				* <ul>
				* <li> setParent(widget1);  sets the parent </li>
				* <li> prepairForNewEntry(); creates the older sibling, if the 
				*      policy m_tryUseOlderSiblingMechanism is on </li> 
				* <li> widget2 = new ...; creation of the entry widget </li>
				* <li> setEntry(widget2); sets the Widget </li>
				* </ul>
				* Then the sibling is created before the entry is set to the 
				* parent, and the older siblings mechanism can be used.
				*/
				void setParent(QWidget *);

				/*! 
				* sets the entry value:
				*  most usefull used in this sequence:
				* <ul>
				* <li> setParent(widget1);  sets the parent </li>
				* <li> prepairForNewEntry(); creates the older sibling, if the 
				*      policy m_tryUseOlderSiblingMechanism is on </li> 
				* <li> widget2 = new ...; creation of the entry widget </li>
				* <li> setEntry(widget2); sets the Widget </li>
				* </ul>
				* Then the sibling is created before the entry is set to the 
				* parent, and the older siblings mechanism can be used.
				*/
				void setEntry(QPointer<QWidget> );

				/*! 
				*  creates the older sibling, if the policy 
				*  m_tryUseOlderSiblingMechanism is on 
				*  most usefull used in this sequence:
				* <ul>
				* <li> setParent(widget1);  sets the parent </li>
				* <li> prepairForNewEntry(); creates the older sibling, if the 
				*      policy m_tryUseOlderSiblingMechanism is on </li> 
				* <li> widget2 = new ...; creation of the entry widget </li>
				* <li> setEntry(widget2); sets the Widget </li>
				* </ul>
				* Then the sibling is created before the entry is set to the 
				* parent, and the older siblings mechanism can be used.
				*/
				void prepairForNewEntry();

				/*!
				* second variant to set Parent anD Widget, the 
				* older siblings policy can't be used here. The 
				* widget is set as entry value and its parent as 
				* parent value
				*/
				void setEntryAndReadEntryParent(QWidget *);

				/*!
				* virtual method called within the
				* destructor of the parent widget, all children are
				* already gone 
				*/
				virtual void onDestroyParent();

				/*!
				* virtual method called within the  
				* destructor of the entry widget  
				*/
				virtual void onDestroyEntry();

				/*!
				* virtual method called within the
				* destructor of the sibling object. If the Helper
				* Object remains an older sibling, this object should be
				* destroyed before the entry and rescue missions (s.a)
				* can be done. Caution: The sequence of cildren can be 
				* changed by the functions QWidget::raise(), QWidget::lower()
				*/
				virtual void onDestroySibling();


				/*!
				* returns the set parent value
				* \return the set parent value
				*/
				QPointer<QWidget> getParent() const;

				/*!
				* returns the entry value
				* \return the entry value
				*/
				QPointer<QWidget> getEntry() const;

				/*!
				* returns the information as string,
				* used in log messages
				* \return VarString containing a textual representation of WidgetPointer
				*/
				basar::VarString toString() const;

			public slots:

				/*!
				* slot that is connected to the signal 
				* destroy( QObject * obj    ) on the
				* parent
				* @param obj Destroyed object
				*/
				void onDestroyParentSlot( QObject * obj    );

				/*!
				* slot that is connected to the signal 
				* destroy( QObject * obj    ) on the
				* entry
				* @param obj Destroyed object
				*/
				void onDestroyEntrySlot( QObject * obj    );

				/*!
				* slot that is connected to the signal 
				* destroy( QObject * obj    ) on the
				* siblings object 
				* @param obj Destroyed object
				*/
				void onDestroySiblingSlot( QObject * obj    );

			};
		}
	}
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
