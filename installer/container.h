#include <QString>
#include <QFile>
#include <QResource>
#include <QDataStream>
#include <vector>
#include <string>
#include "..\share\windows_os\windows_os.h"



class container {
private:
	QString resources_path;
	QString install_path;
	QString registry_path;
	unsigned char get_metrics(std::wstring disk_label,
		std::vector<unsigned char> &data, 
		std::wstring &name);
	unsigned char sign_metrics(std::wstring name,
		std::vector<unsigned char> data, 
		std::vector<unsigned char> &signature);
	unsigned char save_signature_from_reg_key(std::vector<unsigned char> signature);
	unsigned char exec();
public:
	container(QString res_path, QString in_path, QString reg_path);
	~container();
	unsigned char unpack();
};