//----------------------------------------------------------------------------
/*! \file
 *  \brief  
 *  \author Marco Köppendörfer
 *  \date   06.03.2014
 */
//----------------------------------------------------------------------------
#include <UnitTest++/UnitTest++.h>

#include "libbasarcmnutil.h"
#include "libbasarqtwidget.h"



SUITE(TestTablewidget)
{
    TEST(TableWidgetFlags)
    {
        BasarTableWidget* tw = new BasarTableWidget(NULL);
        
        tw->setColumnCount(1);
        
        tw->insertRow(0);

        tw->setItem(0, 0, new QTableWidgetItem());
        
        tw->setFlagsRow(0, Qt::ItemIsSelectable);
        
        QTableWidgetItem* i = tw->item(0, 0);
        
        CHECK_EQUAL(i->flags(), Qt::ItemIsSelectable);
        
        delete tw;
    }

}