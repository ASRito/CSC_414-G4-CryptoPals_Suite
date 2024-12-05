#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdint>
#include "BaseConversions.h"

// AES block size in bytes
const int BLOCK_SIZE = 16;

// AES S-box
const unsigned char sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 
};

// AES inverse S-box
const unsigned char inv_sbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d 
};

// AES round constant
const unsigned char Rcon[11] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

// Adds PKCS#7 padding to the input vector
vector<uint8_t> addPadding(const vector<uint8_t>& input) {
    size_t paddingSize = BLOCK_SIZE - (input.size() % BLOCK_SIZE);
    std::vector<uint8_t> paddedInput = input;

    // Add padding bytes
    for (size_t i = 0; i < paddingSize; ++i) {
        paddedInput.push_back(static_cast<uint8_t>(paddingSize));
    }

    return paddedInput;
}

// Removes PKCS#7 padding from the input vector
vector<uint8_t> removePadding(const vector<uint8_t>& input) {
    if (input.empty()) {
        throw std::invalid_argument("Input is empty");
    }

    // Get the last byte which indicates the number of padding bytes
    uint8_t paddingSize = input.back();

    // Check for valid padding size
    if (paddingSize < 1 || paddingSize > BLOCK_SIZE) {
        throw std::invalid_argument("Invalid padding size");
    }

    // Check if the padding bytes match the padding size
    for (size_t i = 1; i <= paddingSize; ++i) {
        if (input[input.size() - i] != paddingSize) {
            throw std::invalid_argument("Invalid padding");
        }
    }

    // Remove padding
    return vector<uint8_t>(input.begin(), input.end() - paddingSize);
}

vector<uint8_t> hexStringToBytes(const string& hex) {
    vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteString = hex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(stoul(byteString, nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

string bytesToHexString(const vector<uint8_t>& bytes) {
    stringstream ss;
    for (const auto& byte : bytes) {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

void RotWord(unsigned char* _w, unsigned char _d = 1, bool _right = 0) {
    //row is shifted _d times to the right if(_right) else left
    for (int r = 0; r < _d; r++)
    {
        unsigned char temp = _w[!_right ? 0 : 3];

        _w[!_right ? 0 : 3] = _w[!_right ? 1 : 2];
        _w[!_right ? 1 : 2] = _w[!_right ? 2 : 1];
        _w[!_right ? 2 : 1] = _w[!_right ? 3 : 0];
        _w[!_right ? 3 : 0] = temp;
    }
};

unsigned char SubByte(unsigned char _b)
{
    return sbox[_b];
}

void SubWord(unsigned char* _w) {
    for (int b = 0; b < 4; b++)
    {
        _w[b] = SubByte(_w[b]);
    }
}

// Key Expansion
void KeyExpansion(unsigned char* key, unsigned char* expandedKey) {
    const int Nk = 4; // Number of 32-bit words in the key (128 bits / 32 bits)
    const int Nb = 4; // Number of columns (32-bit words) in the state
    const int Nr = 10; // Number of rounds for AES-128

    for (int i = 0; i < Nk; i++) {
        expandedKey[i * 4]     = key[i * 4];
        expandedKey[i * 4 + 1] = key[i * 4 + 1];
        expandedKey[i * 4 + 2] = key[i * 4 + 2];
        expandedKey[i * 4 + 3] = key[i * 4 + 3];
    }

    unsigned char temp[4];
    // Generate round keys
    for (int i = Nk; i < Nb * (Nr + 1); i++) {
        temp[0] = expandedKey[(i - 1) * 4 + 0];
        temp[1] = expandedKey[(i - 1) * 4 + 1];
        temp[2] = expandedKey[(i - 1) * 4 + 2];
        temp[3] = expandedKey[(i - 1) * 4 + 3];

        if (i % Nk == 0) {
            // Rotate
            RotWord(temp);
            // Apply S-box
            SubWord(temp);

            // XOR with Rcon
            temp[0] ^= Rcon[i / Nk];
        } 

        expandedKey[i * 4 + 0] = expandedKey[(i - Nk) * 4 + 0] ^ temp[0];
        expandedKey[i * 4 + 1] = expandedKey[(i - Nk) * 4 + 1] ^ temp[1];
        expandedKey[i * 4 + 2] = expandedKey[(i - Nk) * 4 + 2] ^ temp[2];
        expandedKey[i * 4 + 3] = expandedKey[(i - Nk) * 4 + 3] ^ temp[3];
    }

    //cout << "DEBUGGING CHECK: PRINT EXPANDED KEY >>" << endl << expandedKey << endl << endl;
}

// Add Round Key
void AddRoundKey(unsigned char* state, unsigned char* roundKey) {
    for (int i = 0; i < 16; i++) {
        state[i] ^= roundKey[i];
    }
}

// Inverse Sub Bytes
void InvSubBytes(unsigned char* state) {
    for (int i = 0; i < 16; i++) {
        state[i] = inv_sbox[state[i]];
    }
}

// Inverse Shift Rows
void InvShiftRows(unsigned char* state) {
    unsigned char tempState[16];
    unsigned char tempWord[4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            tempWord[j] = state[(j * 4) + i];
        }

        RotWord(tempWord, i, 1);


        for (int j = 0; j < 4; j++)
        {
            tempState[(j * 4) + i] = tempWord[j];
        }
    }

    memcpy(state, tempState, 16);
}

uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1)
            p ^= a;
        bool hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set)
            a ^= 0x1B; // x^8 + x^4 + x^3 + x + 1
        b >>= 1;
    }
    return p;
}

