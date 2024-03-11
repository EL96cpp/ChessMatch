#include "GameIDs.h"


int GameIDs::OccupieID() {

    std::lock_guard lock_guard(mutex);

    int id;

    if (released_ids.empty()) {

        occupied_ids.push_back(++(*occupied_ids.rbegin()));
        id = *occupied_ids.rbegin();

    } else {

        id = *released_ids.rbegin();
        released_ids.pop_back();

    }

    return id;

}

bool GameIDs::ReleaseID(const int& id) {

    std::lock_guard lock_guard(mutex);
    
    if ((std::find(occupied_ids.begin(), occupied_ids.end(), id) != occupied_ids.end())) {

        occupied_ids.remove(id);
        released_ids.push_back(id);

        return true;

    } else {

        return false;    

    }



}


