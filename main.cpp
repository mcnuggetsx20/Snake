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
int x_apple, y_apple, r_factor = time(NULL), ans = 0, score = 0, mode = 1, counter = 0, diff = 1, x;
pair <int, int> hp;
bool screen_on = true, eaten = false, start = false, refresh = true, lost = false, vim_practice = true, over_screen = false;

Color bck_color(170, 218, 24);

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

    RenderWindow screen {VideoMode{768, 704}, "snake v1.0"};
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
    int l;
    h_s1 >> l;
    h_s1.close();

    Font font;
    font.loadFromFile("assets/Minecraft.ttf");

    Text score_display, game_over;
    score_display.setFont(font);
    score_display.setCharacterSize(24);
    score_display.setFillColor(Color::Black);

    game_over.setFont(font);
    game_over.setCharacterSize(40);
    game_over.setFillColor(Color::Black);

    Event event;  

    Clock clock;

    Keyboard::Key up;
    Keyboard::Key down;
    Keyboard::Key rt;
    Keyboard::Key lt;

    if(vim_practice){
        up = Keyboard::K;
        down = Keyboard::J;
        rt = Keyboard::L;
        lt = Keyboard::H;
    }
    else{
        up = Keyboard::Up;
        down = Keyboard::Down;
        rt = Keyboard::Right;
        lt = Keyboard::Left;
    }

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
                if(Keyboard::isKeyPressed(up) && mode != 2){
                    mode = 1;
                    start = true;
                }
                if(Keyboard::isKeyPressed(down) && mode != 1){
                    mode = 2;
                    start = true;
                }
                if(Keyboard::isKeyPressed(rt) && mode != 3){
                    mode = 4;
                    start = true;
                }
                if(Keyboard::isKeyPressed(lt) && mode != 4){
                    mode = 3;
                    start = true;
                }
                if(Keyboard::isKeyPressed(Keyboard::Space) && over_screen){
                    over_screen = false;
                }
            }
        }
        if(over_screen){

            game_over.setPosition(265, 315);
            game_over.setString("Game Over");
            screen.draw(game_over);
            screen.display();
             while(over_screen){
                while(screen.pollEvent(event)){
                    if(event.type == Event::Closed){
                        over_screen = false;
                        screen_on = false;
                    }

                    else if(event.type == Event::KeyPressed){
                        if(Keyboard::isKeyPressed(Keyboard::Space)){
                            over_screen = false;
                        }
                        else if(Keyboard::isKeyPressed(Keyboard::Escape)){
                            over_screen = false;
                            screen_on = false;
                        }
                    }
                }
            }
        }

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
                lost = true;
            }
        }

        //wypisywanie
        apple.setPosition(16 * x_apple, 16 * y_apple);
        head.setPosition(16 * hp.first, 16 * hp.second);
        
        screen.draw(apple);
        screen.draw(head);

        if(hp.first == x_apple && hp.second == y_apple){
            eaten = true;
        }

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
            screen.draw(body);
            if(pos[i] == hp){
                lost = true;
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
        
        //cout << pos.size() << "\n";
        screen.display();
        screen.clear();
        screen.draw(bck);
        
        score_display.setPosition(0, -2);
        score_display.setString("Score: " + to_string(score));
        screen.draw(score_display);
        
        score_display.setPosition(0, 25);
        score_display.setString("High Score: " + to_string(l));
        screen.draw(score_display); 

        if(lost){
            pos.clear();
            hp.first = 23;
            hp.second =23;
            pos.push_back(make_pair(23, 26));        
            pos.push_back(make_pair(23, 25));     
            pos.push_back(make_pair(23, 24));    
            pos.push_back(hp);
            h_s1.open("high_score.txt");
            h_s1 >> l;
            h_s1.close();
            
            h_s2.open("high_score.txt");
            x = max(l, score);
            h_s2 << x;
            l = x;
            score = 0;
            start = false;
            lost = false;
            h_s2.close();
            srand(r_factor + time(NULL));
            x_apple = (rand()%39) + 4;
            y_apple = (rand()%35) + 4;
            over_screen = true;
            mode = 1; 
        }
    }
    h_s1.open("high_score.txt");
    h_s1 >> l;
    h_s1.close();

    h_s2.open("high_score.txt");
    x = max(l, score);
    h_s2 << x;
    h_s2.close();
}
