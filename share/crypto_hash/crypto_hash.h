#include <Windows.h>
#include <vector>
#include "..\error_codes\error_codes.h"



class crypto_hash {
private:
	HCRYPTHASH hCryptHash;
	bool inited; 
public:
	crypto_hash(HCRYPTPROV hCryptProv);
	~crypto_hash();
	unsigned char hash_data(std::vector<unsigned char> data);
	unsigned char get_signature(std::vector<unsigned char> &signature);
	bool verify(std::vector<unsigned char> signature, HCRYPTKEY hCryptKey);
};