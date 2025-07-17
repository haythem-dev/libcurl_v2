#ifndef GUARD_TESTRECEIPTNOTICEPRINTDATA_H
#define GUARD_TESTRECEIPTNOTICEPRINTDATA_H

namespace narcotics
{

	class TestReceiptNoticePrintData : public basar::cmnutil::IPrintDataProvider
	{
	public:
		TestReceiptNoticePrintData(const basar::VarString& rPsFile, const basar::VarString& rFormName);
		~TestReceiptNoticePrintData();

		virtual void					onSendData(basar::cmnutil::CollPrintRawInput& collPrintRawInput);

	protected:
		virtual	basar::VarString		getFormName() const;
		virtual	basar::VarString		getFormFileName() const;

	private:

		basar::I18nString				getHeaderLines();
		basar::I18nString				getDataLine();

		basar::VarString				m_PsFile;			//!< name of postscript file
		basar::VarString				m_FormName;			//!< name of form (to find in ps-file!)

		bool							m_DataSent = false;
	};

}

#endif //GUARD_TESTRECEIPTNOTICEPRINTDATA_H