//----------------------------------------------------------------------------
/*! \file
 *  \brief  build SQL - statements implementation
 *  \author Roland Kiefert
 *  \date   14.07.2005
 */
//----------------------------------------------------------------------------

#include "sqlpreparedstringbuilderimpl.h"
#include "libbasarproperty_propertytype.h"
#include "libbasardbaspect_stringbuildertypeindexes.h"

#include "loggerpool.h"

/*--------------------------------------------------------------------------------------------------*/
namespace basar
{
    namespace db
    {
        namespace aspect
        {

            /*--------------------------------------------------------------------------------------------------*/
            const VarString SQLPreparedStringBuilderImpl::scm_maskReplacementString  = "?";


            /*--------------------------------------------------------------------------------------------------*/
            SQLPreparedStringBuilderImpl::~SQLPreparedStringBuilderImpl()
            {
            }

            /*--------------------------------------------------------------------------------------------------*/
            /*! \throw no-throw
                \return IndexTypeCollection */
            const IndexTypeCollection& SQLPreparedStringBuilderImpl::getPreparedParameterTypePositionCollection() const
            {
                return m_ParameterTypePositionCollection;
            }

            void SQLPreparedStringBuilderImpl::clear()
            {
                SQLStringBuilderBaseImpl::clear();
                m_ParameterTypePositionCollection.clear();
            }

            /*--------------------------------------------------------------------------------------------------*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! \throw UnknownPropertyNameException
                \param rPattern string which is interpreted in this function */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
            void SQLPreparedStringBuilderImpl::resolve( const VarString& rPattern )
            {
                VarString own = rPattern;
                VarString::size_type beginPos = 0;
                VarString::size_type endPos = 0;
                VarString help;
                basar::Int32 indexPrepStmt = 0;

                while ( endPos != VarString::npos && beginPos < own.size() )
                {
                    help = getSubstringToNextMaskString( scm_maskStartString, own, beginPos, endPos );
                    literal( help );

                    if ( endPos != VarString::npos )
                    {
                        beginPos = endPos + scm_maskStartString.length();

                        if ( beginPos < own.length() )
                        {
                            help = getSubstringToNextMaskString( scm_maskEndString, own, beginPos, endPos );

                            //replace mask with ?
                            literal( SQLPreparedStringBuilderImpl::scm_maskReplacementString );
                            //store index of help (our property name) in index / property collection
                            addTypeAndIndex( help, indexPrepStmt );

                            if ( endPos != VarString::npos )
                            {
                                beginPos = endPos + scm_maskEndString.length();
                                ++indexPrepStmt;
                            }
                        }
                    }
                }
            }

            void SQLPreparedStringBuilderImpl::addTypeAndIndex( const VarString& paramName, basar::Int32 indexPrepStmt )
            {
                property::PropertyType type;

                basar::Int32 indexYit = -1;

                AccessorPropertyTable_YIterator yit = getPropertyList();

                if ( false == yit.isNull() )
                {
                    indexYit = static_cast<basar::Int32>( yit.getIndex( paramName ) );
                    type = yit.getType( indexYit );
                }

                m_ParameterTypePositionCollection.push_back( IndexTypeCollection::value_type( indexPrepStmt, indexYit, type ) );
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        }//aspect
    }//db
}//basar
