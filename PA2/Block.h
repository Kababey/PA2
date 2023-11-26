#ifndef PA2_BLOCK_H
#define PA2_BLOCK_H

#include <vector>

using namespace std;

class Block {
public:

    Block(vector<vector<bool>> &matrix);

    
    ~Block();

    vector<vector<bool>> shape; // Two-dimensional vector corresponding to the block's shape
    Block * right_rotation = nullptr; // Pointer to the block's clockwise neighbor block (its right rotation)
    Block * left_rotation = nullptr; // Pointer to the block's counter-clockwise neighbor block (its left rotation)
    Block * next_block = nullptr; // Pointer to the next block to appear in the game
    
    bool Block::operator==(const Block& other) const {
    if (this->shape == other.shape) {
        return true;
    }
    return false;
}




bool Block::operator!=(const Block& other) const {
    if (this->shape != other.shape) {
        return true;
    }
    return false;
}
    Block right_rotate(); // Rotates the block to the right
    Block left_rotate(); // Rotates the block to the left
};


Block::~Block() {
    delete this->right_rotation;
    delete this->left_rotation;
    delete this->next_block;      //***************************Possibliy error
    // TODO: delete the block's rotations and the next block
}


/**
 * Rotates the block to the right 
 * @return 
*/
Block Block::right_rotate() {
    int numofRow = this->shape.size();
    int numofColumn = this->shape[0].size();
    vector<vector<bool>> newShape = vector<vector<bool>>(numofColumn, vector<bool>(numofRow)); 
    for (int i = 0; i < numofRow; ++i) {
        for (int j = 0; j < numofColumn; ++j) {
            newShape[j][numofRow - i - 1] = this->shape[i][j];
        }
    }
    return Block(newShape);
}

/**
 * Rotates the block to the left
 * @return 
 */

Block Block::left_rotate() {
    int numofRow = this->shape.size();
    int numofColumn = this->shape[0].size();
    vector<vector<bool>> newShape = vector<vector<bool>>(numofColumn, vector<bool>(numofRow)); 
    for (int i = 0; i < numofRow; ++i) {
        for (int j = 0; j < numofColumn; ++j) {
            newShape[numofColumn - j -1][i] = this->shape[i][j];
        }
    }
    return Block(newShape);
}


#endif //PA2_BLOCK_H
