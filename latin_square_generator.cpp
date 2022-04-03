#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <cstdlib>

#include <random> 



//Wrapper function to increase readability
void inline purge(std::vector<int> &line, int target) {
    line.erase(std::remove(line.begin(), line.end(), target), line.end());
}

//Wrapper function to increase readability.
void inline fill(std::vector<int> &line) {
    std::iota(line.begin(), line.end(), 1);
}


/**
 * @brief Generate a latin square by initializing 1 row and 1 column,
 *      then randomly creating row column pairs and seeing if they fit.
 * @param N - Size of the latin square to be generating.
 */
std::vector<std::vector<int>> generate_square(int N) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto eng = std::default_random_engine(seed);
    
    //Initialize a latin square, then generate the first line.
    std::vector<std::vector<int>> latin(N, std::vector<int>(N, 0));
    fill(latin.at(0));
    std::shuffle(latin.at(0).begin(), latin.at(0).end(), eng);
    
    //Generate the first column of the square.
    std::vector<int> latin_line(N, 0);
    fill(latin_line);
    purge(latin_line, latin.at(0).at(0));
    std::shuffle(latin_line.begin(), latin_line.end(), eng);
    
    for(int j = 1; j < N; j++) {
        latin.at(j).at(0) = latin_line.at(j - 1);
    }
    
    //Generate the square
    for(int i = 1; i < N; i++) {
        
        //Generate a column
        latin_line = std::vector<int>(N, 0);
        fill(latin_line);
        for(int j = 0; j < i; j++) {
            purge(latin_line, latin.at(j).at(i));
        }
        
        int flag = 1;
        int counter = 0;
        do {
            std::shuffle(latin_line.begin(), latin_line.end(), eng);
            flag = 0;
            for(int j = i; j < N && flag == 0; j++) {
                for(int k = 0; k < i && flag == 0; k++) {
                    flag = latin.at(j).at(k) == latin_line.at(j - i);
                }
            }
            if(counter++ > (1 << (N - i - 1))) return std::vector<std::vector<int>>(0);
        } while(flag);
        
        for(int j = i; j < N; j++) {
            latin.at(j).at(i) = latin_line.at(j - i);
        }
        
        //Generate a row
        latin_line = std::vector<int>(N, 0);
        fill(latin_line);
        for(int j = 0; j < i + 1; j++) {
            purge(latin_line, latin.at(i).at(j));
        }
        
        flag = 1;
        counter = 0;
        do {
            std::shuffle(latin_line.begin(), latin_line.end(), eng);
            flag = 0;
            for(int j = 0; j < i + 1 && flag == 0; j++) {
                for(int k = i + 1; k < N && flag == 0; k++) {
                    flag = latin.at(j).at(k) == latin_line.at(k - i - 1);
                }
            }
            if(counter++ > (1 << (N - i - 1))) return std::vector<std::vector<int>>(0);
            
        } while(flag);
        
        for(int j = i + 1; j < N; j++) {
            latin.at(i).at(j) = latin_line.at(j - i - 1);
        }
    }
    
    return latin;
}

/**
 * @brief Validate that the square handed in is a latin square of size N by N.
 * @param latin - The square to be validated.
 * @param N - The sidelength of the square.
 * @return The number of rows or columns that are incorrect.
 */
int check_square(std::vector<std::vector<int>> latin) {
    int counter = 0;
    int N = latin.size();
    for(int i = 0; i < N; i++) {
        std::vector<int> check_row(N, 0);
        std::vector<int> check_col(N, 0);
        fill(check_row);
        fill(check_col);
        for(int j = 0; j < N; j++) {
            purge(check_row, latin.at(j).at(i));
            purge(check_row, latin.at(i).at(j));
        }

        counter += check_row.size() + check_col.size();
    }
    return counter;
}

/**
 * @brief Generate a latin square of a set length
 * @param N Side length
 */
std::vector<std::vector<int>> generate_latin_square(int N) {
    
    std::vector<std::vector<int>> latin = generate_square(N);
    
    while(latin.empty()) {
        latin = generate_square(N);
    }
    
    check_square(latin);
    
    return latin;
    
}
 /**
  * @brief Produces the clues for a latin square.
  * @param latin
  * @param N
  * @return 
  */
std::vector<int> get_tower_clues(std::vector<std::vector<int>> latin) {
    int N = latin.size();;
    std::vector<std::vector<int *>> vectors(4, std::vector<int *>(N, 0));
    std::vector<int> clues(4 * N);

    
    //This is copied from the optimized latin tower solver, which is why its a little convoluted.
    for (int j = 0; j < N; j++) {
        //Generate a vectors of pointers for the rows and columns. One vector for each direction.
        for(int x = 0; x < N; x++) {
            vectors.at(0).at(x) = (&(*(latin.at(x).begin() + j)));
            vectors.at(1).at(x) = (&(*(latin.at(j).begin() + x)));
            vectors.at(2).at(x) = (&(*(latin.at(N - 1 - x).begin() + j)));
            vectors.at(3).at(x) = (&(*(latin.at(j).begin() + N - 1 - x)));
        }
        int* bearings[] = {&clues.at(j), &clues.at(4 * N - 1 - j), &clues.at(3 * N - 1 - j), &clues.at(j + N)};
        for (int x = 0; x < 2; x++) {
            int watermarkLeft = *vectors.at(x).at(0);
            int watermarkRight = *vectors.at(x).at(N - 1);
            int risesLeft = 1;
            int risesRight = 1;
            
            for(int k = 1; k < N; k++) {
                if(*vectors.at(x).at(k) > watermarkLeft) {
                    watermarkLeft = *vectors[x][k];
                    risesLeft++;
                }
            }
            for(int k = 1; k < N; k++) {
                if(*vectors.at(x).at(N - 1 - k) > watermarkRight) {
                    watermarkRight = *vectors.at(x).at(N - 1 - k);
                    risesRight++;
                }
            }
            *bearings[x] = risesLeft;
            *bearings[x + 2] = risesRight;
        }
    }
    
    return clues;
}

/**
 * @brief Removes values from a Latin square
 * @param latin - The square to have values removed from
 * @param N - The sidelength of the square
 * @param cull - The number of values to be culled.
 */
std::vector<std::vector<int>> cull_latin_square(std::vector<std::vector<int>> latin, int cull) {
    
    int N = latin.size();
    int culled = 0;
    int limit = 0;

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(latin.at(i).at(j) > 0) limit++;
        }

        if(limit > cull) {
            limit = cull;
            break;
        }
    }
    
    std::vector<std::vector<int>> culled_latin = latin;
    
    while(culled < limit) {
        int row = rand() % N;
        int col = rand() % N;
        
        if(culled_latin.at(row).at(col) > 0) {
            culled_latin.at(row).at(col) = 0;
            culled++;
        }
    }
    
    return culled_latin;
}