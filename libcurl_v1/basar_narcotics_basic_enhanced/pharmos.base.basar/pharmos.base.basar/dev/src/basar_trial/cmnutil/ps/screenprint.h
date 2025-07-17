
#ifndef GUARD_SCREENPRINT_H
#define GUARD_SCREENPRINT_H

//------------------------------------------------------------------------------
namespace gui_for_test	{

//------------------------------------------------------------------------------
class ScreenPrint : public basar::cmnutil::IScreenPrint
{
public:
						ScreenPrint			();
					   ~ScreenPrint			();

	virtual Int32		onOpen				();
	virtual Int32		onClose				();
	virtual Int32		onSize				(Int32   width, 
											 Int32   height, 
											 Int32   raster, 
											 UInt8 * pImage
											);
	virtual Int32		onPage				();

private:
	Int32   m_width;
	Int32   m_height; 
	Int32   m_raster; 
	UInt8 * m_pImage;
};

//------------------------------------------------------------------------------
}	// namespace gui_for_test


#endif
