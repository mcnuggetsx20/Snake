#include <iostream>
#include <random>
#include <bitset>
#include <time.h>
#include <vector>

using namespace std;

void move(vector <pair <int, int>> bldr, pair<int, int> &hp, int diff, int mode, int x_apple, int y_apple, bool &lost, bool &eaten){

    if(mode==1 && hp.second != 4){
        hp.second -= diff;
    }
    else if(mode == 2 && hp.second != 39){
        hp.second += diff;
    }
    else if(mode == 3 && hp.first != 4){
        hp.first -= diff;
    }
    else if(mode == 4 && hp.first != 43){
        hp.first += diff;
    }

    if(hp.first == -1 || hp.first == 51 || hp.second == -1 || hp.second == 44){
        lost = true;
    }
    if(hp.first == x_apple && hp.second == y_apple){
        eaten = true;
    }
	for(auto i: bldr){
		if(hp.first * 16 >= i.first * 32 && hp.first * 16 <= i.first * 32 + 16 && hp.second * 16 >= i.second * 32 && hp.second * 16 <= i.second * 32 + 16){
			lost = true;
		}
	}
}

void update(vector <pair <int, int>> &pos, vector <bitset <45>> &taken, pair <int, int> hp, bool &refresh){
    pos.push_back(hp);
    taken[hp.second][hp.first] = 1;
    
    if(refresh){
        pos.erase(pos.begin());
        taken[pos[0].second][pos[0].first] = 0;
    }
    else{
        refresh = true;
    }
}

void eat(vector <bitset <45>> taken, int &r_factor, int &x_apple, int &y_apple, int &score, bool &eaten, bool &refresh){
    srand(r_factor);
    x_apple = (rand()%39) + 4;
    y_apple = (rand()%35) + 4; 
    if(taken[y_apple][x_apple]){
        cout << "zajete" << "\n";
        while(taken[y_apple][x_apple]){
            r_factor += time(NULL);
            srand(r_factor);
            x_apple = (rand()%39) + 4;
            y_apple = (rand()%35) + 4;                     
        }
    }  
    eaten = false;
    ++score;
    refresh = false;
}

void get_em_lose(vector <pair <int, int>> &pos, pair <int, int> &hp, int &mode, bool &start, bool &lost, bool &over_screen, bool &game){
    pos.clear();
    hp.first = 23;
    hp.second =23;
    pos.push_back(make_pair(23, 26));        
    pos.push_back(make_pair(23, 25));     
    pos.push_back(make_pair(23, 24));    
    pos.push_back(hp);
    start = false;
    lost = false;
    over_screen = true;
    mode = 1;
    game = false; 
}
