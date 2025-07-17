
#include "sqlstringbuilderbaseimpl.h"

#include "loggerpool.h"

namespace basar
{
    namespace db
    {
        namespace aspect
        {
/*--------------------------------------------------------------------------------------------------*/
            const VarString SQLStringBuilderBaseImpl::scm_maskStartString = "#";
            const VarString SQLStringBuilderBaseImpl::scm_maskEndString   = "#";
            const VarString SQLStringBuilderBaseImpl::scm_maskMaskString  = "\\";
/*--------------------------------------------------------------------------------------------------*/
            SQLStringBuilderBaseImpl::~SQLStringBuilderBaseImpl()
            {
            }


            /*--------------------------------------------------------------------------------------------------*/
            /*! \throw no-throw
                \param propertyList is an iterator to a row of the Propertytable */
            void SQLStringBuilderBaseImpl::setPropertyList( const AccessorPropertyTable_YIterator& propertyList )
            {
                m_Yit = propertyList;
            }

            /*--------------------------------------------------------------------------------------------------*/
            /*! \throw no-throw
                \return AccessorPropertyTable_YIterator */
            const AccessorPropertyTable_YIterator SQLStringBuilderBaseImpl::getPropertyList() const
            {
                return m_Yit;
            }

            /*--------------------------------------------------------------------------------------------------*/
            /*! \throw no-throw
                \param paramList list containing default parameters for all sql-statements of this builder */
            void SQLStringBuilderBaseImpl::setParameterList( const basar::cmnutil::ParameterList paramList )
            {
                // all properties of default list are per definition in status "UNKNOWN"!
                // so they can take every state
                m_ParamList = paramList;

                if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    basar::cmnutil::ParameterList_Iterator it = paramList.begin();
                    basar::I18nString msg = "setParameterList(): default parameter set to stringbuilder: ";

                    while ( it != paramList.end() )
                    {
                        msg.append( "key<" );
                        msg.append( it->first.c_str() );
                        msg.append( ">-value<" );

                        if ( !paramList.isValueEmpty( it->first ) )
                        {
                            msg.append( it->second.c_str() );
                        }

                        msg.append( ">; " );
                        ++it;
                    }

                    LoggerPool::sqlStringBuilderImpl().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
                }
            }

            /*--------------------------------------------------------------------------------------------------*/
            /*! \throw no-throw
                \return ParameterList */
            const basar::cmnutil::ParameterList&  SQLStringBuilderBaseImpl::getParameterList() const
            {
                return m_ParamList;
            }

            /*--------------------------------------------------------------------------------------------------*/
            /*! \throw BasarNotImplementedException
                \return nothing (always throws) */
            const IndexTypeCollection& SQLStringBuilderBaseImpl::getPreparedParameterTypePositionCollection() const
            {
                throw basar::cmnutil::BasarNotImplementedException( basar::ExceptInfo( "SQLStringBuilderBaseImpl::getPreparedParameterTypePositionMap", "method not implemented", __FILE__, __LINE__ ) );
            }

            /*--------------------------------------------------------------------------------------------------*/
            /*! \throw no-throw */
            void SQLStringBuilderBaseImpl::clear()
            {
                m_SQLString = "";
            }

            /*--------------------------------------------------------------------------------------------------*/
            /*! \throw no-throw
                \return SQL-statement */
            const VarString& SQLStringBuilderBaseImpl::getSQLString() const
            {
                if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    basar::VarString msg = "getSQLString(): the following sql-string was returned: ";
                    msg.append( m_SQLString.c_str() );
                    LoggerPool::sqlStringBuilderImpl().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
                }

                return m_SQLString;
            }

            /*--------------------------------------------------------------------------------------------------*/
            /*! \throw no-throw
                \param rLiteral is a string which is appended to the SQLString */
            void SQLStringBuilderBaseImpl::literal( const VarString& rLiteral )
            {
                m_SQLString += rLiteral;
            }

