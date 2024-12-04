#pragma once

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

string read_ciphertext_from_file(const string& filename) {
    ifstream file(filename);
    stringstream buffer;

    if (file) {
        buffer << file.rdbuf();  // Read file content into buffer
        return buffer.str();     // Convert buffer to string
    }
    else {
        throw runtime_error("Could not open file: " + filename);
    }
}

vector<string> get_ciphertext_lines_from_file(const string& filename)
{
    ifstream InputFile(filename);
    vector<string> bufferList;
    string line;

    if (InputFile) {
        while (getline(InputFile, line))
        {
            bufferList.push_back(line);
        }

        return bufferList;
    }
    else {
        throw runtime_error("Could not open file: " + filename);
    }
}