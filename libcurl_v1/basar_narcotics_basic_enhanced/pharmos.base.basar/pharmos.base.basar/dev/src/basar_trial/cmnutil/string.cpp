#include <libbasarcmnutil.h>

#include "testcmnutil.h"

#ifdef WIN32

#define VC_EXTRALEAN			//!< less Visual C headers
#define WIN32_LEAN_AND_MEAN		//!< less Windows headers
#include <windows.h>

#endif

//------------------------------------------------------------------------------
using basar::cmnutil::Locale;
using basar::cmnutil::CharString;
using basar::cmnutil::BString;

//------------------------------------------------------------------------------
void tests()
{
	printf("\n");
	printf("--- test string ---------------------------\n");
	printf("\n");

	{ // c'tor
		printf("BString()                  <%s>\n", BString(      ).c_str());
		printf("BString('b')               <%s>\n", BString('b'   ).c_str());
		printf("BString(\"test\")            <%s>\n", BString("test").c_str());

		CharString cpCh = "copyCharString";
		printf("BString(\"copyCharString\")  <%s>\n", BString(cpCh  ).c_str());

		BString cpB     = "copyBString";
		printf("BString(\"copyBString\")     <%s>\n", BString(cpB   ).c_str());

		printf("BString(\"copyBString\", 0 , 1)     <%s>\n", BString(cpB, 0,  1).c_str());
		printf("BString(\"copyBString\", 1 , 1)     <%s>\n", BString(cpB, 1,  1).c_str());
		printf("BString(\"copyBString\", 1, 40)     <%s>\n", BString(cpB, 1, 40).c_str());
		try
		{
			printf("BString(\"copyBString\", 40, 1)     <%s>\n", BString(cpB,40, 1).c_str());
		}
		catch (const std::out_of_range & except)
		{
			printf("BString(\"copyBString\", 40, 1): std::out_of_range exception caught <%s> => ok\n", except.what());
		}

		BString range = "0123456789";
		printf("\"%s\"[1, 8]   <%s>\n", range.c_str(), BString(range.begin() + 1, range.end() - 1).c_str());
	}

	printf("\n");

	{	// substr(), right(), left()
		BString org = "0123456789";
		printf("org <%s>\n", org.c_str());

		printf("left('3')     <%s>, right('3') <%s>\n", org.left('3').c_str(), org.right('3').c_str());
		printf("left('a')     <%s>, right('a') <%s>\n", org.left('a').c_str(), org.right('a').c_str());

		printf("left(\"345\") <%s>, right(\"345\") <%s>\n", org.left("345").c_str(), org.right("345").c_str());
		printf("left(\"35\")  <%s>, right(\"35\" ) <%s>\n", org.left("35" ).c_str(), org.right("35" ).c_str());

		printf("substr(0, 100) <%s>, substr(0, 8) <%s>\n", org.substr(0, 100).c_str(), org.substr(0, 8).c_str());
		printf("substr(4, 100) <%s>, substr(4, 2) <%s>\n", org.substr(4, 100).c_str(), org.substr(4, 2).c_str());

		try
		{
			printf("substr(100, 3) <%s>\n", org.substr(100, 3).c_str());
		}
		catch (const std::out_of_range & except)
		{
			printf("substr(100, 3): std::out_of_range exception caught <%s> => ok\n", except.what());
		}
	}

	printf("\n");

	{	// digit
		BString strTest = " 123";
		printf("string: <%s>, isDigit() %s\n", strTest.c_str(), strTest.isDigit() ? "true" : "false");
		strTest = " -123";
		printf("string: <%s>, isDigit() %s\n", strTest.c_str(), strTest.isDigit() ? "true" : "false");
		strTest = " 123-";
		printf("string: <%s>, isDigit() %s\n", strTest.c_str(), strTest.isDigit() ? "true" : "false");
		strTest = " 123f";
		printf("string: <%s>, isDigit() %s\n", strTest.c_str(), strTest.isDigit() ? "true" : "false");
	}

	printf("\n");

	{	// string to int, ...
		BString hallo1 = "1234";
		basar::Int32 i = hallo1.stoi();
		printf("string: <%s> => int  %d\n", hallo1.c_str(), i);

		hallo1 = "45678901";
		basar::Long32 l = hallo1.stol();
		printf("string: <%s> => long %ld\n", hallo1.c_str(), l);

		basar::Float64 f ;

		{
			Locale::setLocale(Locale::ENGLISH_GB);
			printf("set locale Locale::ENGLISH\n");

			hallo1 = "123.456";
			f = hallo1.stof();
			printf("string: <%s> => float %f\n", hallo1.c_str(), f);

			hallo1 = "123,456";
			f = hallo1.stof();
			printf("string: <%s> => float %f\n", hallo1.c_str(), f);

			hallo1.ftos(f);
			printf("float %f     => string <%s>\n", f, hallo1.c_str());
		
			// ---------

			Locale::setLocale(Locale::GERMAN_DE);
			printf("set locale Locale::GERMAN\n");

			hallo1 = "123.456";
			basar::Float64 g = hallo1.stof();
			printf("string: <%s> => float %f\n", hallo1.c_str(), g);

			hallo1 = "123,456";
			g = hallo1.stof();
			printf("string: <%s> => float %f\n", hallo1.c_str(), g);

			hallo1.ftos(f);
			printf("float %f     => string <%s>\n", f, hallo1.c_str());
		}

		hallo1.itos(i);
		printf("int %d        => string <%s>\n", i, hallo1.c_str());
		
		BString hallo2;
		hallo2.ltos(l);
		printf("long %ld      => string <%s>\n", l, hallo2.c_str());
	}


	printf("\n");

	{	// upper
		ConstString c   = "ÄÖÜäöü<key1>=<value1>ABCabcß§$%<key2><key3> = <value3>"; 
		VarString   str1 = c;
		VarString   str2 = c;
		VarString   msg;

		str1.upper();
		str2.lower();
		msg.format("string  : \"%s\"\n"
			       "\n"
		           "upper:\t\"%s\"\n" 
			       "lower:\t\"%s\"\n", 
				   c, str1.c_str(), str2.c_str());

#ifdef WIN32
		MessageBox(0, msg.c_str(), "", MB_OK);
#else
		printf(msg.c_str());
#endif
	}

	printf("\n");

	{	// cleanchar, reverse
		ConstString c   = "abc\tde\r\nfg\nhiöjkl";
		VarString   str = c;
		str.cleanchar();
		printf("string: <%s>; cleaned: <%s>\n", c, str.c_str());

		str = "abcde";
		printf("string <%s>, ", str.c_str());
		str.reverse();
		printf("reversed <%s>\n", str.c_str());

		str = "abcde";
		printf("string <%s>, ", str.c_str());
		str.reverse(1);
		printf("reversed <%s>\n", str.c_str());

		str = "abcde";
		printf("string <%s>, ", str.c_str());
		str.reverse(7);
		printf("reversed <%s>\n", str.c_str());
	}

	printf("\n");

	{	// exchange
		ConstString c   = "abcXdeXfg";
		VarString   str;
		
		str = c;
		str.exchange('X', "Y");
		printf("string: <%s>; exch'ed(X,Y): <%s>\n", c, str.c_str());

		str = c;
		str.exchange('X', "MNO");
		printf("string: <%s>; exch'ed(X,MNO): <%s>\n", c, str.c_str());

		ConstString d = "XdeX";
		str = d;
		str.exchange('X', "Y");
		printf("string: <%s>; exch'ed(X,Y): <%s>\n", d, str.c_str());

		str = d;
		str.exchange('X', "MNO");
		printf("string: <%s>; exch'ed(X,MNO): <%s>\n", d, str.c_str());

		str = d;
		str.exchange('X', "");
		printf("string: <%s>; exch'ed(X,\"\"): <%s>\n", d, str.c_str());

		printf("\n");

		str = c;
		str.exchange("X", "Y");
		printf("string: <%s>; exch'ed(X,Y): <%s>\n", c, str.c_str());

		str = c;
		str.exchange("X", "MNO");
		printf("string: <%s>; exch'ed(X,MNO): <%s>\n", c, str.c_str());

		str = d;
		str.exchange("X", "Y");
		printf("string: <%s>; exch'ed(X,Y): <%s>\n", d, str.c_str());

		str = d;
		str.exchange("X", "MNO");
		printf("string: <%s>; exch'ed(X,MNO): <%s>\n", d, str.c_str());

		str = d;
		str.exchange("X", "");
		printf("string: <%s>; exch'ed(X,\"\"): <%s>\n", d, str.c_str());

		printf("\n");

		ConstString e = "abcDEfghDEEijkDEEDlmn";
		str = e;
		str.exchange("DE", "XYZABC");
		printf("string: <%s>; exch'ed(DE,XYZABC): <%s>\n", e, str.c_str());

		str = e;
		str.exchange("DE", "DEDE");
		printf("string: <%s>; exch'ed(DE,DEDE): <%s>\n", e, str.c_str());

		str = e;
		str.exchange("DE", "XYZABCD");
		printf("string: <%s>; exch'ed(DE,XYZABCD): <%s>\n", e, str.c_str());
	}

	printf("\n");

	{	// remove
		ConstString a = "";
		ConstString b = " ";
		ConstString c = " bc n ";
		ConstString d = " bc bbcc ";
		VarString str;
		
		str = a;
		str.remove("");
		printf("string: <%s>; removed(\"\"): <%s>\n", a, str.c_str());

		str = a;
		str.remove(" ");
		printf("string: <%s>; removed(\" \"): <%s>\n", a, str.c_str());
		
		str = b;
		str.remove("");
		printf("string: <%s>; removed(\"\"): <%s>\n", b, str.c_str());

		str = b;
		str.remove(" ");
		printf("string: <%s>; removed(\" \"): <%s>\n", b, str.c_str());

		str = c;
		str.remove(" ");
		printf("string: <%s>; removed(\" \"): <%s>\n", c, str.c_str());

		str = c;
		str.remove("  ");
		printf("string: <%s>; removed(\"  \"): <%s>\n", c, str.c_str());

		str = d;
		str.remove("bc");
		printf("string: <%s>; removed(bc): <%s>\n", d, str.c_str());
	}

	printf("\n");

	{	// count
		ConstString a = "";
		ConstString b = " ";
		ConstString c = "  ";
		ConstString d = " b  ";
		VarString   str;

		str = a;
		printf("string <%s>; count(' ') %d\n", a, str.getCount(' '));

		str = b;
		printf("string <%s>; count(' ') %d\n", b, str.getCount(' '));

		str = c;
		printf("string <%s>; count(' ') %d\n", c, str.getCount(' '));

		str = d;
		printf("string <%s>; count(' ') %d\n", d, str.getCount(' '));

		str = d;
		printf("string <%s>; count(0xff) %d\n", d, str.getCount(-1));
	}


	printf("\n");

	{	// cleanchar
		char str1[8] = { 'a', 'b' , 0x10, 'c', 'ü', 'd', 'Ä', 0x00 };

		VarString str2(str1);
		printf("str2 before cleanchar() <%s>\n", str2.c_str());
		str2.cleanchar();
		printf("str2 after  cleanchar() <%s>\n", str2.c_str());
	}

	printf("\n");

	{	// signed/unsigned char
		char test = 'Ü';
		printf("char: <%c>, %d => %s\n", test, test, test < 0 ? "signed char": "unsigned char");	// is number negativ ? --> char is signed
	}
}

