#include <string>
#include <vector>
#include "..\crypto_provider\crypto_provider.h"
#include "..\error_codes\error_codes.h"

union w_to_c {
	wchar_t w;
	char c[2];
};

class windows_os {
public:
	static unsigned int get_user_name(std::wstring &user_name);
	static unsigned int get_computer_name(std::wstring &computer_name);
	static unsigned int get_windows_path(std::wstring &windows_path);
	static unsigned int get_system_path(std::wstring &system_path);
	static unsigned int get_keyboard_types(std::wstring &keyboard_types);
	static unsigned int get_screen_width(std::wstring &get_screen_width);
	static unsigned int get_disk_devices(std::wstring &get_disk_devices);
	static unsigned int get_disk_capacity(std::wstring disk_label, std::wstring &disk_capacity);
	static unsigned int get_current_disk_label(std::wstring &disk_label);
	static unsigned int sign_data(std::wstring user_name,
		std::vector <unsigned char> data, 
		std::vector<unsigned char> &signature);
	static unsigned int verify_data(std::wstring user_name,
		std::vector <unsigned char> data, 
		std::vector<unsigned char> signature, 
		bool &ok);
	static unsigned int write_binary_data_to_reg_key(std::wstring path, 
		std::wstring name, 
		std::vector<unsigned char> value);
	static unsigned int read_binary_data_from_reg_key(std::wstring path, 
		std::wstring name, 
		std::vector<unsigned char> &value);
	static unsigned int exec_wait_delete_app(std::wstring path);
};