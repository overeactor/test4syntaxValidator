#include <iostream>
#include <stack>
#include <vector>
#include <fstream>

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

void checkBrackets(const string& code) {
    stack<pair<char, int>> bracketStack;
    vector<int> errorPositions;
    bool inString = false;
    bool inChar = false;
    bool inSingleComment = false;
    bool inMultiComment = false;

    for (size_t i = 0; i < code.size(); i++) {
        char ch = code[i];

        // Handle string and character literals
        if (ch == '"' && !inChar && !inSingleComment && !inMultiComment) {
            inString = !inString;
        }
        else if (ch == '\'' && !inString && !inSingleComment && !inMultiComment) {
            inChar = !inChar;
        }

        // Handle single-line comments
        if (ch == '/' && i + 1 < code.size() && code[i + 1] == '/' && !inString && !inChar && !inMultiComment) {
            inSingleComment = true;
        }
        if (ch == '\n' && inSingleComment) {
            inSingleComment = false;
        }

        // Handle multi-line comments
        if (ch == '/' && i + 1 < code.size() && code[i + 1] == '*' && !inString && !inChar && !inSingleComment) {
            inMultiComment = true;
        }
        if (ch == '*' && i + 1 < code.size() && code[i + 1] == '/' && inMultiComment) {
            inMultiComment = false;
            i++; // Skip closing '/'
            continue;
        }

        if (inString || inChar || inSingleComment || inMultiComment) {
            continue;
        }

        // Process brackets
        if (isOpeningBracket(ch)) {
            bracketStack.push({ ch, i });
        }
        else if (isClosingBracket(ch)) {
            if (!bracketStack.empty() && isMatchingPair(bracketStack.top().first, ch)) {
                bracketStack.pop();
            }
            else {
                errorPositions.push_back(i);
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
        cout << "Unmatched brackets at positions: ";
        for (int pos : errorPositions) {
            cout << pos << " ";
        }
        cout << endl;
    }
}

int main() {
    ifstream file("test.cpp");
    string code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    checkBrackets(code);
    return 0;
}
