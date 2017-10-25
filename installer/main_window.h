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
#include <QFileDialog>
#include "container.h"


class main_window : public QMainWindow
{
    Q_OBJECT
public:
    main_window(QWidget *parent = 0);
    ~main_window();
private:
	bool fs_flag;
	bool reg_flag;
	unsigned int code;
	std::wstring name;
	std::vector<unsigned char> data;
	QGridLayout *grid;
	QLabel *fs_label;
	QLabel *reg_label;
	QLineEdit *fs_path;
	QLineEdit *reg_path;
	QRegExp *rx;
	QPushButton *choose_button;
	QPushButton *install_button;
	void init_menu_bar();
	void init_greed();
	void init_status_bar();
	void check_flags();
private slots:
    void quit();
    void about();
    void check_reg_exp();
    void choose_dir();
    void install();
};