#pragma once

using namespace std;
#include <string>
#include "BaseConversions.h"
#include "FixedXOR.h"
#include "SingleByteXOR.h"
#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>
#include <iomanip>

int calculateEditDistance(const string& textA, const string& textB, unsigned char _base = 0);

string repeatingKeyXORCipher(const string& text, const string& key) {
    string result;

    // XOR each character of the text with the repeating key
    for (int i = 0; i < text.length(); ++i) {
        // XOR the current character with the corresponding key character
        char encryptedChar = FixedCharXOR(text[i], key[i % key.length()]);

        // Append the result as ASCII
        result += encryptedChar;
    }

    return result;
}

int calculateEditDistance(const string& textA, const string& textB, unsigned char _base)
{
    unsigned int differingBits = 0;

    string tmp_textA;
    string tmp_textB;

    switch (_base)
    {
    case 0:
        //cout << textA << " | " << textB << " : ";
        tmp_textA = ASCII_str_to_hex_str(textA);
        tmp_textB = ASCII_str_to_hex_str(textB);
        break;
    case 2:
        tmp_textA = bin_str_to_hex_str(textA);
        tmp_textB = bin_str_to_hex_str(textB);
        break;
    case 16:
        tmp_textA = textA;
        tmp_textB = textB;
        break;
    case 64:
        tmp_textA = b64_str_to_hex_str(textA);
        tmp_textB = b64_str_to_hex_str(textB);
        break;
    }

    for (unsigned int h = 0; h < tmp_textA.length(); h++)
    {
        string tmp_binA = hex_char_to_bin(tmp_textA[h]);
        string tmp_binB = hex_char_to_bin(tmp_textB[h]);

        for (char b = 0; b < 4; b++)
        {
            differingBits += (tmp_binA[b] == tmp_binB[b]) ? 0 : 1;
            //cout << "Bit check 1: " << tmp_binA[b] << " : Bit check 2: " << tmp_binB[b] << endl;
        }
    }

    //cout << differingBits << endl;
    //cout << tmp_textA << " | " << tmp_textB << " : " << differingBits << endl;
    return differingBits;
}


// average normalized edit distance for a given key size
double calculateNormalizedEditDistance(const string& byteText, int keySize)
{
    //int totalDistance = 0;
    int subDistance = 0;
    //int numberOfComparisons = 0;

    // Divide the binary string into chunks keySize
    //for (unsigned long int i = 0; (i + 4 * keySize) <= byteText.length(); i += keySize)
    //{
        //subDistance = 0;
    string chunkA = byteText.substr(0, keySize);
    string chunkB = byteText.substr(1 * keySize, keySize);
    string chunkC = byteText.substr(2 * keySize, keySize);
    string chunkD = byteText.substr(3 * keySize, keySize);

    /*
    int distance1 = calculateEditDistance(chunkA, chunkB);
    int distance2 = calculateEditDistance(chunkC, chunkD);

    if (distance1 >= 0 && distance2 >= 0)
    {
        totalDistance += (distance1 + distance2);
        numberOfComparisons += 2;
    }
    */

    //all 6 combinations of the above chunks
    subDistance += calculateEditDistance(chunkA, chunkB);
    subDistance += calculateEditDistance(chunkA, chunkC);
    subDistance += calculateEditDistance(chunkA, chunkD);
    subDistance += calculateEditDistance(chunkB, chunkC);
    subDistance += calculateEditDistance(chunkB, chunkD);
    subDistance += calculateEditDistance(chunkC, chunkD);
    double avgDistance = (double)subDistance / (double)6;
    //cout << "avg: " << avgDistance << endl;
//}

// normalized edit distance
//return (numberOfComparisons > 0) ? (double)totalDistance / (keySize * numberOfComparisons) : numeric_limits<double>::max();
    return avgDistance / (double)keySize;
}


/*
double calculateNormalizedEditDistance(const string& byteText, int keySize)
{
    string chunkA = byteText.substr(0, keySize);
    string chunkB = byteText.substr(keySize, keySize);

    double distance = (double)calculateEditDistance(chunkA, chunkB) / (double)keySize;

    return distance;
}
*/

// find the most likely key size based on the lowest normalized Hamming distance
vector<int> findLikelyKeySizes(const string& byteText, int minKeySize, int maxKeySize, int numKeys)
{
    if (maxKeySize > 100)
    {
        throw invalid_argument("Max key size is too large");
    }


    vector<pair<int, double>> keySizesWithDistances;

    for (int keySize = minKeySize; keySize <= maxKeySize; keySize++)
    {
        double normalizedDistance = calculateNormalizedEditDistance(byteText, keySize);

        cout << setw(10) << keySize << " | " << fixed << setprecision(6) << normalizedDistance << endl;

        keySizesWithDistances.push_back({ keySize, normalizedDistance });
    }

    sort(keySizesWithDistances.begin(), keySizesWithDistances.end(),
        [](const pair<int, double>& a, const pair<int, double>& b) {
            return a.second < b.second;
        });

    vector<int> likelyKeySizes;
    for (int i = 0; i < numKeys && i < keySizesWithDistances.size(); i++)
    {
        likelyKeySizes.push_back(keySizesWithDistances[i].first);
    }

    return likelyKeySizes;
}

vector<string> transposeBlocks(const string& byteText, int keySize)
{
    vector<string> transposedBlocks(keySize);
    for (size_t i = 0; i < byteText.size(); i++) {
        transposedBlocks[i % keySize] += byteText[i];
    }
    return transposedBlocks;
}

// Function to decrypt a block using single-character XOR
pair<char, string> decryptSingleCharacterXOR(const string& block)
{
    char bestKey = 0;
    int bestScore = numeric_limits<int>::min();
    string bestDecodedText;

    for (int key = 0; key <= 255; key++)
    {
        string decodedText;

        for (char c : block)
        {
            decodedText += c ^ key;
        }

        int score = score_message(decodedText);

        // Keep track of the best key and decoded text
        if (score > bestScore)
        {
            bestScore = score;
            bestKey = key;
            bestDecodedText = decodedText;

            //cout << ASCIIEncodedText << endl << bestDecodedText << endl;
        }
    }

    return { bestKey, bestDecodedText };
}

// ddecrypt the entire ciphertext using the repeating key
pair<string, string> decryptRepeatingKeyXOR(const string& byteText, int keySize)
{
    // Transpose the blocks based on the key size
    vector<string> transposedBlocks = transposeBlocks(byteText, keySize);

    // Store the repeating key
    string key;
    vector<string> decodedBlocks(keySize);

    // Decrypt each block using single-character XOR
    for (int i = 0; i < keySize; i++)
    {
        pair <char, string> result = decryptSingleCharacterXOR(transposedBlocks[i]);
        char keyByte = result.first;
        string decodedText = result.second;
        key += keyByte;
        decodedBlocks[i] = decodedText;
    }

    // Combine the decoded blocks to form the final decrypted message
    string decryptedText;
    size_t blockSize = decodedBlocks[0].size();

    for (size_t i = 0; i < blockSize; i++)
    {
        for (int j = 0; j < keySize; j++)
        {
            if (i < decodedBlocks[j].size())
                decryptedText += decodedBlocks[j][i];
        }
    }

    return { key, decryptedText };
}