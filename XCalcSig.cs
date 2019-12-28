        /// <summary>
        /// Calculates a 'roamable' signature for gamesave data. 
        /// 
        /// Gamesaves signed with a roamable signature are transferable between different consoles.
        /// 
        /// This is the equivalent of calling XCalculateSignatureBegin, XCalculateSignatureUpdate, and XCalculateSignatureEnd within the Xbox XCalcSig API
        /// </summary>
        /// <param name="TitleSignatureKey">Signature key stored in the certificate for a given Xbox executable file (.xbe)</param>
        /// <param name="data">Data to be signed</param>
        /// <param name="index">Index to begin signature calculation</param>
        /// <param name="length">Number of bytes to include in signature calculation</param>
        byte[] CalculateRoamable(byte[] TitleSignatureKey, byte[] data, int index, int length)
        {
            //key hard-coded in kernel. used for mixing the title signature key into an 'auth' key which is used to the sign save data 
            byte[] XboxCertificateKey = { 0x5C, 0x07, 0x33, 0xAE, 0x04, 0x01, 0xF7, 0xE8, 0xBA, 0x79, 0x93, 0xFD, 0xCD, 0x2F, 0x1F, 0xE0 };

            HMACSHA1 hmac_sha1;
            byte[] cert_key = new byte[0x10];
            byte[] auth_key = new byte[0x10];
            byte[] title_signature_key = new byte[0x10];
            byte[] roamable_signature;

            //copy keys
            Array.Copy(XboxCertificateKey, 0, cert_key, 0, 0x10);
            Array.Copy(TitleSignatureKey, 0, title_signature_key, 0, 0x10);

            //calc auth key
            hmac_sha1 = new HMACSHA1(cert_key);
            Array.Copy(hmac_sha1.ComputeHash(title_signature_key), 0, auth_key, 0, 0x10);
            hmac_sha1.Dispose();

            //calc roamable signature
            hmac_sha1 = new HMACSHA1(auth_key);
            roamable_signature = hmac_sha1.ComputeHash(data, index, length);
            hmac_sha1.Dispose();

            return roamable_signature;
        }

        /// <summary>
        /// Calculates a 'non-roamable' signature for gamesave data. 
        /// 
        /// Gamesaves signed a non-roamable signature are non-transferable between different consoles.
        /// 
        /// This is the equivalent of calling XCalculateSignatureBegin, XCalculateSignatureUpdate, XCalculateSignatureEnd, and XapiConvertSignatureToNonRoamable within the Xbox XCalcSig API
        /// </summary>
        /// <param name="TitleSignatureKey">Signature key stored in the certificate for a given Xbox executable file (.xbe)</param>
        /// <param name="XboxHDKey">A unique 16 byte (0x10) console key stored in the Xbox EEPROM configuration data</param>
        /// <param name="data">Data to be signed</param>
        /// <param name="index">Index to begin signature calculation</param>
        /// <param name="length">Number of bytes to include in signature calculation</param>
        byte[] CalculateNonRoamable(byte[] TitleSignatureKey, byte[] XboxHDKey, byte[] data, int index, int length)
        {
            HMACSHA1 hmac_sha1;
            byte[] xbox_hd_key = new byte[0x10];
            byte[] roamable_signature;
            byte[] nonroamable_signature;

            //copy keys
            Array.Copy(XboxHDKey, 0, xbox_hd_key, 0, 0x10); //copy 0x10 bytes

            //calc roamable signature
            roamable_signature = CalculateRoamable(TitleSignatureKey, data, index, length);

            //convert roamable signature to nonroamable signature
            hmac_sha1 = new HMACSHA1(xbox_hd_key);
            nonroamable_signature = hmac_sha1.ComputeHash(roamable_signature);
            hmac_sha1.Dispose();

            return nonroamable_signature;
        }