            /*--------------------------------------------------------------------------------------------------*/
            /*! \throw UnknownPropertyNameException
                \param rPropertyNames name of a column in the Propertytable */
            void SQLStringBuilderBaseImpl::value( const PropertyName& rPropertyNames )
            {
                m_SQLString += getParamValue( rPropertyNames );
            }

            /*--------------------------------------------------------------------------------------------------*/
            /*! \throw no-throw
                \param rPattern string which is interpreted in this function
                \param begin position where the search starts
                \param end position of the first searched sign in the string */
            void SQLStringBuilderBaseImpl::findFirstSign( VarString& rPattern, const VarString::size_type begin, VarString::size_type& end ) const
            {

                end = rPattern.find( scm_maskStartString, begin );
            }

// -----------------------------------------------------------------------------------------------
            /*!
             * \return substring until next masked string
             */
            VarString SQLStringBuilderBaseImpl::getSubstringToNextMaskString(
                const VarString& maskstring,
                const VarString& own,
                VarString::size_type beginPos,
                VarString::size_type& endPos ) const
            {
                bool notfound = false;
                VarString retval = "";

                do
                {
                    endPos = own.find( maskstring.c_str(), beginPos );
                    notfound = false;

                    if ( endPos != VarString::npos )
                    {
                        VarString::size_type checkPos = own.find( scm_maskMaskString.c_str(), beginPos );

                        while ( checkPos != VarString::npos && checkPos < endPos )
                        {
                            if ( ( endPos - scm_maskMaskString.length() ) == checkPos )
                            {
                                retval += own.substr( beginPos, checkPos - beginPos );
                                retval += own.substr( endPos, maskstring.length() );
                                beginPos = endPos + maskstring.length();
                                notfound = true;
                                break;
                            }

                            checkPos = own.find( scm_maskMaskString.c_str(), checkPos + 1 );
                        }
                    }
                }
                while ( notfound );

                VarString::size_type endPosVal = endPos;

                if ( VarString::npos == endPos )
                {
                    //VarString::size_type endPosVal = own.length();
                    retval += own.substr( beginPos, endPos );
                }
                else if ( endPosVal > beginPos )
                {
                    retval += own.substr( beginPos, endPos - beginPos );
                }

                if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    VarString loghelp;
                    loghelp.format( "resolve(%s) - find substr %s", own.c_str(), retval.c_str() );
                    LoggerPool::sqlStringBuilderImpl().forcedLog
                    (
                        log4cplus::TRACE_LOG_LEVEL, loghelp, __FILE__, __LINE__
                    );
                }

                return retval;
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! \throw UnknownPropertyNameException
                \return the value of the parameter
                \param rPropertyNames name of a column in the Propertytable */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            const VarString SQLStringBuilderBaseImpl::getParamValue( const PropertyName& rPropertyNames ) const
            {
                VarString retval;

                if (    !m_Yit.isNull()
                        && m_Yit.isContainedAndSet( rPropertyNames ) )
                {
                    if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                    {
                        basar::VarString msg;
                        msg.format ( "getParamValue(): property <%s> for creating sql-string read from YIterator",
                                     rPropertyNames.c_str() );
                        LoggerPool::sqlStringBuilderImpl().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
                    }

                    retval = m_Yit.getSQLString( rPropertyNames );
                }
                else
                {
                    if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                    {
                        basar::VarString msg;
                        msg.format ( "getParamValue(): property <%s> for creating sql-string not found in YIterator - trying to read from default parameters.",
                                     rPropertyNames.c_str() );
                        LoggerPool::sqlStringBuilderImpl().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
                    }

                    // --- duplicate "'" if it occurs in string-value
                    VarString            sqlString = m_ParamList.getValue( rPropertyNames );
                    VarString::size_type pos       = sqlString.find( SQL_VALUE_DELIM );

                    while ( pos != VarString::npos )
                    {
                        sqlString.insert( ++pos, SQL_VALUE_DELIM );
                        pos = sqlString.find( SQL_VALUE_DELIM, ++pos );
                    }

                    retval =  sqlString ;
                }

                if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    basar::VarString msg;
                    msg.format ( "getParamValue(): property <%s> for creating sql-string is set to = %s",
                                 rPropertyNames.c_str(),
                                 retval.c_str() );
                    LoggerPool::sqlStringBuilderImpl().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
                }

