/*
* @file                                            
* @author Anke Klink                      
* @date 2008                            
*/    

#include <UnitTest++/UnitTest++.h>
#include "libbasarguitie.h"


#pragma warning (push)
#pragma warning(disable: 4481)
#include "tablewidgetmatcherholder.h"
#pragma warning (pop)


#include "selects.h"
#include "libbasarqtwidget_tablewidget.h"

#define TEXTOUTPUT(XXX) ;
#define TEXTOUTPUT2(XXX) ;
//#define TEXTOUTPUT(XXX) std::cout << XXX << std::endl;

namespace basar
{
	namespace test
	{
		namespace unit
		{
			namespace guitie
			{
				// ------------------------------------------------------------------------------
				TableWidgetMatcherHolder::TableWidgetMatcherHolder():
			m_guiColCountNorm(1),
				m_guiColCountMore(2),
				m_accRowCount(NUMBER_OF_SELECT_MORE_LINES),
				m_rowfactor(2),
				m_tableWidgetKind(Q)
			{ 
				m_guiRowCountLess = m_accRowCount - m_rowfactor;
				m_guiRowCountMore = m_accRowCount + m_rowfactor;

				m_guiRowCount = m_guiRowCount = m_accRowCount;
				m_tableWidget = 0;
				m_guiColCount = 0;
				m_inverse = m_init = false;
			}
			// -------------------------------------------------------------------------------
			TableWidgetMatcherHolder::~TableWidgetMatcherHolder()
			{
			}

			// -------------------------------------------------------------------------------
			TableWidgetMatcherHolder::StringListNamesEnum TableWidgetMatcherHolder::getStringListNamesEnum() const
			{
				StringListNamesEnum retval = NORMNAMES;
				if (m_inverse)
				{
					if (hasMoreGuiNamesTest())
					{
						retval = MORENAMES_REVERSE;
					}
					else
					{
						retval = NORMNAMES_REVERSE;
					}
				}
				else if (hasMoreGuiNamesTest())
				{
					retval = MORENAMES;
				}
				return retval;
			}
			// -------------------------------------------------------------------------------

			bool TableWidgetMatcherHolder::initTableWidget()
			{
				bool retval = true;
				if (BASAR == m_tableWidgetKind)
				{
					m_tableWidget = new BasarTableWidget(0);  
				}
				else
				{
					m_tableWidget = new QTableWidget(0);  
				}
				if (m_widget != 0)
				{
					delete (m_widget);
				}
				m_widget = m_tableWidget;  
				// init rows and cols:
				{
					m_guiColCount = m_guiColCountNorm; 
					if (hasMoreGuiNamesTest())
					{
                      m_guiColCount = m_guiColCountMore; 
					}
					if (hasMoreGuiRowsTest())
					{
						m_guiRowCount = m_guiRowCountMore;
					}
					else if (hasMoreAccRowsTest())
					{
						m_guiRowCount = m_guiRowCountLess;
					}
					m_tableWidget->setRowCount(m_guiRowCount);
					m_tableWidget->setColumnCount(m_guiColCount);
					m_tableWidget->setHorizontalHeaderLabels( m_stringLists[getStringListNamesEnum()]);
				}
				return retval;
			}
			// -------------------------------------------------------------------------------

			void TableWidgetMatcherHolder::setStringListNames(const QStringList & norm,const  QStringList & added)
			{
				m_stringLists[NORMNAMES] = norm;
				QStringList more = norm;
				more << added;
				m_stringLists[MORENAMES] = more;
				QStringList reverse;
				QStringList::const_iterator iter;
				for (iter = norm.begin(); iter != norm.end(); ++iter)
				{
					reverse.push_front(*iter);
				}
				m_stringLists[NORMNAMES_REVERSE] = reverse;
				reverse << added;
				m_stringLists[MORENAMES_REVERSE] = more;
			    
				m_guiColCountNorm = norm.size();
				m_guiColCountMore = more.size();
				TEXTOUTPUT("TableWidgetMatcherHolder::setStringListNames" << m_guiColCountNorm << " " <<m_guiColCountMore);
			}

			// -------------------------------------------------------------------------------
			bool TableWidgetMatcherHolder::resetGuiValues()
			{
				int rowcount = m_tableWidget->rowCount();
				for (int c=0; c<m_guiColCount; ++c)
				{
					for (int r=0; r<rowcount; ++r)
					{
						ConstString text = getText(r,c);
						TEXTOUTPUT2( "TW(" << r << "," << c << ") = " << text);
						QTableWidgetItem * item = new QTableWidgetItem( text);
						m_tableWidget->setItem(r,c, item );
					}
				}
				return true;
			}

			// -------------------------------------------------------------------------------
			bool TableWidgetMatcherHolder::init(bool inverse ) 
			{
				bool retval  = true;
				if (m_init && inverse != m_inverse)
				{
					m_init = false;   
				}
				if (!m_init )
				{
					m_inverse = inverse; 
					initTableWidget();
					retval = setAccessorInfo();
					if (retval)
					{
						retval = resetAccessorAndMatcher();
						m_init = retval; 
					}
				}
				return retval;
			}
			// -------------------------------------------------------------------------------
			bool TableWidgetMatcherHolder::start()
			{
				bool retval = true;
				if (!m_init)
				{
					retval = init(m_inverse);
				}
				if (retval)
				{
					if (m_needsReset)
					{
						retval = initTableWidget(); 
						m_needsReset = true;
					}
					resetGuiValues();
					retval = BaseMatcherHolder::start();
				}
				return retval;
			}
			// -------------------------------------------------------------------------------
			std::vector< TableWidgetMatcherHolder::TableWidgetKind> TableWidgetMatcherHolder::getTableWidgetKindVector()
			{
				std::vector<TableWidgetKind> retval;
				retval.push_back(Q);
				retval.push_back(BASAR);
				return retval;

			}

			// -------------------------------------------------------------------------------
			bool TableWidgetMatcherHolder::initTableWidgetKind(const  TableWidgetMatcherHolder::TableWidgetKind k)
			{
				m_tableWidgetKind = k;
				m_init = false;
				return init();
			}

			// -------------------------------------------------------------------------------

			}
		}
	}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
