/**
* \brief Calculates a 'roamable' signature for gamesave data.
*
* Gamesaves signed with a roamable signature are transferable between different consoles.
*
* This is the equivalent of calling XCalculateSignatureBegin, XCalculateSignatureUpdate, and XCalculateSignatureEnd within the Xbox XCalcSig API
*
* \param TitleSignatureKey Signature key stored in the certificate for a given Xbox executable file (.xbe)
* \param data Data to be signed
* \param length Number of bytes to include in signature calculation
**/
unsigned char* CalculateRoamable(unsigned char* TitleSignatureKey, unsigned char* data, int length)
{
    //key hard-coded in kernel. used for mixing the title signature key into an 'auth' key which is used to the sign save data 
    unsigned char XboxCertificateKey[0x14] = { 0x5C, 0x07, 0x33, 0xAE, 0x04, 0x01, 0xF7, 0xE8, 0xBA, 0x79, 0x93, 0xFD, 0xCD, 0x2F, 0x1F, 0xE0 };

    unsigned char* cert_key = (unsigned char*)malloc(0x10);
    unsigned char* auth_key = (unsigned char*)malloc(0x10);
    unsigned char* title_signature_key = (unsigned char*)malloc(0x10);
    unsigned char* roamable_signature = (unsigned char*)malloc(0x14);

    //copy keys (not necessary in C but for claritys sake)
    memcpy(cert_key, &XboxCertificateKey, 0x10); //copy 0x10 bytes
    memcpy(title_signature_key, TitleSignatureKey, 0x10); //copy 0x10 bytes

    //calc auth key
    hmac_sha1(cert_key, 0x10, title_signature_key, 0x10, auth_key);

    //calc roamable signature
    hmac_sha1(auth_key, 0x10, data, length, roamable_signature);

    //clean up
    free(cert_key);
    free(auth_key);
    free(title_signature_key);

    return roamable_signature; //must be freed by caller
}

/**
 * \brief Calculates a 'non-roamable' signature for gamesave data. 
 * 
 * Gamesaves signed a non-roamable signature are non-transferable between different consoles.
 * 
 * This is the equivalent of calling XCalculateSignatureBegin, XCalculateSignatureUpdate, XCalculateSignatureEnd, and XapiConvertSignatureToNonRoamable within the Xbox XCalcSig API
 * 
 * \param TitleSignatureKey Signature key stored in the certificate for a given Xbox executable file (.xbe)
 * \param XboxHDKey A unique 16 byte (0x10) console key stored in the Xbox EEPROM configuration data
 * \param data Data to be signed
 * \param length Number of bytes to include in signature calculation
 **/
unsigned char* CalculateNonRoamable(unsigned char* TitleSignatureKey, unsigned char* XboxHDKey, unsigned char* data, int length)
{
    unsigned char* xbox_hd_key = (unsigned char*)malloc(0x10);
    unsigned char* roamable_signature;
    unsigned char* nonroamable_signature = (unsigned char*)malloc(0x14);

    //copy keys (not necessary in C but for claritys sake)
    memcpy(xbox_hd_key, XboxHDKey, 0x10);

    //calc roamable signature
    roamable_signature = CalculateRoamable(TitleSignatureKey, data, length);

    //convert roamable signature to nonroamable signature
    hmac_sha1(xbox_hd_key, 0x10, roamable_signature, 0x14, nonroamable_signature);

    //clean up
    free(xbox_hd_key);
    free(roamable_signature);

    return nonroamable_signature; //must be freed by caller
}