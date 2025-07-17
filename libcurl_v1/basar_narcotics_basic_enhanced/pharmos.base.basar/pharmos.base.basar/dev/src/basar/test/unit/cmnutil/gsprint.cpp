#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#define _rmdir rmdir
#endif

#include <boost/filesystem.hpp>

using boost::filesystem::file_size;
using boost::filesystem::path;
using boost::filesystem::copy;
using boost::filesystem::exists;

// don't remove created output files: only for tracing
//#define TRACE_OUTPUT_FILES

// don't remove created input files: only for tracing
//#define TRACE_INPUT_FILES

extern bool g_RUNSHORT;

static path getDataDir()
{
    path dataDir (basar::cmnutil::getExecDir ().c_str());
	dataDir /= "..";
#ifdef _WIN32
#ifdef _WIN64
	dataDir /= "..";
#endif
    dataDir /= "test/unit/cmnutil";
#endif
    dataDir /= "data";
    return dataDir.generic_path();
}

//------------------------------------------------------------------------------
SUITE(GhostscriptPrint)
{

//------------------------------------------------------------------------------
using basar::Int32;
using basar::ConstString;
using basar::VarString;
using basar::RuntimeException;
using basar::InvalidParameterException;
using basar::cmnutil::InterpretPrint;
using basar::cmnutil::IPrintDataProvider;
using basar::cmnutil::CollPrintRawInput;

//------------------------------------------------------------------------------
TEST(DedicatedPrinterWoName)
{
	if (g_RUNSHORT)
		return;

	InterpretPrint pr;

	pr.setPrintSelect(basar::cmnutil::InterpretPrint::PRINTER_DEDICATED);

	CHECK_THROW(pr.print(""), InvalidParameterException);
}

//------------------------------------------------------------------------------
class PrintDataProvider : public IPrintDataProvider
{
public:
					PrintDataProvider() {};
				   ~PrintDataProvider() {};

	virtual void	onSendData(CollPrintRawInput &)
	{
	};

	VarString		getFormName() const
	{
		return "unittest_form";
	};

	VarString		getFormFileName() const
	{
		return "unittest_filename";
	};
};

//------------------------------------------------------------------------------
TEST(PrintFormPath_bad)
{
	if (g_RUNSHORT)
		return;

	InterpretPrint    pr;
	PrintDataProvider dp;

	// directory print doesn't exist
	CHECK_THROW(pr.savePdf(dp, "", 0, 0), InvalidParameterException);
}

//------------------------------------------------------------------------------
TEST(PrintFormPath_good)
{
	if (g_RUNSHORT)
		return;

	InterpretPrint    pr;
	PrintDataProvider dp;

    path execDir (basar::cmnutil::getExecDir ().c_str());
	execDir /= "print";
	VarString dir = execDir.string();

	// create directory for print forms
#ifdef _WIN32
	Int32 ret = ::_mkdir(dir.c_str());
#else
    Int32 ret = ::mkdir(dir.c_str(), S_IRWXU | S_IRWXG);
#endif
	CHECK_EQUAL(0, ret);

    CHECK_THROW(pr.savePdf(dp, "", 1, 0), RuntimeException);	// ok: no data, no appropriate output file

	ret = ::_rmdir(dir.c_str());
	CHECK_EQUAL(0, ret);
}

//------------------------------------------------------------------------------
TEST(PrintPdf_non_existing_input_file_good)
{
	if (g_RUNSHORT)
		return;

    path dataDir(getDataDir());
    path outfile = dataDir / "xyz.pdf";

    {
	    InterpretPrint pr;

        CHECK_THROW(pr.savePdf("abc.ps", outfile.string()), RuntimeException);
    }

    //no output is expected
#ifndef TRACE_OUTPUT_FILES
    int ret = remove(outfile.string().c_str());
    CHECK_EQUAL(-1, ret);
#endif
}

//------------------------------------------------------------------------------
TEST(PrintPdf_inout_filename_ascii_good)
{
	if (g_RUNSHORT)
		return;

	InterpretPrint  pr;

    path dataDir(getDataDir());
    path infile  = dataDir / "form.ps";
    path outfile = dataDir / "form.pdf";

    pr.savePdf(infile.string(), outfile.string());

    uintmax_t size = file_size(outfile);
    CHECK(size > 0);

#ifndef TRACE_OUTPUT_FILES
    int ret = remove(outfile.string().c_str());
    CHECK_EQUAL(0, ret);
#endif
}

//------------------------------------------------------------------------------
TEST(PrintPdf_output_filename_non_ascii_good)
{
	if (g_RUNSHORT)
		return;

	InterpretPrint  pr;

    path dataDir(getDataDir());
    path infile  = dataDir / "form.ps";
    path outfile = dataDir / "form_\xe4_out.pdf";  // ä

    pr.savePdf(infile.string(), outfile.string());

    uintmax_t size = file_size(outfile);
    CHECK(size > 0);

#ifndef TRACE_OUTPUT_FILES
    int ret = remove(outfile.string().c_str());
    CHECK_EQUAL(0, ret);
#endif
}

//------------------------------------------------------------------------------
TEST(PrintPdf_input_filename_non_ascii_good)
{
	if (g_RUNSHORT)
		return;

	InterpretPrint  pr;

    int ret = -1;

    path dataDir(getDataDir());
    path srcfile = dataDir / "form.ps";
    path infile  = dataDir / "form_\xdf\xf6_in.ps"; // ßö
    path outfile = dataDir / "form_ssoe_out.pdf";

    copy(srcfile, infile);
    CHECK_EQUAL(true, exists(infile));

    pr.savePdf(infile.string(), outfile.string());

    uintmax_t size = file_size(outfile);
    CHECK(size > 0);

#ifndef TRACE_OUTPUT_FILES
    ret = remove(outfile.string().c_str());
    CHECK_EQUAL(0, ret);
#endif
#ifndef TRACE_INPUT_FILES
    ret = remove(infile.string().c_str());
    CHECK_EQUAL(0, ret);
#endif
}

//------------------------------------------------------------------------------
TEST(PrintPdf_inoutput_filename_non_ascii_good)
{
	if (g_RUNSHORT)
		return;

	InterpretPrint  pr;

    int ret = -1;

    path dataDir(getDataDir());
    path srcfile = dataDir / "form.ps";
    path infile  = dataDir / "form_\xf1\xfc_in.ps"; // ñü
    path outfile = dataDir / "form_\xf1\xfc_out.pdf";

    copy(srcfile, infile);
    CHECK_EQUAL(true, exists(infile));

    pr.savePdf(infile.string(), outfile.string());

    uintmax_t size = file_size(outfile);
    CHECK(size > 0);

#ifndef TRACE_OUTPUT_FILES
    ret = remove(outfile.string().c_str());
    CHECK_EQUAL(0, ret);
#endif
#ifndef TRACE_INPUT_FILES
    ret = remove(infile.string().c_str());
    CHECK_EQUAL(0, ret);
#endif
}

//------------------------------------------------------------------------------
}	// SUITE
