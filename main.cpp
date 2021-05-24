#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include "libr.h"
#include <fstream>

using namespace sf;

// 1 - gora
// 2 - dol
// 3 - lewo
// 4 - prawo

vector <pair <int, int>> pos;
vector <pair <int, int>> bldr(3);
vector <bitset <45>> taken(40);
int x_apple, y_apple, x_boulder, y_boulder, r_factor = time(NULL), ans = 0, score = 0, mode = 1, counter = 0, diff = 0, x, arrow = 0;
pair <int, int> hp;
bool screen_on = true, eaten = false, start = false, refresh = true, lost = false, vim_practice = false, over_screen = false, game = false, options = false,  menu = true, new_score = false, clicked1 = false, clicked2 = false, clicked3 = false, clicked4 = false, clicked5= false;
Vector2i _mouse;

Color bck_color(170, 218, 24);

int main()
{
    hp.first = 23;
    hp.second = 23;
        
    pos.push_back(make_pair(23, 26));        
    pos.push_back(make_pair(23, 25));     
    pos.push_back(make_pair(23, 24));    
    pos.push_back(hp);

	taken[26][23] = 1;
	taken[25][23] = 1;
	taken[24][23] = 1;
	taken[23][23] = 1;
    
	for(int i = 0 ; i < 3; ++ i){
		r_factor += i * i + 6577;
		choose(bldr[i].first, bldr[i].second, taken, r_factor, 30, 30, 6, 6, 1, 1);
		taken[bldr[i].second][bldr[i].first] = true;
		taken[bldr[i].second][bldr[i].first + 1] = true;
		taken[bldr[i].second + 1][bldr[i].first + 1] = true;
		taken[bldr[i].second + 1][bldr[i].first] = true;
	}
	
	choose(x_apple, y_apple, taken, r_factor, 39, 35, 4, 4, 0, 0);

	Image icon;
	icon.loadFromFile("assets/head.png");

    RenderWindow screen {VideoMode{768, 704}, "snake v1.0"};
	screen.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    screen.setFramerateLimit(60);

    Texture iapple, ihead, ibody, ibck, imain, iquit, iplay, iopts, ioptions_screen, iboulder, iover, ipa, imb, ion, ioff;
    iapple.loadFromFile("assets/apple.png");
    ihead.loadFromFile("assets/head.png");    
    ibody.loadFromFile("assets/body.png");
    ibck.loadFromFile("assets/background.png");
    imain.loadFromFile("assets/main_menu.png");
    iquit.loadFromFile("assets/quit.png");
    iplay.loadFromFile("assets/play.png");
    iopts.loadFromFile("assets/opts.png");
    ioptions_screen.loadFromFile("assets/options.png");
	iboulder.loadFromFile("assets/boulder.png");
	iover.loadFromFile("assets/gameover.png");
	ipa.loadFromFile("assets/play_again.png");
	imb.loadFromFile("assets/main_button.png");
	ion.loadFromFile("assets/frame_on.png");
	ioff.loadFromFile("assets/frame_off.png");
    
    Sprite apple(iapple), head(ihead), body(ibody), bck(ibck), main(imain), quit(iquit), play(iplay), opts(iopts), options_screen(ioptions_screen), boulder(iboulder), over(iover), pa(ipa), mb(imb), f_on(ion), f_off(ioff);
    bck.setPosition(0,0);
    main.setPosition(0,0);
	mb.setPosition(107, 508);
	pa.setPosition(438, 508);
	quit.setPosition(268, 463);
	opts.setPosition(268, 376);
	play.setPosition(268, 289);
	f_on.setPosition(610, 136);
	f_off.setPosition(610, 136);

    ifstream h_s1;
    ofstream h_s2;
    h_s1.open("high_score.txt");
    int l;
    h_s1 >> l;
    h_s1.close();

	h_s1.open("config.txt");
	h_s1 >> vim_practice;
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
        r_factor += time(NULL);

        while(screen.pollEvent(event)){  
            if(event.type == Event::Closed){
                screen_on = false;
                break;
            }

            else if(event.type == Event::KeyPressed){
                if(Keyboard::isKeyPressed(Keyboard::Escape)){
                    if(over_screen){
                        over_screen = false;
                        menu = true;
                    }
                    else if(menu){
                        menu = false;
                        screen_on = false;
                    }
                    break;
                }
				if(game){
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
				}
                if(Keyboard::isKeyPressed(Keyboard::Space)){
                    if(over_screen){
                        over_screen = false;
                        game = true;
                        mode = 1;
                        new_score = false;
                    }
                    else if(menu){
                        menu = false;
                        game = true;
                        mode = 1;
                    }
                }
                break;
            }

            else if(event.type == Event::MouseButtonPressed){
                if(Mouse::isButtonPressed(Mouse::Left)){
                    _mouse = Mouse::getPosition(screen);
                    if(menu){
                        if((_mouse.x >= 268 && _mouse.x <= 491) && (_mouse.y >= 285 && _mouse.y <= 351)){                                
                            clicked1 = true;
                        }
						else if((_mouse.x >= 268 && _mouse.x <= 491) && (_mouse.y >= 372 && _mouse.y <= 438)){
							clicked2 = true;
						}
                        else if((_mouse.x >= 268 && _mouse.x <= 491) && (_mouse.y >= 459 && _mouse.y <= 525)){
                            clicked3 = true;
                        }
                    }
					else if(over_screen){
						if(_mouse.x >= 107 && _mouse.x <= 319 && _mouse.y >= 504 && _mouse.y <= 570){
							clicked4 = true;
						}	
						else if(_mouse.x >= 438 && _mouse.x <= 661 && _mouse.y >= 504 && _mouse.y <= 570){
							clicked5 = true;
						}
					}
                }
            }

            else if(event.type == Event::MouseButtonReleased){
                if(event.mouseButton.button == Mouse::Left){
                    _mouse = Mouse::getPosition(screen);
					clicked1 = false;
					clicked2 = false;
					clicked3 = false;
					clicked4 = false;
					clicked5 = false;
                    if(menu){
                        if((_mouse.x >= 268 && _mouse.x <= 491) && (_mouse.y >= 285 && _mouse.y <= 351)){         
                            menu = false;
                            game = true;
                            mode = 1;
                        }
                        else if((_mouse.x >= 268 && _mouse.x <= 491) && (_mouse.y >= 459 && _mouse.y <= 525)){
                            menu = false;
                            screen_on = false;
                        }
						else if((_mouse.x >= 268 && _mouse.x <= 491) && (_mouse.y >= 372 && _mouse.y <= 438)){
							menu = false;
							options = true;
						}
                    }
					else if(options){ 
						arrow = 0;
						arrow += (_mouse.x >= 70 && _mouse.x <= 119) && (_mouse.y >= 49 && _mouse.y <= 70);
						arrow += (_mouse.x >= 63 && _mouse.x <= 69) && (_mouse.y >= 35 && _mouse.y <= 84);
						arrow += (_mouse.x >= 56 && _mouse.x <= 62) && (_mouse.y >= 42 && _mouse.y <= 77);
						arrow += (_mouse.x >= 49 && _mouse.x <= 55) && (_mouse.y >= 49 && _mouse.y <= 70);
						arrow += (_mouse.x >= 42 && _mouse.x <= 48) && (_mouse.y >= 56 && _mouse.y <= 63);
						if(arrow){
							options = false;
							menu = true;
						}
						else if(_mouse.x >= 610 && _mouse.x <= 705 && _mouse.y >= 136 && _mouse.y <= 186){
							vim_practice = !vim_practice;
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
							h_s2.open("config.txt");
							h_s2 << vim_practice;
							h_s2.close();
						}
					}
					else if(over_screen){
						if(_mouse.x >= 107 && _mouse.x <= 319 && _mouse.y >= 504 && _mouse.y <= 570){
							over_screen = 0;
							menu = 1;
						}	
						else if(_mouse.x >= 438 && _mouse.x <= 661 && _mouse.y >= 504 && _mouse.y <= 570){
							over_screen = false;
							game = true;
							mode = 1;
							new_score = false;
						}
					}
                }
            }
        }

        if(menu){
            screen.clear();
            screen.draw(main);
            if(clicked1){
                screen.draw(play);
            }
			else if(clicked2){
				screen.draw(opts);
			}
            else if(clicked3){
                screen.draw(quit);
            }
            screen.display();
        }

		else if(game){
            ++counter;
            diff = !(counter%6);
            counter %= 6;

            if(start){
                move(bldr, hp, diff, mode, x_apple, y_apple, lost, eaten);
            }

            if(diff && start){
                update(pos, taken, hp, refresh);
            }

            //wypisywanie
            apple.setPosition(16 * x_apple, 16 * y_apple);
            head.setPosition(16 * hp.first, 16 * hp.second);

			for(auto i: bldr){
				boulder.setPosition(16 * i.first, 16 * i.second);
				screen.draw(boulder);
			}
           
            screen.draw(apple);
            screen.draw(head);
            for(int i = 0; i < pos.size()-1; ++i){
                body.setPosition(pos[i].first * 16, pos[i].second * 16);
                screen.draw(body);
                if(pos[i] == hp){
                    lost = true;
                    break;
                }
            }

            if(eaten){
                eat(taken, r_factor, x_apple, y_apple, score, eaten, refresh);
            }
            
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
                get_em_lose(pos, hp, mode, start, lost, over_screen, game);
                h_s1.open("high_score.txt");
                h_s1 >> l;
                h_s1.close();
                
                h_s2.open("high_score.txt");
                x = max(l, score);
                h_s2 << x;
                new_score = score > l; 
                l = x;
                score = 0;
                h_s2.close();
				
				vector <bitset <45>> taken(40);
				taken[26][23] = 1;
				taken[25][23] = 1;
				taken[24][23] = 1;
				taken[23][23] = 1;
				r_factor = time(NULL);

				choose(x_apple, y_apple, taken, r_factor, 39, 35, 4, 4, 0, 0);

				for(int i = 0 ; i < 3; ++ i){
					r_factor += i * i + 6577;
					choose(bldr[i].first, bldr[i].second, taken, r_factor, 30, 30, 6, 6, 1, 1);
					taken[bldr[i].second][bldr[i].first] = true;
					taken[bldr[i].second][bldr[i].first + 1] = true;
					taken[bldr[i].second + 1][bldr[i].first + 1] = true;
					taken[bldr[i].second + 1][bldr[i].first] = true;
				}
            }
        }

		else if(over_screen){
            over.setPosition(64, 64);
			screen.draw(over);

            if(new_score){
                game_over.setPosition(307, 370);
                game_over.setCharacterSize(20);       
                game_over.setString("New High Score!");
                screen.draw(game_over);

                game_over.setPosition(385, 400);
                game_over.setString(to_string(l));
                screen.draw(game_over);
            }
			if(clicked4){
				screen.draw(mb);
			}
			else if(clicked5){
				screen.draw(pa);
			}
            screen.display();
        }

		else if(options){
            screen.clear();
            screen.draw(options_screen);
			if(vim_practice){
				screen.draw(f_on);
			}
			else{
				screen.draw(f_off);
			}
            screen.display();
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
