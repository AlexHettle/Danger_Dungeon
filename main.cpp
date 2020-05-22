#include <iostream>
#include <map>
#include <random>
#include <time.h>
#include <stdlib.h>
#include <utility>
#include <string>
#include <conio.h>
std::string custom_print=R"(

  _____          _   _  _____ ______ _____    _____  _    _ _   _  _____ ______ ____  _   _
 |  __ \   /\   | \ | |/ ____|  ____|  __ \  |  __ \| |  | | \ | |/ ____|  ____/ __ \| \ | |
 | |  | | /  \  |  \| | |  __| |__  | |__) | | |  | | |  | |  \| | |  __| |__ | |  | |  \| |
 | |  | |/ /\ \ | . ` | | |_ |  __| |  _  /  | |  | | |  | | . ` | | |_ |  __|| |  | | . ` |
 | |__| / ____ \| |\  | |__| | |____| | \ \  | |__| | |__| | |\  | |__| | |___| |__| | |\  |
 |_____/_/    \_\_| \_|\_____|______|_|  \_\ |_____/ \____/|_| \_|\_____|______\____/|_| \_|

)"+std::string(20, '\n');
std::string victory_text=R"(

 __     ______  _    _   ______  _____  _____          _____  ______ _____  _ _ _
 \ \   / / __ \| |  | | |  ____|/ ____|/ ____|   /\   |  __ \|  ____|  __ \| | | |
  \ \_/ / |  | | |  | | | |__  | (___ | |       /  \  | |__) | |__  | |  | | | | |
   \   /| |  | | |  | | |  __|  \___ \| |      / /\ \ |  ___/|  __| | |  | | | | |
    | | | |__| | |__| | | |____ ____) | |____ / ____ \| |    | |____| |__| |_|_|_|
    |_|  \____/ \____/  |______|_____/ \_____/_/    \_\_|    |______|_____/(_|_|_)
)";
std::map<std::pair<int,int>,std::string> create_objects_in_room(){
    srand(time(0));
    std::map<std::pair<int,int>,std::string> locations_and_objects;
    locations_and_objects[std::make_pair(rand()%25+1,rand()%5+1)]="CHARACTER";
    for(int i=0;i<rand()%25+20;i++){
        auto point=std::make_pair(rand()%25+1,rand()%5+1);
        if (locations_and_objects.find(point)!=locations_and_objects.end()) {
            i-=1;
        }else{
            if(locations_and_objects.size()==1){
                locations_and_objects[point]="KEY";
            }else if(locations_and_objects.size()==2){
                locations_and_objects[point]="EXIT";
            }else{
                locations_and_objects[point]="EVENT";
            }
        }
    }
    return locations_and_objects;
}
int event(int health){
    srand(time(0));
    int choice=rand()%7+1;
    if(choice==1){
        std::cout<<"You stepped in a spike trap! Ouch!\n";
        health-=1;
    }else if(choice==2){
        std::cout<<"A spider jumps at you and bites you!\n";
        health-=1;
    }else if(choice==3){
        std::cout<<"A find what looks like a key but is\nactually a small creature that bites you!\n";
        health-=1;
    }else if(choice==4){
        std::cout<<"A goblin jumps out of nowhere, bites you, and runs away!\n";
        health-=1;
    }else if(choice==5){
        std::cout<<"A rock strikes you on the head!\n";
        health-=1;
    }else if(choice==6){
        std::cout<<"You accidentally step on a snake ad it bites you!\n";
        health-=1;
    }else{
        std::cout<<"You stub your foot on a rock and fall!\n";
        health-=1;
    }
    return health;
}
void print_map(std::map<std::pair<int,int>,std::string> all_object_locations){
    std::cout<<std::string(32,'X')<<"\n";

    for(int y=0;y<8;y++){
        std::cout<<'X';
        for(int x=0;x<30;x++){
            for(const auto & i:all_object_locations){
                auto key_pair = i.first;
                if(key_pair.first==x&&key_pair.second==y){
                    if(i.second=="CHARACTER"){
                        std::cout<<"C";
                    }else if(i.second=="EXIT"){
                        std::cout<<"E";
                    }else{
                        std::cout<<"#";
                    }
                    x+=1;
                }
            }
            std::cout<<" ";
        }
        std::cout<<"X\n";
    }
    std::cout<<std::string(32,'X')<<"\n";
}
void run_game(std::map<std::pair<int,int>,std::string> &all_object_locations,bool has_key,int health){
    auto movement=[](auto &the_movement,auto &all_object_locations,bool &has_key,int &health,auto &no_movement){
        std::cout<<std::string(40,'\n');
        if(all_object_locations.find(the_movement)!=all_object_locations.end()){
            if(all_object_locations[the_movement]=="KEY"){
                std::cout<<"You found the key!\nGet out of here as fast as you can!\n";
                has_key=true;
                all_object_locations[the_movement]="CHARACTER";
                run_game(all_object_locations,has_key,health);
            }else if(all_object_locations[the_movement]=="EXIT"){
                if(has_key==true){
                    all_object_locations[the_movement]="CHARACTER";
                    std::cout<<victory_text;
                }else{
                    all_object_locations[no_movement]="CHARACTER";
                    std::cout<<"You can't exit yet!\nYou still need to find the key!\n";
                    run_game(all_object_locations,has_key,health);
                }
            }else{
                health=event(health);
                if(health>0){
                    all_object_locations[the_movement]="CHARACTER";
                    run_game(all_object_locations,has_key,health);
                }else{
                    std::cout<<"!!!YOU LOSE!!!";
                }
            }
        }else{
            all_object_locations[the_movement]="CHARACTER";
            run_game(all_object_locations,has_key,health);
        }
    };
    std::pair<int,int> pairing;
    for(auto i:all_object_locations){
        if(i.second=="CHARACTER"){
            pairing=i.first;
        }
    }
    std::cout<<"HEALTH: "<<health<<"\n";
    print_map(all_object_locations);
    std::cout<<"Enter a direction to move:\n";
    char choice=getch();
    auto it=all_object_locations.find(pairing);
    all_object_locations.erase(it);
    std::pair<int,int> no_movement=std::make_pair(pairing.first,pairing.second);
    if(pairing.first>0&&tolower(choice)=='a'){
        std::pair<int,int> move_location=std::make_pair(pairing.first-1,pairing.second);
        movement(move_location,all_object_locations,has_key,health,no_movement);
    }else if(pairing.first<28&&tolower(choice)=='d'){
        std::pair<int,int> move_location=std::make_pair(pairing.first+1,pairing.second);
        movement(move_location,all_object_locations,has_key,health,no_movement);
    }else if(pairing.second>0&&tolower(choice)=='w'){
        std::pair<int,int> move_location=std::make_pair(pairing.first,pairing.second-1);
        movement(move_location,all_object_locations,has_key,health,no_movement);
    }else if(pairing.second<7&&tolower(choice)=='s'){
       std::pair<int,int> move_location=std::make_pair(pairing.first,pairing.second+1);
        movement(move_location,all_object_locations,has_key,health,no_movement);
    }else{
        std::cout<<std::string(40,'\n');
        all_object_locations[no_movement]="CHARACTER";
        run_game(all_object_locations,has_key,health);
    }
}
void start_up_screen(){
    std::cout<<custom_print;
    std::cout<<"\nPress any key to start the game!";
    getch();
}
int main(){
    std::cout<<std::string(40,'\n');
    int health=10;
    bool has_key=false;
    start_up_screen();
    std::map<std::pair<int,int>,std::string> all_object_locations=create_objects_in_room();
    std::cout<<std::string(20,'\n');
    run_game(all_object_locations,has_key,health);
}
