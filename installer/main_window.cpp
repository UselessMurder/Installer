#include "main_window.h"

//form

main_window::main_window(QWidget *parent) {
	this->init_menu_bar();
	this->init_greed();
	this->init_status_bar();
	fs_flag = false, reg_flag = true;
	QWidget *base = new QWidget;
    base->setLayout(grid);
    setCentralWidget(base);
    QDesktopWidget dw;
	int x=dw.width()*0.18;
	int y=dw.height()*0.18;
	this->resize(x,y);
	rx = new QRegExp("[A-Za-z0-9]{1,30}");
}

void main_window::init_menu_bar() {
	QMenu *file = menuBar()->addMenu("Файл");
    QMenu *help = menuBar()->addMenu("Справка");

    QAction *quit = new QAction(tr("Выход"), this);
    QAction *about = new QAction(tr("О программе..."), this);

    quit->setShortcut(QKeySequence::Close);
    about->setShortcut(QKeySequence::HelpContents);

    connect(quit, SIGNAL(triggered()), this, SLOT(quit()));
    connect(about, SIGNAL(triggered()), this, SLOT(about()));

    file->addAction(quit);
    help->addAction(about);
}

void main_window::init_greed() {
	grid = new QGridLayout(this);
	fs_label = new QLabel("Директория установки:");
	fs_path = new QLineEdit("", this);
	fs_path->setReadOnly(true);
	choose_button = new QPushButton("&Выбрать", this);
	reg_label = new QLabel("Путь к сигнатуре:", this);
	reg_path = new QLineEdit("Markov", this);
	install_button = new QPushButton("&Установить", this);
	install_button->setEnabled(false);
	connect(reg_path, SIGNAL(textChanged(const QString &)), this, SLOT(check_reg_exp()));
	connect(choose_button, SIGNAL(clicked()), this, SLOT(choose_dir()));
	connect(install_button, SIGNAL(clicked()), this, SLOT(install()));
	grid->addWidget(fs_label, 0, 0);
	grid->addWidget(fs_path, 1, 0);
	grid->addWidget(choose_button, 2, 0);
	grid->addWidget(reg_label, 3, 0);
	grid->addWidget(reg_path, 4, 0);
	grid->addWidget(install_button, 5, 0);
}

void main_window::init_status_bar() {
	statusBar()->showMessage(tr("Укажите директорию для установки!"));
}

main_window::~main_window() {}



//slots


void main_window::quit() {
	this->close();
}

void main_window::about() {
	QMessageBox::information(this, "Обо мне", "Выполнил: Марков А.К. \n Группа: Т-509.13");
}

void main_window::check_reg_exp() {
	if (rx->exactMatch(reg_path->text())) 
	    reg_flag = true;
	else 
	    reg_flag = false;
	this->check_flags();
}

void main_window::choose_dir() {
	QFileDialog dialog;
	fs_path->setText("");
	fs_flag = false;
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setOption(QFileDialog::ShowDirsOnly);
	if (dialog.exec()) {
		QStringList list = dialog.selectedFiles();
		if (list.size() != 0) {
			QString path = list[0];
			if (path.length() > 0) {
				if (path[path.length() - 1] != "/")
					path+="/";
				fs_path->setText(path);
				fs_flag = true;
			}
		}
	}
	this->check_flags();
}

void main_window::install() {
	this->setEnabled(false);
	container c("resources.rcc", fs_path->text(), reg_path->text());
	code = c.unpack();
	this->setEnabled(true);
	if (code) {
		QString i_code_str = QString::number(code);
		QString os_code_str = QString::number(GetLastError());
		QString str("Ошибка при установке приложения.\n");
		QString str1("Внутренний код ошибки: ");
		QString str2("Код ошибки windows: ");
		QString str3("\n");
		QMessageBox::information(this, "Ошибка!", str + str1 + i_code_str + str3 + str2 + os_code_str + str3);
	}
}


//logic 

void main_window::check_flags() {
	if(fs_flag && reg_flag)
		install_button->setEnabled(true);
	else
		install_button->setEnabled(false);
}
