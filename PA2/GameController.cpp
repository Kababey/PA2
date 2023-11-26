#include "GameController.h"

bool GameController::play(BlockFall& game, const string& commands_file){

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.

    // TODO: create a LeaderboardEntry instance with the player name and the current score and add it to the leaderboard
    
    
    LeaderboardEntry* entry = new LeaderboardEntry(game.current_score, time(nullptr), game.player_name);
    game.leaderboard.insert_new_entry(entry);
    vector<string> commands = read_commands(commands_file);
    execute_commands(game, commands);
    
    if (game.active_rotation == nullptr){
        is_game_over = true;
    }

    return is_game_over ;
}


void GameController::execute_commands(BlockFall &game , const vector<string>& commands){

    for (const std::string& command : commands) {

        if (command == std::string("PRINT_GRID")) {
            // TODO: Implement PRINT_GRID functionality
            unsigned long highest_score = std::max(game.leaderboard.head_leaderboard_entry->score, game.current_score);
            cout << "Score: " << game.current_score << endl;
            cout << "High Score: " <<  highest_score << endl;
            print_grid(game);
        }
        else if (command == std::string("ROTATE_RIGHT")) {
            // TODO: Implement ROTATE_RIGHT functionality
            rotate_right(game);
        }
        else if (command == std::string("ROTATE_LEFT")) {
            // TODO: Implement ROTATE_LEFT functionality
            rotate_left(game);
        }
        else if (command == std::string("MOVE_LEFT")) {
            // TODO: Implement MOVE_LEFT functionality
            move_left(game);
        }
        else if (command == std::string("MOVE_RIGHT")) {
            // TODO: Implement MOVE_RIGHT functionality
            move_right(game);
        }
        else if (command == std::string("DROP")) {
            // TODO: Implement DROP functionality
            drop(game);
            if(!is_game_over){
                cout << "Game Over!" << endl;
                break;
            }
        }
        else if (command == std::string("GRAVITY_SWITCH")) {
            // TODO: Implement GRAVITY_SWITCH functionality
            game.gravity_mode_on = !game.gravity_mode_on;
            if(game.gravity_mode_on){
                return_of_newton(game);
            }
        }
        else {
            // TODO: Handle unknown command
        }
    }
}


vector<string> GameController::read_commands(const string& commands_file){

    fstream file(commands_file, ios::in);
    if( !file.is_open() ){
        cout << "Error: cannot open the commands file" << endl;
        return;
    }

    string line;
    vector<string> commands;
    while(getline(file, line)){
        commands.push_back(line);
    }
    file.close();
    return commands;

}

void GameController::print_grid(BlockFall& game){
    
    // printing place could be changed

    vector<vector<bool>> block_shape = game.active_rotation->shape;
    vector<vector<int>> grid = game.grid;
    // insert the active block top left corner to the grid 
    for(int i = 0; i < block_shape.size(); i++){
        for(int j = 0; j < block_shape[i].size(); j++){
            if(block_shape[i][j]){
                grid[i][j] = 1;
            }
        }
    }

    // print the grid
    for (int i = 0; i < grid.size() ;i++){
        for(int j = 0; j < grid[0].size(); j++){
            if(grid[i][j] == 1)
                cout << occupiedCellChar;
            else
                cout << unoccupiedCellChar;
        }
        cout << endl;
    }
    
}


void GameController::rotate_right(BlockFall& game){
    
        if (horizontal_collision_check(game.grid, game.active_rotation->right_rotation->shape, game.block_horizontal_position)){
            return;
        }
    
        game.active_rotation = game.active_rotation->right_rotation;
}

void GameController::rotate_left(BlockFall& game){

    if (horizontal_collision_check(game.grid, game.active_rotation->left_rotation->shape, game.block_horizontal_position)){
        return;
    }

    game.active_rotation = game.active_rotation->left_rotation;

}

void GameController::move_left(BlockFall& game){
        
        if (horizontal_collision_check(game.grid, game.active_rotation->shape, game.block_horizontal_position - 1)){
            return;
        }
    
        game.block_horizontal_position -= 1;
    
}

void GameController::move_right(BlockFall& game){

    if (horizontal_collision_check(game.grid, game.active_rotation->shape, game.block_horizontal_position + 1)){
        return;
    }

    game.block_horizontal_position += 1;

}


bool GameController::horizontal_collision_check(vector<vector<int>>& grid, vector<vector<bool>>& shape, int horizontal_index){

    // check bounds
    if(horizontal_index < 0 || horizontal_index + shape[0].size() > grid[0].size()){
        return true;
    }

    // check collision
    for(int i = 0; i < shape.size(); i++){
        for(int j = 0; j < shape[i].size(); j++){
            if(shape[i][j] && grid[i][j + horizontal_index] == 1){
                return true;
            }
        }
    }
    return false;
}


