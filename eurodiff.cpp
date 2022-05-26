#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;

const int MAX_SIZE = 20;
const int INIT_COINS = 1000000;
const int REPRESENTITIVE_COINS = 1000;

map<int, pair<int, string>> countryIDs;
map<int, bool> countryComplete;

typedef struct City{
    int country;
    vector<int> coins;
    vector<int> sending_coins;
    
    bool isComplete;
    City():country(-1), isComplete(false){};
} City;

const vector<int> grid_indecies = {0, 1, 0, -1, 0};

void printMap(const int& numberOfCountries, const int& caseNumber, vector<pair<int,int>>& completeCountryTime){
     sort(completeCountryTime.begin(), completeCountryTime.end(), [&](const auto& left, const auto& right){
        if (left.second == right.second){
            return countryIDs[left.first].second < countryIDs[right.second].second;
        }
        return left.second < right.second;
    });
    
    cout << "Case Number" << caseNumber << endl;
    for (int i = 0; i < completeCountryTime.size(); i++){
       cout << countryIDs[completeCountryTime[i].first].second << " " << (numberOfCountries == 1 ? 0 : completeCountryTime[i].second) << endl;
    }
}
void readInput(const int& numberOfCountries, vector<vector<City>>& grid){
     // Read Input
    for (int country = 0; country < numberOfCountries; country++){
        string name;
        int xl, xh, yl, yh;
        cin >> name >> xl >> yl >> xh >> yh;
        if (xl < 0 || yl < 0 || xh < 0 || yh < 0){
            cout << "No negative coordinates\n";
            exit(-1);
        }
            
         
        countryIDs[country] = {(xh - xl + 1)*(yh - yl + 1),name};
        countryComplete[country] = false;
         
        for (int x = xl; x <= xh; x++){
            for (int y = yl; y <= yh; y++){
                grid[x][y].country = country;
                grid[x][y].coins.resize(numberOfCountries, 0);
                grid[x][y].sending_coins.resize(numberOfCountries, 0);
                grid[x][y].coins[country] = INIT_COINS;
            }
        }
    }
}

void init(vector<vector<City>>& grid){
   // Init grid
    
    for (int x = 0; x < MAX_SIZE; x++){
        vector<City> v_c;
        grid.push_back(v_c);
        for (int y = 0; y < MAX_SIZE; y++){
            City c;
            grid[x].push_back(c);
        }
    } 
}

void solve(int numberOfCountries, int caseNumber){
    if (numberOfCountries == 0){
        return ;
    }
    // Init grid
    vector<vector<City>> grid;
    init(grid);
    // Read Input
    readInput(numberOfCountries, grid);
    
    int completeCountries = 0;
    int days = 1;
    vector<pair<int, int>> completeCountryTime;
    while (completeCountries < numberOfCountries){
        
        // income from the cities
        for (int x = 0; x < MAX_SIZE; x++){
            for (int y = 0; y < MAX_SIZE; y++){
                if (grid[x][y].country != -1){
                    for (int country = 0; country < (int)grid[x][y].coins.size(); country++){
                        // 1. For all neibghour cities send all types of coins
                        int hasChanges = 0;
                        for (int grid_index = 0; grid_index < grid_indecies.size() - 1; grid_index++){
                            if (x + grid_indecies[grid_index]     < MAX_SIZE && 
                                y + grid_indecies[grid_index + 1] < MAX_SIZE &&
                                x + grid_indecies[grid_index]     >= 0       && 
                                y + grid_indecies[grid_index + 1] >= 0       &&
                            grid[x + grid_indecies[grid_index]][y + grid_indecies[grid_index + 1]].country != -1){
                                grid[x + grid_indecies[grid_index]][y + grid_indecies[grid_index + 1]].sending_coins[country] += grid[x][y].coins[country] / REPRESENTITIVE_COINS;
                                hasChanges++;
                            }
                        }
                      
                        if (hasChanges != 0){
                            grid[x][y].sending_coins[country] -= hasChanges * (grid[x][y].coins[country] / REPRESENTITIVE_COINS);
                        }
                    }
                }
            }
        }
        
        // outcome to the cities
        map<int, int> countryCount;
        for (int x = 0; x < MAX_SIZE; x++){
            for (int y = 0; y < MAX_SIZE; y++){
                if (grid[x][y].country != -1){
                    int isComplete = 0;
                    for (int country = 0; country < (int)grid[x][y].coins.size(); country++){
                        grid[x][y].coins[country] += grid[x][y].sending_coins[country];
                        grid[x][y].sending_coins[country] = 0;
                        
                        if (grid[x][y].coins[country] > 0){
                            isComplete++;
                        }
                    }
                    if (grid[x][y].isComplete){
                        countryCount[grid[x][y].country]++;
                    }else if (isComplete == (int)grid[x][y].coins.size()){
                        grid[x][y].isComplete = true;
                        countryCount[grid[x][y].country]++;
                    }
                }
            }
        }
        // match completed countries 
        for (const auto& country : countryIDs){
            if (countryCount.find(country.first) != countryCount.end()){
                if (countryCount[country.first] == country.second.first && !countryComplete[country.first]){
                    completeCountryTime.push_back({country.first, days});
                    countryComplete[country.first] = true;
                    completeCountries++;
                }
            }
        }
        
        days++;
    }
    // Print map
    printMap(numberOfCountries, caseNumber, completeCountryTime);  
}

int main()
{ 
    int numberOfCountries = -1;
    int caseNumber = 1;
    while (numberOfCountries != 0){
        countryIDs.clear();
        countryComplete.clear();
        
        cin >> numberOfCountries;
        solve(numberOfCountries, caseNumber);
        
        caseNumber++;
    }

    return 0;
}