                return retval;
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! Checks if all paramter given in rPropertyNames are set in the Y-Iterator
                \throw no-throw
                \return true if all given properties are set, so that sqlstring is executable
                \return false, otherwise
                \param rPropertyNames List of Properties (Columnnames)*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool SQLStringBuilderBaseImpl::arePropertiesSet( const PropertyName& rPropertyNames ) const
            {
                return arePropertiesSet( basar::cmnutil::ParameterList( rPropertyNames ) );
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! Checks if all paramter given in rParaList are set in the Y-Iterator
                \throw no-throw
                \return true if all given properties are set, so that sqlstring is executable
                \return false, otherwise
                \param rParaList names of columns with associated values in a Propertytable */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool SQLStringBuilderBaseImpl::arePropertiesSet( const basar::cmnutil::ParameterList& rParaList ) const
            {
                bool retval = false;

                basar::cmnutil::ParameterList unsetParaList = getNonDefaultParameter( rParaList );

                if ( 0 == unsetParaList.size() )
                {
                    if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                    {
                        basar::VarString msg = "arePropertiesSet(): all properties set in default parameters.";
                        LoggerPool::sqlStringBuilderImpl().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
                    }

                    retval = true;
                }
                else
                {
                    if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                    {

                        basar::VarString msg;
                        msg.format ( "arePropertiesSet(): not all properties set in default parameters; following are checked in YIterator <%s>.",
                                     unsetParaList.toString().c_str() );
                        LoggerPool::sqlStringBuilderImpl().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
                    }

                    if ( m_Yit.isNull() )
                    {
                        retval = false;
                    }
                    else if ( m_Yit.isEnd() )
                    {
                        retval = false;
                    }
                    else
                    {
                        retval = m_Yit.arePropertiesSet( unsetParaList );
                    }
                }

                return retval;
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
                at least one is in the state rState
                \throw  no-throw
                \return true if all given properties are set and at least one of the
                        given properties is in one of the given states,
                        so that sqlstring is executable
                \return false, otherwise
                \param rPropertyNames List of Properties (Columnnames)
                \param rState state of the Property */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool SQLStringBuilderBaseImpl::arePropertiesSetAndOneMustBeInState( const PropertyName& rPropertyNames,
                    const PropertyStateSet& rState ) const
            {
                return arePropertiesSetAndOneMustBeInState( basar::cmnutil::ParameterList( rPropertyNames ), rState );
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
                at least one is in the state rState
                \throw  no-throw
                \return true if all given properties are set and at least one of the
                        given properties is in one of the given states,
                        so that sqlstring is executable
                \return false, otherwise
                \param rPropertyNames List of Properties (Columnnames)
                \param rState state of the Property */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool SQLStringBuilderBaseImpl::arePropertiesSetAndOneMustBeInState( const basar::cmnutil::ParameterList& rPropertyNames,
                    const VarString& rState ) const
            {
                return arePropertiesSetAndOneMustBeInState( basar::cmnutil::ParameterList( rPropertyNames ), convertToPropertyStateSet( rState ) );
            }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
                at least one is in the state rState
                \throw no-throw
                \return true if all given properties are set and at least one
                        of the given properties is in one of the given states,
                        so that sqlstring is executable
                \return false, otherwise
                \param rPropertyNames List of Properties (Columnnames)
                \param rState state of the Property */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool SQLStringBuilderBaseImpl::arePropertiesSetAndOneMustBeInState(
                const PropertyName& rPropertyNames,
                const VarString& rState ) const
            {
                return arePropertiesSetAndOneMustBeInState( rPropertyNames, convertToPropertyStateSet( rState ) );
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
                at least one is in the state rState
                \throw no-throw
                \return true if all given properties are set and at
                        least one of the given
                        properties is in one of the given states,
                        so that sqlstring is executable
                \return false, otherwise
                \param  rParaList List of Properties (Columnnames)
                \param  rState state of the Property */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool SQLStringBuilderBaseImpl::arePropertiesSetAndOneMustBeInState(
                const basar::cmnutil::ParameterList& rParaList,
                const PropertyStateSet& rState ) const
            {
                // part 1 set :
                bool retval = arePropertiesSet( rParaList );

                // part 2 in state:
                if ( retval )
                {
                    // we have to check all parameters for state, as a parameter can be listed
                    // in both y-Iterator and default list:
                    retval = m_Yit.isNull() ? false : m_Yit.isOnePropertyInState( rParaList, rState );

                    if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                    {
                        basar::VarString msg;

                        if ( retval )
                        {
                            msg.format ( "arePropertiesSetAndOneMustBeInState():sucess ." );
                        }
                        else
                        {
                            msg.format ( "arePropertiesSetAndOneMustBeInState(): no property %s in state .",
                                         rParaList.toString().c_str() );
                        }

                        LoggerPool::sqlStringBuilderImpl().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
                    }
                }
                else if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    basar::VarString msg;
                    msg.format ( "arePropertiesSetAndOneMustBeInState(): not all properties %s are set .",
                                 rParaList.toString().c_str() );
                    LoggerPool::sqlStringBuilderImpl().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
                }

                return retval;
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
                all are in the state rState
                \throw no-throw
                \return true if all given properties are set and all of
                        the given properties are in one of the given states,
                        so that sqlstring is executable
                \return at least one of the properties has to be listed in the Y-iterator
                        only default Paramter returns false
                \return false, otherwise
                \param rPropertyNames List of Properties (Columnnames)
                \param rState possible states for the properties has to be in.*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool SQLStringBuilderBaseImpl::arePropertiesSetAndAllInState(
                const PropertyName& rPropertyNames,
                const PropertyStateSet& rState ) const
            {
                return arePropertiesSetAndAllInState( basar::cmnutil::ParameterList( rPropertyNames ), rState );
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
                all are in the state rState
                \throw no-throw
                \return true if all given properties are set and all of the
                        given properties are in one of the given states,
                        so that sqlstring is executable
                \return at least one of the properties has to be listed in the Y-iterator
                        only default Paramter returns false
                \return false, otherwise
                \param rPropertyNames List of Properties (Columnnames)
                \param rState possible states for the properties has to be in.*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool SQLStringBuilderBaseImpl::arePropertiesSetAndAllInState(
                const basar::cmnutil::ParameterList&  rPropertyNames,
                const VarString& rState ) const
            {
                return arePropertiesSetAndAllInState( rPropertyNames, convertToPropertyStateSet( rState ) );
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
                all are in the state rState
                \throw no-throw
                \return true if all given properties are set and all of
                        the given properties are in one of the given states,
                        at least one of the properties has to be listed in the Y-iterator
                        (only default Paramter returns false),
                        so that sqlstring is executable
                \return at least one of the properties has to be listed in the Y-iterator
                        only default Paramter returns false
                \return false, otherwise
                \param rPropertyNames List of Properties (Columnnames)
                \param rState possible states for the properties has to be in.*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool SQLStringBuilderBaseImpl::arePropertiesSetAndAllInState(
                const PropertyName& rPropertyNames,
                const VarString& rState ) const
            {
                return arePropertiesSetAndAllInState( basar::cmnutil::ParameterList( rPropertyNames ), convertToPropertyStateSet( rState ) );
            }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! Checks if all paramter given in rPropertyNames are set in the Y-Iterator and if
                all are in the state rState
                \throw  no-throw
                \return true if all given properties are set and all of the given
                        properties are in one of the given states,
                        at least one of the properties has to be listed in the Y-iterator,
                        so that sqlstring is executable
                \return at least one of the properties has to be listed in the Y-iterator
                        only default Paramter returns false
                \param rParaList List of Properties (Columnnames)
                \param rState possible states for the properties to be in.*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool SQLStringBuilderBaseImpl::arePropertiesSetAndAllInState(
                const basar::cmnutil::ParameterList& rParaList,
                const PropertyStateSet& rState ) const
            {
                bool retval = false;

                std::pair<basar::cmnutil::ParameterList, basar::cmnutil::ParameterList>
                seperateLists = partParamterListInDefaultandNondefaultParameter( rParaList );

                if ( 0 == seperateLists.second.size() )
                {
                    if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                    {
                        basar::VarString msg = "arePropertiesSetAndAllInState(): all properties set in default parameters.";
                        LoggerPool::sqlStringBuilderImpl().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
                    }

                    // at least one of the default parameter has to be also stated in the
                    // Y-Iterator and in state
                    retval = m_Yit.isNull() ? false : m_Yit.areKnownAndSetPropertiesInState( seperateLists.first, rState, false );
                }
                else
                {
                    if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                    {
                        basar::VarString msg;
                        msg.format ( "arePropertiesSetAndAllInState(): not all properties set in default parameters; following are checked in YIterator <%s>.",
                                     seperateLists.second.toString().c_str() );
                        LoggerPool::sqlStringBuilderImpl().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
                    }

                    // check set and states of the non default list
                    retval = m_Yit.isNull() ? false : m_Yit.arePropertiesSetAndAllInState( seperateLists.second , rState );

                    if ( retval )
                    {
                        // if one of the default paramter is listed in the Y-Iterator, it
                        // has to be in state
                        retval = m_Yit.isNull() ? false : m_Yit.areKnownAndSetPropertiesInState( seperateLists.first, rState, true );
                    }
                }

                return retval;
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*!  Checks is all the Properties listed in the 1st parameter are set in
                in the aggregated AccessorPropertyTable_YIterator
                \return true if all given properties (1st parameter as ParameterList) are set
                        and at least one of the given properties
                        (2nd parameter as ParameterList) are in one of the given states,
                        so that the sqlstring can be executed.
                \return false, otherwise
                \param  plShouldBeSet represents the properties which have to be set
                        in the aggregated AccessorPropertyTable_YIterator.
                \param  plOneMustBeInState represents the properties from with at least one
                        has to be in one of the given states (pss).
                \param  rState represents the possible states at least one of the properties of the
                        2nd parameter (propertyNamesOneMustBeInState)
                        has to be in.
                \throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool SQLStringBuilderBaseImpl::are1stPropertiesSetAnd2ndOneInState(
                const basar::cmnutil::ParameterList& plShouldBeSet,
                const basar::cmnutil::ParameterList& plOneMustBeInState,
                const VarString& rState ) const
            {
                return are1stPropertiesSetAnd2ndOneInState( plShouldBeSet, plOneMustBeInState, convertToPropertyStateSet( rState ) );
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*!  Checks is all the Properties listed in the 1st parameter are set in
                in the aggregated AccessorPropertyTable_YIterator
                \return true if all given properties (1st parameter as ParameterList) are set
                        and and at least one
                        of the given properties
                        (2nd parameter as ParameterList) are in one of the given states,
                        so that the sqlstring can be executed.
                \return false, otherwise
                \param  propertyNamesShouldBeSet represents the properties which have to be set in the aggregated
                        AccessorPropertyTable_YIterator.
                \param  propertyNamesOneMustBeInState represents the properties which have to be set in the aggregated
                        AccessorPropertyTable_YIterator and have to be in one of the given states (pss).
                \param  pss represents the possible states one of the properties of the 2nd parameter (propertyNamesOneMustBeInState)
                        has to be in.
                \throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool SQLStringBuilderBaseImpl::are1stPropertiesSetAnd2ndOneInState(
                const PropertyName& propertyNamesShouldBeSet,
                const PropertyName& propertyNamesOneMustBeInState,
                const PropertyStateSet& pss ) const
            {
                return are1stPropertiesSetAnd2ndOneInState(
                           basar::cmnutil::ParameterList( propertyNamesShouldBeSet ),
                           basar::cmnutil::ParameterList( propertyNamesOneMustBeInState ),
                           pss );
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*!  Checks is all the Properties listed in the 1st parameter are set in
                in the aggregated AccessorPropertyTable_YIterator
                \return true if all given properties (1st parameter as ParameterList) are set
                        and and at least one
                        of the given properties
                        (2nd parameter as ParameterList) are in one of the given states,
                        so that the sqlstring can be executed.
                \return false, otherwise
                \param  propertyNamesShouldBeSet represents the properties which have to be set in the aggregated
                        AccessorPropertyTable_YIterator.
                \param  propertyNamesOneMustBeInState represents the properties which have to be set in the aggregated
                        AccessorPropertyTable_YIterator and have to be in one of the given states (pss).
                \param  rState represents the possible states one of the properties of the 2nd parameter (propertyNamesOneMustBeInState)
                        has to be in.
                \throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool SQLStringBuilderBaseImpl::are1stPropertiesSetAnd2ndOneInState(
                const PropertyName& propertyNamesShouldBeSet,
                const PropertyName& propertyNamesOneMustBeInState,
                const VarString& rState ) const
            {
                return are1stPropertiesSetAnd2ndOneInState(
                           propertyNamesShouldBeSet,
                           propertyNamesOneMustBeInState,
                           convertToPropertyStateSet( rState )  );
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*!  Checks is all the Properties listed in the 1st parameter are set in
                in the aggregated AccessorPropertyTable_YIterator
                \return true if all given properties (1st parameter as ParameterList) are set
                        and and at least one
                        of the given properties
                        (2nd parameter as ParameterList) are in one of the given states,
                        so that the sqlstring can be executed.
                \return false, otherwise
                \param  plShouldBeSet represents the properties which have to be set in the aggregated
                        AccessorPropertyTable_YIterator.
                \param  plOneMustBeInState represents the properties which have to be set in the aggregated
                        AccessorPropertyTable_YIterator and have to be in one of the given states (pss).
                \param  pss represents the possible states one of the properties of the 2nd parameter (propertyNamesOneMustBeInState)
                        has to be in.
                \throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool SQLStringBuilderBaseImpl::are1stPropertiesSetAnd2ndOneInState(
                const basar::cmnutil::ParameterList& plShouldBeSet,
                const basar::cmnutil::ParameterList& plOneMustBeInState,
                const PropertyStateSet& pss ) const
            {
                bool retval = false;
                retval = arePropertiesSet( plShouldBeSet );

                if ( retval )
                {
                    retval = m_Yit.isNull() ? false : m_Yit.isOnePropertyInState( plOneMustBeInState, pss );
                }

                return retval;
            }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! \retval true if given property is set and yiterator is not at end
                \return otherwise returns false
                \param  rPropertyName property to check if it is set
                \throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool    SQLStringBuilderBaseImpl::isNotEndAndSet( const PropertyName& rPropertyName ) const
            {
                if ( m_ParamList.isKeyContained ( rPropertyName ) == true &&
                        m_ParamList.isValueEmpty   ( rPropertyName ) == false )
                {
                    if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                    {
                        basar::VarString msg;
                        msg.format( "isNotEndAndSet(): property <%s> set in default parameters.",
                                    rPropertyName.c_str() );
                        LoggerPool::sqlStringBuilderImpl().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
                    }

                    return true;
                }

                if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    basar::VarString msg;
                    msg.format( "isNotEndAndSet(): property <%s> not set in default parameters -> check in YIterator.",
                                rPropertyName.c_str() );
                    LoggerPool::sqlStringBuilderImpl().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
                }

                return m_Yit.isNull() ? false : m_Yit.isNotEndAndSet( rPropertyName );
            }

///////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! \retval true if given property is set and contained in yiterator
                \return otherwise returns false
                \param  rPropertyName property to check
                \throw  no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool    SQLStringBuilderBaseImpl::isContainedAndSet( const PropertyName& rPropertyName ) const
            {
                if ( m_ParamList.isKeyContained ( rPropertyName ) == true &&
                        m_ParamList.isValueEmpty   ( rPropertyName ) == false )
                {
                    if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                    {
                        basar::VarString msg;
                        msg.format( "isContainedAndSet(): property <%s> set in default parameters.",
                                    rPropertyName.c_str() );
                        LoggerPool::sqlStringBuilderImpl().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
                    }

                    return true;
                }

                if ( LoggerPool::sqlStringBuilderImpl().isEnabledFor( log4cplus::TRACE_LOG_LEVEL ) )
                {
                    basar::VarString msg;
                    msg.format( "isContainedAndSet(): property <%s> not set in default parameters -> check in YIterator.",
                                rPropertyName.c_str() );
                    LoggerPool::sqlStringBuilderImpl().forcedLog( log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__ );
                }

                return m_Yit.isNull() ? false : m_Yit.isContainedAndSet( rPropertyName );
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! \retval PropertyStateSet possible states
                \param  rState represents the state as string that should be converted to a PropertyStateSet
                \throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            const PropertyStateSet SQLStringBuilderBaseImpl::convertToPropertyStateSet( const VarString& rState ) const
            {
                PropertyStateSet state;

                if      ( rState == "UNKNOWN" )
                {
                    state = basar::SS_UNKNOWN;
                }
                else if ( rState == "CLEAN" )
                {
                    state = basar::SS_CLEAN;
                }
                else if ( rState == "UNSET" )
                {
                    state = basar::SS_UNSET;
                }
                else if ( rState == "INSERT" )
                {
                    state = basar::SS_INSERT;
                }
                else if ( rState == "UPDATE" )
                {
                    state = basar::SS_UPDATE;
                }
                else if ( rState == "DELETE" )
                {
                    state = basar::SS_DELETE;
                }

                return state;
            }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! writes all Parameters, that are not set in the m_ParamList (default list)
             * in a  ParameterList
             * \throw no-throw
             * \return ParameterList containing all parameters not set in m_ParamList (default list)
             */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            basar::cmnutil::ParameterList SQLStringBuilderBaseImpl::getNonDefaultParameter(
                const basar::cmnutil::ParameterList& rParaList  //!< ParameterList, with column names
            ) const
            {
                basar::cmnutil::ParameterList retval;
                basar::cmnutil::ParameterList_Iterator it;

                for ( it = rParaList.begin(); it != rParaList.end(); ++it )
                {
                    if ( ! m_ParamList.hasValue( it->first ) )
                    {
                        retval.push_back( it->first );
                    }
                }

                return retval;
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /*! writes all  Parameters, that are not set in the m_ParamList (default list)
             * in the second ParameterList and all the others in the first ParameterList
             * \return std::pair containing two ParameterLists, first containing all parameters not set in m_ParamList, second all other parameters
             * \throw no-throw */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            std::pair<basar::cmnutil::ParameterList, basar::cmnutil::ParameterList>
            SQLStringBuilderBaseImpl::partParamterListInDefaultandNondefaultParameter(
                const basar::cmnutil::ParameterList& rParaList   //!< ParameterList, with column names
            ) const
            {
                std::pair<basar::cmnutil::ParameterList, basar::cmnutil::ParameterList>  retval;

                basar::cmnutil::ParameterList_Iterator it;

                for ( it = rParaList.begin(); it != rParaList.end(); ++it )
                {
                    if ( m_ParamList.hasValue( it->first ) )
                    {
                        retval.first.push_back( it->first );
                    }
                    else
                    {
                        retval.second.push_back( it->first );
                    }
                }
                return retval;
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        }
    }
}