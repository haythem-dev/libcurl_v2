//----------------------------------------------------------------------------
/*! \file
 *  \brief  string builder declaration for protocol-accessor
 *  \author Kerstin Reuther
 *  \date   21.03.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_ACC_NARCOTICSCATALOG_H
#define GUARD_ACC_NARCOTICSCATALOG_H

//----------------------------------------------------------------------------
//! namespace for protocol-accessor
namespace acc_narcoticsprotocol
{

//---------------------------------------------------------------------------//
// SELECTS
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
//! class for selecting from narcprotocol \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelPro)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------//
// INSERTS
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------
//! class for inserting in narcprotocol when  \n final class
BEGIN_WRITE_BUILDER_DECLARATION(InsProUpdate)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for inserting in narcprotocol \n final class
BEGIN_WRITE_BUILDER_DECLARATION(InsProDelete)
END_BUILDER_DECLARATION

//---------------------------------------------------------------------------
//! class for inserting in narcprotocol \n final class
BEGIN_WRITE_BUILDER_DECLARATION(InsProRestore)
END_BUILDER_DECLARATION

} //namespace acc_narcoticsprotocol

namespace acc_narcoticstransactiondata {
//---------------------------------------------------------------------------
//! class for selecting from narctransactioncatalog \n final class
BEGIN_QUERY_BUILDER_DECLARATION(SelTCat)
END_BUILDER_DECLARATION
}

#endif //GUARD_ACC_NARCOTICSCATALOG_H