//------------------------------------------------------------------------------
void test_trim()
{
	printf("\n");
	printf("--- test string trim  --------------------\n");
	printf("\n");

	{	// trim
		ConstString input[] = 
		{
			"",
			" ",
			"  ",
			"a",
			"ab",
			"a b",
			" a  b ",
//           12345678901234567
            "                 ",
            "                ",
            "               ",
            "              ",
            "             "
		};
		VarString   str;

		for (unsigned int i = 0; i < sizeof(input)/sizeof(input[0]); ++i)
		{
			str = input[i];
			str.trim(VarString::ALL);
			printf("string: <%s>%*s, trim(ALL)       <%s>\n", input[i], 25 - strlen(input[i]), " ", str.c_str());

			str = input[i];
			str.trim(VarString::FIRST);
			printf("string: <%s>%*s, trim(FIRST)     <%s>\n", input[i], 25 - strlen(input[i]), " ", str.c_str());

			str = input[i];
			str.trim(VarString::END);
			printf("string: <%s>%*s, trim(END)       <%s>\n", input[i], 25 - strlen(input[i]), " ", str.c_str());

			str = input[i];
			str.trim(VarString::FIRST_END);
			printf("string: <%s>%*s, trim(FIRST_END) <%s>\n", input[i], 25 - strlen(input[i]), " ", str.c_str());
		}
	}
}

