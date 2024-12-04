#include "pch.h"
#include "../CSC414_Group4_CryptoSuite/AES128.h"
#include "../CSC414_Group4_CryptoSuite/BaseConversions.h"
#include "../CSC414_Group4_CryptoSuite/ECBDetection.h"
#include "../CSC414_Group4_CryptoSuite/FixedXOR.h"
#include "../CSC414_Group4_CryptoSuite/RepeatingKeyXOR.h"
#include "../CSC414_Group4_CryptoSuite/SingleByteXOR.h"
#include "../CSC414_Group4_CryptoSuite/TestingTemplates.h"
#include <fstream>

//TEST SUITE PROTOTYPE




//Test if task completes with expected input
//Requirement 1: Independent Paths
TEST(Task1, ExpectedInputTest)
{
    string inputHexA = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    string outputBin;
    string outputASCII;
    string outputBase64;
    string ExpectedBinary = "010010010010011101101101001000000110101101101001011011000110110001101001011011100110011100100000011110010110111101110101011100100010000001100010011100100110000101101001011011100010000001101100011010010110101101100101001000000110000100100000011100000110111101101001011100110110111101101110011011110111010101110011001000000110110101110101011100110110100001110010011011110110111101101101";
    string ExpectedASCII = "I'm killing your brain like a poisonous mushroom";
    string ExpectedBase64 = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

    outputBin = hex_str_to_bin_str(inputHexA);
    outputASCII = bin_str_to_ASCII_str(outputBin);
    outputBase64 = hex_str_to_b64_str(inputHexA);

    EXPECT_EQ(ExpectedBinary, outputBin);
    EXPECT_EQ(ExpectedASCII, outputASCII);
    EXPECT_EQ(ExpectedBase64, outputBase64);
}

//Test if the task function can handle unexpected input
//Test Requirement 3: Operational Bounds, test is program can handle bad input without crashing
TEST(Task1, BadInputTest)
{
    string inputHexA = "4927////6b696c6c696e----796f757220627261696e206c696b652||||0706f$$$$$736f6e6f7573206d757368726f6f6d";
    string outputBin = "Did Not Return";
    string outputASCII = "Did Not Return";
    string outputBase64 = "Did Not Return";

    outputBin = hex_str_to_bin_str(inputHexA);
    outputASCII = bin_str_to_ASCII_str(outputBin);
    outputBase64 = hex_str_to_b64_str(inputHexA);


    EXPECT_NE("Did Not Return", outputBin);
    EXPECT_NE("Did Not Return", outputASCII);
    EXPECT_NE("Did Not Return", outputBase64);
}

//Test if task completes with expected input
//Requirement 1: Independent Paths
TEST(Task2, ExpectedInputTest)
{
    string inputHexA = "1c0111001f010100061a024b53535009181c";
    string inputHexB = "686974207468652062756c6c277320657965";
    string ExpectedOut = "746865206b696420646f6e277420706c6179";
    string outputHex = FixedXOR_hex(inputHexA, inputHexB);

    EXPECT_EQ(ExpectedOut, outputHex);
}

//Test if task can handle unequal input and return approriate exception
//Requirement 2: Logical Decisions
TEST(Task2, UnequalInputTest)
{
    string inputHexA = "1c0111001f010100061a024b53535009181c58584857385828572fhdhfyh28743";
    string inputHexB = "686974207468652062756c6c277320657965";

    EXPECT_THROW(string outputHex = FixedXOR_hex(inputHexA, inputHexB); , invalid_argument);
}

//Test if task can handle bad input and still produce encoded output
//Requirement 3: Operational Bounds
TEST(Task2, BadInputTest)
{
    string inputHexA = "1c0111001f0101$$$61a024b535&&&&9181c";
    string inputHexB = "68697420-468652062756c###773206@@@65";

    string ExpectedOut = "746865201b696420646f6e4b542320691879";
    string outputHex = FixedXOR_hex(inputHexA, inputHexB);

    EXPECT_EQ(ExpectedOut, outputHex);
}

//Test if task completes with expected input
//Requirement 1: Independent Paths
TEST(Task3, ExpectedInputTest)
{
    string inputHexA = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    string OutputMessage;
    string ExpectedMessage = "Cooking MC's like a pound of bacon";
    OutputMessage = DecryptSingleByteXOR(inputHexA, 1);
    EXPECT_EQ(ExpectedMessage, OutputMessage);
}

