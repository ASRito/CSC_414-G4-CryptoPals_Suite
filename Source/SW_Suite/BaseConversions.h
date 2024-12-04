// BaseConversions.h: contains quick conversion functions for base 2, 16, and 64, as well as ASCII standard

#pragma once

#include <iostream>
#include <string>
#include <bitset>
#include <utility>

using namespace std;

const char* hexLookup = "0123456789abcdef";
const char* b64Lookup = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//The ASCII Lookup array below omits the first 32 ASCII characters, which are not printable characters. This may need resolving later.
//const char* ASCIILookup = " !\"#\$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

//Takes 1 hexadecimal argument and converts to binary equivalent, returned as an array of 4 characters.
const char* hex_char_to_bin(char c)
{
    //Submitted hexadecimal character is forced into upper-case, then used as a switch argument.
    //Because there are only 16 values to choose from, and they must return a binary string, --
    //  this seemed more efficient than looping through the lookup table for a match and converting that index to binary

    switch (toupper(c))
    {
    case '0': return "0000";
    case '1': return "0001";
    case '2': return "0010";
    case '3': return "0011";
    case '4': return "0100";
    case '5': return "0101";
    case '6': return "0110";
    case '7': return "0111";
    case '8': return "1000";
    case '9': return "1001";
    case 'A': return "1010";
    case 'B': return "1011";
    case 'C': return "1100";
    case 'D': return "1101";
    case 'E': return "1110";
    case 'F': return "1111";
    default: return "0000";     //in the case that the submitted character matches no values, all 0's are returned.
    }
}

//Takes a pointer to the necessary binary string, and an unsigned int argument to determine where to begin the substring. Returns single hex encoded char. 
char bin_to_hex_char(const string& _bin, unsigned _ind)
{
    //4 binary bits -> 1 hexadecimal digit, so the return is only 1 char.
    char tmp_hex;

    //Take only the 4-bit segment of the binary string we are decoding here.
    string tmp_bin = _bin.substr(_ind, 4);
    //Convert that substring to an equivalent integer, treating the string correctly as printed binary
    int tmp_dec = stoi(tmp_bin, nullptr, 2);
    //Use the converted binary integer as an index in the lookup table for this base
    tmp_hex = hexLookup[tmp_dec];

    return tmp_hex;
}
//Alternate method definition for converting single digits where a starting index is not needed
char bin_to_hex_char(const string& _bin)
{
    char hex;
    string tmp_bin = _bin;

    while (tmp_bin.length() % 4 != 0)
    {
        tmp_bin += '0';
    }

    hex = bin_to_hex_char(tmp_bin, 0);

    return hex;
}

//Takes 1 base64 encoded char and converts to binary equivalent, returned as string of 6 chars.
string b64_char_to_bin(char c)
{
    if (c == '\n' || c == '=') return "";

    //Scan through the lookup table for an exact char match, --
    //  if the index var reaches 64, the index is out of range of the table and should error (return the 63rd table entry)
    unsigned tmp_ind = 0;
    while (c != b64Lookup[tmp_ind] && tmp_ind < 64)
    {
        tmp_ind++;
    }

    //convert the index integet into a 6-bit binary form, then to string for return
    string tmp_bin = bitset<6>(tmp_ind).to_string();

    return tmp_bin;
}

//Takes a pointer to the necessary binary string, and an unsigned int argument to determine where to begin the substring. Returns a pair of base 64 encoded chars as a string.
string bin_to_b64_char(const string& _bin, unsigned _ind)
{
    string tmp_b64;

    //Because base 64 functions on sets of 6 bits, this encoding must be done 2 base 64 chars at a time. 
    //  This divides 3 hexadecimal decoded values worth of binary into 2 base 64 encoded values
    for (unsigned long int i = 0; i < 2; i++)
    {
        //Take a substring from the starting index (0 on first loop, then 6 on the second) that is 6-bits long
        string tmp_bin = _bin.substr(_ind + (i * 6), 6);
        //Convert 6-bit binary substring into equivalent int value
        int tmp_dec = stoi(tmp_bin, nullptr, 2);
        //Use the converted binary integer as an index in the lookup table for this base
        tmp_b64 += b64Lookup[tmp_dec];
    }

    return tmp_b64;
}

//This is mainly for attempting to view the decoded message as plain text before encoding again
//Takes a pointer to the necessary binary string, and an unsigned int argument to determine where to begin the substring. Returns single ASCII encoded char.
char bin_to_ASCII_char(const string& _bin, unsigned _ind)
{
    //Define default return value as char '0' for the case where the integer exceeds the look up table's range
    char tmp_ascii = '0';

    //Take a substring from the starting index that is 8-bits long
    string tmp_bin = _bin.substr(_ind, 8);
    //Convert 8-bit binary substring into equivalent int value
    int tmp_dec = stoi(tmp_bin, nullptr, 2);
    //Use the converted binary integer as an index in the lookup table for ASCII encoding (minus 32 to account for omitted non-printable type characters in ASCII)
    tmp_ascii = (unsigned char)tmp_dec;

    return tmp_ascii;
}

