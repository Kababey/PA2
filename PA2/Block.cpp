#include "Block.h"


Block::Block(vector<vector<bool>> &matrix) {
    this->shape = matrix;
    // TODO: create the block's rotations and properly set the pointers

}

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
                 
