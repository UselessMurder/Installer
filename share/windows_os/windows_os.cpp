#include "windows_os.h"



unsigned int windows_os::get_user_name(std::wstring &user_name) {
	unsigned int code = 0;
	LPWSTR name_array = NULL;
	DWORD size = 0;
	if (GetUserNameW(name_array, &size)) {
		user_name = name_array;
	} else {
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
			name_array = new wchar_t[size];
			if(GetUserNameW(name_array, &size)) {
				user_name = name_array;
			} else {
				code = error_with_get_user_name;
			}
			delete [] name_array;
		}
	}
	return code;
}

unsigned int windows_os::get_computer_name(std::wstring &computer_name) {
	unsigned int code = 0;
	LPWSTR name_array = NULL;
	DWORD size = 0;
	if (GetComputerNameW(name_array, &size)) {
		computer_name = name_array;
	} else {
		if (GetLastError() == ERROR_BUFFER_OVERFLOW) {
			name_array = new wchar_t[size];
			if(GetComputerNameW(name_array, &size)) {
				computer_name = name_array;
			} else {
				code = error_with_get_computer_name;
			}
			delete [] name_array;
		}
	}
	return code;
}

unsigned int windows_os::get_windows_path(std::wstring &get_windows_path) {
	unsigned int code = 0;
	LPWSTR path_array = NULL;
	DWORD size = 0;
	size = GetWindowsDirectoryW(path_array, size);
	path_array = new wchar_t[size];
	if(GetWindowsDirectoryW(path_array, size)) {
		get_windows_path = path_array;
	} else {
		code = error_with_get_windows_path;
	}
	delete [] path_array;
	return code;
}

unsigned int windows_os::get_system_path(std::wstring &system_path) {
	unsigned int code = 0;
	LPWSTR path_array = NULL;
	DWORD size = 0;
	size = GetSystemDirectoryW(path_array, size);
	path_array = new wchar_t[size];
	if(GetSystemDirectoryW(path_array, size)) {
		system_path = path_array;
	} else {
		code = error_with_get_system_path;
	}
	delete [] path_array;
	return code;
}

unsigned int windows_os::get_keyboard_types(std::wstring &keyboard_types) {
	int type = GetKeyboardType(0);
	int sub_type = GetKeyboardType(1);
	keyboard_types = std::to_wstring(type) + L" " + std::to_wstring(sub_type);
	return 0;
}

unsigned int windows_os::get_screen_width(std::wstring &screen_width) {
	int width = GetSystemMetrics(SM_CXSCREEN);
	screen_width = std::to_wstring(width);
	return 0;
}

unsigned int windows_os::get_disk_devices(std::wstring &disk_devices) {
	LPWSTR devices_str = NULL;
	DWORD size = 0;
	size = GetLogicalDriveStringsW(size, devices_str);
	size++;
	devices_str = new wchar_t[size];
	GetLogicalDriveStringsW(size, devices_str);
	disk_devices = devices_str;
	delete [] devices_str;
	return 0;
}

unsigned int windows_os::get_disk_capacity(std::wstring disk_label, std::wstring &disk_capacity) {
	DWORD metrics[4] = {0,};
	if(!GetDiskFreeSpaceW(disk_label.c_str(), 
		&metrics[0],
		&metrics[1],
		NULL,
		&metrics[3]))
	{
		return error_with_get_disk_capacity;
	}
	disk_capacity = std::to_wstring(metrics[0] * metrics[1] * metrics[3]);
	return 0;
}

unsigned int windows_os::get_current_disk_label(std::wstring &disk_label) {
	unsigned int code = 0;
	LPWSTR path_array = NULL;
	DWORD size = 0;
	size = GetCurrentDirectoryW(size, path_array);
	path_array = new wchar_t[size];
	if(GetCurrentDirectoryW(size, path_array)) {
		if (size < 3)
			code = error_invalid_disk_label;
		else {
			disk_label = L"";
			for(int i = 0; i < 3; i++)
				disk_label+= path_array[i];
		}
	} else {
		code = error_with_get_current_directory;
	}
	delete [] path_array;
	return code;
}

unsigned int windows_os::sign_data(std::wstring user_name,
		std::vector <unsigned char> data, 
		std::vector<unsigned char> &signature) {
	crypto_provider cp(user_name);
	return cp.sign(data, signature);
}

unsigned int windows_os::verify_data(std::wstring user_name,
		std::vector <unsigned char> data, 
		std::vector<unsigned char> signature, 
		bool &ok) {
	crypto_provider cp(user_name);
	return cp.verify(data, signature, ok);
}

unsigned int windows_os::write_binary_data_to_reg_key(std::wstring path, 
		std::wstring name, 
		std::vector<unsigned char> value) {
	HKEY key = NULL;
	unsigned int code = 0;
	if (RegCreateKeyExW(HKEY_CURRENT_USER, 
		path.c_str(), 
		0, 0, 
		REG_OPTION_NON_VOLATILE, 
		KEY_ALL_ACCESS | KEY_WOW64_64KEY, 
		0, &key, 0) != 0) 
	{
		code = error_with_create_or_open_reg_key;
	}
	else {
		if (RegSetValueExW(key, 
			name.c_str(), 
			0, REG_BINARY, 
			(LPBYTE)&value[0], 
			value.size()) != 0)
		{
			code = error_with_set_reg_key_value;
		}
		RegCloseKey(key);
	}
	return code;
}

unsigned int windows_os::read_binary_data_from_reg_key(std::wstring path, 
		std::wstring name, 
		std::vector<unsigned char> &value) {
	HKEY key = NULL;
	unsigned int code = 0;
	if (RegOpenKeyExW(HKEY_CURRENT_USER, 
		path.c_str(), REG_OPTION_OPEN_LINK, 
		KEY_ALL_ACCESS | KEY_WOW64_64KEY, &key) != 0)
	{
		code = error_with_create_or_open_reg_key;
	} else {
		DWORD size = 0;
		RegQueryValueExW(key, name.c_str(), 
			NULL, NULL, NULL, &size);
		if (size == 0)
		{
			code = error_with_get_reg_key_size;
		} else {
			BYTE *tmp_array = new BYTE[size];
			if (RegQueryValueExW(key, name.c_str(), 
				NULL, NULL, (LPBYTE)tmp_array, &size) != 0)
			{
				code = error_with_get_reg_key_value;
			} else {
				value.clear();
				for (unsigned int i = 0; i < size; i++)
					value.push_back(tmp_array[i]);
			}
			delete[] tmp_array;
		}
		RegCloseKey(key);
	}
	return code;
}


unsigned int windows_os::exec_wait_delete_app(std::wstring path) {
	STARTUPINFOW si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	if (CreateProcessW(NULL, (LPWSTR)path.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		if (!DeleteFileW(path.c_str()))
			return error_with_delete_file;
	} else {
		return error_with_create_process;
	}
	return 0;
}