#ifndef GUARD_PRINTFOLDERLOCATOR
#define GUARD_PRINTFOLDERLOCATOR

//-------------------------------------------------------------------------------------------------//
// namespace
//-------------------------------------------------------------------------------------------------//
namespace narcotic
{
namespace useCase
{
namespace services
{

class PrintFolderLocator
{
public:
	static basar::VarString getPrintFolderPath(); 

	PrintFolderLocator( void );
	~PrintFolderLocator( void );

private:
	static QString recGetPrintFolderPath( QDir currentDir, QString& searchedFolderName );
};

} //namespace services
} //namespace useCase
} //namespace narcotic

#endif //GUARD_PRINTFOLDERLOCATOR