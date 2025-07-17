//-------------------------------------------------------------------------------------------------//
// includes
//-------------------------------------------------------------------------------------------------//
#include "commonheader.h"
#include "printfolderlocator.h"

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotic
{
namespace useCase
{
namespace services
{

//-------------------------------------------------------------------------------------------------//
QString PrintFolderLocator::recGetPrintFolderPath( QDir currentDir, QString& searchedFolderName )
{
	QFileInfoList files = currentDir.entryInfoList();
	QString name = currentDir.absolutePath();

	for( int i=0; i<files.size(); i++ )
	{
		QFileInfo fileInfo = files.at(i);
		name = fileInfo.absoluteFilePath();

		if( fileInfo.isDir() )
		{
			name = fileInfo.absoluteFilePath();

			if( fileInfo.absoluteFilePath().contains( searchedFolderName ) )
			{
				name = fileInfo.absoluteFilePath();
				return fileInfo.absoluteFilePath();
			}			
		}
	}
	
	if( !currentDir.isRoot() )
	{
		currentDir.cdUp();
		name = currentDir.absolutePath();
		QString pathResult = recGetPrintFolderPath( currentDir, searchedFolderName );

		return pathResult;
	}
	return "";
}

//-------------------------------------------------------------------------------------------------//
basar::VarString PrintFolderLocator::getPrintFolderPath()
{
	QDir currentDir( QCoreApplication::applicationDirPath() );	
	QString printFolderName( constants::PRINTFOLDERNAME );
	QString pathResult = recGetPrintFolderPath( currentDir, printFolderName );

	return basar::VarString( pathResult.toLocal8Bit() );	
}

//-------------------------------------------------------------------------------------------------//
PrintFolderLocator::PrintFolderLocator( void )
{
}

//-------------------------------------------------------------------------------------------------//
PrintFolderLocator::~PrintFolderLocator( void )
{
}

} //namespace services
} //namespace useCase
} //namespace narcotic