        string XapiSaveGameHashEncode(string name, int length)
        {
            //calculate hash value
            ulong hash = 0;
            for (int i = 0; i < length; i++)
            {
                hash = ((name[i] + (hash << 0x10)) % 0xFFFFFFFFFFC5);
            }
            //convert to hex string (swap endian of output)
            char[] result = new char[12];
            char value;
            int bit_index = 0;
            int char_index = 11;
            do
            {
                value = (char)((hash >> bit_index) & 0xF);
                if (value > 9)
                {
                    value = (char)(value + 55);
                }
                else
                {
                    value = (char)(value + 48);
                }
                result[char_index] = value;
                bit_index += 4;
                char_index--;
            }
            while (bit_index <= 44);
            return new string(result);
        }
