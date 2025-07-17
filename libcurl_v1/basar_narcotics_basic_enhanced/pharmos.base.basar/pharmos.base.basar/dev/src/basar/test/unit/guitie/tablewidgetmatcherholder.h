/** $Id$                                          
*                                                  
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    


#ifndef  __BASAR_TEST_UNIT_GUITIE_TABLEWIDGETMATCHERHOLDER_H__ 
#define  __BASAR_TEST_UNIT_GUITIE_TABLEWIDGETMATCHERHOLDER_H__ 


#include "basematcherholder.h"

class QTableWidget;

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				/**
				* Testclass for a Matcher on a TableWidget
				*/ 
				class TableWidgetMatcherHolder : public BaseMatcherHolder
				{  

				protected:
					bool m_inverse;                   //!< inverse Test
					bool m_init;                      //!< initialized
					int m_guiRowCountLess;            //!< number of Rows (less < norm < more)
					int m_guiRowCountMore;            //!< number of Rows (less < norm < more)
					int m_guiColCountNorm;            //!< number of Cols (norm < more)
					int m_guiColCountMore;	          //!< number of Cols (norm < more)

				protected:
					int m_accRowCount;                //!< actual number of Rows in the AccessorInstance  
					int m_guiRowCount;                //!< actual number of Rows in the GuiPropertyTable
					int m_guiColCount;                //!< actual number of Cols in the GuiPropertyTable  
					int m_rowfactor;                  //!< difference between (less, norm and more)   

					/** 
					* with kind of TableWidget is to be testes
					* QTableWidget or BasarTableWidget
					*/
					enum TableWidgetKind
					{
						Q,
						BASAR
					} m_tableWidgetKind;


					/**
					 * the TableWidget
					 */
					QTableWidget  * m_tableWidget;

					/**
					 * Defines the Columns on "more and norm" column Tests for
					 * inverse and normal
					 */
					enum StringListNamesEnum
					{
						NORMNAMES,
						MORENAMES,
						NORMNAMES_REVERSE,
						MORENAMES_REVERSE
					};

					/**
					 * defines the Column names
					 */
					std::map<StringListNamesEnum, QStringList> m_stringLists;

					/**
					 * returns the used StringListNamesEnum Value
					 */
					StringListNamesEnum getStringListNamesEnum() const;
				protected:

					/**
					 * defines the Column names 
					 * @param norm - Normal Column names (the sequence 
					 * will be inverted 
					 * for (m_inverse == true)
					 * @param added - More Culumn names, that are added to the norm
					 * mames for (MoreCol tests). This sequence will not be inverted.
					 */
					void setStringListNames(const QStringList & norm, const QStringList & added);

					/**
					 * returns a test name for logging
					 */
					virtual basar::ConstString getText(int row, int col) = 0;

					/**
					 * sets the AcessorInfo
					 */
					virtual bool setAccessorInfo() = 0;	

					/**
					 * init TableWidget
					 */
					virtual bool initTableWidget();
				public:

					TableWidgetMatcherHolder();
					~TableWidgetMatcherHolder();

					/**
					 * returns a vector with all kinfs of TableWidgets
					 */
					std::vector<TableWidgetKind> getTableWidgetKindVector();

					/**
					 * initialized the object with one TableWidgetKind
					 */
					bool initTableWidgetKind(const TableWidgetKind k);

					/**
					 * resets the Gui Values
					 */
					virtual bool resetGuiValues();

					/**
					 * initializes the object
					 */
					virtual bool init(bool inverse = false) ;

					/**
					 * starts the object test
					 */
					virtual bool start();

				};
			}
		}
	}
}
#endif 

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
