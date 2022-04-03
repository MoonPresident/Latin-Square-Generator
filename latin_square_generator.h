/**
 * Author: MoonPresident
 * Date: July 30th 2020
 * 
 * Functions for generating, culling and validating latin squares.
**/

#ifndef LATIN_SQUARE_GENERATOR
#define LATIN_SQUARE_GENERATOR



std::vector<std::vector<int>> generate_latin_square(int N);

std::vector<int> get_tower_clues(std::vector<std::vector<int>> latin);

std::vector<std::vector<int>> cull_latin_square(std::vector<std::vector<int>> latin, int cull);

#endif /* LATIN_SQUARE_GENERATOR */