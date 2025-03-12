#include <iostream>
#include <stack>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

bool isOpeningBracket(char ch) {
    return ch == '(' || ch == '[' || ch == '{';
}

bool isClosingBracket(char ch) {
    return ch == ')' || ch == ']' || ch == '}';
}

bool isMatchingPair(char open, char close) {
    return (open == '(' && close == ')') ||
        (open == '[' && close == ']') ||
        (open == '{' && close == '}');
}

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
