//----------------------------------------------------------------------------
/*! \file
 *  \brief  main interface for all own transformer
 *  \author Roland Kiefert
 *  \date   21.03.2006
 */
//----------------------------------------------------------------------------
#ifndef GUARD_NARCOTICSTRANSFORMER_H
#define GUARD_NARCOTICSTRANSFORMER_H

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotics 
{
namespace viewConn 
{

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
class QuantityTransformer : public basar::gui::tie::Transformer
{
public:
	//! destructor										\n no-throw
					~QuantityTransformer				();

	//! constructor	for property name transformer		\n no-throw
					QuantityTransformer					();

	//! transform values from right to left iterator	\n UnknownPropertyTypeException
	virtual void	RightToLeft							(basar::gui::tie::GuiPropertyTable_YIterator	          guiIt,	//!< iterator to GuiPropertyTable row
														 const basar::db::aspect::AccessorPropertyTable_YIterator accIt	//!< iterator to property table/cache controller row
														);
	//! transform values from left to right iterator	\n TypecastFailedException
	virtual void	LeftToRight							(const basar::gui::tie::GuiPropertyTable_YIterator        guiIt,	//!< iterator to GuiPropertyTable row
														 basar::db::aspect::AccessorPropertyTable_YIterator       accIt	//!< iterator to property table/cache controller row
														);

	//! creates cloned instance							\n no-throw
	virtual boost::shared_ptr < basar::gui::tie::Transformer > 	
														create	();
};

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
class VariableDateTransformer : public basar::gui::tie::Transformer
{
public:
	//! destructor										\n no-throw
					~VariableDateTransformer			();

	//! constructor	for property name transformer		\n no-throw
					VariableDateTransformer				(basar::I18nString propertyname, basar::I18nString fieldname);

	//! transform values from right to left iterator	\n UnknownPropertyTypeException
	virtual void	RightToLeft							(basar::gui::tie::GuiPropertyTable_YIterator	          guiIt,	//!< iterator to GuiPropertyTable row
														 const basar::db::aspect::AccessorPropertyTable_YIterator accIt	//!< iterator to property table/cache controller row
														);
	//! transform values from left to right iterator	\n TypecastFailedException
	virtual void	LeftToRight							(const basar::gui::tie::GuiPropertyTable_YIterator        guiIt,	//!< iterator to GuiPropertyTable row
														 basar::db::aspect::AccessorPropertyTable_YIterator       accIt	//!< iterator to property table/cache controller row
														);

	//! creates cloned instance							\n no-throw
	virtual boost::shared_ptr < basar::gui::tie::Transformer > 	
														create	();

private:
	//! member for declaring name of target property
	basar::I18nString									m_Propertyname;
	//! member for declaring name of target field
	basar::I18nString									m_Fieldname;
};

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
class VariableTimeTransformer : public basar::gui::tie::Transformer
{
public:
	//! destructor										\n no-throw
					~VariableTimeTransformer				();

	//! constructor	for property name transformer		\n no-throw
					VariableTimeTransformer					(basar::I18nString propertyname, basar::I18nString fieldname);

	//! transform values from right to left iterator	\n UnknownPropertyTypeException
	virtual void	RightToLeft							(basar::gui::tie::GuiPropertyTable_YIterator	          guiIt,	//!< iterator to GuiPropertyTable row
														 const basar::db::aspect::AccessorPropertyTable_YIterator accIt	//!< iterator to property table/cache controller row
														);
	//! transform values from left to right iterator	\n TypecastFailedException
	virtual void	LeftToRight							(const basar::gui::tie::GuiPropertyTable_YIterator        guiIt,	//!< iterator to GuiPropertyTable row
														 basar::db::aspect::AccessorPropertyTable_YIterator       accIt	//!< iterator to property table/cache controller row
														);

	//! creates cloned instance							\n no-throw
	virtual boost::shared_ptr < basar::gui::tie::Transformer > 	
														create	();
	
private:
	//! member for declaring name of target property
	basar::I18nString									m_Propertyname;
	//! member for declaring name of target field
	basar::I18nString									m_Fieldname;
};


//-------------------------------------------------------------------------------------------------//
class PrintedCustomerAdressTransformer : public basar::gui::tie::Transformer
{
public:
	//! destructor										\n no-throw
					~PrintedCustomerAdressTransformer				();

	//! constructor	for property name transformer		\n no-throw
					PrintedCustomerAdressTransformer	();

	//! transform values from right to left iterator	\n UnknownPropertyTypeException
	virtual void	RightToLeft							(basar::gui::tie::GuiPropertyTable_YIterator	          guiIt,	//!< iterator to GuiPropertyTable row
														 const basar::db::aspect::AccessorPropertyTable_YIterator accIt	//!< iterator to property table/cache controller row
														);
	//! transform values from left to right iterator	\n TypecastFailedException
	virtual void	LeftToRight							(const basar::gui::tie::GuiPropertyTable_YIterator        guiIt,	//!< iterator to GuiPropertyTable row
														 basar::db::aspect::AccessorPropertyTable_YIterator       accIt	//!< iterator to property table/cache controller row
														);

	//! creates cloned instance							\n no-throw
	virtual boost::shared_ptr < basar::gui::tie::Transformer > 	
														create	();
};

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
class SignTransformer : public basar::gui::tie::Transformer
{
public:
	//! destructor										\n no-throw
					~SignTransformer				();

	//! constructor	for property name transformer		\n no-throw
					SignTransformer					();

	//! transform values from right to left iterator	\n UnknownPropertyTypeException
	virtual void	RightToLeft							(basar::gui::tie::GuiPropertyTable_YIterator	          guiIt,	//!< iterator to GuiPropertyTable row
														 const basar::db::aspect::AccessorPropertyTable_YIterator accIt	//!< iterator to property table/cache controller row
														);
	//! transform values from left to right iterator	\n TypecastFailedException
	virtual void	LeftToRight							(const basar::gui::tie::GuiPropertyTable_YIterator        guiIt,	//!< iterator to GuiPropertyTable row
														 basar::db::aspect::AccessorPropertyTable_YIterator       accIt	//!< iterator to property table/cache controller row
														);

	//! creates cloned instance							\n no-throw
	virtual boost::shared_ptr < basar::gui::tie::Transformer > 	
														create	();
};

//-------------------------------------------------------------------------------------------------//
// class declaration
//-------------------------------------------------------------------------------------------------//
class ProtocolTransformer : public basar::gui::tie::Transformer
{
public:
	//! destructor										\n no-throw
					~ProtocolTransformer				();

	//! constructor	for property name transformer		\n no-throw
					ProtocolTransformer					();

	//! transform values from right to left iterator	\n UnknownPropertyTypeException
	virtual void	RightToLeft							(basar::gui::tie::GuiPropertyTable_YIterator	          guiIt,	//!< iterator to GuiPropertyTable row
														 const basar::db::aspect::AccessorPropertyTable_YIterator accIt	//!< iterator to property table/cache controller row
														);
	//! transform values from left to right iterator	\n TypecastFailedException
	virtual void	LeftToRight							(const basar::gui::tie::GuiPropertyTable_YIterator        guiIt,	//!< iterator to GuiPropertyTable row
														 basar::db::aspect::AccessorPropertyTable_YIterator       accIt	//!< iterator to property table/cache controller row
														);

	//! transform values from left to right iterator	\n TypecastFailedException
	virtual void	LeftToRight							(const basar::VarString							         attributeName,	//!< name of attribute entered by user in gui
														 basar::db::aspect::AccessorPropertyTable_YIterator       accIt	//!< iterator to property table/cache controller row
														);

	//! creates cloned instance							\n no-throw
	virtual boost::shared_ptr < basar::gui::tie::Transformer > 	
														create	();
};

} //namespace viewConn
} //namespace narcotics

#endif //GUARD_NARCOTICSTRANSFORMER_H