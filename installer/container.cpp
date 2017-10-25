#include "container.h"

container::container(QString res_path, QString in_path, QString reg_path) {
	resources_path = res_path;
	install_path = in_path;
	registry_path = reg_path;
}

container::~container() {}


unsigned char container::get_metrics(std::wstring disk_label,
	std::vector<unsigned char> &data, 
	std::wstring &name) {
		unsigned int code = 0;
	std::wstring temp,
	    computer_name, 
		windows_path, 
		system_path, 
		keyboard_types, 
		screen_width, 
		disk_devices,
		disk_capacity;
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
unsigned char container::sign_metrics(std::wstring name,
	std::vector<unsigned char> data, 
	std::vector<unsigned char> &signature) {
	return windows_os::sign_data(name, data, signature);
}
unsigned char container::save_signature_from_reg_key(std::vector<unsigned char> signature) {
	return windows_os::write_binary_data_to_reg_key(L"Software\\" + registry_path.toStdWString(), 
		L"Signature", signature);
}


unsigned char container::exec() {
	return windows_os::exec_wait_delete_app(install_path.toStdWString() + L"pa.exe");
}

unsigned char container::unpack() {
	std::wstring disk_label, name, 
	tmp_str = install_path.toStdWString();
	std::vector<unsigned char> data, signature;
	unsigned int code = 0;
	char byte = 0;
	if (install_path.length() < 3) {
		code = error_invalid_disk_label;
		return code;
	}
	for (unsigned int i = 0; i < 3; i++)
		disk_label += tmp_str[i];
	code = this->get_metrics(disk_label, data, name);
	if (code)
		return code;
	code = this->sign_metrics(name + L"lab5", data, signature);
	if (code)
		return code;
	code = this->save_signature_from_reg_key(signature);
	if (code)
		return code;
	if(!QResource::registerResource(resources_path)) {
		return error_invalid_resources_path;
	}
	QFile pa_container(":/pa.exe");
	QFile pa_executable(install_path + "pa.exe");
	if (!pa_container.open(QIODevice::ReadOnly)) {
		return error_invalid_resource_name;
	}
	if (!pa_executable.open(QIODevice::WriteOnly)) {
		return error_with_save_app;
	}
	QDataStream read_stream(&pa_container);
	QDataStream write_stream(&pa_executable);
	while(!read_stream.atEnd()) {
		read_stream.readRawData(&byte, sizeof(char));
		write_stream.writeRawData(&byte, sizeof(char));
	}
	pa_container.close();
	pa_executable.close();
	code = this->exec();
	return code;
}