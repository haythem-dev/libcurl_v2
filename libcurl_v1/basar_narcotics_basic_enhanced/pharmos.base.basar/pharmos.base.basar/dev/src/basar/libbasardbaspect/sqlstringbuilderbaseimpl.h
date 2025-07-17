#ifndef GUARD_SQLSTRINGBUILDERBASE_H
#define GUARD_SQLSTRINGBUILDERBASE_H
//--------------------------------------------------------------------------------------------------------//
// includes
//--------------------------------------------------------------------------------------------------------//
#include "libbasar_definitions.h"
#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_parameterlist.h"
#include "libbasardbaspect_definitions.h"
#include "libbasardbaspect_accessorpropertytable_yiterator.h"
#include "isqlstringbuilderimpl.h"

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
            /*! \brief  A base class for hosting common methods used by deriveds.

            This is an abstract class.
            */
//--------------------------------------------------------------------------------------------------------//
            class SQLStringBuilderBaseImpl : public ISQLStringBuilderImpl
            {
                public:

                    virtual ~SQLStringBuilderBaseImpl();

                    /*! \brief return the statement. \n no-throw */
                    virtual const basar::cmnutil::BString&              getSQLString()  const;

                    virtual void                                        literal(            const VarString& literal                            );

                    /*! \brief append the value of the parametername to the SQL-statement. \n UnknownPropertyNameException */
                    virtual void                                        value(              const PropertyName& rPropertyNames                  );

                    /*! \brief solve the parameter and append the pattern to the SQL-statement. \n UnknownPropertyNameException */
                    virtual void                                        resolve(            const VarString& pattern                            ) = 0;

                    /*! \brief Return the value of the parametername. \n UnknownPropertyNameException */
                    virtual const VarString                             getParamValue(      const PropertyName& rPropertyNames                  ) const;

                    /*! \brief Set the PropertyList. \n no-throw */
                    virtual void                                        setPropertyList(    const AccessorPropertyTable_YIterator& propertyList );

                    /*! \brief Get the PropertyList. \n no-throw */
                    virtual const AccessorPropertyTable_YIterator       getPropertyList() const;

                    /*! \brief get IndexTypeCollection for this string builder \n no-throw */
                    virtual const IndexTypeCollection& getPreparedParameterTypePositionCollection() const;

                    /*! \brief set default parameters for all sql-statements of this builder \n no-throw */
                    virtual void                                        setParameterList(   const basar::cmnutil::ParameterList paramList );

                    /*! \brief get default parameters for all sql-statements of this builder \n no-throw */
                    virtual const basar::cmnutil::ParameterList&        getParameterList() const;

                    //---------------------------------------------------------------------------------------------//
                    // help methods
                    //---------------------------------------------------------------------------------------------//

                    /*! \brief return true, if all given properties of this yiterator are set, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSet(                       const PropertyName& rPropertyNames ) const;

                    /*! \brief return true, if all given properties of this yiterator are set, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSet(                       const basar::cmnutil::ParameterList& rParaList ) const;

                    /*! \brief return true, if all given properties are set and at least one of the given properties
                    is in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSetAndOneMustBeInState(
                        const PropertyName& rPropertyNames,
                        const PropertyStateSet& rState ) const;

                    /*! \brief return true, if all given properties are set and at least one of the given properties
                    is in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSetAndOneMustBeInState(
                        const PropertyName& rPropertyNames,
                        const VarString& rState ) const;

                    /*! \brief return true, if all given properties are set and at least one of the given properties
                    is in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSetAndOneMustBeInState(
                        const basar::cmnutil::ParameterList& rPropertyNames,
                        const PropertyStateSet& rState ) const;

                    /*! \brief return true, if all given properties are set and at least one of the given properties
                    is in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSetAndOneMustBeInState(
                        const basar::cmnutil::ParameterList& rPropertyNames,
                        const VarString& rState ) const;

                    /*! \brief return true, if all given properties are set and all of the given properties
                    are in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSetAndAllInState(
                        const PropertyName& rPropertyNames,
                        const PropertyStateSet& rState ) const;

                    /*! \brief return true, if all given properties are set and all of the given properties
                    are in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool                                        arePropertiesSetAndAllInState(
                        const PropertyName& rPropertyNames,
                        const VarString& rState ) const;


                    /*! \brief The method returns true, if all given properties are set and all of the given properties
                    are in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool arePropertiesSetAndAllInState(
                        const basar::cmnutil::ParameterList& rPropertyNames,
                        const PropertyStateSet& rState ) const;

                    /*! \brief The method returns true, if all given properties are set and all of the given properties
                    are in one of the given states, so that sqlstring is executable
                    \n     no-throw */
                    virtual bool arePropertiesSetAndAllInState(
                        const basar::cmnutil::ParameterList& rPropertyNames,
                        const VarString& rState ) const;

                    /*! \brief  return true, if all given properties (1st parameter)
                    are set and at least one of the given properties
                    (2nd parameter) is in one of the given states,
                    so that sqlstring is executable. \n no-throw */
                    virtual bool are1stPropertiesSetAnd2ndOneInState(
                        const basar::cmnutil::ParameterList& plShouldBeSet,
                        const basar::cmnutil::ParameterList& plOneMustBeInState,
                        const PropertyStateSet& pss
                    ) const;

                    /*! \brief  return true, if all given properties (1st parameter)
                    are set and at least one of the given properties
                    (2nd parameter) is in one of the given states,
                    so that sqlstring is executable. \n no-throw */
                    virtual bool are1stPropertiesSetAnd2ndOneInState(
                        const PropertyName& propertyNamesShouldBeSet,
                        const PropertyName& propertyNamesOneMustBeInState,
                        const PropertyStateSet& pss ) const;

                    /*! \brief  return true, if all given properties (1st parameter) are set and at least one of the given properties
                    (2nd parameter) is in one of the given states, so that sqlstring is executable. \n no-throw */
                    virtual bool are1stPropertiesSetAnd2ndOneInState(
                        const PropertyName& propertyNamesShouldBeSet,
                        const PropertyName& propertyNamesOneMustBeInState,
                        const VarString& pss ) const;

                    /*! \brief  return true, if all given properties (1st parameter) are set and at least one of the given properties
                    (2nd parameter) is in one of the given states, so that sqlstring is executable. \n no-throw */
                    virtual bool are1stPropertiesSetAnd2ndOneInState(
                        const basar::cmnutil::ParameterList& propertyNamesShouldBeSet,
                        const basar::cmnutil::ParameterList& propertyNamesOneMustBeInState,
                        const VarString& rState ) const;

                    /*! \brief  return true, if given property is not in unset state and yiterator is not at the end
                    \n no-throw */
                    virtual bool    isNotEndAndSet( const PropertyName& rPropertyName ) const;

                    /*! \brief  return true, if yiterator contains given property and property is not in unset state
                    \n no-throw */
                    virtual bool    isContainedAndSet( const PropertyName& rPropertyName ) const;

                    /*! \brief set member m_SQLString to empty string. \n no-throw */
                    virtual void clear();

                protected:
                    //----------//
                    // methods
                    //----------//

                    /*! \brief returns the unset Parameter */
                    basar::cmnutil::ParameterList getNonDefaultParameter( const basar::cmnutil::ParameterList& list1 ) const;

                    std::pair<basar::cmnutil::ParameterList, basar::cmnutil::ParameterList>
                    partParamterListInDefaultandNondefaultParameter( const basar::cmnutil::ParameterList& list1 ) const;

                    /*! \brief  convert an as string passed state to a propertystateset. \n no-throw */
                    const PropertyStateSet                          convertToPropertyStateSet(  const VarString& rState ) const;

                    /*! \brief find the first ? outside ' '. \n no-throw */
                    void                                            findFirstSign(          VarString& rPattern,
                            const VarString::size_type begin,
                            VarString::size_type& end ) const;

                    /*! helper method to find the appropriate Substring */
                    VarString                                       getSubstringToNextMaskString(
                        const VarString& maskstring,   //!< maskstring
                        const VarString& rPattern,     //!< patern
                        VarString::size_type begin,    //!< startingpoint
                        VarString::size_type& end      //!< endpoint
                    ) const;

                    //----------//
                    // members
                    //----------//
                    //to save a few function calls
                    static const VarString scm_maskStartString; //!< start String to mask a value replacement
                    static const VarString scm_maskEndString;   //!< end String to mask a value replacement
                    static const VarString scm_maskMaskString;  //!< mask String to mask a value replacement mask string

                private:

                    VarString                                   m_SQLString;    //!< SQL-statement
                    AccessorPropertyTable_YIterator             m_Yit;          //!< y-Iterator to a cacher
                    basar::cmnutil::ParameterList               m_ParamList;    /*!< list of default parameters; set once for each
                                                                                     stringbuilder; used for all statements */
            };
        }
    }
}

#endif
