#pragma once

using namespace std;
#include <string>
//#include <iostream>
#include "BaseConversions.h"

//deprecated in favor of reusing existing conversions, apologies Cameron
/*
// Helper function to convert a hexadecimal character to its integer value
int hexCharToInt(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    else if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    else if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    throw invalid_argument("Invalid hex character");
}

// Helper function to convert an integer (0-15) to a hexadecimal character
char intToHexChar(int i) {
    if (i >= 0 && i <= 9) return '0' + i;
    else if (i >= 10 && i <= 15) return 'a' + (i - 10);
    throw invalid_argument("Invalid integer value");
}
*/

//XOR two binary strings of equal length (core module of other bases XOR functions as well)
string FixedStringXOR(const string& binA, const string& binB)
{
    string result = "";
    if (binA.length() != binB.length()) {
        throw invalid_argument("Strings must be of equal length.");
    }

    //iterate through length of string applying XOR between corresponding bits of the two strings
    for (int i = 0; i < binA.length(); i++) {
        if ((binA[i] != '0' && binA[i] != '1') || (binB[i] != '0' && binB[i] != '1'))
            throw invalid_argument("Strings must represent binary decodings.");

        result += (binA[i] == binB[i]) ? '0' : '1';
    }
    return result;
}

char FixedCharXOR(const char& cA, const char& cB)
{
    return bin_to_ASCII_char(FixedStringXOR(bitset<8>(cA).to_string(), bitset<8>(cB).to_string()), 0);
}

// XOR two hexadecimal strings of equal length
string FixedXOR_hex(const string& hexA, const string& hexB)
{
    if (hexA.length() != hexB.length()) {
        throw invalid_argument("Hexadecimal strings must be of equal length.");
    }

    //convert hex strings to binary
    string tmp_binA = hex_str_to_bin_str(hexA);
    string tmp_binB = hex_str_to_bin_str(hexB);
    //pass through binary fixedXOR, then encode result back into hex
    return bin_str_to_hex_str(FixedStringXOR(tmp_binA, tmp_binB));

    //deprecated in favor of reusing existing conversions
    /*string result = "";
    for (size_t i = 0; i < hexA.length(); ++i) {
        int valA = hexCharToInt(hexA[i]);
        int valB = hexCharToInt(hexB[i]);
        int xorResult = valA ^ valB;
        result += intToHexChar(xorResult);
    }
    return result;*/
}

// XOR two base64 strings of equal length
string FixedXOR_b64(const string& b64A, const string& b64B)
{
    if (b64A.length() != b64B.length())
        throw invalid_argument("Base64 strings must be of equal length.");

    //convert b64 strings to binary
    string tmp_binA = b64_str_to_bin_str(b64A);
    string tmp_binB = b64_str_to_bin_str(b64B);
    //pass through binary fixedXOR, then encode result back into base 64
    return bin_str_to_b64_str(FixedStringXOR(tmp_binA, tmp_binB));
}