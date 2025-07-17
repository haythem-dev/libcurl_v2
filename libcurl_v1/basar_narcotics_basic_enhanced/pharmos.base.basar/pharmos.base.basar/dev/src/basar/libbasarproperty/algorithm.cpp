//////////////////////////////////////////////////////////////////////////////
/*! \file
 *  \brief  basar::property global functions to check if some properties are set 
			or in a certain state in comparing a propertytable and a paremterlist
 *  \author Bischof Björn 
 *  \date   25.07.2005
 */
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// includes
//////////////////////////////////////////////////////////////////////////////

#include "libbasarproperty_algorithm.h"

#include "libbasarcmnutil_parameterlist.h"
#include "libbasarproperty_definitions.h"
#include "libbasarproperty_propertystate.h"
#include "libbasarproperty_propertytable_xiterator.h"
#include "libbasarproperty_propertytable_yiterator.h"

#include "loggerpool.h"

//--------------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if all given properties in pl are found in the yiterator and 
			no attribute of the yiterator is in the state SS_UNSET.
	\return otherwise returns false
	\param	yit is the yiterator compared with the paremeterlist
	\param	pl is the parameterlist compared with an yiterator (row)
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
bool arePropertiesSet( const PropertyTable_YIterator & yit, 
					   const cmnutil::ParameterList  & pl )
{
    bool retval = false;
    cmnutil::ParameterList_Iterator pit;

    ColumnName cn;
    PropertyTable_XIterator xit;
    if (yit.size() > 0)
    {
        retval = true;
        for( pit = pl.begin(); pit != pl.end() && retval; ++pit )
        {
            cn = pit->first;
            for( xit = yit.begin(); xit != yit.end(); ++xit )
            {
                if( xit.getName() == cn )
                {
                    if( xit.getState().getState() == SS_UNSET )
                    {
                        if (LoggerPool::algorithm().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
                        {
                            VarString msg;
                            msg.format("property '%s' known, but UNSET.", cn.c_str());
                            LoggerPool::algorithm().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
                        }
                        retval =  false;
                    }
                    break;
                }
            }
            if( xit == yit.end() )
            {
                if (LoggerPool::algorithm().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
                {
                    VarString msg;
                    msg.format("property '%s' NOT known.", cn.c_str());
                    LoggerPool::algorithm().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
                }
                retval = false;
            }
        }
    }
    return retval;
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if all given properties in pl are found in the yiterator and 
			no attribute of the yiterator is in the state SS_UNSET.
	\param	yit is the yiterator compared with the parameterlist
	\param	pl is the parameterlist compared with an yiterator (row)
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
bool arePropertiesSet( const PropertyTable_YIterator& yit, const VarString& pl )
{
	return arePropertiesSet( yit, cmnutil::ParameterList(pl) );
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if all given properties in pl are found in the yiterator and 
			no attribute of the yiterator is in the state SS_UNSET and 
			all attributes of the yiterator are in a state which is contained 
			in the propertystateset pss.
	\param	yit is the yiterator compared with the parameterlist
	\param	pl is the parameterlist compared with an yiterator (row)
	\param	pss propertystateset containing states every attribute of the yiterator 
			must be in.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
bool arePropertiesSetAndAllInState(const PropertyTable_YIterator & yit, 
                                   const cmnutil::ParameterList  & pl, 
								   const PropertyStateSet        & pss )
{
	cmnutil::ParameterList_Iterator pit;
	ColumnName cn;
	PropertyTable_XIterator xit;
	bool continueOuterLoop = false;

	for( pit = pl.begin(); pit != pl.end(); ++pit ){
		cn = pit->first;
		xit = yit.begin();
		continueOuterLoop = false;

		for( ; xit != yit.end(); ++xit )
		{
			if( xit.getName() == cn ){
				
				if( xit.getState().getState() == SS_UNSET )
                {
					if (LoggerPool::algorithm().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
					{
						VarString msg;
						msg.format("property '%s' known, but UNSET.", cn.c_str());
						LoggerPool::algorithm().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
					}
					return false;
				}
				else if( ! xit.isInState(pss) )
                {
					if (LoggerPool::algorithm().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
					{
						VarString msg;
						msg.format("property '%s' known, but NOT in expected state '%s', parameter is in state '%s'", 
                            cn.c_str(), 
                            pss.toString().c_str(), 
                            xit.getState().toString().c_str());
						LoggerPool::algorithm().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
					}
					return false;
				}
				continueOuterLoop = true;
				break;
			}
		}
		if( continueOuterLoop ){
			continue;
		}
		if( xit == yit.end() ){
			if (LoggerPool::algorithm().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
			{
				VarString msg;
				msg.format("property '%s' NOT known.", cn.c_str());
				LoggerPool::algorithm().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
			}
			return false;
		}
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if all given properties in pl are found in the yiterator and 
			no attribute of the yiterator is in the state SS_UNSET and 
			all attributes of the yiterator are in a state which is contained 
			in the propertystateset pss.
	\param	yit is the yiterator compared with the parameterlist
	\param	pl is the parameterlist compared with an yiterator (row)
	\param	pss propertystateset containing states every attribute of the yiterator 
			must be in.
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
bool arePropertiesSetAndAllInState( const PropertyTable_YIterator & yit,
                                    const VarString               & pl, 
									const PropertyStateSet        & pss )
{
	return arePropertiesSetAndAllInState( yit, cmnutil::ParameterList(pl), pss );
}

///////////////////////////////////////////////////////////////////////////////////////////////
/*! \retval true if all given properties in pl are found and not in State SS_UNSET
			AND at least one of the given properties is in one of the given states in pss	
	\param	yit is the YIterator compared with the parameterlist
	\param	pl is the parameterlist compared with a row (YIterator)
	\param	pss is the state the xit (column element) is tested against
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
bool arePropertiesSetAndOneMustBeInState( const PropertyTable_YIterator & yit, 
										  const cmnutil::ParameterList  & pl, 
										  const PropertyStateSet        & pss )
{
	cmnutil::ParameterList_Iterator pit;
	ColumnName cn;
	PropertyTable_XIterator xit;
	bool stateFound = false;
	bool continueOuterLoop = false;

	for( pit = pl.begin(); pit != pl.end(); ++pit ){
		cn = pit->first;
		xit = yit.begin();
		continueOuterLoop = false;

		for( ; xit != yit.end(); ++xit )
		{
			if( xit.getName() == cn ){
				
				if( xit.getState().getState() == SS_UNSET )
                {
					if (LoggerPool::algorithm().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
					{
						VarString msg;
						msg.format("property '%s' known, but UNSET.", cn.c_str());
						LoggerPool::algorithm().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
					}
					return false;
				}

				if( xit.isInState(pss) ){
					stateFound = true;
				}
				continueOuterLoop = true;
				break;
			}
		}
		if( continueOuterLoop ){
			continue;
		}
		if( xit == yit.end() ){
			if (LoggerPool::algorithm().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
			{
				VarString msg;
				msg.format("property '%s' NOT known.", cn.c_str());
				LoggerPool::algorithm().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
			}
			return false;
		}
	}

	if (false == stateFound && LoggerPool::algorithm().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("all properties known, but none in expected state '%s'.", pss.toString().c_str());
		LoggerPool::algorithm().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

	return stateFound;
}
///////////////////////////////////////////////////////////////////////////////////////////////
/*!	\retval true if all given properties in pl are found and not in State SS_UNSET 
			AND at least one of the given properties is in one of the given states in pss	
	\param	yit is handed through YIterator
	\param	pl is constructed to a parameterlist which is handed through
	\param	pss is handed through
	\throw	no-throw */
///////////////////////////////////////////////////////////////////////////////////////////////
bool arePropertiesSetAndOneMustBeInState( const PropertyTable_YIterator & yit, 
										  const VarString               & pl, 
										  const PropertyStateSet        & pss )
{
	return arePropertiesSetAndOneMustBeInState( yit, cmnutil::ParameterList(pl), pss );
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*!	returns true, if all given properties in plShouldBeSet 
			are found and not in State Unset set in Row yit 
			AND at least one of the given properties in plOneMustBeInState
            is in one of the given states in pss
	\param yit iterator to row of propertytable
	\param plShouldBeSet list of properties to check if they are set
	\param plOneMustBeInState list of properties to check state
	\param pss state that given properties (plOneMustBeInState) must be in
	\retval  true, if all given properties in plShouldBeSet 
			are found and not in State Unset set in Row yit 
			AND at least one of the given properties in plOneMustBeInState
            is in one of the given states in pss
	\n no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool are1stPropertiesSetAnd2ndOneInState( const PropertyTable_YIterator & yit, 
                                          const VarString               & plShouldBeSet, 
										  const VarString               & plOneMustBeInState, 
                                          const PropertyStateSet        & pss )
{
	return are1stPropertiesSetAnd2ndOneInState( yit, 
												cmnutil::ParameterList(plShouldBeSet),
												cmnutil::ParameterList(plOneMustBeInState),
												pss);
}
////////////////////////////////////////////////////////////////////////////////////////////////
/*!	returns true, if all given properties in plShouldBeSet 
			are found and not in State Unset set in Row yit 
			AND at least one of the given properties in plOneMustBeInState
            is in one of the given states in pss
	\param yit iterator to row of propertytable
	\param plShouldBeSet list of properties to check if they are set
	\param plOneMustBeInState list of properties to check state
	\param pss state that given properties must be in
	\retval  true, if all given properties in plShouldBeSet 
			are found and not in State Unset set in Row yit 
			AND at least one of the given properties in plOneMustBeInState
            is in one of the given states in pss
	\n no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////
bool are1stPropertiesSetAnd2ndOneInState( const PropertyTable_YIterator & yit, 
                                          const cmnutil::ParameterList  & plShouldBeSet, 
										  const cmnutil::ParameterList  & plOneMustBeInState, 
                                          const PropertyStateSet        & pss )
{
    bool retval = false;
	try 
    {
		retval = ((arePropertiesSet( yit, plShouldBeSet )) && 
				  (isOnePropertyInState( yit, plOneMustBeInState, pss)));
	} 
    catch( ... )
    {
		retval = false;
	}
    return retval;
}


/*!  returns true, if  at least one of the properties in 
     the list (pl) is in one of the given states in pss, 
     this function is used in are1stPropertiesSetAnd2ndOneInState
	\param yit iterator to row of propertytable
	\param pl  list of properties to check state 
	\param pss state that given properties must be in
    */
bool isOnePropertyInState( const PropertyTable_YIterator & yit, 
                           const cmnutil::ParameterList  & pl, 
                           const PropertyStateSet        & pss)
{
    cmnutil::ParameterList_Iterator piter;
    PropertyTable_XIterator xit;
    bool stateFound = false;

    for(xit = yit.begin(); xit != yit.end() && !stateFound; ++xit )
    {
        if (xit.isInState(pss) )
        {
            for( piter = pl.begin(); piter != pl.end(); ++piter )
            {
                if( xit.getName() == piter->first )
                {
                    stateFound = true;
                    break;
                }
            }
        }
    }

    if (false == stateFound && LoggerPool::algorithm().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
    {
        VarString msg;
        msg.format("no property is in expected state '%s'.", pss.toString().c_str());
        LoggerPool::algorithm().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
    }

    return stateFound;
}

/*! returns true, if all the  properties in the list (pl),
    that are found in yit are in one of the given states in pss 
    If none of the properties are found the value of 
    acceptNoProperty is returned

    \param yit iterator to row of propertytable
    \param pl list of properties to check state
    \param pss state that given properties must be in
    \param acceptNoProperty returnvalue if no listed property is found
*/
bool areKnownPropertiesInState( const PropertyTable_YIterator & yit, 
                                const cmnutil::ParameterList  & pl, 
                                const PropertyStateSet        & pss,
                                bool                            acceptNoProperty)
{
    cmnutil::ParameterList_Iterator piter;
    PropertyTable_XIterator xit;
    bool retval = true;
    bool foundProperty = false;

    for( piter = pl.begin(); piter != pl.end() && retval; ++piter )
    {
        for(xit = yit.begin(); xit != yit.end() && retval; ++xit )
        {
            if( xit.getName() == piter->first )
            {
                foundProperty = true;
                if (!xit.isInState(pss) )
                {
                    retval = false; 
                } 
                break;
            }
        }
    }
    if ((!retval) && (LoggerPool::algorithm().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL)))
    {
        VarString msg;
        msg.format("areSetPropertiesInState: at least one property is not in expected state '%s'.", pss.toString().c_str());
        LoggerPool::algorithm().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
    }
    if (!foundProperty)
    {
        if (!acceptNoProperty)
        {
            retval = false;
        }
        if (LoggerPool::algorithm().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
        {
            VarString msg;
            msg.format("areSetPropertiesInState: no property checked, result = '%s'.", (retval)?"true":"false");
            LoggerPool::algorithm().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
        }

    }
    return retval;
}



/*! returns true, if all the  properties of the list (pl),
    that are not unset  in yit are in one of the given states in pss (unset 
    parameters are ignored)
    If none of the properties are found the value of acceptNoProperty is returned

    \param yit iterator to row of propertytable
    \param pl list of properties to check state
    \param pss state that given properties must be in
    \param acceptNoProperty returnvalue if no listed property is found
*/
bool areKnownAndSetPropertiesInState( const PropertyTable_YIterator & yit, 
									  const cmnutil::ParameterList  & pl, 
									  const PropertyStateSet        & pss,
									  bool                            acceptNoProperty)
{
    cmnutil::ParameterList_Iterator piter;
    PropertyTable_XIterator xit;
    bool retval = true;
    bool foundProperty = false;

    for( piter = pl.begin(); piter != pl.end() && retval; ++piter )
    {
        for(xit = yit.begin(); xit != yit.end() && retval; ++xit )
        {
            if( xit.getName() == piter->first )
            {
                if (LoggerPool::algorithm().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
                {  
                    VarString msg;
                    msg.format("areKnownAndSetPropertiesInState<%s>: checking %s " , pss.toString().c_str(), piter->first.c_str() );
                    LoggerPool::algorithm().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
                }
                if (!xit.isInState(SS_UNSET))
                {
                 foundProperty = true;
                 if (!xit.isInState(pss) )
                  {
                    retval = false; 
                    if (LoggerPool::algorithm().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
                    {  
                        VarString msg;
                        msg.format("areKnownAndSetPropertiesInState<%s>:  %s is in state %s " , pss.toString().c_str(), piter->first.c_str(), xit.getState().toString().c_str() );
                        LoggerPool::algorithm().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
                    }
                  } 
                }
                break;
            }
        }
    }
    if ((!retval) && (LoggerPool::algorithm().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL)))
    {
        VarString msg;
        msg.format("areKnownAndSetPropertiesInState<%s>: at least one property <%s> is not in expected state '%s'.",  pss.toString().c_str(), pl.toString().c_str(), pss.toString().c_str());
        LoggerPool::algorithm().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
    }
    if (!foundProperty)
    {
        if (!acceptNoProperty)
        {
            retval = false;
        }
        if (LoggerPool::algorithm().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
        {  
            VarString msg;
            msg.format("areKnownAndSetPropertiesInState<%s>: no property <%s> checked in Y-Iterator, result = '%s'.",  pss.toString().c_str(), pl.toString().c_str(), (retval)?"true":"false");
            LoggerPool::algorithm().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
        }

    }
    return retval;
}

} // End namespace property
} // End namespace basar
