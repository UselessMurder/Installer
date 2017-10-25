#include "crypto_provider.h"



crypto_provider::crypto_provider(std::wstring user_name) {
	hCryptProv = (unsigned int)NULL;
	bool inited = false;
	if(CryptAcquireContextW(
		&hCryptProv,
		user_name.c_str(),
		NULL,
		PROV_RSA_FULL,
		0))
	{
		inited = true;
	} else {
		if(GetLastError() == (unsigned int)NTE_BAD_KEYSET) {
			if(CryptAcquireContextW(
				&hCryptProv,
				user_name.c_str(),
				NULL,
				PROV_RSA_FULL,
				CRYPT_NEWKEYSET))
			{
				inited = true;
			}
		}
	}
}

crypto_provider::~crypto_provider() {
	if (inited) {
		CryptReleaseContext(hCryptProv, 0);
	}
}


unsigned char crypto_provider::sign(std::vector <unsigned char> data, std::vector<unsigned char> &signature) {
	unsigned int code = 0;
	crypto_key ck(hCryptProv);
	crypto_hash ch(hCryptProv);
	code = ch.hash_data(data);
	if (!code) {
		code = ch.get_signature(signature);
	}
	return code;
}

unsigned char crypto_provider::verify(std::vector <unsigned char> data, std::vector<unsigned char> signature, bool &ok) {
	unsigned int code = 0;
	crypto_key ck(hCryptProv);
	crypto_hash ch(hCryptProv);
	code = ch.hash_data(data);
	if (!code) {
		ok = ch.verify(signature, ck.get());
	}
	return code;
}