string ASCII_char_to_bin(const char& _c)
{
    if (_c >= 0 && _c < 256)
        return bitset<8>(_c).to_string();
    else
        return bitset<8>((char)32).to_string();
}

//Takes pointer to the necessary hex string and converts to binary equivalent, returns binary string.
string hex_str_to_bin_str(const string& hex)
{
    string bin;

    //Loop through each hex character and convert to binary set, then add to total string
    for (unsigned long int i = 0; i != hex.length(); ++i)
        bin += hex_char_to_bin(hex[i]);

    return bin;
}

//Takes pointer to the necessary binary string and converts to base64 equivalent, returns base
string bin_str_to_b64_str(const string& bin)
{
    //create duplicate binary string for manipulation of length (padding)
    string tmp_bin = bin;
    string base64;

    //if the length of the binary string is not evenly divisible into sextets, add trailing 0's as padding
    while (tmp_bin.length() % 6 != 0)
    {
        tmp_bin += '0';
    }

    //Loop through all binary characters in groups of 12 (for even division of data) and convert each sextet to base64
    for (unsigned long int j = 0; j < tmp_bin.length(); j += 12)
        base64 += bin_to_b64_char(tmp_bin, j);

    return base64;
}

//Takes pointer to base 64 string and converts it to binary equivalent, returns binary string
string b64_str_to_bin_str(const string& b64)
{
    string bin;

    //TODO: account for base64 padding characters?

    // Loop through each base64 char and convert it to a sextet of binary characters
    for (unsigned long int i = 0; i < b64.length(); i++)
        bin += b64_char_to_bin(b64[i]);

    return bin;
}

//Takes pointer to binary string and converts it to hexadecimal equivalent, returns hexadecimal string
string bin_str_to_hex_str(const string& bin)
{
    //duplicate binary string for length manipulation (padding)
    string hex;
    string tmp_bin = bin;

    //if the length of the binary string is not evenly divisible into quads, add trailing 0's as padding
    while (tmp_bin.length() % 4 != 0)
    {
        tmp_bin += '0';
    }

    //Loop through all binary characters in groups of 4 and convert each quad to hex
    for (unsigned long int j = 0; j < tmp_bin.length(); j += 4)
        hex += bin_to_hex_char(tmp_bin, j);

    return hex;
}

//Shortcut function: takes pointer to hex string and return base64 string equivalent
string hex_str_to_b64_str(const string& hex)
{
    string bin = hex_str_to_bin_str(hex);
    string base64 = bin_str_to_b64_str(bin);

    return base64;
}

//Shortcut function: takes pointer to base64 string and return hex string equivalent
string b64_str_to_hex_str(const string& b64)
{
    string bin = b64_str_to_bin_str(b64);
    string hex = bin_str_to_hex_str(bin);

    return hex;
}

//Takes pointer to binary string and converts it to ASCII standard equivalent, returns ASCII string (plain-text)
string bin_str_to_ASCII_str(const string& bin)
{
    //duplicate binary string for length manipulation (padding)
    string ascii;
    string tmp_bin = bin;

    //if the length of the binary string is not evenly divisible into octets, add trailing 0's as padding
    while (tmp_bin.length() % 8 != 0)
    {
        tmp_bin += '0';
    }

    //Loop through all binary characters in groups of 8 and decode each octet according to ASCII
    for (unsigned long int j = 0; j < tmp_bin.length(); j += 8)
        ascii += bin_to_ASCII_char(tmp_bin, j);

    return ascii;
}

string ASCII_str_to_bin_str(const string& ascii)
{
    string tmp_bin;

    //Loop through all binary characters in groups of 8 and decode each octet according to ASCII
    for (unsigned long int i = 0; i < ascii.length(); i++)
        tmp_bin += ASCII_char_to_bin(ascii[i]);

    return tmp_bin;
}

string ASCII_str_to_hex_str(const string& ascii)
{
    string tmp_bin = ASCII_str_to_bin_str(ascii);

    return bin_str_to_hex_str(tmp_bin);
}

string hex_str_to_ASCII_str(const string& hex)
{
    string tmp_bin = hex_str_to_bin_str(hex);

    return bin_str_to_ASCII_str(tmp_bin);
}

string b64_str_to_ASCII_str(const string& b64)
{
    string tmp_bin = b64_str_to_bin_str(b64);

    return bin_str_to_ASCII_str(tmp_bin);
}
