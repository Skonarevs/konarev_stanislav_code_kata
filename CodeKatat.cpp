#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <stdexcept>

using namespace std;

class StringCalculator {
public:
    int Calculate(const string& input) {
        if (input.empty()) return 0;

        string numbers = input;
        vector<string> delimiters = { ",", "\n" };
        if (input.rfind("//", 0) == 0) {
            size_t newlinePos = input.find("\n");
            string delimiterPart = input.substr(2, newlinePos - 2);
            numbers = input.substr(newlinePos + 1);

            delimiters.clear();
            regex multiDelimiterRegex("\\[(.*?)\\]");
            sregex_iterator iter(delimiterPart.begin(), delimiterPart.end(), multiDelimiterRegex);
            sregex_iterator end;
            if (iter != end) {
                for (; iter != end; ++iter) {
                    delimiters.push_back((*iter)[1].str());
                }
            }
            else {
                delimiters.push_back(delimiterPart);
            }
        }

        vector<int> nums = splitAndConvert(numbers, delimiters);
        checkNegatives(nums);

        int sum = 0;
        for (int n : nums) {
            if (n <= 1000) sum += n;
        }
        return sum;
    }

private:
    vector<int> splitAndConvert(const string& numbers, const vector<string>& delimiters) {
        vector<int> result;
        string s = numbers;

        for (const auto& delim : delimiters) {
            size_t pos = 0;
            while ((pos = s.find(delim, pos)) != string::npos) {
                s.replace(pos, delim.length(), ",");
                pos += 1;
            }
        }

        stringstream ss(s);
        string token;
        while (getline(ss, token, ',')) {
            if (!token.empty()) {
                result.push_back(stoi(token));
            }
        }
        return result;
    }

    void checkNegatives(const vector<int>& nums) {
        vector<int> negatives;
        for (int n : nums) {
            if (n < 0) negatives.push_back(n);
        }
        if (!negatives.empty()) {
            string msg = "Negatives not allowed: ";
            for (size_t i = 0; i < negatives.size(); ++i) {
                msg += to_string(negatives[i]);
                if (i < negatives.size() - 1) msg += ", ";
            }
            throw runtime_error(msg);
        }
    }
};

int main() {
    StringCalculator calc;

    cout << calc.Calculate("") << "\n";                // 0
    cout << calc.Calculate("5") << "\n";               // 5
    cout << calc.Calculate("1,2") << "\n";             // 3
    cout << calc.Calculate("1\n2,3") << "\n";          // 6
    cout << calc.Calculate("//[***]\n1***2***3") << "\n"; // 6
    cout << calc.Calculate("//[*][%]\n1*2%3") << "\n";    // 6

    try {
        cout << calc.Calculate("-1,2") << "\n";        // Throws
    }
    catch (exception& e) {
        cout << e.what() << "\n";
    }

    cout << calc.Calculate("2,1001") << "\n";          // 2
}