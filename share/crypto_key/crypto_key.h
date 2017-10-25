#include <Windows.h>
#include "..\error_codes\error_codes.h"



class crypto_key {
private:
	HCRYPTKEY hCryptKey;
	bool inited;
public:
	crypto_key(HCRYPTPROV hCryptProv);
	~crypto_key();
	HCRYPTKEY get();
};