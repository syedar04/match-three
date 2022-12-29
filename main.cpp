#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <iomanip>
using namespace sf;
using namespace std;
#define SFML_NO_DEPRECATED_WARNINGS

int ts = 65;
Vector2i offset(380, 84);

    
class tile
{
public:
    int xcord, ycord, column, row, var, match, alpha;
    tile() { match = 0; alpha = 500; }
}
matrix[10][10];


//Function for Swapping
void swap(tile block1, tile block2)
{
    std::swap(block1.column, block2.column);
    std::swap(block1.row, block2.row);

    matrix[block1.row][block1.column] = block1;
    matrix[block2.row][block2.column] = block2;
}


//MainMenu
void mainMenu(RenderWindow& app, Sprite& background, Sprite& menugems, Sprite& startb, Clock& gclock, IntRect& ch) {
    app.clear();

    if (gclock.getElapsedTime().asSeconds() > 1.5f) {
        if (ch.left == 294) {
            ch.left = 0;
        }
        else {
            ch.left += 49;
        }
        menugems.setTextureRect(ch);
        gclock.restart();
    }
    startb.setOrigin(256, 64);
    startb.setPosition(640, 320);
    menugems.setScale(2, 2);
    menugems.setOrigin(48, 48);
    menugems.setPosition(695, 230);
    app.draw(startb);
    app.draw(menugems);
    app.draw(background);
}
//NormalGame
void normal_game(RenderWindow& app, Sprite& background2, Sprite& background, Sprite& gems, Sprite& ends, int& p_score, int& click, int& ver, int& hor, Vector2i& pos, bool& isSwap, bool& moving, int& xknot, int& yknot, Clock& clock, Event& e, Font& font, bool& start, int& t_clicks, bool& normalmode) {
    int gameMoves = 20 - t_clicks;
    app.clear();
    background2.setPosition(375, 80);
    if (gameMoves > 0) {
        if (click == 1)
        {
            xknot = pos.x / ts + 1;
            yknot = pos.y / ts + 1;
        }
        if (click == 2)
        {
            t_clicks++;
            hor = pos.x / ts + 1;
            ver = pos.y / ts + 1;
            if (abs(hor - xknot) + abs(ver - yknot) == 1)
            {
                swap(matrix[yknot][xknot], matrix[ver][hor]); isSwap = 1; click = 0;
            }
            else click = 1;
        }

        //Finding a match
        int i = 1;
        while (i <= 8)
        {
            int j = 1;
            while (j <= 8)
            {
                if (matrix[i][j].var == matrix[i + 1][j].var)
                    if (matrix[i][j].var == matrix[i - 1][j].var) {
                        for (int n = -1; n <= 1; n++) { matrix[i + n][j].match++; }
                    }
                if (matrix[i][j].var == matrix[i][j + 1].var)
                    if (matrix[i][j].var == matrix[i][j - 1].var) {
                        for (int n = -1; n <= 1; n++) { matrix[i][j + n].match++; }
                    }
                j++;
            }
            i++;
        }

        //For smooth Animations
        moving = false;
        i = 1;
        while (i <= 8)
        {
            int j = 1;
            while (j <= 8)
            {
                int n = 0;
                tile& p = matrix[i][j];
                int dx, dy;
                while (n < 4)
                {
                    dx = p.xcord - p.column * ts;
                    dy = p.ycord - p.row * ts;
                    if (dx) p.xcord -= dx / abs(dx);
                    if (dy) p.ycord -= dy / abs(dy);
                    n++;
                }
                if (dx || dy) moving = 1;
                j++;
            }
            i++;
        }
        if (!moving)
        {
            int i = 1;
            while (i <= 8)
            {
                int j = 1;
                while (j <= 8)
                {
                    if (matrix[i][j].match) if (matrix[i][j].alpha > 10)
                    {
                        matrix[i][j].alpha -= 10; moving = true;
                        p_score++;
                        p_score / 100;
                    }
                    j++;
                }
                i++;
            }
        }
        
        //Getting Score
        int score = 0;
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                score += matrix[i][j].match;
            }
        }

        //If no match from score, swapping back
        if (isSwap && !moving)
        {
            if (!score) swap(matrix[yknot][xknot], matrix[ver][hor]); isSwap = 0;
        }

        //Update matrix
        if (!moving)
        {
            int i = 1;
            while (i <= 8)
            {
                int j = 8;
                while (j > 0)
                {
                    int k = 1;
                    while (k <= 8)
                    {
                        int n = k;
                        if (matrix[j][k].match)
                            while (n > 0)
                            {
                                if (!matrix[n][j].match) { swap(matrix[n][j], matrix[i][j]); break; };
                                n--;
                            }
                        int j = 1;
                        while (j <= 8)
                        {
                            int i = 8, n = 0;
                            //for (int i = 8, n = 0; i > 0; i--)
                            while (i > 0)
                            {
                                if (matrix[i][j].match)
                                {
                                    matrix[i][j].var = rand() % 7;
                                    matrix[i][j].ycord = -ts * n++;
                                    matrix[i][j].match = 0;
                                    matrix[i][j].alpha = 255;
                                }
                                i--;
                            }
                            j++;
                        }
                        k++;
                    }
                    j--;
                }
                i++;
            }
        }
        //Texts
        Text g_Moves,pb;
        string ss = to_string(gameMoves);
        g_Moves.setFont(font);
        g_Moves.setString("Moves Left: " + ss);
        g_Moves.setStyle(Text::Italic);
        string str = to_string(p_score);
        pb.setFont(font);
        pb.setString("Score: " +str);
        pb.setStyle(Text::Underlined);
        pb.setPosition(600, 0);

        app.draw(g_Moves);
        app.draw(pb);
        app.draw(background2);
        //Code For ProgressBar
        RectangleShape progressbar;
        progressbar.setFillColor(Color::Yellow);
        progressbar.setOutlineThickness(0.5);
        progressbar.setOutlineColor(Color::White);
        progressbar.setPosition(280, 50);
        progressbar.setSize(Vector2f((p_score)/10, 20));
        app.draw(progressbar);
        //Updating the Tiles
        i = 1;
        while (i <= 8)
        {
            int j = 1;
            while (j <= 8)
            {
                tile p = matrix[i][j];
                gems.setTextureRect(IntRect(p.var * 49, 0, 49, 49));
                gems.setColor(Color(255, 255, 255, p.alpha));
                gems.setPosition(p.xcord, p.ycord);
                gems.move(offset.x - ts, offset.y - ts);
                app.draw(gems);
                j++;
            }
            i++;
        }
    }
    //The condition for playing becomes false aka GameOver
    else {
        Texture t4; t4.loadFromFile("images/overb.png"); Sprite overb(t4);
        app.clear(Color::Black);
        app.draw(overb);
        app.draw(ends);
        ends.setOrigin(128, 128);
        ends.setPosition(640, 150);
        while (app.pollEvent(e)) {
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Escape)
                {
                    app.close();
                }
                else if (e.key.code == Keyboard::Enter) {
                    start = false;
                    clock.restart();
                    p_score = 0;
                    t_clicks = 0;
                    normalmode = false;
                }
            }
        }

    }

}
//Test Game
void test_game(RenderWindow& app, Sprite& background2, Sprite& background, Sprite& gems, Sprite& ends, int& p_score, int& click, int& ver, int& hor, Vector2i& pos, bool& isSwap, bool& moving, int& xknot, int& yknot, Clock& clock, Event& e, Font& font, bool& start, int& t_clicks, bool& normalmode) {
    int gameMoves = 9999;
    app.clear();
    background2.setPosition(375, 80);
    if (gameMoves > 0) {
        if (click == 1)
        {
            xknot = pos.x / ts + 1;
            yknot = pos.y / ts + 1;
        }
        if (click == 2)
        {
            t_clicks++;
            hor = pos.x / ts + 1;
            ver = pos.y / ts + 1;
            if (abs(hor - xknot) + abs(ver - yknot) == 1)
            {
                swap(matrix[yknot][xknot], matrix[ver][hor]); isSwap = 1; click = 0;
            }
            else click = 1;
        }

        //Match finding
        int i = 1;
        while (i <= 8)
        {
            int j = 1;
            while (j <= 8)
            {
                if (matrix[i][j].var == matrix[i + 1][j].var)
                    if (matrix[i][j].var == matrix[i - 1][j].var) {
                        for (int n = -1; n <= 1; n++) { matrix[i + n][j].match++; }
                    }
                if (matrix[i][j].var == matrix[i][j + 1].var)
                    if (matrix[i][j].var == matrix[i][j - 1].var) {
                        for (int n = -1; n <= 1; n++) { matrix[i][j + n].match++; }
                    }
                j++;
            }
            i++;
        }

        //Moving animation
        moving = false;
        i = 1;
        while (i <= 8)
        {
            int j = 1;
            while (j <= 8)
            {
                int n = 0;
                tile& p = matrix[i][j];
                int dx, dy;
                while (n < 4)
                {
                    dx = p.xcord - p.column * ts;
                    dy = p.ycord - p.row * ts;
                    if (dx) p.xcord -= dx / abs(dx);
                    if (dy) p.ycord -= dy / abs(dy);
                    n++;
                }
                if (dx || dy) moving = 1;
                j++;
            }
            i++;
        }
        if (!moving)
        {
            int i = 1;
            while (i <= 8)
            {
                int j = 1;
                while (j <= 8)
                {
                    if (matrix[i][j].match) if (matrix[i][j].alpha > 10)
                    {
                        matrix[i][j].alpha -= 10; moving = true;
                        p_score++;
                        p_score / 100;
                    }
                    j++;
                }
                i++;
            }
        }

        //Get score
        int score = 0;
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                score += matrix[i][j].match;
            }
        }

        //Second swap if no match
        if (isSwap && !moving)
        {
            if (!score) swap(matrix[yknot][xknot], matrix[ver][hor]); isSwap = 0;
        }

        //Update matrix
        if (!moving)
        {
            int i = 1;
            while (i <= 8)
            {
                int j = 8;
                while (j > 0)
                {
                    int k = 1;
                    while (k <= 8)
                    {
                        int n = k;
                        if (matrix[j][k].match)
                            while (n > 0)
                            {
                                if (!matrix[n][j].match) { swap(matrix[n][j], matrix[i][j]); break; };
                                n--;
                            }
                        int j = 1;
                        while (j <= 8)
                        {
                            int i = 8, n = 0;
                            //for (int i = 8, n = 0; i > 0; i--)
                            while (i > 0)
                            {
                                if (matrix[i][j].match)
                                {
                                    matrix[i][j].var = rand() % 7;
                                    matrix[i][j].ycord = -ts * n++;
                                    matrix[i][j].match = 0;
                                    matrix[i][j].alpha = 255;
                                }
                                i--;
                            }
                            j++;
                        }
                        k++;
                    }
                    j--;
                }
                i++;
            }
        }
        Text g_Moves, pb;
        string ss = to_string(gameMoves);
        g_Moves.setFont(font);
        g_Moves.setString("Moves Left: " + ss);
        g_Moves.setStyle(Text::Italic);
        string str = to_string(p_score);
        pb.setFont(font);
        pb.setString("Score: " + str);
        pb.setStyle(Text::Underlined);
        pb.setPosition(600, 0);

        app.draw(g_Moves);
        app.draw(pb);
        app.draw(background2);
        //Code For ProgressBar
        RectangleShape progressbar;
        progressbar.setFillColor(Color::Yellow);
        progressbar.setOutlineThickness(0.5);
        progressbar.setOutlineColor(Color::White);
        progressbar.setPosition(280, 50);
        progressbar.setSize(Vector2f((p_score) / 10, 20));
        app.draw(progressbar);
        //Updating the Tiles
        i = 1;
        while (i <= 8)
        {
            int j = 1;
            while (j <= 8)
            {
                tile p = matrix[i][j];
                gems.setTextureRect(IntRect(p.var * 49, 0, 49, 49));
                gems.setColor(Color(255, 255, 255, p.alpha));
                gems.setPosition(p.xcord, p.ycord);
                gems.move(offset.x - ts, offset.y - ts);
                app.draw(gems);
                j++;
            }
            i++;
        }
    }
    //The condition for playing becomes false aka GameOver
    else {
        Texture t4; t4.loadFromFile("images/overb.png"); Sprite overb(t4);
        app.clear(Color::Black);
        app.draw(overb);
        app.draw(ends);
        ends.setOrigin(128, 128);
        ends.setPosition(640, 150);
        while (app.pollEvent(e)) {
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Escape)
                {
                    app.close();
                }
                else if (e.key.code == Keyboard::Enter) {
                    start = false;
                    clock.restart();
                    p_score = 0;
                    t_clicks = 0;
                    normalmode = false;
                }
            }
        }

    }
}
//Time Game
void time_game(RenderWindow& app, Sprite& background2, Sprite& background, Sprite& gems, Sprite& ends, int& p_score, int& click, int& ver, int& hor, Vector2i& pos, bool& isSwap, bool& moving, int& xknot, int& yknot, Clock& clock, Event& e, Font& font, bool& start, bool& timemode) {
    int gameTimer = clock.getElapsedTime().asSeconds();
    app.clear();
    background2.setPosition(375, 80);
    if (gameTimer < 100) {
        if (click == 1)
        {
            xknot = pos.x / ts + 1;
            yknot = pos.y / ts + 1;
        }
        if (click == 2)
        {
            hor = pos.x / ts + 1;
            ver = pos.y / ts + 1;
            if (abs(hor - xknot) + abs(ver - yknot) == 1)
            {
                swap(matrix[yknot][xknot], matrix[ver][hor]); isSwap = 1; click = 0;
            }
            else click = 1;
        }

        //Match finding
        int i = 1;
        while (i <= 8)
        {
            int j = 1;
            while (j <= 8)
            {
                if (matrix[i][j].var == matrix[i + 1][j].var)
                    if (matrix[i][j].var == matrix[i - 1][j].var) {
                        for (int n = -1; n <= 1; n++) { matrix[i + n][j].match++; }
                    }
                if (matrix[i][j].var == matrix[i][j + 1].var)
                    if (matrix[i][j].var == matrix[i][j - 1].var) {
                        for (int n = -1; n <= 1; n++) { matrix[i][j + n].match++; }
                    }
                j++;
            }
            i++;
        }

        //Moving animation
        moving = false;
        i = 1;
        while (i <= 8)
        {
            int j = 1;
            while (j <= 8)
            {
                int n = 0;
                tile& p = matrix[i][j];
                int dx, dy;
                while (n < 4)   // 4 - speed
                {
                    dx = p.xcord - p.column * ts;
                    dy = p.ycord - p.row * ts;
                    if (dx) p.xcord -= dx / abs(dx);
                    if (dy) p.ycord -= dy / abs(dy);
                    n++;
                }
                if (dx || dy) moving = 1;
                j++;
            }
            i++;
        }
        if (!moving)
        {
            int i = 1;
            while (i <= 8)
            {
                int j = 1;
                while (j <= 8)
                {
                    if (matrix[i][j].match) if (matrix[i][j].alpha > 10)
                    {
                        matrix[i][j].alpha -= 10; moving = true;
                        p_score++;
                        p_score / 100;
                    }
                    j++;
                }
                i++;
            }
        }
        cout << p_score << " ";
        //Get score
        int score = 0;
        for (int i = 1; i <= 8; i++) {
            for (int j = 1; j <= 8; j++) {
                score += matrix[i][j].match;
            }
        }

        //Second swap if no match
        if (isSwap && !moving)
        {
            if (!score) swap(matrix[yknot][xknot], matrix[ver][hor]); isSwap = 0;
        }

        //Update matrix
        if (!moving)
        {
            int i = 1;
            while (i <= 8)
            {
                int j = 8;
                while (j > 0)
                {
                    int k = 1;
                    while (k <= 8)
                    {
                        int n = k;
                        if (matrix[j][k].match)
                            while (n > 0)
                            {
                                if (!matrix[n][j].match) { swap(matrix[n][j], matrix[i][j]); break; };
                                n--;
                            }
                        int j = 1;
                        while (j <= 8)
                        {
                            int i = 8, n = 0;
                            //for (int i = 8, n = 0; i > 0; i--)
                            while (i > 0)
                            {
                                if (matrix[i][j].match)
                                {
                                    matrix[i][j].var = rand() % 7;
                                    matrix[i][j].ycord = -ts * n++;
                                    matrix[i][j].match = 0;
                                    matrix[i][j].alpha = 255;
                                }
                                i--;
                            }
                            j++;
                        }
                        k++;
                    }
                    j--;
                }
                i++;
            }
        }
        Text g_timer,pb;
        string ss = to_string(gameTimer);
        g_timer.setFont(font);
        g_timer.setString("Time: " + ss);
        g_timer.setStyle(Text::Italic);
        string str = to_string(p_score);
        pb.setFont(font);
        pb.setString("Score: " + str);
        pb.setStyle(Text::Underlined);
        pb.setPosition(600, 0);

        app.draw(g_timer);
        app.draw(pb);
        app.draw(background2);

        RectangleShape progressbar;
        progressbar.setFillColor(Color::Yellow);
        progressbar.setOutlineThickness(0.5);
        progressbar.setOutlineColor(Color::White);
        progressbar.setPosition(200, 50);
        progressbar.setSize(Vector2f((p_score * 10) / 50, 20));
        app.draw(progressbar);

        i = 1;
        while (i <= 8)
        {
            int j = 1;
            while (j <= 8)
            {
                tile p = matrix[i][j];
                gems.setTextureRect(IntRect(p.var * 49, 0, 49, 49));
                gems.setColor(Color(255, 255, 255, p.alpha));
                gems.setPosition(p.xcord, p.ycord);
                gems.move(offset.x - ts, offset.y - ts);
                app.draw(gems);
                j++;
            }
            i++;
        }
    }
    else {
        Texture t4; t4.loadFromFile("images/overb.png"); Sprite overb(t4);
        app.clear(Color::Black);
        app.draw(overb);
        app.draw(ends);
        ends.setOrigin(128, 128);
        ends.setPosition(640, 150);
        while (app.pollEvent(e)) {
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Escape)
                {
                    app.close();
                }
                else if (e.key.code == Keyboard::Enter) {
                    start = false;
                    clock.restart();
                    p_score = 0;
                    timemode = false;
                }
            }
        }

    }
}
//Menu Selection
void menuSelection(RenderWindow& app, Event e, bool& timemodes, bool& normalmodes, bool& testmodes,bool& isSwap, bool& moving,int& click) {
    app.clear();
    Texture t1, t2, t3;

    RectangleShape r1, r2, r3;
    r1.setFillColor(Color::Green);
    r1.setOutlineThickness(2);
    r1.setOutlineColor(Color::White);
    r1.setSize(Vector2f(512, 128));
    r1.setOrigin(256, 64);
    r1.setPosition(640, 430);
    r2.setFillColor(Color::Red);
    r2.setOutlineThickness(2);
    r2.setOutlineColor(Color::White);
    r2.setSize(Vector2f(512, 128));
    r2.setOrigin(256, 64);
    r2.setPosition(640, 300);
    r3.setFillColor(Color::Blue);
    r3.setOutlineThickness(2);
    r3.setOutlineColor(Color::White);
    r3.setSize(Vector2f(512, 128));
    r3.setOrigin(256, 64);
    r3.setPosition(640, 560);

    app.draw(r1);
    app.draw(r2);
    app.draw(r3);

    t1.loadFromFile("images/timemode.png");
    t2.loadFromFile("images/normalmode.png");
    t3.loadFromFile("images/testmode.png");
    Sprite timemode(t1), normalmode(t2), testmode(t3);

    normalmode.setOrigin(256, 64);
    normalmode.setPosition(640, 300);
    timemode.setOrigin(256, 64);
    timemode.setPosition(640, 430);
    testmode.setOrigin(256, 64);
    testmode.setPosition(640, 560);

    while (app.pollEvent(e))
    {
        if (e.type == Event::Closed)
            app.close();

            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::A) {
                    normalmodes = true;
                    testmodes = false;
                    timemodes = false;
                }
                else if (e.key.code == Keyboard::S) {
                    timemodes = true;
                    testmodes = false;
                    normalmodes = false;
                }
                else if (e.key.code == Keyboard::D) {
                    timemodes = false;
                    testmodes = true;
                    normalmodes = false;
                }
            }
    }

    //Drawing

    app.draw(timemode);
    app.draw(normalmode);
    app.draw(testmode);
}

