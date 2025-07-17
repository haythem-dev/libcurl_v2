
#include <libbasarcmnutil.h>

#include "testcmnutil.h"

#ifdef WIN32
#include <gsprint.h>
#endif

#ifdef WIN32
#include <io.h>
#endif

#include <fcntl.h>


#include "printdata.h"

using basar::cmnutil::CollPrintRawInput;

#ifdef WIN32
#define PRINTPATH	"/marathon/lib/basar/test/component/cmnutil/print/"
//#define PRINTPATH	"/marathon/test/bpp/print/"
#else
#define PRINTPATH	"print/"
#endif

//#define LPD			"129.100.57.30"		// -> wfupr30: location at ME's workplace; must be running
//#define LPD				"129.100.1.63"		// TALLY 9035
#define LPD				"129.100.80.112"	// HP LJ 4250
//#define LPD				"wfupr30"
//#define LPD				"123.123.123.212"


//------------------------------------------------------------------------------
class PrintDataProvider : public basar::cmnutil::IPrintDataProvider
{
public:
						PrintDataProvider()
						{
							file = open(
								        //PRINTPATH "oceformular.data", O_RDONLY);
										//PRINTPATH "oceformular.ps",O_RDONLY);
										PRINTPATH "bookdata.txt",O_RDONLY);
										//"bookdata.txt",O_RDONLY);
										//PRINTPATH "b.data",O_RDONLY);
										//PRINTPATH "colourswin.ps",O_RDONLY);
										//"/temp/pr/hp2300", O_RDONLY|O_BINARY);
										//"/marathon/test/lpr/b.data", O_RDONLY);

						};

						~PrintDataProvider()
						{
							close(file);
						};

	virtual void		onSendData				(CollPrintRawInput & collPrintRawInput)
	{
		static const Int32 BUFLEN = 8192;
		char               buffer[BUFLEN];
		Int32              readbytes;
		const char * pData;

		if (0 < (readbytes = read(file, buffer, BUFLEN - 1)) )
		{
			for (pData = buffer; readbytes > 0; --readbytes, ++pData)
				collPrintRawInput.push(*pData);
		}
	};

	VarString getFormName() const
	{
		return "NarcoticsBook";
	}

	//------------------------------------------------------------------------------
	VarString getFormFileName() const
	{
		return  "NarcoticsBook";
				// or "%flash%NarcoticsBook";
				// or "/NarcoticsBook";
	}

	Int32 file;
};

//------------------------------------------------------------------------------
static void test_rawprint1()
{
	printf("\n");
	printf("--- test_rawprint1 ---------------------------\n");
	printf("\n");

	static ConstString server  = LPD;
	static ConstString printer = "lp";

	log4cplus::Logger logger = basar::cmnutil::Logging::getInstance("testcmnutil.test_rawprint1");

	basar::cmnutil::PrintRetCodeEnum eRet;

	{
		BLOG_INFO(logger, "start test");

		PrintDataProvider prov;

		basar::cmnutil::RawPrint rwpr;

		eRet = rwpr.print ( server, 
							printer, 
							prov);

		BLOG_INFO(logger, "stop test");
	}

	printf("\n printed with data provider returns %d\n\n", eRet);
}

//------------------------------------------------------------------------------
static void test_rawprint2()
{
	printf("\n");
	printf("--- test_rawprint2 ---------------------------\n");
	printf("\n");

	static ConstString server  = LPD;
	static ConstString printer = "lp";

	basar::cmnutil::RawPrint rwpr;

	ConstString file = PRINTPATH "a.data";

	rwpr.print(server, printer, file);

	printf("\n%s printed via lpr\n\n", file);
}

//------------------------------------------------------------------------------
static void test_rawprint3()
{
	printf("\n");
	printf("--- test_rawprint3 ---------------------------\n");
	printf("\n");

	log4cplus::Logger logger = basar::cmnutil::Logging::getInstance("testcmnutil.test_rawprint1");

	basar::cmnutil::RawPrint rwpr0;
}

//------------------------------------------------------------------------------
void test_printing()
{
#ifdef TEST_XXX

	test_rawprint1();

#endif

#ifdef TEST_YYY
	test_gsprint();
#endif
}
