///The program works correctly overall
///The provided main shows good evidence of testing
///Please see below for more comments

#include <iostream>
#include <string>
#include <vector>

std::string get_column(const std::vector<std::string>& grid, int col_index) {
    std::string column;
    for (int r = 0; r < grid.size(); r++) {
        column += grid[r][col_index];
    }
    return column;
}

std::string get_row(const std::vector<std::string>& grid, int row_index) {
    return grid[row_index];
}


///The following function could benefit from further decomposition into suitably named functions
///for finding the word in a row, and for writing the word into the corresponding place in solution
///This reduces the level of nesting in this function and improves readability

// Search the grid horizontally and change the solution if found
bool search_horizontal(const std::vector<std::string>& grid, const std::string& word, std::vector<std::string>& solution) {
    for (int r = 0; r < grid.size(); r++) {
        std::string row = get_row(grid, r);

        if (word.size() <= row.size()) {

            int possibilities = row.size() - word.size();

            for (int i = 0; i <= possibilities; i++) {
                bool match = true;

                ///The following loop could be terminated as soon as the first character mismatch occurs
                ///by updating the condition to: j < word.size() && match

                for (int j = 0; j < word.size(); j++) {
                    if (row[i + j] != word[j]) {
                        match = false;
                    }
                }
                
                if (match) {
                    for (int j = 0; j < word.size(); j++) {
                        solution[r][i + j] = word[j];
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

///Similar to above, this function could benefit from further decomposition into suitably named functions
///for finding the word in a column, and for writing the word into the corresponding place in solution
///This reduces the level of nesting in this function and improves readability

// Search the grid vertically and change the solution if found
bool search_vertical(const std::vector<std::string>& grid, const std::string& word, std::vector<std::string>& solution) {
    for (int c = 0; c < grid[0].size(); c++) {
        std::string col = get_column(grid, c);

        if (word.size() <= col.size()){
            
            int possibilities = col.size() - word.size();

            for (int i = 0; i <= possibilities; i++) {
                bool match = true;
                for (int j = 0; j < word.size(); j++) {
                    if (col[i + j] != word[j]) {
                        match = false;
                    }
                }

                if (match) {
                    for (int j = 0; j < word.size(); j++) {
                        solution[i + j][c] = word[j];
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

// Function to find words in the grid and update the solution and not_found vectors
void find_words(const std::vector<std::string>& grid, const std::vector<std::string>& words, std::vector<std::string>& solution, std::vector<std::string>& not_found) {
    int rows = grid.size();
    int cols = grid[0].size();

    std::string empty_column;
    for (int i = 0; i < cols; i++) {
        empty_column += "#";
    }

    // Create empty grid
    for (int i = 0; i < rows; i++) {
        solution.push_back(empty_column);
    }

    // Search for each word
    for (int i = 0; i < words.size(); i++) {
        std::string word = words[i];
        bool found = search_horizontal(grid, word, solution) || search_vertical(grid, word, solution);
        if (!found) {
            not_found.push_back(word);
        }
    }

}


void test1(){
 
    std::vector<std::string> grid = {
        "xappleg",
        "dzeazkg",
        "zwazzbx",
        "dxnqzoy",
        "qyueytp",
        "pythtyg"
    };
 
    std::vector<std::string> words = {"peanut", "bot", "mango", "apple", "cinnamon"};
    std::vector<std::string> solution;
    std::vector<std::string> not_found;
 
    find_words(grid, words, solution, not_found); 
 
    std::vector<std::string> expected_solution = {
        "#apple#",
        "##e####",
        "##a##b#",
        "##n##o#",
        "##u##t#",
        "##t####"
    };
 
    std::vector<std::string> expected_not_found = {"mango", "cinnamon"};
 
    if( (solution == expected_solution) && (not_found == expected_not_found) ){
        std::cout << "test 1 passed" << std::endl;
    }
    else{
        std::cout << "test 1 failed" << std::endl;
    }
 
    std::cout << "solution:" << std::endl;
    for(int i = 0; i < solution.size(); i++){
        std::cout << solution[i] << std::endl;
    }
 
    std::cout << "not found:" << std::endl;
    for(int i = 0; i < not_found.size(); i++){
        std::cout << not_found[i] << std::endl;
    }
 
}

// Test with overlapping words
void test2(){
 
    std::vector<std::string> grid = {
        "xxxxaxxxx",
        "xxxadaxxx",
        "xxxxdxxxx",
        "xxxxaxxxx",
        "carxdxxxx"
    };
 
    std::vector<std::string> words = {"add", "dad", "car", "dog"};
    std::vector<std::string> solution;
    std::vector<std::string> not_found;
 
    find_words(grid, words, solution, not_found); 
 
    std::vector<std::string> expected_solution = {
        "####a####",
        "####d####",
        "####d####",
        "####a####",
        "car#d####"
    };
 
    std::vector<std::string> expected_not_found = {"dog"};
 
    if( (solution == expected_solution) && (not_found == expected_not_found) ){
        std::cout << "test 2 passed" << std::endl;
    }
    else{
        std::cout << "test 2 failed" << std::endl;
    }
 
    std::cout << "solution:" << std::endl;
    for(int i = 0; i < solution.size(); i++){
        std::cout << solution[i] << std::endl;
    }
 
    std::cout << "not found:" << std::endl;
    for(int i = 0; i < not_found.size(); i++){
        std::cout << not_found[i] << std::endl;
    }
 
}


int main() {
    test1();
    std::cout << std::endl;
    test2();
}