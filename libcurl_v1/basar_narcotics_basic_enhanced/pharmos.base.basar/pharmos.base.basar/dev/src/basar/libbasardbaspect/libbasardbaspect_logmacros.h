#ifndef GUARD_LIBBASARDBASPECT_LOGMACROS_H
#define GUARD_LIBBASARDBASPECT_LOGMACROS_H

//----------------------------------------------------------------------------//
// include section
//----------------------------------------------------------------------------//
#include <sstream>
#include <iomanip>
#include <libbasarproperty_propertydescriptionlistref.h>
#include <libbasarproperty_propertytable_xiterator.h>
#include <libbasarproperty_propertystate.h>
#include <libbasarcmnutil_logging.h>

namespace basar
{
    namespace db
    {
        namespace aspect
        {
//----------------------------------------------------------------------------//
// define declaration section
//----------------------------------------------------------------------------//
#ifdef NDEBUG
#define BLOG_TRACE_SQLSTRING( logger ) {void();}
#define BLOG_YIT_IN_LINE( logger, yit, fun, lineend ) {void();}
#define BLOG_YIT( logger, yit, fun )  {void();}
#define BLOG_PROPTAB_NORM( logger, propTab, fun , columnseperator ) {void();}
#define BLOG_PROPTAB( logger, propTab, fun ) {void();}
#else

#define BLOG_TRACE_SQLSTRING( logger ) BLOG_TRACE( logger, getSQLString() )

#define BLOG_YIT_IN_LINE( logger, yit, fun, lineend)                                                 \
 {                                                                                                   \
     std::stringstream ss;                                                                           \
     ss << fun << ": ";                                                                              \
     if( yit.isNull() ){                                                                             \
         ss << "YIterator is Null()!";                                                               \
     } else if( yit.isEnd() ){                                                                       \
         ss << "YIterator is empty()!";                                                              \
     } else {                                                                                        \
         ss << lineend;                                                                              \
         basar::db::aspect::AccessorPropertyTableRef propTab = yit.getPropertyTable();               \
         basar::db::aspect::PropertyDescriptionListRef descriptList =                                \
             propTab.getPropertyDescriptionList();                                                   \
         basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin();                       \
         basar::I18nString value = "";                                                               \
         for( int i = 1; !xit.isEnd(); ++xit, ++i ){                                                 \
             value = "";                                                                             \
             if( basar::SS_UNSET != xit.getState().getState() ){                                     \
                 value = xit.getSQLString();                                                         \
             }                                                                                       \
             ss << std::left << std::setw(2) << i << ". " << xit.getName() << "(>" << value << "<, " \
                 << xit.getState().toString() << ")" << lineend;                                     \
         }                                                                                           \
     }                                                                                               \
     BLOG_DEBUG_STREAM( logger, ss.str() );                                                         \
 }

#define BLOG_YIT( logger, yit, fun ) BLOG_YIT_IN_LINE( logger, yit, fun , std::endl)

#define BLOG_PROPTAB_NORM( logger, propTab, fun , columnseperator)                     \
 {                                                                             \
     basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin(); \
     if( propTab.isNull() ){                                                   \
         BLOG_DEBUG_STREAM( logger, fun << ": " << "PropertyTable is Null()!" );                               \
     } else if( yit.isEnd() ){                                                 \
         BLOG_DEBUG_STREAM( logger, fun << ": " << "PropertyTable is empty()!" );                               \
     } else {                                                                  \
         for(; !yit.isEnd(); ++yit ){                                          \
             BLOG_YIT_IN_LINE( logger, yit, fun , columnseperator )              \
         }                                                                     \
     }                                                                         \
 }

#define BLOG_PROPTAB( logger, propTab, fun )                  \
    {                                                                             \
        basar::db::aspect::AccessorPropertyTable_YIterator yit = propTab.begin(); \
        if( propTab.isNull() ){                                                   \
                BLOG_DEBUG_STREAM( logger, fun << ": " << "PropertyTable is Null()!" );                               \
        } else if( yit.isEnd() ){                                                 \
                BLOG_DEBUG_STREAM( logger, fun << ": " << "PropertyTable is empty()!" );                              \
        } else {                                                                  \
            for(; !yit.isEnd(); ++yit ){                                          \
                BLOG_YIT( logger, yit, fun )                      \
            }                                                                     \
        }                                                                         \
    }

#endif // NDEBUG
        }
    }
}


#endif
