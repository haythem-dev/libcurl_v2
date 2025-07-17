//----------------------------------------------------------------------------
/*! \file
 *  \brief  internal list of pages (as pixmaps) for printpreview
 *  \author Thomas Hörath 
 *  \date   04.05.2006
 */
//----------------------------------------------------------------------------

#ifndef GUARD_PICTURELIST_H
#define GUARD_PICTURELIST_H

//----------------------------------------------------------------------------
#include <boost/shared_array.hpp>
#include "libbasar_definitions.h"

#pragma warning (push)
#pragma warning(disable: 4127 4481 4512)
#include <QtCore/QObject>
#include <QtCore/QVector>
#include <QtGui/QColor>
#pragma warning (pop)

//----------------------------------------------------------------------------
class QPixmap;

//----------------------------------------------------------------------------
namespace basar	{
namespace gui	{
namespace qt	{

//----------------------------------------------------------------------------
/*! \brief	Container object to reference preview pages
	
	Final class. 
	Throws no exceptions */
class PictureList : public QObject
{
public:
	
    /*! \brief Constructor of picture list
		\n     no-throw */
										PictureList		( Int32     bitspixel  ,	//!< bits per pixel
														  QWidget * pParent = 0		//!< parent widget of picture list
														);
    /*! \brief Destructor of picture list
		\n     no-throw */
									   ~PictureList		();


	/*! \brief delete all pictures from collection
		\n     no-throw */
	void								reset			();
	/*! \brief create new picture and append it to list
		\n     no-throw */
	bool								append			(Int32         width,		//!< raster image width
														 Int32         height,		//!< raster image height
														 Int32         rowbytes,	//!< byte count of raster image row	
														 const UInt8 * pImage		//!< raster image data
														);
	/*! \brief return current picture of list
		\n     no-throw */
	const QPixmap 						getPic			(const Int32   idx,			//!< pixmap index in collection
														 const Float64 zoom			//!< zoom factor
														);
	/*! \brief return number of pictures in list
		\n     no-throw */
	Int32								count			() const;


private:

	typedef boost::shared_array < UInt8 >		ImageSharedPtr;		//!< smart pointer to image data

	//! bitmap info
	struct BmpInfo
	{
		ImageSharedPtr pImageZip;	//!< compressed image
		Int32          zipBytes;	//!< compressed image size
		Int32          rowBytes;	//!< count bytes per row
		Int32		   height;		//!< image rows
		Int32          width;		//!< image width
	};

	typedef QVector	< BmpInfo >			CollPic;				//!< collection of preview pages
	typedef QVector < QRgb    >			CollColors;				//!< bitmap color table
	

	//! init bitmap color table					\n no-throw
	CollColors							initColorTable	();

	//! create bitmap from buffered data		\n NullReferenceException
	QPixmap								createBitmap	(const BmpInfo    & info,	//!< bitmap info
														 const QByteArray & bmp ,	//!< bitmap data
														 const Float64      zoom	//!< zoom factor
														);

	Int32								m_BitsPixel;	//!< bits per pixel
	CollPic								m_CollPic;		//!< collection of preview pictures
};


//----------------------------------------------------------------------------
}	// namespace qt
}	// namespace gui
}	// namespace bassar

#endif // GUARD_PICTURELIST_H
