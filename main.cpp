#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <time.h>
#include <vector>
#include <fstream>
#include <bitset>

using namespace std;
using namespace sf;

// 1 - gora
// 2 - dol
// 3 - lewo
// 4 - prawo

vector <pair <int, int>> pos;
vector <bitset <45>> taken(40);
int x_apple, y_apple, r_factor = time(NULL), ans = 0, score = 0, mode = 0, counter = 0, diff = 1;
pair <int, int> hp;
bool screen_on = true, eaten = false, start = false, refresh = true;

Color bck_color(170, 218, 24), score_color(195, 195, 195);

int main()
{
    hp.first = 23;
    hp.second = 23;
        
    pos.push_back(make_pair(23, 26));        
    pos.push_back(make_pair(23, 25));     
    pos.push_back(make_pair(23, 24));    
    pos.push_back(hp);
    
    srand(r_factor);
    x_apple = (rand()%39) + 4;
    y_apple = (rand()%35) + 4;    

    RenderWindow screen {VideoMode{768, 704}, "snake"};
    screen.setFramerateLimit(60);
    
    Texture iapple, ihead, ibody, ibck;
    iapple.loadFromFile("assets/apple.png");
    ihead.loadFromFile("assets/head.png");    
    ibody.loadFromFile("assets/body.png");
    ibck.loadFromFile("assets/background.png");
    Sprite apple(iapple), head(ihead), body(ibody), bck(ibck);
    bck.setPosition(0,0);
    
    ifstream h_s1;
    ofstream h_s2;
    h_s1.open("high_score.txt");
    string l;
    h_s1 >> l;

    Font font;
    font.loadFromFile("assets/Minecraft.ttf");

    Text score_display;
    score_display.setFont(font);
    score_display.setCharacterSize(24);
    score_display.setFillColor(Color::Black);

    Event event;  

    Clock clock;

    while(screen_on)
    {
        ++counter;   
        r_factor += time(NULL);  

        while(screen.pollEvent(event)){  
            if(event.type == Event::Closed){
                screen_on = false;
                break;
            }

            else if(event.type == Event::KeyPressed){
                if(Keyboard::isKeyPressed(Keyboard::Escape)){
                    if(start){
                        start = false;
                    }
                    else{
                        screen_on = false;
                    }
                    break;
                }

                if(Keyboard::isKeyPressed(Keyboard::Up) && mode != 2){
                    mode = 1;
                }
                if(Keyboard::isKeyPressed(Keyboard::Down) && mode != 1){
                    mode = 2;
                }
                if(Keyboard::isKeyPressed(Keyboard::Right) && mode != 3){
                    mode = 4;
                }
                if(Keyboard::isKeyPressed(Keyboard::Left) && mode != 4){
                    mode = 3;
                }
                if(!start){
                    start = true;
                }
            }
        }      
        //8 zer
        if(counter != 6){
            diff = 0;
        }
        else{
            diff = 1;
            counter = 0;
        }
        
        if(start){
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
                cout << "u lost";
                screen_on = false;
            }
        }

        //cout << hp.first << " " << hp.second << "\n";

        //wypisywanie
        for(int i = 0; (i < 44 && screen_on); ++ i){
            for(int j = 0; j < 50; ++j){
                ans = 0;
                if(i == y_apple && j == x_apple){
                    apple.setPosition(16*j, 16*i);
                    screen.draw(apple);
                    ++ans;                    
                }
                if(i == hp.second && j == hp.first){
                    head.setPosition(j*16, i*16);
                    screen.draw(head);
                    ++ans;
                }
                if(ans == 2){
                    eaten = true;
                }
            }
        }

        //cout << diff << " " << start << " " << refresh << "\n";
        if(diff && start){
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


        for(int i = 0; i < pos.size()-1; ++i){
            body.setPosition(pos[i].first * 16, pos[i].second * 16);
            //cout << pos[i].first << " " << pos[i].second << "\n";
            screen.draw(body);
            if(pos[i] == hp){
                cout << "u lost";
                screen_on = false;
                break;
            }
        }

        if(eaten){
            srand(r_factor);
            x_apple = (rand()%39) + 4;
            y_apple = (rand()%35) + 4; 
            if(taken[y_apple][x_apple]){
                cout << "zajete" << "\n";
                while(taken[y_apple][x_apple]){
                    srand(r_factor);
                    x_apple = (rand()%39) + 4;
                    y_apple = (rand()%35) + 4;                     
                }
            }  
            eaten = false;
            ++score;
            refresh = false;
        }

        //cout << pos.size() << "\n";
        screen.display();
        screen.clear();
        screen.draw(bck);
        
        score_display.setPosition(0, -2);
        score_display.setString("Score: " + to_string(score));
        screen.draw(score_display);
        
        score_display.setPosition(0, 25);
        score_display.setString("High Score: " + l);
        screen.draw(score_display); 
    }
    h_s1.close();
    h_s2.open("high_score.txt");
    int prev = stoi(l);
    int x = max(prev, score);
    h_s2 << x;
    h_s2.close();
}