//------------------------------------------------------------------------------
void test_format()
{
	printf("\n");
	printf("--- test string format --------------------\n");
	printf("\n");

	{
		static const char * const FORMAT[] = 
		{
			"",
			" ",
			"A",
			"ABC",
			"12345678901234",
			"123456789012345",
			"1234567890123456",
			"12345678901234567"
		};

		for (unsigned int i = 0; i < sizeof(FORMAT)/sizeof(*FORMAT); ++i)
		{
			VarString str;
			str.format(FORMAT[i]);

			printf("string <%s>%*s; size = %3d, formatted <%s>\n", 
				  FORMAT[i], 20 - strlen(FORMAT[i]), " ",
				  str.size(),
				  str.c_str());
		}
	}

	{
		VarString str;

		str.format("");
		printf("str formatted <%s>; size = %d\n", str.c_str(), str.size());

		str.format(0);
		printf("str formatted <%s>; size = %d\n", str.c_str(), str.size());
	}

	{
		VarString str;

		str.format("%c %hd %c%s%c", 'a', 3, 'b', "GGGG", 'c');
		printf("str formatted <%s>; size = %d\n", str.c_str(), str.size());

		str.format("%c %hd %c%-40s%c", 'a', 3, 'b', "GGGG", 'c');
		printf("str formatted <%s>; size = %d\n", str.c_str(), str.size());
	}

	{
		VarString str;

		str.format("%c%c%c", 'a', 'b', 'c');
		printf("str formatted <%s>; size = %d\n", str.c_str(), str.size());
	}
}

