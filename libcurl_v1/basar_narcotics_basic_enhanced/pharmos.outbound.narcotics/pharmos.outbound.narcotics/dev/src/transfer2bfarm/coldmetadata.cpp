//------------------------------------------------------------------------------
//includes
//------------------------------------------------------------------------------
#include "commonheader.h"
#include "coldmetadata.h"
#include "definitions.h"
#include "contributionvoucherentry.h"
#include "contributionvoucherfile.h"
#include "bgabranch.h"

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------
namespace narcotics
{
namespace archiving
{
using narcotics::contributionvoucherentry::ContributionVoucherEntryPtr;
using narcotics::contributionvoucherfile::ContributionVoucherFilePtr;

//------------------------------------------------------------------------------
void ColdMetaData::addFileEntry(ContributionVoucherEntryPtr dbEntry, ContributionVoucherFilePtr file, basar::VarString kofaxPath)
{
	MetaDataEntry entry;
	entry.ContributionVoucherNo = dbEntry->getVoucherNo();
	entry.FilePath = file->absoluteFilePath().toStdString();
	entry.PrintedDate = dbEntry->getPrintedDate();
	entry.PurchaseOrderNo = dbEntry->getPurchaseOrderNo();
	entry.Subscriber = dbEntry->getSubscriber();
	entry.TransactionDate = dbEntry->getTransactionDate();
	entry.CustomerSupplierNo = dbEntry->getCustomerSupplierNo();
	entry.FileType = file->getFileType().toStdString();
	entry.kofaxPath = kofaxPath;
	m_entries.push_back(entry);
}

//------------------------------------------------------------------------------
QString ColdMetaData::toString() 
{
	std::vector<MetaDataEntry>::iterator entryIt = m_entries.begin();

	QString stringVal;

	while(entryIt != m_entries.end())
	{		
		//necessary because QString::arg has too many overloads and compiler cannot decide which one to use: .arg(custSuppNo,7,10,'0')
		QString custSuppNoStr = QString::number(entryIt->CustomerSupplierNo);
		QString purchaseOrderNoStr = QString::number(entryIt->PurchaseOrderNo);
		QString branchnoStr = QString::number(m_branchno);
		QString printedDateStr = QString::number(entryIt->PrintedDate.getDate());

		QString line = QString("\"\",\"\",\"%1\",\"%2\",\"%3\",\"%4\",\"%5\",\"%6\",\"%7\",\"%8\",\"%9\",\"%10\",\"%11\",\"%12\",\"%13\",\"%14\",\"%15\",\"%16\",\"%17\"\n")
			.arg(definitions::BRANCHNO)
			.arg(branchnoStr,2,'0')
			.arg(definitions::TRANSACTIONDATE)
			.arg(entryIt->TransactionDate.getDate())
			.arg(definitions::CUSTOMERSUPPLIERNO)
			.arg(custSuppNoStr,7,'0')
			.arg(definitions::CONTRIBUTIONVOUCHERNO)
			.arg(entryIt->ContributionVoucherNo)
			.arg(definitions::PURCHASEORDERNO)
			.arg(purchaseOrderNoStr,4,'0')
			.arg(definitions::SUBSCRIBER)
			.arg(QString::fromLocal8Bit(entryIt->Subscriber.c_str()),20,' ')
			.arg(definitions::FILETYPE)
			.arg(QString::fromLocal8Bit(entryIt->FileType.c_str()))
			.arg(definitions::PRINTEDDATE)
			.arg(printedDateStr,8,'0')
			.arg(QString::fromLocal8Bit(entryIt->kofaxPath.c_str()));
			
		stringVal.append(line);
		entryIt++;		
	}

	return stringVal;
}

//------------------------------------------------------------------------------
ColdMetaData::ColdMetaData(basar::Int16 branchno)
{
	m_branchno = branchno;
}

//------------------------------------------------------------------------------
ColdMetaData::~ColdMetaData()
{
}

//------------------------------------------------------------------------------

} //namespace archiving
} //namespace narcotics

//------------------------------------------------------------------------------
