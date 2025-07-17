//----------------------------------------------------------------------------
/*! \file
 *  \brief  class implementation of parameter list
 *  \author Thomas Hörath
 *  \date   12.08.2005
 */
//----------------------------------------------------------------------------


#include "libbasarcmnutil_parameterlist.h"

#include <sstream>
#include <libbasardebug.h>
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_i18nstring.h"
#include "loggerpool.h"
#include "libbasarcmnutil_exceptions.h"

//----------------------------------------------------------------------------
namespace basar   {
namespace cmnutil {

//----------------------------------------------------------------------------
void dumpParamList(size_t addr)
{
	if (0 == addr)
		return;

	const ParameterList * const pObj = reinterpret_cast<const ParameterList * const>(addr);

	std::ostringstream outStr;

	pObj->toStream(outStr);

	debug::printDbgMsg(outStr.str().c_str());
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
ParameterList::ParameterList() : m_ColParamListMap(new ColParamListMap)
{
}

//----------------------------------------------------------------------------
/*!	\throw BasarNotTerminatedException 
	\param rPairStr string in format "name1=value1; name2; name3=value3;"... from which new list-values are constructed */
ParameterList::ParameterList (const ParamPair & rPairStr) 
                             : m_ColParamListMap(new ColParamListMap)
{
    VarString pair = rPairStr;
    pair.trim();

    CollBString substrings = pair.tokenizeAndTrim(VarString(SIGN_DELIM), 
												  BString::FIRST_END, 
												  true,		// there has to be no last ";" Value 
												  true);	// no check on empty values, it is possible to just have ";" - Description

    for (CollBString::const_iterator iter = substrings.begin(); iter != substrings.end(); ++iter)
    {
        BString::size_type pos = iter->find(SIGN_ASSIGN);
        if (BString::npos == pos )
        {
            // key alone:
            push_back(*iter);
        }
        else
        {        
            // key:
            BString key = iter->substr(0, pos);
            key.trim(BString::FIRST_END);
           
            // value:
            BString            value ;
            BString::size_type length = iter->length();

            if (length > pos)
            {
                value = iter->substr(pos+1, length - pos);
            }
            value.trim(BString::FIRST_END);
            push_back(key, value);
        }
    }
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\param r Reference to ParameterList-Object to copy from */
ParameterList::ParameterList (const ParameterList& r) 
                             : m_ColParamListMap(new ColParamListMap)
{
	assign(r);
}


//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\param r reference to list which is set to current object */
void ParameterList::assign(const ParameterList& r)
{
	m_ColParamListMap->clear();
	ColParamListMap::iterator it = r.m_ColParamListMap->begin();

	while ( it != r.m_ColParamListMap->end() )
	{
		m_ColParamListMap->insert(*it);
		++it;
	}
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\param r reference to list which is set to current object 
	\return reference to ParameterList object which was assigned
*/
ParameterList & ParameterList::operator = (const ParameterList& r)
{
	if ( &r != this )
	{
		assign(r);	
	}
	return *this;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\param rKey key-name to add to list, value is empty */
void ParameterList::push_back(const ParamName & rKey)
{
	push_back((ColParamListMap::value_type(rKey,"")));
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\param rKey key-name to add to list
	\param rVal value for key */
void ParameterList::push_back(const ParamName & rKey, const ParamValue & rVal)
{
	push_back((ColParamListMap::value_type(rKey, rVal)));
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\param rParamPair pair of key and value to add to list */
void ParameterList::push_back(const ParamListPair & rParamPair)
{
	m_ColParamListMap->insert(rParamPair);
	
	if (LoggerPool::paramList().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		basar::VarString msg;
		msg.format("pair: key <%s> - value <%s> inserted in parameterlist", 
					rParamPair.first.c_str(), 
					rParamPair.second.c_str());
		LoggerPool::paramList().forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return number of lines in list */
ParamListSize ParameterList::size() const
{
	return m_ColParamListMap->size();
}

//----------------------------------------------------------------------------
/*!	\throw UnknownPropertyNameException 
	\return value to given key 
	\param rKey name of key for which the value is asked */
const ParamValue ParameterList::getValue(const ParamName& rKey) const
{
	ColParamListMapConstIt const_it = m_ColParamListMap->find(rKey);

	if ( const_it == m_ColParamListMap->end() )
	{
		VarString errMsg;
		errMsg.format("key <%s> to search value for is not in parameterlist", rKey.c_str());
		
		ExceptInfo sInfo("basar.cmnutil.ParameterList.getValue", errMsg, __FILE__, __LINE__);
		throw UnknownPropertyNameException(sInfo);
	}

	return const_it->second;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\retval true if key is known in list
	\return otherwise returns false
	\param rKey name of key to look for */
bool ParameterList::isKeyContained	(const ParamName& rKey) const
{
	return 	m_ColParamListMap->find(rKey) != m_ColParamListMap->end();
}

//----------------------------------------------------------------------------
/*!	\throw UnknownPropertyNameException 
	\retval true if no value is set to given key 
	\return otherwise returns false
	\param rKey name of key for which the value is asked */
bool ParameterList::isValueEmpty (const ParamName& rKey) const
{
	return getValue(rKey).empty();
}

//----------------------------------------------------------------------------
/*! \brief	returns true when the key is cobtained and a value is set.
			\n     no-throw 
	\param rKey name of key for which containment and a set value is asked
	\return bool indicating if key is contained and has a value set
*/
bool ParameterList::hasValue (const ParamName& rKey ) const
{
    if (isKeyContained(rKey))
        return !(isValueEmpty(rKey));

    return false;
}


//----------------------------------------------------------------------------
/*!	\throw UnknownPropertyNameException 
	\param rVal value to add to list for given key  
	\param rKey name of key to add value to */
void ParameterList::setValue(const ParamName& rKey, const ParamValue& rVal)
{
	ColParamListMapIt it = m_ColParamListMap->find(rKey);

	if ( m_ColParamListMap->end() == it)
	{
		VarString errMsg;
		errMsg.format("key <%s> to set value to is not in parameterlist", rKey.c_str());
		
		ExceptInfo sInfo("basar.cmnutil.ParameterList.setValue", errMsg, __FILE__, __LINE__);
		throw UnknownPropertyNameException(sInfo);
	}

	it->second = rVal;
	
	if (LoggerPool::paramList().isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
	{
		basar::VarString msg;
		msg.format("pair: value <%s> set to key <%s> in parameterlist", 
					rVal.c_str(), 
					rKey.c_str());
		LoggerPool::paramList().forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}
}

//----------------------------------------------------------------------------
/*!	\throw no-throw
	\return iterator to first element of ParameterList */
ParameterList_Iterator ParameterList::begin() const
{
	return ParameterList_Iterator(m_ColParamListMap, m_ColParamListMap->begin());
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return iterator to element one beyond last element of ParameterList */
ParameterList_Iterator ParameterList::end() const
{
	return ParameterList_Iterator(m_ColParamListMap, m_ColParamListMap->end());
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void ParameterList::clear()
{
	m_ColParamListMap.get()->clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \param	strm parameter is default initialized and could be overwritten by any std::ostream object,
			e.g. by the error channel or any object of a ofstream file.
  	\throw  no-throw */
////////////////////////////////////////////////////////////////////////////////////////////////////
void ParameterList::toStream( std::ostream& strm ) const
{
	ColParamListMapConstIt itBegin = m_ColParamListMap->begin();
	ColParamListMapConstIt itEnd   = m_ColParamListMap->end  ();
		
	ColParamListMap::difference_type dt; 

	for (ColParamListMapConstIt it = itBegin; it != itEnd; ++it)
	{
		if ( it == itBegin)
		{
			VarString addr;
			addr.format("0x%0*lx", sizeof(size_t) * 2, this);

			strm << "--- ParameterList @ " << addr
				 << " -------------------" << std::endl;
		}

		dt = std::distance(itBegin, it);

		VarString no;
		no.format("#%-5d", dt);

		strm << no
			 <<       " name : <" << it->first         << ">"      << std::endl
			 << "       value: <" << it->second        << ">"      << std::endl
			 << "------------------------------------------------" << std::endl;
	}
}

//------------------------------------------------------------------------------
VarString ParameterList::toString() const
{
    VarString              retval;
    ColParamListMapConstIt itEnd = m_ColParamListMap->end();
	
    for (ColParamListMapConstIt it = m_ColParamListMap->begin(); it != itEnd; ++it)
    {
        retval += it->first;
		
        if (it->second.size() > 0)
        {
            retval += SIGN_ASSIGN;
            retval += it->second;
        }
		
        retval += SIGN_DELIM;
    }
	
    return retval;
}

//------------------------------------------------------------------------------
bool ParameterList::operator == (const ParameterList & l) const
{
	if (l.size() != size())
		return false;
	
	ColParamListMapConstIt itEnd = m_ColParamListMap->end();
		
	for (ColParamListMapConstIt it = m_ColParamListMap->begin(); it != itEnd; ++it)
	{
		if (! l.isKeyContained(it->first))
			return false;

		if (it->second != l.getValue(it->first)) 
			return false;
	}
		
	return true;
}

//------------------------------------------------------------------------------
}//cmnutil
}//basar

