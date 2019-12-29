/**
* \brief Calculates the signature key used for signing the header data of ContentMeta.xbx files
* \param TitleID Unique title identifier value stored in the certificate for a given Xbox executable file (.xbe)
* \param XboxHDKey A unique 16 byte (0x10) console key stored in the Xbox EEPROM configuration data
**/
unsigned char* ComputeContentSignatureKey(unsigned int TitleID, unsigned char* XboxHDKey)
{
    unsigned char* xbox_hd_key = (unsigned char*)malloc(0x10);
    unsigned char* title_id = (unsigned char*)malloc(4);
    unsigned char* content_signature_key = (unsigned char*)malloc(0x14);

    //bring titleid into buffer (little endian)
    title_id[0] = (unsigned char)(TitleID & 0xFF);
    title_id[1] = (unsigned char)((TitleID >> 8) & 0xFF);
    title_id[2] = (unsigned char)((TitleID >> 16) & 0xFF);
    title_id[3] = (unsigned char)((TitleID >> 24) & 0xFF);

    //copy key
    memcpy(xbox_hd_key, XboxHDKey, 0x10);

    //calc content key
    hmac_sha1(xbox_hd_key, 0x10, title_id, 4, content_signature_key);

    //clean up
    free(xbox_hd_key);
    free(title_id);

    return content_signature_key; //must be freed by caller
}

/**
* \brief Calculates a 'non-roamable' signature for ContentMeta.xbx header data
*
* Applicable to all downloadable content and title updates
*
* \param TitleID Unique title identifier value stored in the certificate for a given Xbox executable file (.xbe)
* \param XboxHDKey A unique 16 byte (0x10) console key stored in the Xbox EEPROM configuration data
* \param data Data to be signed
* \param length Number of bytes to include in signature calculation
**/
unsigned char* ComputeContentNonRoamable(unsigned int TitleID, unsigned char* XboxHDKey, unsigned char* data, int length)
{
    unsigned char* content_signature_key;
    unsigned char* content_nonroamable_signature = (unsigned char*)malloc(0x14);

    //calc signature key
    content_signature_key = ComputeContentSignatureKey(TitleID, XboxHDKey);

    //calc signature
    hmac_sha1(content_signature_key, 0x14, data, length, content_nonroamable_signature);

    //clean up
    free(content_signature_key);

    return content_nonroamable_signature; //must be freed by caller
}