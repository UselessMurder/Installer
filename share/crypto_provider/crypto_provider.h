#include <string>
#include <vector>
#include "..\crypto_key\crypto_key.h"
#include "..\crypto_hash\crypto_hash.h"
#include "..\error_codes\error_codes.h"

class crypto_provider {
private:
	HCRYPTPROV hCryptProv;
	bool inited;
public:
	crypto_provider(std::wstring user_name);
	~crypto_provider();
	unsigned char sign(std::vector <unsigned char> data, std::vector<unsigned char> &signature);
	unsigned char verify(std::vector <unsigned char> data, std::vector<unsigned char> signature, bool &ok);
};