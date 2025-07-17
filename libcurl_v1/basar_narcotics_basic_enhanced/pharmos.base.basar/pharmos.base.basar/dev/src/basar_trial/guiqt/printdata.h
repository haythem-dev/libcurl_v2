
#ifndef GUARD_PRINTDATA_H
#define GUARD_PRINTDATA_H


//------------------------------------------------------------------------------
class PrintData : public basar::cmnutil::IPrintDataProvider
{
public:
							PrintData			(const basar::VarString & rPsFile,
												 const basar::VarString & rFormName,
												 const basar::VarString & rFile = "");
						   ~PrintData			();

	virtual void			onSendData			(basar::cmnutil::CollPrintRawInput & collPrintRawInput
												);
protected:

	virtual	basar::VarString		getFormName			() const;
	virtual	basar::VarString		getFormFileName		() const;

private:
	basar::Int32	m_hFile;

	basar::VarString m_PsFile;
	basar::VarString m_FormName;
};

#endif