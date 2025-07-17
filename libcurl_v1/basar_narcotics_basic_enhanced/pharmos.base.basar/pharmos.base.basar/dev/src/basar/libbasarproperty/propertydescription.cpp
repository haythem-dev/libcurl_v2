/** $Id$
*
* @file 
* @author Anke Klink
* @date 2008
*/
//--------------------------------------------------------------------------------------------//

#include "libbasarproperty_propertydescription.h"

//--------------------------------------------------------------------------------------------//
namespace basar
{
namespace property
{

//----------------------------------------------------------------------------------------------
PropertyDescription::PropertyDescription(): m_name(""),
										    m_type(UNKNOWN)
{

}
//----------------------------------------------------------------------------------------------

PropertyDescription::PropertyDescription ( ConstString             name, 
										   const SupportedTypeEnum type)
										 : m_name(name),
										   m_type(type)
{

}

//----------------------------------------------------------------------------------------------
PropertyDescription::PropertyDescription ( ConstString          name, 
										   const PropertyType & type)
										 : m_name(name),
										   m_type(type)
{
}
//----------------------------------------------------------------------------------------------
PropertyDescription::PropertyDescription ( ConstString name, 
										   ConstString type_name)
										 : m_name(name),
										   m_type(type_name)
{
}
//----------------------------------------------------------------------------------------------
PropertyDescription::PropertyDescription ( ConstString name)
                                         : m_name(name),
										   m_type(UNKNOWN)
{
}

//----------------------------------------------------------------------------------------------
bool PropertyDescription::operator == (const PropertyDescription & p) const
{
	bool retval = (p.m_type.getType() == m_type.getType());
	if (retval)
	{
		retval = (p.m_name == m_name);
	}
	return retval;
}

//----------------------------------------------------------------------------------------------

const VarString & PropertyDescription::getName() const
{
	return m_name;
}

//----------------------------------------------------------------------------------------------

const PropertyType & PropertyDescription::getType() const
{
	return m_type;
}

//----------------------------------------------------------------------------------------------

PropertyDescription::operator const VarString() const
{
	return m_name;
}

//----------------------------------------------------------------------------------------------

VarString PropertyDescription::toDescriptionString() const
{
	VarString retval;
	retval += m_type.toString();
	retval += " ";
	retval += m_name;
	retval += ";";
	return retval;
}

//----------------------------------------------------------------------------------------------
VarString PropertyDescription::toPropertyString() const
{
	return (m_name + ";");
}
//----------------------------------------------------------------------------------------------

VarString PropertyDescription::toSQLReplacementString() const
{
	return ("#" + m_name + "#");
}
//----------------------------------------------------------------------------------------------

VarString PropertyDescription::toStringWithTableName(ConstString tablename, bool useAs) const
{
	VarString retval;
	retval.append(tablename);
	retval.append(".");
	retval.append(m_name); 
	if (useAs)
	{
		retval.append(" as ");
		retval.append(m_name);
	}
	return retval; 
}

//----------------------------------------------------------------------------------------------
VarString PropertyDescription::toName(ConstString pre, ConstString post) const
{
	VarString retval;
	retval.append(pre); 
	retval.append(m_name);  
	retval.append(post); 
	return retval; 
}
//----------------------------------------------------------------------------------------------
VarString PropertyDescription::toEqualString(ConstString pre1, ConstString post1,ConstString pre2, ConstString post2) const
{
	return toInEqualString(" = ", pre1, post1, pre2, post2);
}

//----------------------------------------------------------------------------------------------
VarString PropertyDescription::toSQLReplacementString(ConstString pre, ConstString post) const
{
	VarString retval;
	retval.append(pre); 
	retval.append(toSQLReplacementString());  
	retval.append(post); 
	return retval; 
}
//----------------------------------------------------------------------------------------------
VarString PropertyDescription::toEqualString( 
	ConstString preString2, 
	ConstString postString2) const
{ 
	return toEqualString("","", preString2, postString2);
}
//----------------------------------------------------------------------------------------------
VarString PropertyDescription::toInEqualString(
	ConstString equilitySign,  
	ConstString preString1, 
	ConstString postString1,
	ConstString preString2, 
	ConstString postString2) const
{ 
	VarString retval;
	retval.append( toName(preString1, postString1));
	retval.append(equilitySign);
	retval.append(toSQLReplacementString(preString2, postString2)); 
	return retval;
}
//----------------------------------------------------------------------------------------------

VarString PropertyDescription::toInEqualString(  
	ConstString equilitySign,  
	ConstString preString2, 
	ConstString postString2) const
{ 
	return toInEqualString(equilitySign,"","", preString2, postString2);
}

//----------------------------------------------------------------------------------------------

VarString PropertyDescription::toAsStatment(
	ConstString tableName, 
	const PropertyDescription & prop2)const
{ 
	VarString retval = toStringWithTableName(tableName, false);
	retval.append(" as ");
	retval.append(prop2.getName());
	return retval;
}

//----------------------------------------------------------------------------------------------

VarString PropertyDescription::toAsStatment(  
	const PropertyDescription & prop2)const
{ 
	VarString retval = this->getName();
	retval.append(" as ");
	retval.append(prop2.getName());
	return retval;
}
//----------------------------------------------------------------------------------------------

VarString PropertyDescription::toNVLString( 
	ConstString nvlValue,
	ConstString preString1,
	ConstString postString1)const
{
	VarString retval;  
	retval += " NVL(";
	retval += preString1;
	retval += getName();
	retval += postString1;
	retval += ",";
	retval += nvlValue;
	retval += ") ";
	return retval;
}

//-------------------------------------------------------------------------------------------------//
VarString PropertyDescription::toNVLStringAs( 
	ConstString nvlValue,
	ConstString preString1,
	ConstString postString1)const
{
	return toNVLStringAs(*this, nvlValue, preString1,postString1);
}

//-------------------------------------------------------------------------------------------------//
VarString PropertyDescription::toNVLStringAs( 
	const PropertyDescription & prop2,
	ConstString nvlValue,
	ConstString preString1,
	ConstString postString1)const
{
	VarString retval = toNVLString(nvlValue,preString1,  postString1);
	retval += " as ";
	retval += prop2.getName();
	retval += " ";
	return retval;
}

//-------------------------------------------------------------------------------------------------//
}
}
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
