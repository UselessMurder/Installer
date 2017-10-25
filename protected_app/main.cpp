#include "main_window.h"


int main(int argc, char **argv) {
	QApplication app(argc, argv);
	bool flag = true;
	main_window w(flag);
	if (!flag)
		return 0;
	w.show();
	return app.exec();
}