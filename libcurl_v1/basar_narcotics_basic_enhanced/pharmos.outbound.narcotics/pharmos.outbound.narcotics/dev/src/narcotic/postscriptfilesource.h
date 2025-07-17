#ifndef GUARD_POSTSCRIPTFILESOURCE
#define GUARD_POSTSCRIPTFILESOURCE

//---------------------------------------------------------------------------
//include section
//---------------------------------------------------------------------------
#include "iprerequisitedatasource.h"

#pragma warning (push) // to avoid warnings from QT always put QT-includes between push - pop
#pragma warning(disable: 4127 4244 4311 4312 4481 4512 4800)
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#pragma warning (pop)

#include <set>

//---------------------------------------------------------------------------
//namespace section
//---------------------------------------------------------------------------
namespace narcotic
{
	
namespace dataAccess
{

//---------------------------------------------------------------------------
class PostScriptFileSource : public narcotic::useCase::services::IPrerequisiteDataSource
{
private:

	mutable QFile m_File;

	mutable std::set<QString> m_UsernameCache;

	bool doesCacheContainString( const basar::VarString& searchedString ) const;

	bool doesFileContainString( const basar::VarString& searchedString ) const;

	bool doesLineContainString( const QString& line,const QString& searchedString )const;


public:
	virtual bool doesContainString( const basar::VarString& searchedString ) const;
	
	virtual bool doesContainBitmapForUserName( basar::VarString username ) const;

	PostScriptFileSource( const basar::VarString& path );
	virtual ~PostScriptFileSource();
};

} //namespace dataAccess
} //namespace narcotic

#endif //GUARD_POSTSCRIPTFILESOURCE