void GameController::drop(BlockFall& game){
    
    int fall_distance = check_fall_distance(game);
    int cell_amount = cell_count(game.active_rotation->shape);
    game.current_score += fall_distance * cell_amount;
    // Gravity mode on
    if (game.gravity_mode_on){
        return_of_newton(game);
    }
    calculate_score(game);
    game.active_rotation = game.active_rotation->next_block;
    game.block_horizontal_position = 0;
    if(horizontal_collision_check(game.grid, game.active_rotation->shape, game.block_horizontal_position)){
        is_game_over = false;
    }
}

bool GameController::power_up_check(BlockFall& game){
    
    int puWidth = game.power_up[0].size();
    int puHeight = game.power_up.size();

    int gridWidth = game.grid[0].size();
    int gridHeight = game.grid.size();

    for(int i = 0; i < gridWidth - puWidth; i++){
        for(int j = 0; j < gridHeight - puHeight; j++){
            bool powerUpFound = true;
            for(int k = 0; k < puWidth; k++){
                for(int l = 0; l < puHeight; l++){
                    if(game.power_up[k][l] && game.grid[i+k][j+l] == 0){
                        powerUpFound = false;
                        break;
                    }
                }
                if(!powerUpFound){
                    break;
                }
            }
            if(powerUpFound){
                return true;
            }
        }
    }
    return false;
}




vector<int> GameController::clean_line_check(BlockFall& game){
    vector<int> cleanLines;
    for(int i = 0; i < game.grid.size(); i++){
        bool cleanLine = true;
        for(int j = 0; j < game.grid[i].size(); j++){
            if(game.grid[i][j] == 0){
                cleanLine = false;
                break;
            }
        }
        if(cleanLine){
            cleanLines.push_back(i);
        }
    }
    return cleanLines;
}



/**
 * Cleans the line at the given index
 * @param game the game object
 * @param line_index the indices of the lines to be cleaned
 */
void GameController::clean_line(BlockFall& game, vector<int> line_index){
    for(int i = 0; i < line_index.size(); i++){
        game.grid[line_index[i]] = vector<int>(game.grid[line_index[i]].size(), 0);
    }
}


/**
 * Slides the blocks down after cleaning a line
 * @param game the game object
 * @param line_index the indices of the lines to be cleaned
 */
void GameController::slide_blocks(BlockFall& game, vector<int> line_index){
    clean_line(game, line_index);
    for (int i = 0; i < line_index.size(); i++){
        for (int j = line_index[i] ; j > 0; j--){
            game.grid[j] = game.grid[j-1];
        }
        game.grid[0] = vector<int>(game.grid[0].size(), 0);
    }
}


int GameController::check_fall_distance(BlockFall& game){

    vector<vector<bool>> block_shape = game.active_rotation->shape;
    vector<vector<int>> grid = game.grid;
    int horizontal_index = game.block_horizontal_position;
    int distance = grid.size()-1;

    for(int i = 0; i < block_shape.size(); i++){
        for(int j = 0; j < block_shape[i].size(); j++){

            distance = min_distance((grid.size()- i), distance);

            if(block_shape[i][j]){

                for(int k = 0;k < grid.size(); k++){

                    if(grid[k][j + horizontal_index] == 1){

                        distance = min_distance((k - i), distance); ///////////check this
                        break;

                    }
                } 
            }
        }
    }
    return distance -1; // -1 because the block is already in the grid

}

int GameController::min_distance(int a, int b){
    if(a < b){
        return a;
    }
    return b;
}

void GameController::clean_grid(BlockFall& game){
    for(int i = 0; i < game.grid.size(); i++){
        game.grid[i] = vector<int>(game.grid[i].size(), 0);
    }
}

int GameController::cell_count(vector<vector<bool>> shape){
    int count = 0;
    for(int i = 0; i < shape.size(); i++){
        for(int j = 0; j < shape[i].size(); j++){
            if(shape[i][j]){
                count++;
            }
        }
    }
    return count;
}

int GameController::cell_count(vector<vector<int>> grid){
    int count = 0;
    for(int i = 0; i < grid.size(); i++){
        for(int j = 0; j < grid[i].size(); j++){
            if(grid[i][j] == 1){
                count++;
            }
        }
    }
    return count;   
}


// All blocks which are over of the space it will fall down 
void GameController::return_of_newton(BlockFall& game){
    
    bool hasChanged = true;
    while (hasChanged) {
    hasChanged = false;

    for (int col = 0; col < game.grid[0].size(); col++) {
      int prior = -1;
      for (int row = game.grid.size() - 1; row >= 0; row--) {
        if (game.grid[row][col] == 0) {
          prior = row;
        } else if (prior != -1) {
          game.grid[prior][col] = game.grid[row][col];
          game.grid[row][col] = 0;
          prior--;
          hasChanged = true; // Mark that a change has occurred
        }
      }
    }
  }
}


void GameController::calculate_score(BlockFall& game){
    if(power_up_check(game)){
        int cell_amount = cell_count(game.grid);
        clean_grid(game);
        game.current_score += cell_amount;
        game.current_score += 1000;
    }

    vector<int> cleanLines = clean_line_check(game);
    if (cleanLines.size() > 0){
        slide_blocks(game, cleanLines);
        game.current_score += game.grid[0].size() * cleanLines.size();
    }
}
