#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay

    void execute_commands(BlockFall &game, const vector<string> &commands); // Executes the commands read from the input file
    
    void print_grid(BlockFall &game); // Prints the grid
    bool is_game_over = true; // Checks if the game is over


private:
    vector<string> read_commands( const string &commands_file); // Reads the commands from the input file and returns them as a vector of strings
    
    void rotate_right(BlockFall &game); // Rotates the current block to the right
    void rotate_left(BlockFall &game); // Rotates the current block to the left

    void move_left(BlockFall &game); // Moves the current block to the left
    void move_right(BlockFall &game); // Moves the current block to the right

    void drop(BlockFall &game); // Drops the current block
    void gravity_switch(BlockFall &game); // Switches the gravity mode

    bool horizontal_collision_check(vector<vector<int>> &grid, vector<vector<bool>> &shape, int horizontal_index); // Checks if there is a collision
    bool power_up_check(BlockFall &game); // Checks if the power up is activated
    vector<int> clean_line_check(BlockFall &game); // Checks if there is a clean  lines, returns the indices of the lines to be cleaned
    void clean_line(BlockFall &game, vector<int> line_index); // Cleans the line at the given index
    void slide_blocks(BlockFall &game, vector<int> line_index); // Slides the blocks down after cleaning a line
    int check_fall_distance(BlockFall &game); 
    int min_distance(int a, int b); // Returns the minimum of two integers
    void clean_grid(BlockFall &game); // Cleans the grid
    void return_of_newton(BlockFall &game); // Activates the gravity and cells will be falling down
    int cell_count(vector<vector<bool>> shape); // Counts the number of cells in the block
    int cell_count(vector<vector<int>> grid); // Counts the number of cells in the grid
    void calculate_score(BlockFall &game); // Calculates the score
};




#endif //PA2_GAMECONTROLLER_H
