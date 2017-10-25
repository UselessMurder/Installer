#include "main_window.h"

//form

main_window::main_window(bool &fl, QWidget *parent) {
	this->init_menu_bar();
	this->init_greed();
	this->init_status_bar();
	code = this->init_name_and_data();
	if (code) {
		QString i_code_str = QString::number(code);
		QString os_code_str = QString::number(GetLastError());
		QString str("Ошибка при получении параметров системы.\n");
		QString str1("Внутренний код ошибки: ");
		QString str2("Код ошибки windows: ");
		QString str3("\n");
		QMessageBox::information(this, "Ошибка!", str + str1 + i_code_str + str3 + str2 + os_code_str + str3);
		fl = false;
	}
	QWidget *base = new QWidget;
    base->setLayout(grid);
    setCentralWidget(base);
    QDesktopWidget dw;
	int x=dw.width()*0.15;
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
	label = new QLabel("Путь к сигнатуре:", this);
	path = new QLineEdit("Markov", this);
	check = new QPushButton("&Проверить", this);
	connect(path, SIGNAL(textChanged(const QString &)), this, SLOT(check_reg_exp()));
	connect(check, SIGNAL(clicked()), this, SLOT(check_signature()));
	grid->addWidget(label, 0, 0);
	grid->addWidget(path, 1, 0);
	grid->addWidget(check, 2, 0);
}

void main_window::init_status_bar() {
	statusBar()->showMessage(tr("Укажите путь к сигнатуре в реестре!"));
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
	if (rx->exactMatch(path->text())) 
	    check->setEnabled(true);
	else 
	    check->setEnabled(false);
}

void main_window::check_signature() {
	 bool ok = false;
	 std::vector<unsigned char> signature;
	 unsigned int code = windows_os::read_binary_data_from_reg_key(L"Software\\" + path->text().toStdWString(),
	 	L"Signature",
	 	signature);
	 if (code) {
	 	QString i_code_str = QString::number(code);
		QString os_code_str = QString::number(GetLastError());
		QString str("Ошибка при получении сигнатуры из реестра.\n");
		QString str1("Внутренний код ошибки: ");
		QString str2("Код ошибки windows: ");
		QString str3("\n");
		QMessageBox::information(this, "Ошибка!", str + str1 + i_code_str + str3 + str2 + os_code_str + str3);
		statusBar()->showMessage(tr("Ошибка, проверьте путь!"));
	 	return;
	 } 
	 code = windows_os::verify_data(name + L"lab5", data, signature, ok);
	 if (code) {
	 	QString i_code_str = QString::number(code);
		QString os_code_str = QString::number(GetLastError());
		QString str("Ошибка при верификации сигнатуры.\n");
		QString str1("Внутренний код ошибки: ");
		QString str2("Код ошибки windows: ");
		QString str3("\n");
		QMessageBox::information(this, "Ошибка!", str + str1 + i_code_str + str3 + str2 + os_code_str + str3);
		statusBar()->showMessage(tr("Ошибка, проверьте путь!"));
	 	return;
	 } 
	 if (!ok) {
	 	QMessageBox::information(this, "Неудача", "Неверная сигнатура, программа будет завершена.");
	 	this->close();
	 } else {
	 	QMessageBox::information(this, "Успех", "Верная сигнатура.");
	 }
}


//logic 

unsigned int main_window::init_name_and_data() {
	unsigned int code = 0;
	std::wstring temp,
	    computer_name, 
		windows_path, 
		system_path, 
		keyboard_types, 
		screen_width, 
		disk_devices,
		disk_capacity,
		disk_label;
	code = windows_os::get_user_name(name);
	if (code)
		return code;
	temp += name;
	code = windows_os::get_computer_name(computer_name);
	if (code)
		return code;
	temp += computer_name;
	code = windows_os::get_windows_path(windows_path);
	if (code)
		return code;
	temp += windows_path;
	code = windows_os::get_system_path(system_path);
	if (code)
		return code;
	temp += system_path;
	code = windows_os::get_keyboard_types(keyboard_types);
	if (code)
		return code;
	temp += keyboard_types;
	code = windows_os::get_screen_width(screen_width);
	if (code)
		return code;
	temp += screen_width;
	code = windows_os::get_disk_devices(disk_devices);
	if (code)
		return code;
	temp += disk_devices;
	code = windows_os::get_current_disk_label(disk_label);
	if (code)
		return code;
	code = windows_os::get_disk_capacity(disk_label, disk_capacity);
	if (code)
		return code;
	temp += disk_capacity;
	w_to_c wc;
	for(unsigned int i = 0; i < temp.length(); i++){
		wc.w = temp[i];
		data.push_back(wc.c[0]);
		data.push_back(wc.c[1]);
	}
	return code;
}