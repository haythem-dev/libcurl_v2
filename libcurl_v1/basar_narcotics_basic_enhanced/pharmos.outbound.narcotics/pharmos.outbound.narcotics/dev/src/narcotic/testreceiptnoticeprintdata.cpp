#include "commonheader.h"
#include "testreceiptnoticeprintdata.h"

namespace narcotics
{
	TestReceiptNoticePrintData::TestReceiptNoticePrintData(const basar::VarString& rPsFile, const basar::VarString& rFormName)
		: m_PsFile(rPsFile),
		m_FormName(rFormName)
	{
	}

	TestReceiptNoticePrintData::~TestReceiptNoticePrintData()
	{
	}

	void TestReceiptNoticePrintData::onSendData(basar::cmnutil::CollPrintRawInput& collPrintRawInput)
	{
		basar::VarString data;
		data.append("h01a");

		static const basar::Int32 BUFLEN = 1024;
		char   buffer[BUFLEN];
		const char* pBuf;
		const char* pBufEnd;
		basar::Int32  i;

		strcpy(buffer, data.c_str());

		if (!m_DataSent)
		{
			strcpy(buffer, data.c_str());
			m_DataSent = true;
		}
		else 
		{
			strcpy(buffer, "\0");
		}

		size_t readbytes = strlen(buffer);

		for (i = 0, pBuf = buffer, pBufEnd = (buffer + readbytes);
			pBuf < pBufEnd;
			pBuf++)
		{
			collPrintRawInput.push(*pBuf);
		}

		if (0 == readbytes) 
		{
			m_DataSent = false;
		}
	}

	basar::VarString TestReceiptNoticePrintData::getFormName() const
	{
		return m_FormName;
	}

	basar::VarString TestReceiptNoticePrintData::getFormFileName() const
	{
		return m_PsFile;
	}
}