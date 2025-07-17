//-------------------------------------------------------------------------------------------------//
/*! \file
 *  \brief  implemtation of domain module "article"
 *  \author	Roland Kiefert
 *  \date   03.04.2006
 */
//-------------------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "articlemodule.h"
#include "loggerpool.h"

//-------------------------------------------------------------------------------------------------//
// namespaces
//-------------------------------------------------------------------------------------------------//
namespace domMod
{

//-------------------------------------------------------------------------------------------------//
// using declarations
//-------------------------------------------------------------------------------------------------//
using basar::Int16;
using basar::Int32;
using basar::db::aspect::ConnectionRef;
using basar::db::aspect::AccessorPropertyTableRef;
using basar::db::aspect::AccessorPropertyTable_YIterator;

//-------------------------------------------------------------------------------------------------//
/*!	\throw		no-throw */
//-------------------------------------------------------------------------------------------------//
ArticleDM::ArticleDM()
:m_Initialized( 0 )
{
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void ArticleDM::init( const Int16 branchNo, const ConnectionRef activeConn )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "init of ArticleDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( 0 == m_Initialized )
	{
		m_accArticle	= basar::db::aspect::Manager::getInstance().createAccessorInstance( 
							"Acc_CentralArticle", "CentralArticle", activeConn );
		basar::VarString defParam;
		defParam.format("%s=%d;",
			constants::BRANCHNO,
			branchNo);
		m_accArticle.setDefaultParameter(defParam);
	}
	++m_Initialized;
}

//-------------------------------------------------------------------------------------------------//
/*!	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
void ArticleDM::shutdown()
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::ConstString msg = "shutdown of ArticleDM.";
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	if( m_Initialized == 0 ){
		return;
	}
	--m_Initialized;
	if( 0 == m_Initialized ){
		m_accArticle.reset();
	}
}

//-------------------------------------------------------------------------------------------------//
/*!	\return a iterator of a property table line
	\throw no-throw */
//-------------------------------------------------------------------------------------------------//
AccessorPropertyTable_YIterator ArticleDM::findArticleByPattern( AccessorPropertyTable_YIterator yit )
{
	if( narcotics::LoggerPool::loggerDomModules.isEnabledFor(log4cplus::TRACE_LOG_LEVEL) )
	{
		basar::VarString msg = "findArticleByPattern of ArticleDM. Parameter: \n ";
		for( basar::db::aspect::AccessorPropertyTable_XIterator xit = yit.begin(); xit != yit.end(); ++xit )
		{
			if( !(xit.getState() == basar::SS_UNSET) ){
				msg = msg + xit.getName() + ":" + xit.getSQLString() + "\n";
			}
		}
		narcotics::LoggerPool::loggerDomModules.forcedLog(log4cplus::TRACE_LOG_LEVEL, msg.c_str(), __FILE__, __LINE__);
	}

    m_accArticle.getPropertyTable().clear();

    basar::db::aspect::ExecuteResultInfo res = m_accArticle.execute( "SelArticle", yit );

	return m_accArticle.getPropertyTable().begin();
}

//-------------------------------------------------------------------------------------------------//
basar::Int32 ArticleDM::findArticleNoByArticleCode( basar::db::aspect::AccessorPropertyTable_YIterator yit )
{
	std::pair<bool, basar::cmnutil::Decimal> res;
	res = m_accArticle.executeAggregate( "SelArticleNoByArticleCode", yit );
	if( res.first == true ){
		return res.second.toInt32();
	}
	return -1;
}

} //namespace domMod