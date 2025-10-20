#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

std::string convertDomainsToString(const std::vector<std::string>& domains) {
    std::stringstream ss;
    for (const auto &d : domains) {
        ss << d << "\n";
    }
    return ss.str();
}

std::string convertRequiredLettersToString(const std::map<char, int>& reqLetters) {
    std::stringstream ss;
    ss << "{ ";
    for (const auto &pair : reqLetters) {
        ss << "<" << pair.first << ": " << pair.second << "> ";
    }
    ss << "}\n";
    return ss.str();
}

// Returns a new string that only shows occurrences of 'ch' in the input domain;
// other positions are replaced with '#'.
std::string filterDomainForChar(const std::string &domain, char ch) {
    std::string filtered;
    for (size_t i = 0; i < domain.size(); ++i) {
        filtered += (domain[i] == ch) ? ch : '#';
    }
    return filtered;
}

// Replaces all occurrences of the given character in the domain string with '#'.
// This effectively removes that character from the set of allowed letters.
std::string removeCharFromDomain(const std::string &domain, char ch) {
    std::string modified = domain;
    for (size_t i = 0; i < modified.size(); ++i) {
        if (modified[i] == ch) {
            modified[i] = '#';
        }
    }
    return modified;
}

class WordleHelper {
public:
    // The constructor initializes the allowed letters (domain) for each position.
    WordleHelper(int length) : wordLen(length) {
        for (int i = 0; i < wordLen; ++i) {
            allowedLetters.push_back("abcdefghijklmnopqrstuvwxyz");
        }
    }
    
    // Returns the current domains for each letter position.
    std::vector<std::string> getAllowedLetters() const {
        return allowedLetters;
    }

    // Returns the required letters map (letter -> minimum count).
    std::map<char, int> getRequiredLetters() const {
        std::map<char, int> req;
        for (const auto &entry : required) {
            if (entry.second > 0) {
                req[entry.first] = entry.second;
            }
        }
        return req;
    }

    // The feedback uses '=' (correct position), '+' (in word, wrong position), and '#' (not in word).
    void processAttempt(const std::pair<std::string, std::string>& attempt) {
        const std::string& guess = attempt.first;
        const std::string& feedback = attempt.second;
        std::map<char, int> freqCounter;

        // First pass: update domains for positions with correct or misplaced letters.
        for (int i = 0; i < wordLen; ++i) {
            char letter = guess[i];
            char mark = feedback[i];
            if (mark == '=') {
                // Letter is in the correct spot.
                allowedLetters[i] = filterDomainForChar(allowedLetters[i], letter);
                freqCounter[letter]++;
            } else if (mark == '+') {
                // Letter exists in the word but not in this spot.
                allowedLetters[i] = removeCharFromDomain(allowedLetters[i], letter);
                freqCounter[letter]++;
            }
        }

        // Second pass: if a letter is marked '#' and not recorded as present from earlier,
        // remove it from all allowed domains.
        for (int i = 0; i < wordLen; ++i) {
            char letter = guess[i];
            char mark = feedback[i];
            if (mark == '#' && freqCounter[letter] == 0) {
                for (int j = 0; j < wordLen; ++j) {
                    allowedLetters[j] = removeCharFromDomain(allowedLetters[j], letter);
                }
            }
        }

        // Update the required letters map to reflect the highest count observed so far.
        for (const auto &entry : freqCounter) {
            char letter = entry.first;
            int count = entry.second;
            if (required.find(letter) == required.end()) {
                required[letter] = count;
            } else if (count > required[letter]) {
                required[letter] = count;
            }
        }
    }

