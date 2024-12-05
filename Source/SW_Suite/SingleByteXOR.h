#pragma once

using namespace std;
#include <string>
//#include <iostream>
#include <bitset>
#include <vector>
#include <fstream>
#include "BaseConversions.h"
#include "FixedXOR.h"

// arrays for letters and frequency
const double freq[] = { // frequency distribution for letters - got from google - many sources
    8.17, 1.49, 2.78, 4.25, 12.70, 2.23, 2.02, 6.09, 6.97, 0.15,
    0.77, 4.03, 2.41, 6.75, 7.51, 1.93, 0.10, 5.99, 6.33, 9.06,
    2.76, 0.98, 2.36, 0.15, 1.97, 0.07, 20.00 // frequency for space - got from researchgate
};

// putting letter index in the frequency array
int getFrequencyIndex(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    }
    else if (c == ' ') {
        return 26;
    }
    else {
        return -1;
    }
}

// for scoring a decoded message on letter frequency
double score_message(const string& message) {
    double score = 0.0;

    for (char c : message) {
        int index = getFrequencyIndex(c);
        if (index >= 0) {
            score += freq[index];
        }
        /* Not required.
        else if (isdigit(c)) {
            score -= 5.0; //
        // assigned negative score for numerals because the question says english...
        // letters only and it will reduce the chances of numbered phrases and...
        // negative marking is random but i guess it will work regardless
        // Special characters will automaticaly get 0 points
        } */
    }

    return score;
}

//takes a hex string pointer and a char to encode against, returns ciphered / deciphered string
//uses FixedXOR.FixedStringXOR() to cipher one character at a time, satisfying equal buffer length requirement
string SingleByteXORCipher(const string& hex, char key)
{
    string result = "";
    string tmp_bin = hex_str_to_bin_str(hex);

    for (int c = 0; c < tmp_bin.length(); c += 8)
    {
        result += FixedStringXOR(tmp_bin.substr(c, 8), bitset<8>(key).to_string());
    }

    result = bin_str_to_hex_str(result);

    return result;
}

//given hex string, attempts to brute-force decrypt following single byte XOR rules
string DecryptSingleByteXOR(const string& hex, bool print) {
    //string tmp_bin = hex_str_to_bin_str(hex);

    double best_score = 00;
    int best_key = 0;
    string best_message;

    // Try all 256 single-byte keys
    for (int key = 0; key < 256; ++key) {
        string xor_result = hex_str_to_bin_str(SingleByteXORCipher(hex, key));
        string ascii_result = bin_str_to_ASCII_str(xor_result);
        double score = score_message(ascii_result);

        // Print the results and scores
        //cout << "Key: " << key << " -> Result: " << xor_result << " -> Score: " << score << endl;

        // Check if this is the best until now
        if (score > best_score) {
            best_score = score;
            best_key = key;
            best_message = ascii_result;
        }
    }

    // Print the best key
    if (print)
        cout << "\nBest Key: " << best_key << " -> Best Message: " << best_message << " -> Best Score: " << best_score << endl;
    return best_message;
}

string SBXOR_DetectionAlgorithm(vector<string> TestList)
{
    double High_Score = 0;
    double Test_Score;
    string Best_Message;
    //string Test_Message;
    string Decrypted_Best_Message;

    //Brute force decryption of every string. Keeps track of the most likely candidate.
    for (int i = 0; i < TestList.size(); i++)
    {
        //Test_Message = DecryptSingleByteXOR(TestList[i]); 
        Test_Score = score_message(DecryptSingleByteXOR(TestList[i].substr(0, TestList[i].length() - 2), 0));
        if (Test_Score > High_Score)
        {
            High_Score = Test_Score;
            Best_Message = TestList[i].substr(0, TestList[i].length() - 2);
            //Decrypted_Best_Message = Test_Message;
        }
    }

    Decrypted_Best_Message = DecryptSingleByteXOR(Best_Message, 1);

    cout << "Encrypted Message: " << Best_Message << endl << "Decrypted Message: " << Decrypted_Best_Message << endl << "Score: " << High_Score << endl;

    return Decrypted_Best_Message;
}

//Made this return a string so that it could be tested against
string SBXOR_FileEncryptionDetection(string FileName)
{
    ifstream InputFile(FileName);

    if (InputFile.is_open() == false)
    {
        return "File Could Not Open";
        //This gets returned when the function is ran from the test cpp, for some reason
    }


    string line;
    vector<string> TestList;
    string best_message;

    while (getline(InputFile, line))
    {
        TestList.push_back(line);
    }

    return SBXOR_DetectionAlgorithm(TestList);

}