//Test if task completes with bad input mixed into string
//Requirement 3: Operational Bounds
TEST(Task3, BadInputTest)
{
    string inputHexA = "1b37373331363f78151b7f2#$##431333d---9782@372d++3c78373e783a393b3736";
    string OutputMessage;
    string ExpectedMessage = R"(Cooking MC'xX\ikeXQ xouXd of bacon)";
    OutputMessage = DecryptSingleByteXOR(inputHexA, 1);
    EXPECT_EQ(ExpectedMessage, OutputMessage);
}

//Test if function can handle empty string input
//Requirement 3: Operational Bounds
TEST(Task3, NoInputTest)
{
    string inputHexA = "";
    string OutputMessage;
    string ExpectedMessage = "";
    OutputMessage = DecryptSingleByteXOR(inputHexA, 1);
    EXPECT_EQ(ExpectedMessage, OutputMessage);
}

//Test if task completes with expected input
//Requirement 2: Logical Decisions
TEST(Task4, ExpectedInputTest)
{

    //string inputFile = "SBXORFile.txt";
    string ExpectedMessage = "Now that the party is jumping";
    string OutputMessage;
    OutputMessage = SBXOR_DetectionAlgorithm(SBXOR_INPUT_TEMPLATE);

    EXPECT_EQ(ExpectedMessage, OutputMessage);
}

//Test if function completes even with nonsensical input
//Requirement 3: Operational Bounds
TEST(Task4, BadInputTest)
{
    vector<string> NonsenseList = { "$$$$$-----!!!!@@@@@@", "AAAAABBBBBCCCCDDDDDD", "12345123451234123456" };

    string ExpectedMessage = "         ";
    string OutputMessage;
    OutputMessage = SBXOR_DetectionAlgorithm(NonsenseList);

    EXPECT_EQ(ExpectedMessage, OutputMessage);
}

//Test if task completes with expected input
//Requirement 1: Independent Paths
//Requirement 2: Logical Decisions
TEST(Task5, ExpectedInputtest)
{
    string inputTextA = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
    string key = "ICE";
    string ExpectOutput = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";

    string outputASCII = repeatingKeyXORCipher(inputTextA, key);
    string OutputHex = ASCII_str_to_hex_str(outputASCII);

    EXPECT_EQ(ExpectOutput, OutputHex);
}

//Test if function completes with a key made of nonsense characters
//Requirement 3: Operational Bounds
TEST(Task5, NonsensicalKeyTest)
{
    string inputTextA = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
    string key = "###$$$***!!!---||{}{{}))%%%<><><>????~~";
    string ExpectOutput = "6156514a4d4a4d0a0d444c0d0d444b5c0514085b1a14470e51055449575f551c5f515b1f511713414f462e6d044d450a42534057540d0b141e135b325d414c4457055d1e5f47515c5e53";

    string outputASCII = repeatingKeyXORCipher(inputTextA, key);
    string OutputHex = ASCII_str_to_hex_str(outputASCII);

    EXPECT_EQ(ExpectOutput, OutputHex);
}

//Test if function completes with input mode of nonsense characters and a normal key
//Requirement 3: Operational Bounds
TEST(Task5, NonsensicalInputTest)
{
    string inputTextA = "%%%%$$$$@@@@^^^^<<>><<>>>?????|||||----******@@@@@^^^^^)))((((";
    string key = "ICE";
    string ExpectOutput = "6c66606c67616d6705090305171d1b177f79777d79757d7b777c7a767c7a353f39353f68646e6863696f63696f09030509031b171d1b176a6c606b6d616b";

    string outputASCII = repeatingKeyXORCipher(inputTextA, key);
    string OutputHex = ASCII_str_to_hex_str(outputASCII);

    EXPECT_EQ(ExpectOutput, OutputHex);
}

//Test if function completes with both a nonsense input and nonsense key
//Requirement 3: Operational Bounds
TEST(Task5, NonsensicalKeyAndInputTest)
{
    string inputTextA = "%%%%$$$$@@@@^^^^<<>><<>>>?????|||||----******@@@@@^^^^^)))((((";
    string key = "!@@#$*@$&%&#(@{}|||";
    string ExpectOutput = "04656506000e640066656663761e25234040421f7c7c1d1a147f1b191a195f543c07015151510c6a6a090e006a6466656663761e25232255550868680b0c";

    string outputASCII = repeatingKeyXORCipher(inputTextA, key);
    string OutputHex = ASCII_str_to_hex_str(outputASCII);

    EXPECT_EQ(ExpectOutput, OutputHex);
}