    // Checks if a candidate word satisfies all constraints derived from previous attempts.
    bool isCandidateValid(const std::string& candidate) const {
        // Check allowed letters at each position.
        for (int i = 0; i < wordLen; ++i) {
            char c = candidate[i];
            bool found = false;
            for (int j = 0; j < 26; ++j) {
                if (allowedLetters[i][j] == c) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
        // Count occurrences in the candidate.
        std::map<char, int> candidateFreq;
        for (char ch : candidate) {
            candidateFreq[ch]++;
        }
        // Check that candidate meets the required letters frequency.
        for (const auto &entry : required) {
            char letter = entry.first;
            int needed = entry.second;
            if (candidateFreq[letter] < needed) {
                return false;
            }
        }
        return true;
    }

private:
    int wordLen;
    std::vector<std::string> allowedLetters;
    std::map<char, int> required;
};

int main() {
    // TEST CASE A: Basic test from the specification.
    std::cout << "TEST CASE A:\n";
    WordleHelper helper1(5);
    std::cout << convertDomainsToString(helper1.getAllowedLetters());
    std::cout << convertRequiredLettersToString(helper1.getRequiredLetters());
    std::pair<std::string, std::string> att1 = {"azyfb", "#=++#"};
    helper1.processAttempt(att1);
    std::cout << convertDomainsToString(helper1.getAllowedLetters());
    std::cout << convertRequiredLettersToString(helper1.getRequiredLetters());
    std::cout << helper1.isCandidateValid("fzfyx") << "\n"; 
    std::cout << helper1.isCandidateValid("azyfb") << "\n";
    std::cout << helper1.isCandidateValid("czfcy") << "\n"; 
    std::cout << helper1.isCandidateValid("fzfcc") << "\n";
    // Additional update from spec.
    helper1.processAttempt({ "cccff", "###++" });
    std::cout << convertDomainsToString(helper1.getAllowedLetters());
    std::cout << convertRequiredLettersToString(helper1.getRequiredLetters());
    helper1.processAttempt({ "fzfyx", "=====" });
    std::cout << convertDomainsToString(helper1.getAllowedLetters());
    std::cout << convertRequiredLettersToString(helper1.getRequiredLetters());

    // TEST CASE B: Using three-letter words.
    std::cout << "\nTEST CASE B:\n";
    WordleHelper helper2(3);
    helper2.processAttempt({ "aaa", "###" });
    helper2.processAttempt({ "cde", "##+" });
    std::cout << convertDomainsToString(helper2.getAllowedLetters());
    std::cout << convertRequiredLettersToString(helper2.getRequiredLetters());

    // TEST CASE C: Check behavior with mixed feedback.
    std::cout << "\nTEST CASE C:\n";
    WordleHelper helper3(4);
    std::cout << convertDomainsToString(helper3.getAllowedLetters());
    std::cout << convertRequiredLettersToString(helper3.getRequiredLetters());
    helper3.processAttempt({"time", "#+##"});
    std::cout << convertDomainsToString(helper3.getAllowedLetters());
    std::cout << convertRequiredLettersToString(helper3.getRequiredLetters());
    std::cout << helper3.isCandidateValid("nixy") << "\n";
    std::cout << helper3.isCandidateValid("item") << "\n";
    helper3.processAttempt({"edit", "##+#"});
    std::cout << convertDomainsToString(helper3.getAllowedLetters());
    std::cout << convertRequiredLettersToString(helper3.getRequiredLetters());
    std::cout << helper3.isCandidateValid("xiii") << "\n";
    std::cout << helper3.isCandidateValid("igls") << "\n";
    
    // TEST CASE D: Longer word scenario.
    std::cout << "\nTEST CASE D:\n";
    WordleHelper helper4(10);
    std::cout << convertDomainsToString(helper4.getAllowedLetters());
    std::cout << convertRequiredLettersToString(helper4.getRequiredLetters());
    helper4.processAttempt({"abcdefghij", "#####++###"});
    std::cout << convertDomainsToString(helper4.getAllowedLetters());
    std::cout << convertRequiredLettersToString(helper4.getRequiredLetters());
    std::cout << helper4.isCandidateValid("fgzzzzzzzz") << "\n";
    std::cout << helper4.isCandidateValid("zzzzzzzzzz") << "\n";
    
    // TEST CASE E: Multi-step simulation with several attempts.
    std::cout << "\nTEST CASE E:\n";
    WordleHelper helper5(5);
    std::cout << convertDomainsToString(helper5.getAllowedLetters());
    std::cout << convertRequiredLettersToString(helper5.getRequiredLetters());
    helper5.processAttempt({"crane", "#+##+"});
    std::cout << convertDomainsToString(helper5.getAllowedLetters());
    std::cout << convertRequiredLettersToString(helper5.getRequiredLetters());
    std::cout << helper5.isCandidateValid("trace") << "\n";
    std::cout << helper5.isCandidateValid("crate") << "\n";
    helper5.processAttempt({"share", "==#++"});
    std::cout << convertDomainsToString(helper5.getAllowedLetters());
    std::cout << convertRequiredLettersToString(helper5.getRequiredLetters());
    std::cout << helper5.isCandidateValid("shred") << "\n";
    std::cout << helper5.isCandidateValid("scare") << "\n";
    helper5.processAttempt({"shred", "====="});
    std::cout << convertDomainsToString(helper5.getAllowedLetters());
    std::cout << convertRequiredLettersToString(helper5.getRequiredLetters());
    std::cout << helper5.isCandidateValid("shred") << "\n";
    std::cout << helper5.isCandidateValid("shrew") << "\n";

    // TEST CASE F: Single-letter word test to force domain reduction.
    std::cout << "\nTEST CASE F (Single letter test):\n";
    WordleHelper helper6(1);
    for (char c = 'a'; c < 'z'; ++c) {
        std::string letterStr;
        letterStr.push_back(c);
        helper6.processAttempt({letterStr, "#"});
    }
    std::cout << convertDomainsToString(helper6.getAllowedLetters());
    std::cout << convertRequiredLettersToString(helper6.getRequiredLetters());

    return 0;
}
