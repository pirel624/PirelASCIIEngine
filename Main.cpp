#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <thread>
#include <chrono>


#define KEY_W 119
#define KEY_S 115
#define KEY_A 97
#define KEY_D 100


//Pros
    //Simple to use

//Cons
    //Very not responsive

// Problem
    // stuck in loop



class display
{
private:
    static const int size_x = 100;
    static const int size_y = 30;
    char buffer[size_y][size_x];

public:
    display()
    {
        for(int y = 0; y < size_y; y++)
        {
            for(int x = 0; x < size_x; x++)                      // Filling buffer with empty space
            {
                buffer[y][x] = ' ';
            }
        }
    }

    void draw(int x, int y, char sprite)
    {
        if(x >= 1 && x <= size_x && y >= 1 && y <= size_y)
        {
            buffer[y - 1][x - 1] = sprite;
        }else{}

    }

    void render()
    {

        system("cls");
        system("color a");
        for(int ass = 0; ass < size_x + 2; ass++)
        {
            std::cout << '=';                                            // Rendering upper border
        }
        std::cout << std::endl;

        for(int y = 0; y < size_y; y++)
        {
            std::cout << '|';
            for(int x = 0; x < size_x; x++)                             // Rendering border
            {
                std::cout << buffer[y][x];
            }
            std::cout << '|' << std::endl;
        }

        for(int ass = 0; ass < size_x + 2; ass++)
        {
            std::cout << '=';                                     // Rendering bottom border
        }
        std::cout << std::endl;

    }

    void clean()
    {
        for(int y = 0; y < size_y; y++)
        {
            for(int x = 0; x < size_x; x++)                      // Filling buffer with empty space
            {
                buffer[y][x] = ' ';
            }
        }
    }








};

namespace input
{

    static char pool;

    void poll()
    {
        while(1)
        {
            pool = char(getch());
        }
    }





}



class npc
{
public:
    char sprite;
    int pos_x;
    int pos_y;
    bool life;

};

class enemy: public npc
{
public:


    enemy()
    {
        sprite = '@';
        pos_x = 10;
        pos_y = 10;
        life = true;
    }

    void see_player(int x, int y)
    {
        if(pos_y < y){pos_y++;}
        if(y == pos_y)
        {
            if(pos_x > x){pos_x--;}
            if(pos_x < x){pos_x++;}
        }
        if(pos_y > y){pos_y--;}
    }

    void mov_x(int val){pos_x += val;}
    void mov_y(int val){pos_y += val;}

    void tele_x(int val){pos_x = val;}
    void tele_y(int val){pos_y = val;}


    void kill()
    {
        life = false;
    }
    void refresh(int x = 20, int y = 20)
    {
        life = true;
        pos_x = x;
        pos_y = y;
    }

    void draw(display & layar)
    {
        if(life)
        {
            layar.draw(pos_x, pos_y, sprite);
        }else{}
    }



};

class bullet: public npc
{

private:
    char direct;

public:
    bullet()
    {
        sprite = '*';
        life = false;
    }

    void start(char direction, int x, int y)
    {
        direct = direction;
        pos_x = x;
        pos_y = y;
        life = true;
    }

    void update()
    {
        //if(life)
        {
            if(direct == 'u'){pos_y -= 1;}
            if(direct == 'd'){pos_y += 1;}
            if(direct == 'l'){pos_x -= 1;}
            if(direct == 'r'){pos_x += 1;}
        }//else{}
    }

    void kill()
    {
        life = false;
    }

    void draw(display & layar)
    {
        if(life)
        {
            layar.draw(pos_x, pos_y, sprite);
        }else{}
    }



};

class player:public npc
{
private:
    int health;
    bool life;
    char direction_momentum;

public:

    player():health(100), life(true)
    {
        sprite = 'P';
        pos_x = 20;
        pos_y = 20;
    }

    void kill()
    {
        life = false;
    }

    void revive()
    {
        life = true;
    }


    void mov_x(int val){pos_x += val;if(val >= 0){direction_momentum = 'r';}else if(val < 0){direction_momentum = 'l';}}              // Move based on vector translation
    void mov_y(int val){pos_y += val;if(val >= 0){direction_momentum = 'd';}else if(val < 0){direction_momentum = 'u';}}

    void tele_x(int val){pos_x = val;}             // Instantly move to a sepsific positiin
    void tele_y(int val){pos_y = val;}

    void draw(display& layar)
    {
        layar.draw(pos_x, pos_y, sprite);
    }


};


bool isCollide(npc bb1, npc bb2)
{
    if((abs(bb1.pos_x - bb2.pos_y)) <= 1 && (abs(bb1.pos_y - bb2.pos_y) <= 1)){return true;}
    else{return false;}
}


class animation
{
private:
    const int max_frame;
    char frames[100];
    int position;
    int pos_x;
    int pos_y;
    bool life;

public:
    animation(int maxx, int x, int y):max_frame(maxx), position(1), pos_x(x), pos_y(y), life(true)
    {

        for(int a = 0; a < max_frame; a++)
        {
            frames[a] = ' ';
        }
    }

    void manual_update()
    {
        position++;
        if(position > max_frame){position = 1; life = false;}
    }

    void revive()
    {
        life = true;
        position = 1;
    }

    void draw(display& layar)
    {
        if(life)
        {
            layar.draw(pos_x, pos_y, frames[position - 1]);
            position++;
            if(position > max_frame){position = 1; life = false;}
        }else{}
    }

    void animate(char image, int pos)
    {
        frames[pos - 1] = image;
    }

    void change_position(int x, int y)
    {
        pos_x = x;
        pos_y = y;
    }

    void kill()
    {
        life = false;
    }
};


class barrier: public npc
{
private:
    char id;

public:
    barrier(int x, int y, char idd):id(idd)
    {
        pos_x = x;
        pos_y = y;
        if(id == 'w'){sprite = '#';}
    }

    void draw(display & layar)
    {
        layar.draw(pos_x, pos_y, sprite);
    }
};


namespace world_holder
{






}





int main()
{


    std::thread anjay(input::poll);



    display layar;
    bullet peluru;
    peluru.start('u', 20, 20);

    player pirel;

    animation bias(5, 10, 10);
    bias.animate('*', 1);
    bias.animate('o', 2);
    bias.animate('O', 3);
    bias.animate('@', 4);
    bias.animate('.', 5);

    barrier b1(16, 13, 'w');

    while(true)
    {
        //input
        Sleep(1000);            // is getch() really that slow ?

        if(input::pool == 'w'){pirel.mov_y(-1);}
        if(input::pool == 's'){pirel.mov_y(1);}
        if(input::pool == 'a'){pirel.mov_x(-1);}
        if(input::pool == 'd'){pirel.mov_x(1); bias.revive(); bias.change_position(20, 20);}



        //simulation
        peluru.update();




        //rendering
        layar.clean();
        b1.draw(layar);
        peluru.draw(layar);
        pirel.draw(layar);
        bias.draw(layar);
        layar.render();

        input::pool = ' ';




    }

    return 0;



}
