/// <summary>
/// Calculates the signature key used for signing the header data of ContentMeta.xbx files
/// </summary>
/// <param name="TitleID">Unique title identifier value stored in the certificate for a given Xbox executable file (.xbe)</param>
/// <param name="XboxHDKey">A unique 16 byte (0x10) console key stored in the Xbox EEPROM configuration data</param>
byte[] ComputeContentSignatureKey(uint TitleID, byte[] XboxHDKey)
{
    HMACSHA1 hmac_sha1;
    byte[] xbox_hd_key = new byte[0x10];
    byte[] title_id = new byte[4];
    byte[] content_signature_key;

    //bring titleid into buffer (little endian)
    title_id[0] = (byte)(TitleID & 0xFF);
    title_id[1] = (byte)((TitleID >> 8) & 0xFF);
    title_id[2] = (byte)((TitleID >> 16) & 0xFF);
    title_id[3] = (byte)((TitleID >> 24) & 0xFF);
    
    //copy key
    Array.Copy(XboxHDKey, 0, xbox_hd_key, 0, 0x10); //copy 0x10 bytes

    //calc signature key
    hmac_sha1 = new HMACSHA1(xbox_hd_key);
    content_signature_key = hmac_sha1.ComputeHash(title_id);
    hmac_sha1.Dispose();

    return content_signature_key;
}

/// <summary>
/// Calculates a 'non-roamable' signature for ContentMeta.xbx header data
/// 
/// Applicable to all downloadable content and title updates
/// </summary>
/// <param name="TitleID">Unique title identifier value stored in the certificate for a given Xbox executable file (.xbe)</param>
/// <param name="XboxHDKey">A unique 16 byte (0x10) console key stored in the Xbox EEPROM configuration data</param>
/// <param name="data">Data to be signed</param>
/// <param name="index">Index to begin signature calculation</param>
/// <param name="length">Number of bytes to include in signature calculation</param>
byte[] ComputeContentNonRoamable(uint TitleID, byte[] XboxHDKey, byte[] data, int index, int length)
{
    HMACSHA1 hmac_sha1;
    byte[] content_signature_key;
    byte[] content_nonroamable_signature;

    //calc signature key
    content_signature_key = ComputeContentSignatureKey(TitleID, XboxHDKey);

    //calc signature
    hmac_sha1 = new HMACSHA1(content_signature_key);
    content_nonroamable_signature = hmac_sha1.ComputeHash(data, index, length);
    hmac_sha1.Dispose();

    return content_nonroamable_signature;
}
