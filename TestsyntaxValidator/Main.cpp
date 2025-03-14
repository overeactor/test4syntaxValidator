/**
 * @file bracket_checker.cpp
 * @brief Program to check for unmatched brackets in a source file.
 * @author Alireza Behbahani
 * @date 2025-03-14
 */

#include <iostream>
#include <stack>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

/**
 * @brief Checks if a character is an opening bracket.
 * @param ch Character to check.
 * @return True if the character is '(', '[', or '{'.
 */
bool isOpeningBracket(char ch) {
    return ch == '(' || ch == '[' || ch == '{';
}

/**
 * @brief Checks if a character is a closing bracket.
 * @param ch Character to check.
 * @return True if the character is ')', ']', or '}'.
 */
bool isClosingBracket(char ch) {
    return ch == ')' || ch == ']' || ch == '}';
}

/**
 * @brief Checks if an opening bracket matches a closing bracket.
 * @param open Opening bracket character.
 * @param close Closing bracket character.
 * @return True if they form a matching pair.
 */
bool isMatchingPair(char open, char close) {
    return (open == '(' && close == ')') ||
        (open == '[' && close == ']') ||
        (open == '{' && close == '}');
}

/**
 * @brief Checks the given lines of code for unmatched brackets.
 * @param codeLines Vector of strings representing lines of code.
 */
void checkBrackets(const vector<string>& codeLines) {
    stack<pair<char, pair<int, int>>> bracketStack;
    vector<pair<int, int>> errorPositions;
    bool inString = false;
    bool inChar = false;
    bool inSingleComment = false;
    bool inMultiComment = false;

    for (size_t lineNum = 0; lineNum < codeLines.size(); lineNum++) {
        const string& line = codeLines[lineNum];
        for (size_t i = 0; i < line.size(); i++) {
            char ch = line[i];

            // Handle string and character literals
            if (ch == '"' && !inChar && !inSingleComment && !inMultiComment) {
                inString = !inString;
            }
            else if (ch == '\'' && !inString && !inSingleComment && !inMultiComment) {
                inChar = !inChar;
            }

            // Handle single-line comments
            if (ch == '/' && i + 1 < line.size() && line[i + 1] == '/' && !inString && !inChar && !inMultiComment) {
                inSingleComment = true;
            }
            if (ch == '\n' && inSingleComment) {
                inSingleComment = false;
            }

            // Handle multi-line comments
            if (ch == '/' && i + 1 < line.size() && line[i + 1] == '*' && !inString && !inChar && !inSingleComment) {
                inMultiComment = true;
            }
            if (ch == '*' && i + 1 < line.size() && line[i + 1] == '/' && inMultiComment) {
                inMultiComment = false;
                i++; // Skip closing '/'
                continue;
            }

            if (inString || inChar || inSingleComment || inMultiComment) {
                continue;
            }

            // Process brackets
            if (isOpeningBracket(ch)) {
                bracketStack.push({ ch, {lineNum + 1, i + 1} });
            }
            else if (isClosingBracket(ch)) {
                if (!bracketStack.empty() && isMatchingPair(bracketStack.top().first, ch)) {
                    bracketStack.pop();
                }
                else {
                    errorPositions.push_back({ lineNum + 1, i + 1 });
                }
            }
        }
    }

    // Add remaining unmatched opening brackets
    while (!bracketStack.empty()) {
        errorPositions.push_back(bracketStack.top().second);
        bracketStack.pop();
    }

    // Output results
    if (errorPositions.empty()) {
        cout << "All brackets are correctly closed." << endl;
    }
    else {
        cout << "Unmatched brackets at positions: " << endl;
        for (const auto& pos : errorPositions) {
            cout << "Line " << pos.first << ", Position " << pos.second << endl;
        }
    }
}

/**
 * @brief Main function to read a file and check for bracket errors.
 * @return 0 on successful execution.
 */
int main() {
    ifstream file("test.txt");
    vector<string> codeLines;
    string line;
    while (getline(file, line)) {
        codeLines.push_back(line);
    }

    checkBrackets(codeLines);
    return 0;
}
