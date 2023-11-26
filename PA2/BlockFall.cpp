#include "BlockFall.h"

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)
    
    ifstream file(input_file);
    if(!file.is_open()){
        return;
    }
    string line;
    int numofColumn;
    vector<vector<bool>> matrix;
    bool ishead = true;
    Block *temp = nullptr;
    


/*
                            line contains 
                      <----      |       ---->     
                    ']'         ' '           '['
                <--- &                     <--- & --->
            ' '                         ' '           ']'
    There are 4 cases for the line:
    1. '[' is at the beginning of the line and ']' is at the end of the line
    2. '[' is at the beginning of the line and ']' is not at the end of the line
    3. '[' is not at the beginning of the line and ']' is at the end of the line
    4. '[' is not at the beginning of the line and ']' is not at the end of the line
*/
    while(getline(file, line)){


        if(is_new_matrix(line)){
            
            line = line.substr(1, line.length()-1);
            if(is_end_of_matrix(line)){  // one dimensional (one row)

                line = line.substr(0, line.length()-1);
                numofColumn = line.length();
                matrix.resize(1, vector<bool>(numofColumn));
                for (int i = 0; i < numofColumn; ++i) {
                    for (int j = 0; j < numofColumn; ++j) {
                        matrix[0][j] = line[j] == '1';
                    }
                }
                Block *block = new Block(matrix);
                if(ishead){
                    this->initial_block = block;
                    this->active_rotation = block;
                    ishead = false;
                }
                if(temp != nullptr){
                    create_rotations(*temp, block);
                }
                temp = block;

            } else if (!is_end_of_matrix(line)){  // two dimensional (multiple rows)
                numofColumn = line.length();
                vector<bool> row = vector<bool>(numofColumn);
                for (int i = 0; i < numofColumn; ++i) {
                    row.push_back(line[i] == '1');
                }
                matrix.push_back(row);
                // TODO
            }

        } else if(!is_new_matrix(line)){

            if(is_end_of_matrix(line)){      
                                  //***************************************************
                line = line.substr(0, line.length()-1);
                numofColumn = line.length();
                vector<bool> row = vector<bool>(numofColumn);
                for (int i = 0; i < numofColumn; ++i) 
                    row.push_back(line[i] == '1');
                matrix.push_back(row);

                Block * block = new Block(matrix);
                if(ishead){
                    this->initial_block = block;
                    this->active_rotation = block; // *********************possibly will changed
                    ishead = false;
                }
                // Create rotations of the block
                
                if(temp != nullptr){
                    create_rotations(*temp,block);

                }
                temp = block;



            } else if (!is_end_of_matrix(line)){
                numofColumn = line.length();
                vector<bool> row = vector<bool>(numofColumn);
                for (int i = 0; i < numofColumn; ++i) 
                    row.push_back(line[i] == '1');
                matrix.push_back(row);
            }
        
        }

    }
    file.close();
    this->power_up = temp->shape; // last block is power up
    
    temp = initial_block;

    // traverse the linked list to the before last block
    while(temp->next_block->next_block != nullptr){
        temp = temp->next_block;
    }
    delete temp->next_block;    // delete the last block (power up)
    temp->next_block = nullptr; // remove the last block from the linked list (power up)

}



/**
 * Checks if the line is the end of a matrix
 * @param line the line to check
 * @return true if the line is the end of a matrix, false otherwise
*/
bool BlockFall::is_end_of_matrix(const string &line) {
    return line.find(']') != std::string::npos;
}


/**
 * Checks if the line is a new matrix
 * @param line the line to check
 * @return true if the line is a new matrix, false otherwise
 */
bool BlockFall::is_new_matrix(const string &line) {
    return line.find('[') != std::string::npos;
}


/**
 * Creates the rotations of the block and assigns the next block to the next parameter
 * @param block the block to create rotations for
 * @param next the next block to assign to the block
 * 
*/
void BlockFall::create_rotations(Block &block, Block * next) {

    link_blocks(block, next);

    if(block != block.right_rotate() ){

        block.right_rotation = new Block(block.right_rotate());

        link_blocks(*block.right_rotation, next); // link the right rotation to the next block

        block.right_rotation->left_rotation = &block;

        if(*block.right_rotation == block.left_rotate()){

            block.left_rotation = block.right_rotation;

            block.right_rotation->right_rotation = &block;
            

        } else {
            block.left_rotation = new Block(block.left_rotate());
            
            link_blocks(*block.left_rotation, next); // link the left rotation to the next block

            block.left_rotation->right_rotation = &block;

            block.left_rotation->left_rotation = new Block((*block.left_rotation).left_rotate());

            link_blocks(*block.left_rotation->left_rotation, next); // link the left's left rotation to the next block

            block.left_rotation->left_rotation->right_rotation = block.left_rotation;

            block.left_rotation->left_rotation->left_rotation = block.right_rotation;

            block.right_rotation->right_rotation = block.left_rotation->left_rotation;
            
        }
    } else {
        block.right_rotation = &block;
        block.left_rotation = &block;
    }
}


void BlockFall::link_blocks(Block &block, Block * next) {
    block.next_block = next;
}




void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main

    // Open the .dot file
    ifstream file(input_file);

    if (!file.is_open()) {
        return;
    }

    // Variables to store the grid size
    int rows = 0;
    int cols = 0;

    // 2D vector to store the grid values
    vector<vector<int>> grid;

    // Read the file line by linecb  
    string line;
    while (getline(file, line)) {
        
            // Extract the matrix values and store them in the grid
            vector<int> row;
            int value;
            stringstream ss(line);
            while (ss >> value) {
                row.push_back(value);
            }
            grid.push_back(row);
            rows++;
            cols = row.size();
        
    }

    file.close();

    this->rows = rows;
    this->cols = cols;

    // Initialize the "grid" member variable
    this->grid.resize(rows, vector<int>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            this->grid[i][j] = grid[i][j];
        }
    }
}



BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks
    // * traverse the linked list and delete all blocks

    Block * temp = this->initial_block;
    Block * next = temp->next_block;

    while(next != nullptr){

        while(temp->right_rotation != nullptr){
            if(temp->right_rotation == temp)
                break;
            next = temp->right_rotation;
            temp->right_rotation = nullptr;
            temp = next->right_rotation;
            delete next;
        }

        next = temp->next_block;
        temp->next_block = nullptr;
        delete temp;
        temp = next; 
    }
    
    temp = nullptr;
    next = nullptr;

    

}



