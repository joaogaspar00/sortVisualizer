/* Includes gerais */

#include <iostream>

/* Includes para utilização de gráficos, sons, etc. */

#include <SFML/Graphics.hpp>

/* Definições gerais da aplicação */

#define VECTOR_SIZE 1000

#define WINDOW_HEIGHT 1450
#define WINDOW_WIDTH 2500
#define WINDOW_PROPORTION_MENU 0.25
#define WINDOW_TITLE "SORTVISUALIZER_1.0"

#define BAR_SPACE 0

#define NONE_SORT -1
#define SELECTION_SORT 0
#define QUICK_SORT 1
#define BUBBLE_SORT 2
#define INSERTION_SORT 3

#define ORGINE_TRADE 0
#define NEW_TRADE 1

#define TIME 1

#define SHOW_RESULTS true
#define HIDE_RESULTS false

/* EStutura com os dados */ 

typedef struct dataType{
    float height;
} DATA;

/* Funções */

void renderGUIWindow(sf::RenderWindow& window, int sortAlgorithm, bool results);
void drawGrid(sf::RenderWindow& window);
void drawTitle(sf::RenderWindow& window);
bool loadTextFont();
void drawMenu(sf::RenderWindow& window);
void arrayWindowDraw(sf::RenderWindow& window, DATA vectorData[VECTOR_SIZE], int selected, int actual);
void vectorInitialization(DATA vectorData[VECTOR_SIZE]);
sf::Vector2f barPostionGetter(int i, float barHeight, float barWidth);
float barGetWidth();
void selectionSortRun(sf::RenderWindow& window, DATA vectorData[VECTOR_SIZE]);
int findSmallerNumber(DATA vectorData[VECTOR_SIZE], int actual);
void quickSortRun(sf::RenderWindow& window, DATA vectorData[VECTOR_SIZE], int first, int last);
int partition (sf::RenderWindow& window, DATA vectorData[VECTOR_SIZE], int first, int last);
void swap(sf::RenderWindow& window, DATA vectorData[VECTOR_SIZE], int x, int y, int sortAlgorithm);
void drawSortSelected(sf::RenderWindow& window, int sortSelected);
void bubbleSortRun(sf::RenderWindow& window, DATA vectorData[VECTOR_SIZE]);
void insertionSortRun(sf::RenderWindow& window, DATA vectorData[VECTOR_SIZE]);
void renderResults(sf::RenderWindow& window, bool results);


/* Global variables */

sf::Font textFont;
sf::Clock timeCounter; // starts the clock
sf::Time timeDurantion;
float timeFixed = 0.f;
int swapNumber = 0;

int main()
{
    
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    DATA vectorData[VECTOR_SIZE]; 

    bool sortRun = false;
    int sortAlgorithm = -1;

    if (!loadTextFont()){window.close();exit(0);}  

    vectorInitialization(vectorData);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
            }
                
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                case sf::Keyboard::Num0:
                    sortAlgorithm = SELECTION_SORT;
                    break;
                case sf::Keyboard::Num1:
                    sortAlgorithm = QUICK_SORT;
                    break;
                case sf::Keyboard::Num2:
                    sortAlgorithm = BUBBLE_SORT;
                    break;
                case sf::Keyboard::Num3:
                    sortAlgorithm = INSERTION_SORT;
                    break;
                case sf::Keyboard::Space:
                    if(sortAlgorithm != -1){
                        sortRun = true;
                    }
                    break;
                case sf::Keyboard::R:
                    sortRun = false;
                    sortAlgorithm = NONE_SORT;
                    timeFixed = 0.f;
                    swapNumber = 0;
                    vectorInitialization(vectorData);
                    break;
                default:
                    break;
                }
            }
        }

        if(sortRun){
            
            if(sortAlgorithm == SELECTION_SORT){
                timeCounter.restart();
                selectionSortRun(window, vectorData);
            }
            else if(sortAlgorithm == QUICK_SORT){
                timeCounter.restart();
                quickSortRun(window, vectorData, 0, VECTOR_SIZE-1);;
            }
            else if(sortAlgorithm == BUBBLE_SORT){
                timeCounter.restart();
                bubbleSortRun(window, vectorData);
            }
            else if(sortAlgorithm == INSERTION_SORT){
                timeCounter.restart();
                insertionSortRun(window, vectorData);
            }
            timeDurantion = timeCounter.getElapsedTime();
            timeFixed = timeDurantion.asSeconds();
            sortRun = false;            
        }
        else{
            window.clear(sf::Color::Black);
            renderGUIWindow(window, sortAlgorithm, HIDE_RESULTS);
            arrayWindowDraw(window, vectorData, -1, -1);
            window.display();
            sf::sleep(sf::milliseconds(TIME));
        }   
    }

    return 0;
}

