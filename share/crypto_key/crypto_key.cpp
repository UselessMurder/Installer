#include "crypto_key.h"



crypto_key::crypto_key(HCRYPTPROV hCryptProv) {
	hCryptKey = (unsigned int)NULL;
	inited = false;	  
	if(CryptGetUserKey(   
	   hCryptProv,    
	   AT_SIGNATURE,    
	   &hCryptKey)) 
	{
	    inited = true;
	}
	else
	{
	     if(CryptGenKey(
	        hCryptProv, 
	        CALG_RSA_SIGN, 
	        RSA1024BIT_KEY | CRYPT_EXPORTABLE, 
	        &hCryptKey))
		 {
		 	 inited = true;  
		 }
	}
}

crypto_key::~crypto_key() {
	if (inited) {
		CryptDestroyKey(hCryptKey);
	}
}

HCRYPTKEY crypto_key::get() {
	return hCryptKey;
}