//Test if task completes with expected input
//Requirement 1: Independent Paths
TEST(Task6, ExpectedInputTest)
{
    string inputTextA = b64_str_to_ASCII_str(RKXOR_INPUT_TEMPLATE);
    int minKeySize = 2;
    int maxKeySize = 40;
    int numKeys = 5;
    vector<int> ExpectKeySizes = { 29, 5, 2, 24, 7 };
    vector<int> likelyKeySizes = findLikelyKeySizes(inputTextA, minKeySize, maxKeySize, numKeys);

    EXPECT_EQ(ExpectKeySizes, likelyKeySizes);
}

//Test if function can error handle a max key size too large to handle
//Requirement 3: Operational Bounds
//Requirement 2: Logical Decisions
TEST(Task6, ExtremeKeySizeTest)
{
    string inputTextA = b64_str_to_ASCII_str(RKXOR_INPUT_TEMPLATE);
    int minKeySize = 2;
    int maxKeySize = 99999;
    int numKeys = 100;

    EXPECT_THROW(vector<int> likelyKeySizes = findLikelyKeySizes(inputTextA, minKeySize, maxKeySize, numKeys); , invalid_argument);
}

//Test if function can handle an extreme number of keys
//Requirement 3 Operational Bounds
TEST(Task6, ExtremeNumKeyTest)
{
    string inputTextA = b64_str_to_ASCII_str(RKXOR_INPUT_TEMPLATE);
    int minKeySize = 2;
    int maxKeySize = 40;
    int numKeys = 99999999999999;
    vector<int> ExpectKeySizes = { 29, 5, 2, 24, 7 };
    vector<int> likelyKeySizes = findLikelyKeySizes(inputTextA, minKeySize, maxKeySize, numKeys);

    EXPECT_NE(ExpectKeySizes, likelyKeySizes);
}

//Test if function can handle an min key size to large to handle
//Requirement 3: Operational Bounds
//Requirement 2: Logical Decisions
TEST(Task6, ExtremeMinInputTest)
{
    string inputTextA = b64_str_to_ASCII_str(RKXOR_INPUT_TEMPLATE);
    int minKeySize = 999999999999999999;
    int maxKeySize = 40;
    int numKeys = 5;
    vector<int> ExpectKeySizes = { 29, 5, 2, 24, 7 };

    EXPECT_ANY_THROW(vector<int> likelyKeySizes = findLikelyKeySizes(inputTextA, minKeySize, maxKeySize, numKeys););
}

//Test if functions completes with expected input
TEST(Task7, ExpectedInputTest)
{
    string inputTextA = AES_INPUT_TEMPLATE;
    string key = "YELLOW SUBMARINE";
    string ExpectedOutputHex = AES_OUTPUT_TEMPLATE;

    vector<uint8_t> decryptedBytes = decryptB64String(inputTextA, key);
    string decryptedHex = bytesToHexString(decryptedBytes);

    EXPECT_EQ(decryptedHex, ExpectedOutputHex);
}

//Test if functions successfully reject input not divisible by 16
TEST(Task7, ShortInputTest)
{
    string inputTextA = "81h+aW";
    string key = "987awd98y++1Hv7#";
    EXPECT_THROW(vector<uint8_t> decryptedBytes = decryptHexString(inputTextA, key); , invalid_argument);
}

//Test if functions complete with key shorter than preferred for aes 128 (longer would throw the same)
TEST(Task7, ShortKeyTest)
{
    string inputTextA = "63F397FE940AC14A1B830D014248FEC826BB689137E581703F34175F1CD794A9"; //TESTING 1 2 3, testing 4 5 6!
    string key = "987awd9";

    EXPECT_THROW(vector<uint8_t> decryptedBytes = decryptHexString(inputTextA, key);, invalid_argument);
}

//Test if functions are accurate for known input (hex) encrypted with a meaningless key
TEST(Task7, arbitraryKeyTest)
{
    string inputTextA = "63F397FE940AC14A1B830D014248FEC826BB689137E581703F34175F1CD794A9"; //TESTING 1 2 3, testing 4 5 6! (encrypted, then hex)
    string key = "987awd98y++1Hv7#";
    string ExpectedOutputHex = "54455354494e472031203220332c2074657374696e6720342035203621030303";

    vector<uint8_t> decryptedBytes = decryptHexString(inputTextA, key);
    string decryptedHex = bytesToHexString(decryptedBytes);

    EXPECT_EQ(decryptedHex, ExpectedOutputHex);
}

