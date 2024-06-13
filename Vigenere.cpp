#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

// Function to calculate the frequency of letters in the text
/*
calculateFrequency Function:
Purpose: To calculate the frequency of each letter in the given text.
Parameters: text - A string containing the text whose letter frequencies need to be calculated.
Returns: A map where the keys are characters and the values are their respective frequencies in the text.
 */
std::map<char, int> calculateFrequency(const std::string &text) {
    std::map<char, int> frequency;
    for (char c : text) {
        if (isalpha(c)) {
            frequency[toupper(c)]++;
        }
    }
    return frequency;
}

// Function to perform Vigenere decryption with the guessed key
/*
vigenereDecrypt Function:

Purpose: To decrypt a given ciphertext using the Vigenère cipher with the provided key.
Parameters:
ciphertext - The text to be decrypted.
key - The key used for decryption.
Returns: The decrypted plaintext
 */
std::string vigenereDecrypt(const std::string &ciphertext, const std::string &key) {
    std::string plaintext = "";
    int keyLength = key.length();
    for (int i = 0; i < ciphertext.length(); ++i) {
        if (isalpha(ciphertext[i])) {
            char decryptedChar = ((toupper(ciphertext[i]) - toupper(key[i % keyLength]) + 26) % 26) + 'A';
            plaintext += decryptedChar;
        } else {
            plaintext += ciphertext[i];
        }
    }
    return plaintext;
}

// Function to guess the Vigenere key based on frequency analysis
/*
guessVigenereKey Function:

Purpose: To guess the key of the Vigenère cipher based on frequency analysis.
Parameters:
ciphertext - The text to be analyzed for key guessing.
keyLength - The length of the key to be guessed.
Returns: The guessed key as a string.
 */
std::string guessVigenereKey(const std::string &ciphertext, int keyLength) {
    std::string key = "";
    std::vector<std::string> substrings(keyLength);
    for (int i = 0; i < ciphertext.length(); ++i) {
        substrings[i % keyLength] += ciphertext[i];
    }

    std::vector<double> englishLetterFrequencies = {
        8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 0.772, 4.025,
        2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074
    };

    char mostFrequentLetter = 'E'; // Assuming English plaintext
    for (const std::string &substring : substrings) {
        auto frequency = calculateFrequency(substring);
        char mostFrequentCipherChar = std::max_element(frequency.begin(), frequency.end(),
                                                       [](const auto &a, const auto &b) { return a.second < b.second; })->first;
        char keyChar = ((mostFrequentCipherChar - mostFrequentLetter + 26) % 26) + 'A';
        key += keyChar;
    }
    return key;
}

// Example usage
int main() {
    std::ifstream file("cipher3.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string ciphertext((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    int keyLength = 5; // Block size
    std::string guessedKey = guessVigenereKey(ciphertext, keyLength);
    std::string plaintext = vigenereDecrypt(ciphertext, guessedKey);

    std::cout << "Guessed Vigenere Key: " << guessedKey << std::endl;
    std::cout << "Decrypted Vigenere Plaintext: " << plaintext << std::endl;

    return 0;
}