//------------------------------------------------------------------------------
void test_isdigit()
{
	BString strTest[] = 
	{ 
		" 123",
		" eee",
		"ö",
		"ß"
	};

	for (int i = 0; i < sizeof(strTest)/sizeof(strTest[0]); ++i)
	{
		printf("string: <%s>, isDigit() %s\n",	strTest[i].c_str(), 
												strTest[i].isDigit() ? "true" : "false");
	}
}

//------------------------------------------------------------------------------
void test_alloc()
{
	typedef std::string MyString;

	MyString a("01234");
	MyString b("A123456789" "B123456789" "C123456789" "D123456789" "E123456789" "F123456789");
	MyString t;

#define ZZZ
#define CC

#ifdef ZZ
	MyString * pStr = &a;
#else
	MyString * pStr = &b;
#endif

	Int32 i = 0;


#ifdef AA
	printf("%d: <%s>\n", i++, pStr->c_str());
	printf("%d: <%s>\n", i++, (*pStr)); //.c_str());
#endif

#ifdef BB
	t.format("%d: <%s>\n", i++, pStr->c_str());
	printf("%s\n", t.c_str());
	t.format("%d: <%s>\n", i++, (*pStr)); //.c_str());
	printf("%s\n", t.c_str());
#endif

#ifdef CC
	std::cout << i++ << ": <" << pStr->c_str() << ">" << std::endl;
	std::cout << i++ << ": <" << (*pStr)       << ">" << std::endl;

	std::stringstream stream;

	stream.str("");
	stream << i++ << ": <" << pStr->c_str() << ">" << std::endl;
	printf("%s", stream.str().c_str());


	stream.str("");
	stream << i++ << ": <" << (*pStr)       << ">" << std::endl;
	printf("%s", stream.str().c_str());

#endif

}

//------------------------------------------------------------------------------
void test_string()
{
#ifdef XXXX
	tests();
	test_format();
	test_trim();
	test_isdigit();
#endif

	test_alloc();
}
