
#include <libbasarcmnutil.h>

#include <testcmnutil.h>

#include "screenprint.h"

//------------------------------------------------------------------------------
namespace gui_for_test	{

//------------------------------------------------------------------------------
ScreenPrint::ScreenPrint() : m_width (0),
							 m_height(0),
							 m_raster(0),
							 m_pImage(0)

{
}

//------------------------------------------------------------------------------
ScreenPrint::~ScreenPrint()
{
}

//------------------------------------------------------------------------------
Int32 ScreenPrint::onOpen()
{
	printf("ScreenPrint::onOpen() = 0\n");

    return 0;
}

//------------------------------------------------------------------------------
Int32 ScreenPrint::onClose()
{
	printf("ScreenPrint::onClose() = 0\n");

	return 0;
}

//------------------------------------------------------------------------------
Int32 ScreenPrint::onSize(Int32 width, Int32 height, Int32 raster, UInt8 * pImage)
{
	printf("ScreenPrint::onSize(width %d, height %d, raster %d, pImage 0x%08p) = 0\n",
		   width, height, raster, pImage);

	m_width  = width;
	m_height = height; 
	m_raster = raster; 
	m_pImage = pImage;

	return 0;
}
   
//------------------------------------------------------------------------------
Int32 ScreenPrint::onPage()
{
/*	save in bitmap
	HBITMAP hBmp = ::CreateBitmap(m_raster/(caps.bitspixel/8), m_height, caps.planes, caps.bitspixel, m_pImage);

	QPixmap pixmap = QPixmap::fromWinHBITMAP(hBmp, QPixmap::NoAlpha);
	bool b = pixmap.isNull();
	DeleteObject(hBmp);
*/
	printf("ScreenPrint::onPage() = 0\n");

	return 0;
}

//------------------------------------------------------------------------------
}	// namespace gui_for_test
