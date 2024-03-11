#pragma once

#include <mutex>
#include <list>
#include <algorithm>


class GameIDs {

public:
    int OccupieID();
    bool ReleaseID(const int& id);


private:
    std::mutex mutex;
    std::list<int> occupied_ids;
    std::list<int> released_ids;


};