void swap(sf::RenderWindow& window, DATA vectorData[VECTOR_SIZE], int x, int y, int sortAlgorithm){
    float aux=vectorData[x].height;
    vectorData[x].height = vectorData[y].height;
    vectorData[y].height = aux;

    window.clear(sf::Color::Black);
    renderGUIWindow(window, sortAlgorithm, SHOW_RESULTS);
    arrayWindowDraw(window, vectorData, x, y);
    window.display();
    swapNumber++;

    sf::sleep(sf::milliseconds(TIME));
}

/* Funções para o INSERTION SORT */

void insertionSortRun(sf::RenderWindow& window, DATA vectorData[VECTOR_SIZE]){
    int i, key, j;
    for (i = 1; i < VECTOR_SIZE; i++)
    {
        key = vectorData[i].height;
        j = i - 1;
        while (j >= 0 && vectorData[j].height > key)
        {
            vectorData[j + 1].height = vectorData[j].height;
            j = j - 1;
        }
        vectorData[j + 1].height = key;
        swapNumber++;
        window.clear(sf::Color::Black);
        renderGUIWindow(window, INSERTION_SORT, SHOW_RESULTS);
        arrayWindowDraw(window, vectorData, i, j+1);
        window.display();

        sf::sleep(sf::milliseconds(TIME));
    }
}

/* Funções para o BUBBLE SORT */


void bubbleSortRun(sf::RenderWindow& window, DATA vectorData[VECTOR_SIZE]) 
{ 
    int i, j; 
    for (i = 0; i < VECTOR_SIZE-1; i++)     
        for (j = 0; j < VECTOR_SIZE-i-1; j++) 
            if (vectorData[j].height > vectorData[j+1].height)
                swap(window, vectorData, j, j+1, BUBBLE_SORT);
            
} 

/* Funções para o QUICK SORT */

int partition (sf::RenderWindow& window, DATA vectorData[VECTOR_SIZE], int first, int last){
    int pivot = vectorData[last].height;    // pivot
    int i = (first - 1);  // Index of smaller element
 
    for (int j = first; j <= last- 1; j++){
        if (vectorData[j].height <= pivot)
        {
            i++;    
            swap(window, vectorData, i, j, QUICK_SORT);                  
        }
    }
    swap(window, vectorData, i+1, last, QUICK_SORT);

    return (i + 1);
}

void quickSortRun(sf::RenderWindow& window, DATA vectorData[VECTOR_SIZE], int first, int last){
    if (first < last){
        int index = partition(window, vectorData, first, last);
        quickSortRun(window, vectorData, first, index - 1);
        quickSortRun(window, vectorData, index + 1, last);
    }
}

/* Funções para o SELECTION SORT */

void selectionSortRun(sf::RenderWindow& window, DATA vectorData[VECTOR_SIZE]){

    int actual = 0;
    int selected = 0;

    while(actual != (VECTOR_SIZE-1)){

        selected = findSmallerNumber(vectorData, actual);

        swap(window, vectorData, actual, selected, SELECTION_SORT);

        actual++;
    }
}

int findSmallerNumber(DATA vectorData[VECTOR_SIZE], int actual){
    int small = actual;
    int smallAux = vectorData[actual].height;

    for(int i = actual + 1; i < VECTOR_SIZE; i++){
        if(smallAux > vectorData[i].height){
            small = i;
            smallAux = vectorData[i].height;
        }
    }
    return small;
}

/***********************************************/

void arrayWindowDraw(sf::RenderWindow& window, DATA vectorData[VECTOR_SIZE], int selected, int actual){

    float barWidth = barGetWidth();

    for(int i = 0; i < VECTOR_SIZE; i++){
        sf::RectangleShape rectangle(sf::Vector2f(barWidth, vectorData[i].height));
        rectangle.setPosition(barPostionGetter(i, vectorData[i].height, barWidth));
        if(i == selected){
            rectangle.setFillColor(sf::Color::Magenta);
        }
        else if(i == actual){
            rectangle.setFillColor(sf::Color::Green);
        }
        else{
            rectangle.setFillColor(sf::Color::White);
        }
        window.draw(rectangle);
    }
}

float barGetWidth(){
    return (WINDOW_WIDTH*(1-WINDOW_PROPORTION_MENU)-(VECTOR_SIZE*BAR_SPACE))/VECTOR_SIZE;
}

sf::Vector2f barPostionGetter(int i, float barHeight, float barWidth){
    float x, y;
    x = i * (barWidth+BAR_SPACE);
    y = WINDOW_HEIGHT-barHeight;
    return sf::Vector2f(x, y); 
}