// Inverse Mix Columns
void InvMixColumns(unsigned char* state) {
    for (int i = 0; i < 4; i++) {
        unsigned char a = state[i * 4 + 0];
        unsigned char b = state[i * 4 + 1];
        unsigned char c = state[i * 4 + 2];
        unsigned char d = state[i * 4 + 3];

        state[i * 4 + 0] = gmul(a, 14) ^ gmul(b, 11) ^ gmul(c, 13) ^ gmul(d, 9);
        state[i * 4 + 1] = gmul(a, 9) ^ gmul(b, 14) ^ gmul(c, 11) ^ gmul(d, 13);
        state[i * 4 + 2] = gmul(a, 13) ^ gmul(b, 9) ^ gmul(c, 14) ^ gmul(d, 11);
        state[i * 4 + 3] = gmul(a, 11) ^ gmul(b, 13) ^ gmul(c, 9) ^ gmul(d, 14);
    }
}

void decryptBlock(unsigned char* input, unsigned char* output, unsigned char* expandedKey) {
    const int Nr = 10; // Number of rounds for AES-128
    unsigned char state[16];
    memcpy(state, input, 16);

    // AddRoundKey (the last round key is added first)
    AddRoundKey(state, expandedKey + 16 * Nr);

    for (int round = Nr - 1; round > 0; round--) {
        InvShiftRows(state); // Inverse Shift Rows
        InvSubBytes(state);   // Inverse Sub Bytes
        AddRoundKey(state, expandedKey + 16 * round); // Add Round Key
        InvMixColumns(state); // Inverse Mix Columns
    }

    // Final round (no InvMixColumns)
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, expandedKey); // Add initial round key

    memcpy(output, state, 16);
}

vector<uint8_t> decryptHexString(const string& hexData, const string& _key) {
    if (_key.length() != 16) {
        throw invalid_argument("input key must be 16 bytes, current length: " + _key.length());
    }
    if (hexData.length() < (2 * BLOCK_SIZE)) {
        throw invalid_argument("input must be a multiple of 16 bytes, current length: " + (hexData.length() / 2));
    }

    // Convert hex string to bytes
    vector<uint8_t> encryptedData = hexStringToBytes(hexData);

    // Prepare expanded key
    unsigned char expandedKey[176]; // 16 bytes * (10 rounds + 1)
    unsigned char* key = new unsigned char[16];
    copy(_key.begin(), _key.end(), key);
    KeyExpansion(key, expandedKey);

    // Prepare the decrypted data buffer
    vector<uint8_t> decryptedData(encryptedData.size());

    for (size_t i = 0; i < encryptedData.size(); i += BLOCK_SIZE) {
        unsigned char block[16];
        size_t blockSize = min(size_t(16), encryptedData.size() - i);
        memcpy(block, &encryptedData[i], blockSize);

        unsigned char decryptedBlock[16];
        decryptBlock(block, decryptedBlock, expandedKey);

        memcpy(&decryptedData[i], decryptedBlock, blockSize);
    }

    return decryptedData;
}

vector<uint8_t> decryptB64String(const string& B64Data, const string& key) {
    return decryptHexString(b64_str_to_hex_str(B64Data), key);
}
