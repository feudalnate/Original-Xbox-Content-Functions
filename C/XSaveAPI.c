/**
* \brief Generates SaveId from save name
* \param name Save name (eg: name of the in-game profile)
* \param length Number of characters in the save name
* \returns Returns an ID unique to the savedata name, used as a folder name for storing the savedata (ie: '\UDATA\[TitleId]\[SaveId]\savedatafile')
**/
char* XapiSaveGameHashEncode(char* name, int length)
{
    unsigned long long hash = 0;
    char temp;
    int bit_index = 0;
    int char_index = 11;
    char* result = (char*)malloc(12);

    //calc hash
    for(int i = 0; i < length; i++)
    {
        hash = ((name[i] + (hash << 0x10)) % 0xFFFFFFFFFFC5);
    }

    //convert to hex string (swapping endian of output)
    do
    {
        temp = (char)((hash >> bit_index) & 0xFF);
        if (temp > 9)
        {
            temp = (char)(temp + 55);
        }
        else
        {
            temp = (char)(temp + 48);
        }
        result[char_index] = temp;
        bit_index += 4;
        char_index--;
    }
    while (bit_index <= 44);
    
    return result; //must be freed by caller
}