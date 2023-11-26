#include "Leaderboard.h"

void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).

    if(this->head_leaderboard_entry == nullptr){
        this->head_leaderboard_entry = new_entry;
        return;
    }
    LeaderboardEntry * temp = this->head_leaderboard_entry;
    while (temp->next_leaderboard_entry->score >= new_entry->score && temp->next_leaderboard_entry != nullptr){
        temp = temp->next_leaderboard_entry;
    }
    new_entry->next_leaderboard_entry = temp->next_leaderboard_entry;
    temp->next_leaderboard_entry = new_entry;
    temp = nullptr; // avoid dangling pointer
    this->top_ten(); // delete the entries after the 10th entry
}

void Leaderboard::write_to_file(const string& filename) {
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions

        // Open the file. This will clear the file if it already exists.
    std::ofstream file(filename);

    // Check if the file was opened successfully
    if (!file) {
        // Handle error
        return;
    }

    LeaderboardEntry * temp = this->head_leaderboard_entry;
    int count = 1;
    file << "Leaderboard" << endl << "-----------" << endl;
    while(temp != nullptr){
        file << count << ". " << temp->player_name << " " << temp->score << " " << this->convert_time_format(temp->last_played) << endl;
        temp = temp->next_leaderboard_entry;
        count ++;
    }

    temp = nullptr;
}

void Leaderboard::read_from_file(const string& filename) {
    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.
    
    
    // Create the file if not exist 

    fstream file(filename, ios::in | ios::out); // open the file, if not exist, create it 

    if(!file.is_open()){
        return;
    }

    string line;
    bool isHead = true;
    LeaderboardEntry * temp;

    while(getline(file, line)){
        // split the line into 3 parts: score, timestamp, player name
        
        vector<string> parts;
        stringstream ss(line);
        string part;
        while(getline(ss, part, ',')){
            parts.push_back(part);
        }
        
        string timestamp_str = parts[1];
        string score_str = parts[2];

        string name = parts[0];
        unsigned long score = stoul(score_str);
        time_t timesamp = (time_t) stoi(timestamp_str);

        LeaderboardEntry * new_entry = new LeaderboardEntry(score, timesamp, name);

        if(isHead){
            this->head_leaderboard_entry = new_entry;
            isHead = false;
        }
        if(temp != nullptr){
            temp->next_leaderboard_entry = new_entry;
        }
        temp = new_entry;
    }
    file.close();
    
}

string Leaderboard::convert_time_format(time_t &timestamp) {
    struct tm * tmp;

    char buffer[20];

    time(&timestamp);

    tmp = localtime(&timestamp);

    strftime(buffer, sizeof(buffer), "%H:%M:%S/%d.%m.%Y", tmp);  
    
    tmp = nullptr;

    return buffer;
}

void Leaderboard::top_ten() {
    
    int count = 1; 

    LeaderboardEntry * temp = this->head_leaderboard_entry;
    while(temp != nullptr && count < 10){
        count ++;
        temp = temp->next_leaderboard_entry;
    }

    if(count < 10){
        return;
    }

    LeaderboardEntry * temp2 = temp->next_leaderboard_entry;
    temp->next_leaderboard_entry = nullptr;

    while (temp2 != nullptr) {
        temp = temp2;
        temp2 = temp2->next_leaderboard_entry;
        delete temp;
    }
    
    temp = nullptr;
    temp2 = nullptr;
}

void Leaderboard::print_leaderboard() {
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
    LeaderboardEntry * temp = this->head_leaderboard_entry;
    int count = 1;
    cout << "Leaderboard" << endl << "-----------" << endl;
    while(temp != nullptr){
        cout << count << ". " << temp->player_name << " " << temp->score << " " << this->convert_time_format(temp->last_played) << endl;
        temp = temp->next_leaderboard_entry;
        count ++;
    }

    temp = nullptr;

}

Leaderboard::~Leaderboard() {
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
    LeaderboardEntry * temp = this->head_leaderboard_entry;
    LeaderboardEntry * temp2 = temp->next_leaderboard_entry;

    while (temp2 != nullptr) {
        delete temp;
        temp = temp2;
        temp2 = temp2->next_leaderboard_entry;
    }
    delete temp;
    temp = nullptr;
    temp2 = nullptr;
    head_leaderboard_entry = nullptr;
    
}