//Test if functions completes with expected input
//Requirement 1: All Independent Paths
//Requirement 2: All Logical Derisons
TEST(Task8, ExpectedInputTest)
{
    string inputTextA = ECBDET_INPUT_TEMPLATE;
    bool is_ecb = detect_ecb(inputTextA);

    EXPECT_EQ(true, is_ecb);
}

//Test if function completes with nonsensical input
//Requirement 1: Independent Paths
//Requirement 2: Logical Decisons
TEST(Task8, BadInputTest)
{

    bool is_ecb = detect_ecb("%&#&*%(#*%&@(%&*@(*%&@&**%/*---/*-/-*/|||}{}{}{}{}{}@&%&@??><><><>");

    EXPECT_EQ(false, is_ecb);
}

//Test each case of the hex to binary switch
//Requirement 1: Independent Paths
TEST(SubTask, CharacterSwitchTest)
{
    string Case0 = hex_char_to_bin('0');
    string Case1 = hex_char_to_bin('1');
    string Case2 = hex_char_to_bin('2');
    string Case3 = hex_char_to_bin('3');
    string Case4 = hex_char_to_bin('4');
    string Case5 = hex_char_to_bin('5');
    string Case6 = hex_char_to_bin('6');
    string Case7 = hex_char_to_bin('7');
    string Case8 = hex_char_to_bin('8');
    string Case9 = hex_char_to_bin('9');
    string CaseA = hex_char_to_bin('A');
    string CaseB = hex_char_to_bin('B');
    string CaseC = hex_char_to_bin('C');
    string CaseD = hex_char_to_bin('D');
    string CaseE = hex_char_to_bin('E');
    string CaseF = hex_char_to_bin('F');

    EXPECT_EQ("0000", Case0);
    EXPECT_EQ("0001", Case1);
    EXPECT_EQ("0010", Case2);
    EXPECT_EQ("0011", Case3);
    EXPECT_EQ("0100", Case4);
    EXPECT_EQ("0101", Case5);
    EXPECT_EQ("0110", Case6);
    EXPECT_EQ("0111", Case7);
    EXPECT_EQ("1000", Case8);
    EXPECT_EQ("1001", Case9);
    EXPECT_EQ("1010", CaseA);
    EXPECT_EQ("1011", CaseB);
    EXPECT_EQ("1100", CaseC);
    EXPECT_EQ("1101", CaseD);
    EXPECT_EQ("1110", CaseE);
    EXPECT_EQ("1111", CaseF);

}

//Test all logical paths of the message scoring function
//Requirement 1: Independents Paths
TEST(SubTask, MessageScoreTest)
{
    int caseA = getFrequencyIndex('a');
    int caseSpace = getFrequencyIndex(' ');
    int caseOther = getFrequencyIndex('#');
    double ExpectedScore = 759.5;

    double caseScore = score_message("This is a test that contains letters, spaces, some numbers (24921), and various other symbols (#$@?)!");

    EXPECT_EQ(caseA, 0);
    EXPECT_EQ(caseSpace, 26);
    EXPECT_EQ(caseOther, -1);
    EXPECT_DOUBLE_EQ(caseScore, ExpectedScore);
}

//Test all paths of the EditDistance Function
//Requirement 1: Independent Paths
TEST(SubTask, EditDistanceTest)
{
    int caseASCII = calculateEditDistance("This is ASCII text", "This is also ASCII text", 0);
    int caseBinary = calculateEditDistance("010101000110100001101001011100110010000001101001011100110010000001000001010100110100001101001001010010010010000001110100011001010111001101110100", "0101010001101000011010010111001100100000011010010111001100100000011000010110110001110011011011110010000001000001010100110100001101001001010010010010000001110100011001010111001101110100", 2);
    int caseHex = calculateEditDistance("546869732069732041534349492074657874", "5468697320697320616C736F2041534349492074657874", 16);
    int case64 = calculateEditDistance("VGhpcyBpcyBBU0NJSSB0ZXh0", "VGhpcyBpcyBhbHNvIEFTQ0lJIHRleHQ=", 64);

    EXPECT_EQ(caseASCII, 34);
    EXPECT_EQ(caseBinary, 35);
    EXPECT_EQ(caseHex, 34);
    EXPECT_EQ(case64, 34);
}