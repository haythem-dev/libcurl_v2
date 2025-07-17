#ifndef GUARD_ISQLSTRINGBUILDERIMPL_H
#define GUARD_ISQLSTRINGBUILDERIMPL_H
//--------------------------------------------------------------------------------------------------------//
/*! \file
*  \brief  intern interface for building SQL - statements
*  \author Marco Köppendörfer
*  \date   02.04.2014
*/
//--------------------------------------------------------------------------------------------------------//

#include "libbasar_definitions.h"
#include "libbasardbaspect_definitions.h"

//--------------------------------------------------------------------------------------------------------//
// forward declarations
//--------------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace property
    {
        class PropertyType;
    }

    namespace db
    {
        namespace aspect
        {
            class AccessorPropertyTable_YIterator;
        }
    }
}

//--------------------------------------------------------------------------------------------------------//
// namespaces
//--------------------------------------------------------------------------------------------------------//
namespace basar
{
    namespace db
    {
        namespace aspect
        {

//--------------------------------------------------------------------------------------------------------//
// class declaration
//--------------------------------------------------------------------------------------------------------//
            /*! \brief  A interface class for SQLStringBuilderImplementations SQL - statements.

            This is a interface class.
            */
//--------------------------------------------------------------------------------------------------------//
            class ISQLStringBuilderImpl
            {

                public:
                    virtual ~ISQLStringBuilderImpl() {};

                    /*! \brief return the statement. \n no-throw */
                    virtual const basar::cmnutil::BString&              getSQLString()  const = 0;

                    virtual void                                        literal(            const VarString& literal                            ) = 0;

                    /*! \brief append the value of the parametername to the SQL-statement. \n UnknownPropertyNameException */
                    virtual void                                        value(              const PropertyName& rPropertyNames                  ) = 0;

                    /*! \brief solve the parameter and append the pattern to the SQL-statement. \n UnknownPropertyNameException */
                    virtual void                                        resolve(            const VarString& pattern                            ) = 0;

                    /*! \brief Return the value of the parametername. \n UnknownPropertyNameException */
                    virtual const VarString                             getParamValue(      const PropertyName& rPropertyNames                  ) const = 0;

                    /*! \brief Set the PropertyList. \n no-throw */
                    virtual void                                        setPropertyList(    const AccessorPropertyTable_YIterator& propertyList ) = 0;

                    /*! \brief Get the PropertyList. \n no-throw */
                    virtual const AccessorPropertyTable_YIterator       getPropertyList() const = 0;

                    /*! \brief get IndexTypeCollection for this string builder \n no-throw */
                    virtual const IndexTypeCollection& getPreparedParameterTypePositionCollection() const = 0;

                    /*! \brief set default parameters for all sql-statements of this builder \n no-throw */
                    virtual void                                        setParameterList(   const basar::cmnutil::ParameterList paramList ) = 0;

                    /*! \brief get default parameters for all sql-statements of this builder \n no-throw */
                    virtual const basar::cmnutil::ParameterList&        getParameterList() const = 0;

                    //---------------------------------------------------------------------------------------------//
                    // help methods
                    //---------------------------------------------------------------------------------------------//

                    /*! \brief return true, if all given properties of this yiterator are set, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSet(                       const PropertyName& rPropertyNames ) const = 0;

                    /*! \brief return true, if all given properties of this yiterator are set, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSet(                       const basar::cmnutil::ParameterList& rParaList ) const = 0;

                    /*! \brief return true, if all given properties are set and at least one of the given properties
                    is in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSetAndOneMustBeInState(
                        const PropertyName& rPropertyNames,
                        const PropertyStateSet& rState ) const = 0;

                    /*! \brief return true, if all given properties are set and at least one of the given properties
                    is in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSetAndOneMustBeInState(
                        const PropertyName& rPropertyNames,
                        const VarString& rState ) const = 0;

                    /*! \brief return true, if all given properties are set and at least one of the given properties
                    is in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSetAndOneMustBeInState(
                        const basar::cmnutil::ParameterList& rPropertyNames,
                        const PropertyStateSet& rState ) const = 0;

                    /*! \brief return true, if all given properties are set and at least one of the given properties
                    is in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSetAndOneMustBeInState(
                        const basar::cmnutil::ParameterList& rPropertyNames,
                        const VarString& rState ) const = 0;

                    /*! \brief return true, if all given properties are set and all of the given properties
                    are in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSetAndAllInState(
                        const PropertyName& rPropertyNames,
                        const PropertyStateSet& rState ) const = 0;

                    /*! \brief return true, if all given properties are set and all of the given properties
                    are in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSetAndAllInState(
                        const PropertyName& rPropertyNames,
                        const VarString& rState ) const = 0;


                    /*! \brief The method returns true, if all given properties are set and all of the given properties
                    are in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool arePropertiesSetAndAllInState(
                        const basar::cmnutil::ParameterList& rPropertyNames,
                        const PropertyStateSet& rState ) const = 0;

                    /*! \brief The method returns true, if all given properties are set and all of the given properties
                    are in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool arePropertiesSetAndAllInState(
                        const basar::cmnutil::ParameterList& rPropertyNames,
                        const VarString& rState ) const = 0;

                    /*! \brief  return true, if all given properties (1st parameter)
                    are set and at least one of the given properties
                    (2nd parameter) is in one of the given states,
                    so that sqlstring is executable. \n no-throw */
                    virtual bool are1stPropertiesSetAnd2ndOneInState(
                        const basar::cmnutil::ParameterList& plShouldBeSet,
                        const basar::cmnutil::ParameterList& plOneMustBeInState,
                        const PropertyStateSet& pss
                    ) const = 0;

                    /*! \brief  return true, if all given properties (1st parameter)
                    are set and at least one of the given properties
                    (2nd parameter) is in one of the given states,
                    so that sqlstring is executable. \n no-throw */
                    virtual bool are1stPropertiesSetAnd2ndOneInState(
                        const PropertyName& propertyNamesShouldBeSet,
                        const PropertyName& propertyNamesOneMustBeInState,
                        const PropertyStateSet& pss ) const = 0;

                    /*! \brief  return true, if all given properties (1st parameter) are set and at least one of the given properties
                    (2nd parameter) is in one of the given states, so that sqlstring is executable. \n no-throw */
                    virtual bool are1stPropertiesSetAnd2ndOneInState(
                        const PropertyName& propertyNamesShouldBeSet,
                        const PropertyName& propertyNamesOneMustBeInState,
                        const VarString& pss ) const = 0;

                    /*! \brief  return true, if all given properties (1st parameter) are set and at least one of the given properties
                    (2nd parameter) is in one of the given states, so that sqlstring is executable. \n no-throw */
                    virtual bool are1stPropertiesSetAnd2ndOneInState(
                        const basar::cmnutil::ParameterList& propertyNamesShouldBeSet,
                        const basar::cmnutil::ParameterList& propertyNamesOneMustBeInState,
                        const VarString& rState ) const = 0;



                    /*! \brief  return true, if given property is not in unset state and yiterator is not at the end
                    \n no-throw */
                    virtual bool    isNotEndAndSet( const PropertyName& rPropertyName ) const = 0;

                    /*! \brief  return true, if yiterator contains given property and property is not in unset state
                    \n no-throw */
                    virtual bool    isContainedAndSet( const PropertyName& rPropertyName ) const = 0;

                    /*! \brief set member m_SQLString to empty string. \n no-throw */
                    virtual void clear() = 0;
            };

            /*--------------------------------------------------------------------------------------------------*/
        } // aspect
    } // db
} // basar

/*--------------------------------------------------------------------------------------------------*/
#endif
