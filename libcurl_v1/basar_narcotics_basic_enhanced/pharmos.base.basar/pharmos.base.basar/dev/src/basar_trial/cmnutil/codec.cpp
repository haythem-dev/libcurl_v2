
#include <libbasarcmnutil.h>

#include "testcmnutil.h"

#ifdef WIN32
#include <io.h>
#endif

#include <sys/stat.h>
#include <fcntl.h>


#include <libbasarcmnutil_codec.h>

using basar::cmnutil::QuotedPrint;
using basar::cmnutil::Base64;
using basar::Int8;


static void quoted_printable()
{
	printf("\n");
	printf("--- quoted_printable ---------------------------\n");
	printf("\n");

	Int8  buf   [] = "test\r\naäöüßµ§¤\r\n";		// ¤  EURO
	Int32 buflen   = sizeof(buf) - 1;

	Int32 size_encmax = QuotedPrint::getMaxEncodeLength(buf, buflen);

	std::auto_ptr <Int8 > pBufEncode(new Int8[size_encmax]);
	memset(pBufEncode.get(), 0, size_encmax);

	Int32 size_enc = QuotedPrint::encode(buf, buflen, pBufEncode.get(), size_encmax);

	Int32 size_decmax = buflen;

	std::auto_ptr <Int8> pBufDecode(new Int8[size_decmax]);
	memset(pBufDecode.get(), 0, size_decmax);

	Int32 size_dec = QuotedPrint::decode(pBufEncode.get(), size_enc, pBufDecode.get(), size_decmax);

	printf("in    :\n%s", buf);
	printf("out   :\n%s", pBufEncode.get());
	printf("decode:\n%.*s", buflen, pBufDecode.get());
	printf("size  : %d", size_dec);

	assert(size_dec == buflen);
	assert(0 == memcmp(buf, pBufDecode.get(), buflen));
}

//------------------------------------------------------------------------------
static void base64()
{
	printf("\n");
	printf("--- base64 ---------------------------\n");
	printf("\n");

	struct stat status;
#ifdef WIN32
	Int32       file      = ::open( "libbasardebug.dll", O_RDONLY | O_BINARY );
	::stat("libbasardebug.dll", &status);
#else
	Int32       file      = ::open( "email.o", O_RDONLY, "b");
	::stat("email.o", &status);
#endif

	std::auto_ptr < Int8 > pBuf      (new Int8[status.st_size]);

	Int32 buflen = ::read(file, pBuf.get(), status.st_size);
	assert(buflen == status.st_size);
	::close(file);

	// ---

	Int32 size_encmax = Base64::getMaxEncodeLength(buflen);

	std::auto_ptr <Int8 > pBufEncode(new Int8[size_encmax]);
	memset(pBufEncode.get(), 0, size_encmax);

	Int32 size_enc = Base64::encode(pBuf.get(), buflen, pBufEncode.get(), size_encmax);

#ifdef SAVE__

	{
		Int32 save      = ::open( "cmp.txt", O_CREAT | O_TEXT | O_RDWR , S_IREAD | S_IWRITE );
        Int32 code = errno;

		Int32 written   = ::write(save, pBufEncode.get(), size_enc);
        code = errno;
		::close(save);
	}

#endif



	Int32 size_decmax = Base64::getMaxDecodeLength(size_enc);

	std::auto_ptr <Int8> pBufDecode(new Int8[size_decmax]);
	memset(pBufDecode.get(), 0, size_decmax);

	Int32 size_dec = Base64::decode(pBufEncode.get(), size_enc, pBufDecode.get(), size_decmax);

	size_dec = size_dec;

	assert(size_dec == buflen);
	assert(0 == memcmp(pBuf.get(), pBufDecode.get(), buflen));
}

//------------------------------------------------------------------------------
void wordencoded()
{
	printf("\n");
	printf("--- word encoded ---------------------------\n");
	printf("\n");

	static const Int8  buf   [] = "test\r\naäöüßµ§¤\r\n";		// ¤  EURO
	static ConstString charset  = "ISO-8859-15";
	static const Int32 buflen   = sizeof(buf) - 1;

	Int32 size_encmax = QuotedPrint::getMaxWEncodeLength(buf, buflen, charset);

	std::auto_ptr <Int8 > pBufEncode(new Int8[size_encmax]); 
	memset(pBufEncode.get(), 0, size_encmax);

	Int32 size_enc    = QuotedPrint::wencode(buf, buflen, pBufEncode.get(), size_encmax, charset);

	Int32 size_decmax = buflen;

	std::auto_ptr <Int8> pBufDecode(new Int8[size_decmax]);
	memset(pBufDecode.get(), 0, size_decmax);

	Int32 size_dec = QuotedPrint::wdecode(pBufEncode.get(), size_enc, pBufDecode.get(), size_decmax);

	printf("in    :\n%s", buf);
	printf("out   :\n%s", pBufEncode.get());
	printf("decode:\n%.*s", buflen, pBufDecode.get());
	printf("size  : %d", size_dec);

	assert(size_dec == buflen);
	assert(0 == memcmp(buf, pBufDecode.get(), buflen));
}

//------------------------------------------------------------------------------
void test_codec()
{
	printf("\n");
	printf("--- test_codec ---------------------------\n");
	printf("\n");

	quoted_printable();
	base64();
	wordencoded();
}
