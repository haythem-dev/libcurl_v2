//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declaration for branch-accessor
 *  \author Bjoern Bischof
 *  \date   29.06.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_ACC_BRANCH_H
#define GUARD_ACC_BRANCH_H

//----------------------------------------------------------------------------
//! namespace for branch-accessor
namespace acc_branch
{

//---------------------------------------------------------------------------
//! class for selecting branch data from table 'filiale'. \n final class
BEGIN_QUERY_BUILDER_DECLARATION( SelBranch )
END_BUILDER_DECLARATION

//! class for selecting branch address from table 'filiale'. \n final class
BEGIN_QUERY_BUILDER_DECLARATION( SelBranchAddress )
END_BUILDER_DECLARATION

} //namespace acc_branch

#endif	//GUARD_ACC_BRANCH_H