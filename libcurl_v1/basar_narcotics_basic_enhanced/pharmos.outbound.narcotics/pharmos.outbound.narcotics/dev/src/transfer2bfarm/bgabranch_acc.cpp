//------------------------------------------------------------------------------
//include
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "bgabranch_acc.h"
#include "definitions.h"
#include "loggerpool.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace bgabranch_acc
{

//------------------------------------------------------------------------------
BUILDER_DEFINITION( SelectBranchDataByBGANo )
ENSURE_PROPERTIES_ARE_SET( SelectBranchDataByBGANo, narcotics::constants::BGANO )

//------------------------------------------------------------------------------
void SelectBranchDataByBGANo::buildSQLString()
{
    basar::I18nString sqlStatement;

	sqlStatement.append( "select filialnr AS " + narcotics::constants::BRANCHNO.getName() + " " );
	sqlStatement.append( "from filiale where bganr_ek = " + narcotics::constants::BGANO.toSQLReplacementString() + " " );

    resolve( sqlStatement );

	
	BLOG_TRACE( LoggerPool::getLoggerTransferBfarm(), getSQLString() );
}

//------------------------------------------------------------------------------
BEGIN_ACCESSOR_DEFINITION("AccBGABranch")

	BEGIN_PROPERTY_DESCRIPTION_LIST
		PROPERTY_DESCRIPTION_LIST_ADD(narcotics::constants::BGANO);
		PROPERTY_DESCRIPTION_LIST_ADD(narcotics::constants::BRANCHNO);
	END_PROPERTY_DESCRIPTION_LIST

//------------------------------------------------------------------------------
ACCESS_METHOD("amSelectBranchData")
		SQL_BUILDER_CREATE(SelectBranchDataByBGANo)
		SQL_BUILDER_PUSH_BACK(SelectBranchDataByBGANo)

//------------------------------------------------------------------------------
END_ACCESSOR_DEFINITION

} //namespace bgabranch_acc
} //namespace narcotics