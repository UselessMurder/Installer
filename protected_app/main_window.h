#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QStatusBar>
#include <QLineEdit>
#include <QPushButton>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QRegExp>
#include "..\share\windows_os\windows_os.h"


class main_window : public QMainWindow
{
    Q_OBJECT
public:
    main_window(bool &fl, QWidget *parent = 0);
    ~main_window();
private:
	unsigned int code;
	std::wstring name;
	std::vector<unsigned char> data;
	QGridLayout *grid;
	QLabel *label;
	QPushButton *check;
	QLineEdit *path;
	QRegExp *rx;
	void init_menu_bar();
	void init_greed();
	void init_status_bar();
	unsigned int init_name_and_data();
private slots:
    void quit();
    void about();
    void check_reg_exp();
    void check_signature();
};