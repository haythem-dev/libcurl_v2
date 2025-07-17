
#include <libbasarcmnutil.h>

#ifdef WIN32
#include <io.h>
#endif

#include <fcntl.h>


#include "testcmnutil.h"

using basar::cmnutil::PipedProcess;
using basar::cmnutil::CollStdIo;

#ifdef WIN32
#define PRINTPATH	"\\marathon\\lib\\basar\\test\\component\\cmnutil\\print\\"
#else
#define PRINTPATH	"print/"
#endif

//------------------------------------------------------------------------------
class DataProvider : public basar::cmnutil::IProcessDataProvider
{
public:
						DataProvider()
						{
							file = open(PRINTPATH // "oceformular.data",
												  //"oceformular.ps",
												  "a.data",
												 //"colourswin.ps",
														O_RDONLY); 
						};
						~DataProvider()
						{
							close(file);
						};

	virtual void		onSendInput				(CollStdIo & collInput)
	{
		static const Int32 BUFLEN = 8192;
		char               buffer[BUFLEN];
		Int32              readbytes;
		const char * pData;

		if (0 < (readbytes = read(file, buffer, BUFLEN - 1)) )
		{
			for (pData = buffer; readbytes > 0; --readbytes, ++pData)
				collInput.push(*pData);
		}
	};

	virtual void		onReceiveOutput			(CollStdIo & collOutput)
	{
		while ( !collOutput.empty() )
		{
			printf("%c", collOutput.front());
			collOutput.pop();
		}
	};

	virtual void		onReceiveError			(CollStdIo & collError )
	{
		while ( !collError.empty() )
		{
			m_Error.push(collError.front());
			collError.pop();
		}
	};

	std::queue < char > m_Error;

	Int32 file;

};


//------------------------------------------------------------------------------
static void test_dataprovider()
{
	printf("\n");
	printf("--- test_dataprovider ---------------------------\n");
	printf("\n");

#ifdef WIN32
	static ConstString cmd =	//"nc -w 5 129.100.57.30 9100";
								"nc -h";
								//"more";
	                        	//"dir";
								//"sort /o sort.txt";
								//"sed -e s/RETOUREN/neruoter/";
								//"sed -e s/d/f/";// >" PRINTPATH "sed.txt";
								//"child ";//>" PRINTPATH "out.txt";

#else
	static ConstString cmd = "lpr -P129.100.57.30:lp";
	//static ConstString cmd = "cat"; // > out.txt";
#endif




	DataProvider prov;

	printf("=======================================================================\n");

	printf("stdout:\n");
	printf("<BOF>\n");

	PipedProcess proc(cmd, prov);
	Int32 retcode = proc.execute();
/*
	lseek(file, 0L, SEEK_SET );
	while (0 < (readbytes = read(file, buffer, BUFLEN - 1)) )
	{
		for (pData = buffer; readbytes > 0; --readbytes, ++pData)
			prov.m_Input.push(*pData);
	}
	retcode = proc.execute();
*/
	printf("\n\n\n\"%s\" = ", cmd);
	printf("%d\n", retcode);
	printf("<EOF>\n");

	// --- error
	printf("\nstderror:\n");
	printf("<BOF>\n");
	while (!prov.m_Error.empty())
	{
		printf("%c", prov.m_Error.front());
		prov.m_Error.pop();
	}
	printf("<EOF>\n");

}

//------------------------------------------------------------------------------
static void test_1()
{
	printf("\n");
	printf("--- test_1 ---------------------------\n");
	printf("\n");

#ifdef WIN32
	static ConstString cmd[] = 
	{
		"set	ddd\teee",
		//"set",
		"nc -h",
		"dir",
		"echo %HOMEPATH%",
		//"lpr -S1.1.1.1 -Plp \\oceformular.trigger",
		//"lpr -S1.1.1.1 -Plp \\marathon\\lib\\basar\\test\\component\\cmnutil\\print\\oceformular.trigger"
		//"\\marathon\\lib\\basar\\test\\component\\cmnutil\\process\\child.exe",
	};
#else
	static ConstString cmd[] = 
	{
		"echo $HOME",
		"set",
		"ls	 -l",
		"lpr -P1.1.1.1:lp /oceformular.trigger",
		"lpr -P1.1.1.1:lp $HOME/marathon/lib/basar/test/component/cmnutil/print/oceformular.trigger"
	};
#endif

	for (UInt32 i = 0; i < sizeof(cmd)/sizeof(*cmd); ++i)
	{
		printf("\n\n\n\"%s\" = ", cmd[i]);
		PipedProcess proc(cmd[i]);
		Int32 retcode = proc.execute();

		printf("%d\n", retcode);
		printf("=======================================================================\n");

		printf("stdout:\n");
		CollStdIo & rColl = proc.getOutput();

		printf("<BOF>\n");

		//printf("size %d\n", rColl.size());

		while ( !rColl.empty() )
		{
			printf("%c", rColl.front());
			rColl.pop();
		}

		printf("<EOF>\n");

		// --- error
		printf("\nstderror:\n");
		rColl = proc.getError();

		printf("<BOF>\n");

		while ( !rColl.empty() )
		{
			printf("%c", rColl.front());
			rColl.pop();
		}
	
		printf("<EOF>\n");
	}
}

//------------------------------------------------------------------------------
void test_pipedprocess()
{
#ifdef MMM

	test_1();

#else

	test_dataprovider();

#endif
}
