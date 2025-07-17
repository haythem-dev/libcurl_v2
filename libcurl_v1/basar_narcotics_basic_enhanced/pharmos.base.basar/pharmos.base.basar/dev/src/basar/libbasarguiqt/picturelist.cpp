//----------------------------------------------------------------------------
/*! \file
 *  \brief  print preview picture list
 *  \author Thomas Hörath
 *  \date   17.01.2006
 */
//----------------------------------------------------------------------------

#include "picturelist.h"

#include "libbasarcmnutil_bstring.h"
#include "libbasarcmnutil_exceptions.h"
#include "loggerpool.h"
#include "libbasarcmnutil_logging.h"

#pragma warning (push)
#pragma warning(disable: 4244 4512 4800)
#include <QtWidgets/QWidget>
#pragma warning (pop)

//----------------------------------------------------------------------------
namespace basar {
namespace gui {
namespace qt {

//----------------------------------------------------------------------------
/*!	\throw no-throw */
PictureList::PictureList(Int32     bitspixel,
						 QWidget * parent   ) 
: QObject     (parent   ),
  m_BitsPixel (bitspixel)
{
	if (4 != m_BitsPixel)
	{
		VarString msg;
		msg.format("bpp == %d, only 4 bpp supported!",
			       m_BitsPixel);
		throw InvalidParameterException(ExceptInfo ("basar.gui.qt.PictureList.PictureList()",
													msg.c_str(),
													__FILE__, __LINE__));
	}
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
PictureList::~PictureList()
{
	reset();
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\retval true if appending was successful
	\return otherwise returns false */
bool PictureList::append (Int32         width,	
						  Int32         height,
						  Int32         rowbytes,
						  const UInt8 * pImage)
{
	if (0 == rowbytes * height)
	{
		assert(0);
		return false;
	}

	QByteArray zipBmp = qCompress(pImage, rowbytes * height);

	BmpInfo info;

	info.zipBytes  = zipBmp.size();
	info.pImageZip.reset(new UInt8[info.zipBytes]);
	info.rowBytes  = rowbytes;
	info.height    = height;
	info.width     = width;

	memcpy(info.pImageZip.get(), zipBmp.constData(), info.zipBytes);

	m_CollPic.push_back(info);

	if (LoggerPool::prnPicList().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("append():\n"
			       "  bitmap size = %9d\n"
				   "  zip    size = %9d",
			       rowbytes * height,
				   info.zipBytes);
		LoggerPool::prnPicList().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return true;
}

//----------------------------------------------------------------------------
/*!	\throw no-throw */
void PictureList::reset()
{
    m_CollPic.clear();
	BLOG_TRACE(LoggerPool::prnPicList(), "picture list cleared!");
}

//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return current picture in collection */
const QPixmap PictureList::getPic(const Int32   idx,
								  const Float64 zoom)
{ 
	assert(idx < m_CollPic.size());

	const BmpInfo &  info     = m_CollPic.at(idx);
    const QByteArray unzipBmp = qUncompress(info.pImageZip.get(), info.zipBytes);

	if (LoggerPool::prnPicList().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("getPic(%d):\n"
			       "  bitmap size = %9d\n"
				   "  zip    size = %9d",
				   idx,
			       info.rowBytes * info.height,
				   info.zipBytes);
		LoggerPool::prnPicList().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return createBitmap(info, unzipBmp, zoom);
}

//----------------------------------------------------------------------------
//! \throw no-throw
//! \return collection with colors
PictureList::CollColors PictureList::initColorTable()
{
	CollColors collColor;

	for (Int32 i = 0; i < (1 << (m_BitsPixel - 4)); ++i)
	{
		collColor.append(qRgb(0x00, 0x00, 0x00));
		collColor.append(qRgb(0x00, 0x00, 0xFF));
		collColor.append(qRgb(0x00, 0xFF, 0x00));
		collColor.append(qRgb(0x00, 0xFF, 0xFF));
		collColor.append(qRgb(0xFF, 0x00, 0x00));
		collColor.append(qRgb(0xFF, 0x00, 0xFF));
		collColor.append(qRgb(0xFF, 0xFF, 0x00));
		collColor.append(qRgb(0xFF, 0xFF, 0xFF));
		collColor.append(qRgb(0x00, 0x00, 0x00));
		collColor.append(qRgb(0x00, 0x00, 0xFF));
		collColor.append(qRgb(0x00, 0xFF, 0x00));
		collColor.append(qRgb(0x00, 0xFF, 0xFF));
		collColor.append(qRgb(0xFF, 0x00, 0x00));
		collColor.append(qRgb(0xFF, 0x00, 0xFF));
		collColor.append(qRgb(0xFF, 0xFF, 0x00));
		collColor.append(qRgb(0xFF, 0xFF, 0xFF));
	}

	BLOG_TRACE(LoggerPool::prnPicList(), "color table initialized!");

	return collColor;
}

//----------------------------------------------------------------------------
//! \throw NullReferenceException
//! \return created Bitmap
QPixmap PictureList::createBitmap(const BmpInfo    & info,
								  const QByteArray & bmp ,
								  const Float64      zoom)
{
	static CollColors collColor(initColorTable());		// only once init'ed

	QImage image(info.width, info.height, QImage::Format_Indexed8);

	if ( image.isNull() ||	(0 ==image.width() * image.height()))                        
		throw NullReferenceException(ExceptInfo("basar.gui.qt.PictureList.createBitmap()",
		                                        "creating image failed",
												__FILE__, __LINE__));
	image.setColorCount(1 << m_BitsPixel);
	image.setColorTable(collColor);


	const UInt8 * pSrc;		// saved bitmap data
	UInt8       * pDest;	// to be constructed image
	Int32         x;		// column
	Int32         y;		// row


    for (y = 0; y < info.height; ++y)			// row by row
	{
		pSrc  = reinterpret_cast < const UInt8 * > (bmp.constData()) + y * info.rowBytes;
		pDest = image.scanLine(y);

		for (x = 0; x < (info.width/2); ++x)	// convert nibbles to byte
		{
            *pDest++ = *pSrc >> 4;				// higher nibble
            *pDest++ = *pSrc++ & 0x0F;			// lower nibble
		}

		if ( info.width & 1 )					// odd: last nibble
			*pDest = *pSrc >> 4;				// higher nibble
	}

	if ( ((zoom - 1.0) < 0.1) && ((1.0 - zoom) < 0.1))	// regard: non precise floating point arithmetics
		return QPixmap::fromImage(image);
	else
		return QPixmap::fromImage(image.scaled( image.size() * zoom,
												Qt::KeepAspectRatio,
												Qt::FastTransformation));
}


//----------------------------------------------------------------------------
/*!	\throw no-throw 
	\return number of pictures in collection */
basar::Int32 PictureList::count() const
{ 
	if (LoggerPool::prnPicList().isEnabledFor(log4cplus::DEBUG_LOG_LEVEL))
	{
		VarString msg;
		msg.format("count(): <%d> pictures in list.", m_CollPic.size());
		LoggerPool::prnPicList().forcedLog(log4cplus::DEBUG_LOG_LEVEL, msg, __FILE__, __LINE__);
	}

	return m_CollPic.size();
}

//----------------------------------------------------------------------------
}	// namespace qt
}	// namespace gui
}	// namespace bassar
