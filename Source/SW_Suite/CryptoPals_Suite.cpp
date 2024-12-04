/* 
Institution: University of Southern Mississippi

Authors:
    Bradley Bellipanni
    Cameron Bullock
    Ethan Chiemprabha
    Kuldeep Poptani
    Christopher Reed
    Anthony Rito
    Jaiden Sutton
    
Purpose: A basic menu system for the individual execution of the first 8 task tests 
         provided by the CyrptoPals organization as an introductory exercise in cryptography

         (Also useful as reference for the integration of the individual utilities into other
          projects, showing how they are to be imported, and what functions are accessible)

Start Date:        Sept. 7, 2024
Completion Date:   Nov. 12, 2024 
*/

#include "File_Parser.h"
#include "TestingTemplates.h"

#include "BaseConversions.h"
#include "FixedXOR.h"
//#include "SingleByteXOR.h"
//#include "RepeatingKeyXOR.h"
//#include "AES128.h"
//#include "ECBDetection.h"


#include <iostream>
#include <string>

//Task 1: Convert between base encoding systems (binary, hexadecimal, base 64, and unicode)
void Task1Eval()
{
    string inputHexA = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    string outputBin;
    string outputASCII;
    string outputBase64;
    string ExpectedBase64 = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

    outputBin = hex_str_to_bin_str(inputHexA);
    outputASCII = bin_str_to_ASCII_str(outputBin);
    outputBase64 = hex_str_to_b64_str(inputHexA);

    cout << endl << endl << endl << endl;
    cout << "Task 1 Evaluation: _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _" << endl;
    cout << "Input Hexadecimal: " << inputHexA << endl << "..." << endl << endl;
    cout << "Binary decoded Output: " << outputBin << endl << endl;
    cout << "ASCII decoded binary output: " << outputASCII << endl << endl;
    cout << "Base64 Encoded Output: " << outputBase64 << endl;
    cout << "Expected b64 Output:   " << ExpectedBase64 << endl;
    cout << " _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _" << endl << endl << endl;
}

//Task 2: Takes two equal-length buffers and produces their XOR combination (with wrappers for multiple encoding bases)
void Task2Eval()
{
    string inputHexA = "1c0111001f010100061a024b53535009181c";
    string inputHexB = "686974207468652062756c6c277320657965";
    string outputHex = FixedXOR_hex(inputHexA, inputHexB);
    string expectedHex = "746865206b696420646f6e277420706c6179";

    cout << endl << endl << endl << endl;
    cout << "Task 2 Evaluation: _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _" << endl;
    cout << "Input Hexadecimal A: " << inputHexA << endl << "..." << endl;
    cout << "Input Hexadecimal B: " << inputHexB << endl << "..." << endl << endl;
    cout << "Fixed XOR encrypted Output: " << outputHex << endl;
    cout << "Expected output:            " << expectedHex << endl;
    cout << " _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _" << endl << endl << endl;
}

//Task 3: Find the single-byte key that a given ciphertext has been XOR'd against
void Task3Eval()
{
    //INSERT TASK 3 EVAL CODE HERE
}

//Task 4: Detect the presence of a single-byte XOR encrypted string amongst surplus data
void Task4Eval()
{
    //INSERT TASK 4 EVAL CODE HERE
}

//Task 5: Implement an accurate repeating-key XOR encryption / decryption oracle
void Task5Eval()
{
    //INSERT TASK 5 EVAL CODE HERE
}

//Task 6: Brute-force attack a repeating-key XOR encrypted ciphertext to find the key and plaintext
void Task6Eval()
{
    //INSERT TASK 6 EVAL CODE HERE
}

//Task 7: Implement an accurate AES-128 decryption oracle
void Task7Eval()
{
    //INSERT TASK 7 EVAL CODE HERE
}

//Task 8: Use statistical analysis to detect the presence of 
//        Electronic CodeBook (ECB) encryption amongst surplus data
void Task8Eval()
{
    //INSERT TASK 8 EVAL CODE HERE
}

//Run menu selection program for preforming and re-performing base task evaluation tests
int main()
{
    cout << "\n************************************************************\n";
    cout << "\tCryptoPals Software Suite - G4";
    cout << "\n************************************************************\n";

    char selector = -1;

    

    while (selector != 9)
    {
        cout << "Please select a demo program to run:" << endl;
        cout << "---------------------------------------------------------\n";
        cout << "\tTask (1)...................Convert hex input to base64.\n";
        cout << "\tTask (2)......................Fixed XOR two hex inputs.\n";
        cout << "\tTask (3).Brute-force attack single-byte XOR ciphertext.\n";
        cout << "\tTask (4).....Detect single-byte XOR ciphertext in file.\n";
        cout << "\tTask (5)..Repeating-key XOR encrypt plaintext with key.\n";
        cout << "\tTask (6)......Brute-force repeating-key XOR ciphertext.\n";
        cout << "\tTask (7)..Decrypt AES-128 ECB mode ciphertext with key.\n";
        cout << "\tTask (8).......Detect ECB encrypted ciphertext in file.\n";
        cout << "\t     (9)..................................Exit program.\n";
        cout << "---------------------------------------------------------\n";
        cout << " Selection: ";
        cin >> selector;


        switch (selector)
        {
        default:
            selector = -1;
            cout << endl << "-- Please enter a valid menu option --" << endl << endl;
            break;
        case '1':
            selector = 0;
            Task1Eval();
            break;
        case '2':
            selector = 0;
            Task2Eval();
            break;
        case '3':
            selector = 0;
            Task3Eval();
            break;
        case '4':
            selector = 0;
            Task4Eval();
            break;
        case '5':
            selector = 0;
            Task5Eval();
            break;
        case '6':
            selector = 0;
            Task6Eval();
            break;
        case '7':
            selector = 0;
            Task7Eval();
            break;
        case '8':
            selector = 0;
            Task8Eval();
            break;
        case '9':
            selector = 9;
            break;
        }

        if (selector == 0)
        {
            cout << endl << endl << "Would you like to exit the program? (Y / N) | \t";
            cin >> selector;

            if (toupper(selector) == 'Y')
                selector = 9;
            else if (toupper(selector) == 'N')
                selector = 0;
        }
    }

    return 0;
}
