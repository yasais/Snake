#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>



using namespace std; //permet de le pas utiliser sf:: ou std:: à chaque fois. pour faire cela on a besoin de <iostream> juste en haut
using namespace sf;

int taille = 25;
int grid = 25;
int num = 3;
int dir = 1;
float delay = 150;
bool gameon = false;
bool gameover = false;
int check_sound = 0;
int NumeroScore = 0;



struct snake {  
    int x, y;
}s[100];        

struct fruit {  
    int x, y;
}f;

void Jeu()
{
    for (int i = num; i > 0; i--)
    {
        s[i].x = s[i - 1].x; //les corps se suivent
        s[i].y = s[i - 1].y;
    }

    if (dir == 0) s[0].y -= 1; //haut  
    if (dir == 1) s[0].x += 1; //droite
    if (dir == 2) s[0].y += 1; //bas
    if (dir == 3) s[0].x -= 1; //gauche

    if (s[0].x == f.x && s[0].y == f.y) //ajoute une case au snake lorsqu'il mange un fruit
    {
        num++;
        NumeroScore++;
        f.x = rand() % taille;
        f.y = rand() % taille;
        if(delay>1)            // permet d'aller plus vite quand il mange
            delay=delay/1.1;

       

    }

    for(int i= 0 ; i<= num ; i++) //boucle mirroir
    {
        if(s[i].x < 0) s[i].x = grid-1;
        if(s[i].x >= grid) s[i].x = 0;
        if(s[i].y < 0) s[i].y = grid-1;
        if(s[i].y >= grid) s[i].y = 0;
    }



}


int main()
{


    RenderWindow window(VideoMode(taille*grid, taille*grid), "Snake Game", Style::Titlebar);
    window.setFramerateLimit(60);

    Clock timer;

    srand(time(NULL));
    f.x = rand() % taille;
    f.y = rand() % taille;

//******Pour l'affichage du score******//
    Font Typo;
    Typo.loadFromFile("/home/aissaoui/Dev/ARCADE.TTF");
    Text Score;
    Score.setFont(Typo);
    Score.setString("Score :");
    Score.setCharacterSize(30);
    Score.setFillColor(Color::White);

 

 //*****Pour le Son*****//
 
    SoundBuffer buffer;                                    
       
        if (!buffer.loadFromFile("sonSnake.wav"))
            printf("je ne trouve pas le son snake");

    SoundBuffer buffer2;                              
       
        if (!buffer2.loadFromFile("Sexy.wav"))
            printf("je ne trouve pas le son sexy");


    Sound sound_snake;
    sound_snake.setBuffer(buffer);

    Sound sound_sexy;
    sound_sexy.setBuffer(buffer2);

   

//***** Initialisation de nos shape ********//

    RectangleShape head(Vector2f(taille, taille));
    RectangleShape box(Vector2f(taille, taille));
    box.setFillColor(Color::Green);
   
    CircleShape apple(taille / 2);
   

   
//******* MENU *******//

    Texture texture;
    texture.loadFromFile("snake1.png");
    Sprite sprite;
    sprite.setTexture(texture);
    Vector2f targetSize(taille*grid,taille*grid);
    sprite.setScale(
    targetSize.x/ sprite.getLocalBounds().width,  
    targetSize.y/ sprite.getLocalBounds().height);


//****** Game Over ******//

    Texture tex;
    tex.loadFromFile("minion.png");
    Sprite spri;
    spri.setTexture(tex);
    Vector2f target_Size(taille*grid,taille*grid);
    spri.setScale(
    targetSize.x/ spri.getLocalBounds().width,  
    targetSize.y/ spri.getLocalBounds().height);





    while (window.isOpen())
    {

        Event event;

        while (window.pollEvent(event))  
        {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
           
                while (gameon == false) //si le jeu n'est pas lancer :
                {
                    window.clear();
                    window.draw(sprite);                              
                    window.display();
                   
                    if(Keyboard::isKeyPressed(Keyboard::Enter))
                    {    
                        gameon = true;                                
                        sound_snake.play();                            
                        sound_snake.setVolume(10);                    
                    }
                   
                    if(Keyboard::isKeyPressed(Keyboard::Escape))
                        return 0; //fin de jeu window close

                }


                while (gameover == false) //si tu as perdu :
                {
                    window.clear();
                    window.draw(spri);
                    window.display();
                   
                    if(Keyboard::isKeyPressed(Keyboard::Enter))
                    {    
                        gameover = true;
                        sound_snake.play();  
                        sound_snake.setVolume(10);
                       
                    }
                   
                    if(Keyboard::isKeyPressed(Keyboard::Escape))
                        return 0;

                }

        }



       //***** Changement de décor quand mange 10 fruits *******//
               
        if (num >= 13) //si le corps est plus grand que le nombre :
        {
       
            if(check_sound == 0)
                check_sound = 1; //on active check_sound
        }

   
        if (check_sound == 1) //check sound activé
        {
            check_sound = 2; //on change de couleur de snake et de musique
            box.setFillColor(Color(172, 51, 255));
            sound_snake.stop();
            sound_sexy.play();
            sound_sexy.setPlayingOffset(seconds(10));
            sound_sexy.setVolume(10);  
            sound_sexy.setLoop(true);
        }






   
        //****Les conditions de Game Over + Reset si rejouer*****//
               

        if (timer.getElapsedTime().asMilliseconds() > delay)
        {
            Jeu();
            for (int i = 1; i < num; i++)
            {  
                if (s[0].x  == s[i].x)
                {
                    if (s[0].y == s[i].y)
                    {
                        sound_snake.stop();
                        sound_sexy.stop();
                        gameover = false;
                        NumeroScore = 0;
                        check_sound = 0;
                        num = 3;
                        box.setFillColor(Color::Green);
                        delay = 150;

                    }
                }
            }

            if (s[0].x == taille && s[0].y == taille)
            {
                window.close();
            }

            timer.restart();
        }


        //******** Touche du jeu ********//

        if (Keyboard::isKeyPressed(Keyboard::Up) && dir!=2) dir = 0;
        if (Keyboard::isKeyPressed(Keyboard::Right) && dir!=3) dir = 1;
        if (Keyboard::isKeyPressed(Keyboard::Down) && dir!=0) dir = 2;
        if (Keyboard::isKeyPressed(Keyboard::Left) && dir!=1) dir = 3;
        window.clear();


        //**Changement de couleur de fond**//

        if (num >= 13)
        window.clear(Color(255,51,212 ));



        //*******SCORE*******//

        string NumeroScore_str = to_string(NumeroScore);  
        Text ScoreNum;
        Font Typoo;
        Typoo.loadFromFile("/home/aissaoui/Dev/ARCADE.TTF");
        ScoreNum.setFont(Typoo);
        ScoreNum.setString(NumeroScore_str);
        ScoreNum.setCharacterSize(30);
        ScoreNum.setFillColor(Color::White);
        ScoreNum.setPosition(130,0);


        //*** Assigne box(le corps) à la struct****/

        for (int i = 0; i < num; i++)
        {
            box.setPosition(s[i].x * taille, s[i].y * taille);
            window.draw(box);
        }


        //**** Affichage *****//

        head.setPosition(s[0].x * taille, s[0].y * taille);
        window.draw(head);
        apple.setPosition(f.x * taille, f.y * taille);
        window.draw(apple);
        window.draw(Score);
        window.draw(ScoreNum);
        window.display();

    }

   
}