void vectorInitialization(DATA vectorData[VECTOR_SIZE]){
    for(int i = 0; i < VECTOR_SIZE; i++){
        vectorData[i].height = (float) rand()/RAND_MAX*WINDOW_HEIGHT;
    }
}


/*********************************************************************/

void renderGUIWindow(sf::RenderWindow& window, int sortAlgorithm, bool results){

    drawGrid(window);
    drawTitle(window);
    drawSortSelected(window, sortAlgorithm);
    drawMenu(window);
    renderResults(window, results);

    
}

void renderResults(sf::RenderWindow& window, bool results){
    sf::Text text;
    text.setFont(textFont);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    text.setPosition(WINDOW_WIDTH-WINDOW_WIDTH*WINDOW_PROPORTION_MENU +130 , 1000);
     
    if(results){      
        timeDurantion = timeCounter.getElapsedTime();
        text.setString("Time duration: " + std::to_string(timeDurantion.asSeconds())+ " seconds");
    }
    else{
        text.setString("Time duration: " + std::to_string(timeFixed) + " seconds");
    }
    
    window.draw(text);

    text.setPosition(WINDOW_WIDTH-WINDOW_WIDTH*WINDOW_PROPORTION_MENU +130 , 1060);
    text.setString("Swaps: " + std::to_string(swapNumber));
    window.draw(text);

}

void drawSortSelected(sf::RenderWindow& window, int sortSelected){
    sf::Text text;
    text.setFont(textFont);
    text.setCharacterSize(60);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);

    text.setPosition(WINDOW_WIDTH-WINDOW_WIDTH*WINDOW_PROPORTION_MENU +130 , 820);
    text.setString("Selected sort:");
    window.draw(text);

    text.setCharacterSize(45);
    text.setPosition(WINDOW_WIDTH-WINDOW_WIDTH*WINDOW_PROPORTION_MENU +130 , 900);
    if(sortSelected == NONE_SORT){
        text.setString("Choose one sort");
    }
    else if(sortSelected == SELECTION_SORT){
         text.setString("SELECTION SORT");
    }
    else if(sortSelected == QUICK_SORT){
        text.setString("QUICK SORT");
    }
    else if(sortSelected == BUBBLE_SORT){
        text.setString("BUBBLE SORT");
    }
    else if(sortSelected == INSERTION_SORT){
        text.setString("INSERTION SORT");
    }
    window.draw(text);
}

void drawMenu(sf::RenderWindow& window){
    sf::Text text;
    text.setFont(textFont);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);

    text.setPosition(WINDOW_WIDTH-WINDOW_WIDTH*WINDOW_PROPORTION_MENU +130 , 150);
    text.setString("Selection Sort - 0");
    window.draw(text);

    text.setPosition(WINDOW_WIDTH-WINDOW_WIDTH*WINDOW_PROPORTION_MENU +130 , 210);
    text.setString("Quick Sort - 1");
    window.draw(text);

    text.setPosition(WINDOW_WIDTH-WINDOW_WIDTH*WINDOW_PROPORTION_MENU +130 , 270);
    text.setString("Bubble Sort - 2");
    window.draw(text);

    text.setPosition(WINDOW_WIDTH-WINDOW_WIDTH*WINDOW_PROPORTION_MENU +130 , 330);
    text.setString("Insertion Sort - 3");
    window.draw(text);

    text.setPosition(WINDOW_WIDTH-WINDOW_WIDTH*WINDOW_PROPORTION_MENU +130 , 450);
    text.setString("Reset - R");
    window.draw(text);

    text.setPosition(WINDOW_WIDTH-WINDOW_WIDTH*WINDOW_PROPORTION_MENU +130 , 510);
    text.setString("Start - Space");
    window.draw(text);

    text.setPosition(WINDOW_WIDTH-WINDOW_WIDTH*WINDOW_PROPORTION_MENU +130 , 570);
    text.setString("Sair - ESC");
    window.draw(text);
    
}

void drawTitle(sf::RenderWindow& window){
    sf::Text text;
    text.setFont(textFont);
    text.setString("Sort Visualizer");
    text.setCharacterSize(60);
    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold);
    text.setPosition(WINDOW_WIDTH-WINDOW_WIDTH*WINDOW_PROPORTION_MENU +130 , 5);
    window.draw(text);
}

void drawGrid(sf::RenderWindow& window){
    sf::RectangleShape sideMenu(sf::Vector2f(WINDOW_WIDTH*WINDOW_PROPORTION_MENU,WINDOW_HEIGHT));
    sideMenu.setPosition(WINDOW_WIDTH-WINDOW_WIDTH*WINDOW_PROPORTION_MENU, 0);
    sideMenu.setFillColor(sf::Color::Cyan);
    window.draw(sideMenu);
}

bool loadTextFont(){
    return textFont.loadFromFile("font/RobotoCondensed-Bold.ttf");
}