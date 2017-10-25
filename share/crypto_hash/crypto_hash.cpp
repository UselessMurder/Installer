#include "crypto_hash.h"
#include <stdio.h>



crypto_hash::crypto_hash(HCRYPTPROV hCryptProv) {
	hCryptHash = NULL;
	inited = false;
	if(CryptCreateHash(
	   hCryptProv, 
	   CALG_MD5, 
	   0, 
	   0, 
	   &hCryptHash)) 
	{
	    inited = true; 
	}
}

crypto_hash::~crypto_hash() {
	if(inited) {
		CryptDestroyHash(hCryptHash);
	}
}

unsigned char crypto_hash::hash_data(std::vector<unsigned char> data) {
	unsigned char code = 0;
	BYTE *data_array = new BYTE[data.size()]; 
	for (unsigned int i = 0; i < data.size(); i++)
		 data_array[i] = data[i];
	if(!CryptHashData(
		hCryptHash,
		data_array,
		data.size(),
		0)) 
	{
		code = error_with_calculate_hash;
	}
	delete [] data_array;
	return code;	
}


unsigned char crypto_hash::get_signature(std::vector<unsigned char> &signature) {
	unsigned char code = 0;
	DWORD size = 0;
	BYTE *signature_array = NULL;
	if(!CryptSignHash(
	   hCryptHash, 
	   AT_SIGNATURE, 
	   NULL, 
	   0, 
	   NULL, 
	   &size)) 
	{
	    code = error_with_calculate_signature_size;
	}
	if (!code) {
		signature_array = new BYTE[size];
		if(!CryptSignHash(
		   hCryptHash, 
		   AT_SIGNATURE, 
		   NULL, 
		   0, 
		   signature_array, 
		   &size)) 
		{
		    code = error_with_calculate_signature;
		}
		if (!code) {
			signature.clear();
			for (DWORD i = 0; i < size; i++)
				signature.push_back(signature_array[i]);
		}
		delete [] signature_array;
	}
	return code;
}


bool crypto_hash::verify(std::vector<unsigned char> signature, HCRYPTKEY hCryptKey) {
	BYTE *signature_array = new BYTE[signature.size()];
	for (unsigned int i = 0; i < signature.size(); i++) 
		signature_array[i] = signature[i];
	if (CryptVerifySignature(
		hCryptHash,
		signature_array,
		(DWORD)signature.size(),
		hCryptKey,
		NULL,
		0)) 
	{
		return true;
	}
	delete [] signature_array;
	return false;
}