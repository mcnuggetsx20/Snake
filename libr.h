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
		if(hp.first >= i.first && hp.first <= i.first + 1 && hp.second >= i.second && hp.second <= i.second + 1){
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

void choose(int &x, int &y, vector <bitset <45>> taken, int r_factor, int a, int b, int c, int d, int w, int h){
	srand(r_factor);
	x = (rand()%a) + c;
	y = (rand()%b) + d;
	if(taken[y][x]){
		while(taken[y][x] || taken[y][x+w] || taken[y+h][w] || taken[y+h][y+w]){
			r_factor += time(NULL);
			srand(r_factor);
			x = (rand()%a) + c;
			y = (rand()%b) + d;
		}
	}
}

void eat(vector <bitset <45>> taken, int &r_factor, int &x_apple, int &y_apple, int &score, bool &eaten, bool &refresh){
	choose(x_apple, y_apple, taken, r_factor, 39, 35, 4, 4, 0, 0);
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
