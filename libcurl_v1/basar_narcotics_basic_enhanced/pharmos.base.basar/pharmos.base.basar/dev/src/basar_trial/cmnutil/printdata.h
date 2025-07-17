
#ifndef GUARD_PRINTDATA_H
#define GUARD_PRINTDATA_H

//------------------------------------------------------------------------------
namespace gui_for_test	{

//------------------------------------------------------------------------------
class PrintData: public basar::cmnutil::IPrintDataProvider
{
public:
							PrintData			(const VarString & formFile,
								                 const VarString & formName);

							PrintData           (const VarString & dataFile,
												 const VarString & formFile,
								                 const VarString & formName);
	virtual				   ~PrintData			();

	virtual void			onSendData			(basar::cmnutil::CollPrintRawInput & collPrintRawInput
												);
	virtual	VarString		getFormName			() const;
	virtual	VarString		getFormFileName		() const;

private:
							PrintData			();

	Int32					m_hDataFile;

	VarString				m_FormFile;
	VarString				m_FormName;
};

//------------------------------------------------------------------------------
}	// namespace gui_for_test

#endif