int main()
{
    srand(time(0));


    RenderWindow app(VideoMode(1280, 640), "123");
    app.setFramerateLimit(60);
    //Music music;
    //music.openFromFile("images/doom.ogg");
    //music.play();
    //music.setVolume(50);



    Texture t1, t2, t3, t4, t5;
    t1.loadFromFile("images/background.png");
    t2.loadFromFile("images/qts.png");
    t3.loadFromFile("images/startb.png");
    t4.loadFromFile("images/ends.png");
    t5.loadFromFile("images/matrix.jpeg");

    IntRect ch(0, 0, 49, 49);


    Sprite background(t1), gems(t2), startb(t3), ends(t4), background2(t5), menugems(t2);
    menugems.setTextureRect(ch);
    int i = 1;

    while (i <= 8)
    {
        int j = 1;
        while (j <= 8)
        {
            matrix[i][j].var = rand() % 7;
            matrix[i][j].column = j;
            matrix[i][j].row = i;
            matrix[i][j].xcord = j * ts * 2;
            matrix[i][j].ycord = i * ts * 2;
            j++;
        }
        i++;
    }
    int xknot, yknot, hor, ver, click = 0; Vector2i pos;

    bool isSwap = false, moving = false;

    int p_score = 0, t_clicks = 0;


    Font font;
    font.loadFromFile("Fonts/YagiUhfNo2.ttf");

    //Text scores, g_timer;
    //scores.setFont(font);
    //scores.setString("Score: ");
    //scores.setStyle(Text::Italic);



    bool start = false, timemode = false, normalmode = false, testmode = false;

    Clock gclock, clock;



    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();

            if (e.type == Event::MouseButtonPressed) {
                Vector2i mousePos = sf::Mouse::getPosition(app);
                Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                if (e.key.code == Mouse::Left)
                {
                    if (!isSwap && !moving)
                        click++;
                    pos = Mouse::getPosition(app) - offset;
                    if (startb.getGlobalBounds().contains(mousePosF))
                        start = true;
                }
            }
        }
        if (!start) {
            mainMenu(app, background, menugems, startb, gclock, ch);
        }
        if (start) {
            if (!normalmode && !testmode && !timemode)
                menuSelection(app, e, timemode, normalmode, testmode,isSwap,moving,click);
            if (normalmode)
                normal_game(app, background2, background, gems, ends, p_score, click, ver, hor, pos, isSwap, moving, xknot, yknot, clock, e, font, start, t_clicks, normalmode);
            else if (timemode)
                time_game(app, background2, background, gems, ends, p_score, click, ver, hor, pos, isSwap, moving, xknot, yknot, clock, e, font, start, timemode);
            else if (testmode) {
                p_score = 1000;
                test_game(app, background2, background, gems, ends, p_score, click, ver, hor, pos, isSwap, moving, xknot, yknot, clock, e, font, start, t_clicks, normalmode);
            }
        }
        app.display();

    }

    return 0;
}
