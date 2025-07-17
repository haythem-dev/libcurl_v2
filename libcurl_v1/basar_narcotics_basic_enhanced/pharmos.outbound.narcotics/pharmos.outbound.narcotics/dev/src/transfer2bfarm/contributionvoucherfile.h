#ifndef CONTRIBUTIONVOUCHERFILE_GUARD
#define CONTRIBUTIONVOUCHERFILE_GUARD

//------------------------------------------------------------------------------
//namespace
//------------------------------------------------------------------------------

namespace narcotics {
namespace contributionvoucherfile {

//------------------------------------------------------------------------------
class ContributionVoucherFile : public QFileInfo
{
public:
	enum FileType{CONTRIBUTIONVOUCHER,DELIVERYNOTECOPY};

	basar::Int32 getContributionVoucherNo();
	basar::Int32 getCounter();
	
	QString getFileType();
	
	void setCounter(basar::Int32 counter);
	
	//needed because a simple rename won't work when done across partitions
	void moveTo(QString newPath);

	ContributionVoucherFile(QString filePath);
	
	virtual ~ContributionVoucherFile(void);

private:
	QString m_filePath;

	//would be better using classes but its ok for this need
	FileType m_type;

};

} //namespace contributionvoucherentry
} //namespace narcotics

#endif //CONTRIBUTIONVOUCHERFILE_GUARD