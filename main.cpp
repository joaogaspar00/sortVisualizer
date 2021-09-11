/******************************************************************
Este projeto foi feito com o objetivo de aprender a mexer com a 
livraria gráfica do C++ chamada SFML.

Em caso de algum erro ou melhoria contactar:
jpsg.joao.gaspar@gmail.com  

This project was made with purpose of learn how to use c++
graphical library SFML

if you find any mistake or improement contact:
jpsg.joao.gaspar@gmail.com
*******************************************************************/

/* Includes gerais */

#include <iostream>

/* Includes para utilização de gráficos, sons, etc. */

#include <SFML/Graphics.hpp>

/* Definições gerais da aplicação */

#define VECTOR_SIZE 100

#define WINDOW_HEIGHT 1450
#define WINDOW_WIDTH 2500
#define WINDOW_PROPORTION_MENU 0.25
#define WINDOW_TITLE "SORTVISUALIZER_1.0"

#define BAR_SPACE 2

#define NONE_SORT -1
#define SELECTION_SORT 0
#define QUICK_SORT 1
#define BUBBLE_SORT 2
#define INSERTION_SORT 3

#define ORGINE_TRADE 0
#define NEW_TRADE 1

#define TIME 75

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
bool isUnordered(DATA vectorData[VECTOR_SIZE]);


/* Variáveis globais */

sf::Font textFont;          // Fonte para o texto
sf::Clock timeCounter;      // starts the clock
sf::Time timeDurantion;     // Variável que guarda o tempo decorrido
float timeFixed = 0.f;      // Tempo decorrido convertido em float
int swapNumber = 0;         // Número de trocas feitas pelo algoritmo

/* Função main */

int main()
{
    /* Janela da aplicação */
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Close | sf::Style::Titlebar);
    /* Limitação da atualização da janela */
    window.setFramerateLimit(60);

    /* Vetor com os dados a organizar*/
    DATA vectorData[VECTOR_SIZE];

    /* Variável que guarda os eventos ocorridos na janela */
    sf::Event event; 

    /* Variaável que guarda a informação se o algoritmo está a correr */
    bool sortRun = false;

    /* Qual o algoritmo a correr ou que terá de ser corrido */
    int sortAlgorithm = -1;

    /* Carrega a fonte para o texto - cajo haja erro a aplicação fecha */
    if (!loadTextFont()){window.close();exit(0);}  

    /* Inicialização do vetor com os dados */
    vectorInitialization(vectorData);

    /* Enquando a janela estiver aberta correr o programa */
    while (window.isOpen())
    {
        /* Detecta os eventos da janela */
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){               /* Evento de fecho da janela */
                window.close();
            }
                
            if (event.type == sf::Event::KeyPressed)            /* Evento de tecla premida */
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:                  /* Tecala premida escape */
                        window.close();                         /* fecha a janela */
                        break;
                    case sf::Keyboard::Num0:                    /* Tecala premida 0 */
                        sortAlgorithm = SELECTION_SORT;         /* define o selection sort */
                        break;
                    case sf::Keyboard::Num1:                    /* Tecala premida 1*/
                        sortAlgorithm = QUICK_SORT;             /* define o quick sort */
                        break;
                    case sf::Keyboard::Num2:                    /* Tecala premida 2 */
                        sortAlgorithm = BUBBLE_SORT;            /* define o bubble sort */
                        break;
                    case sf::Keyboard::Num3:                    /* Tecala premida 3 */
                        sortAlgorithm = INSERTION_SORT;         /* defin o insertion sort */
                        break;
                    case sf::Keyboard::Space:                   /* Tecala premida espaço */
                        if(sortAlgorithm != -1){                /* caso haja algoritmo definido */
                            sortRun = true;                     /* inicia o algotitmo */
                        }
                        break;
                    case sf::Keyboard::R:                       /* Tecala premida R */
                        sortRun = false;                        /* Para o algoritmo*/
                        sortAlgorithm = NONE_SORT;              /* Limpa o tipo de algoritmo */
                        timeFixed = 0.f;                        /* reset o tempo decorrido */
                        swapNumber = 0;                         /* reset o número de trocas */
                        vectorInitialization(vectorData);       /* insere novos dados no array */
                        break;
                    default:
                        break;
                }
            }
        }

        if(sortRun){                                                            /* se verdadeiro prepara-se para correr o algoritmo */
            if(!isUnordered(vectorData)){                                       /* verifica se o array ja está ordenado */
                sortRun = false;                                                /* em caso afirmativo para de correr o algoritmo */
                continue;
            }else{                                                              /* caso não esteja ordenado */
                switch (sortAlgorithm)                                          /* correr o algoritmo escolhido*/
                {                                                               /* em todos os caso reinicia o relógio */
                    case SELECTION_SORT:
                        timeCounter.restart();
                        selectionSortRun(window, vectorData);
                        break;
                    case QUICK_SORT:
                        quickSortRun(window, vectorData, 0, VECTOR_SIZE-1);
                        break;
                    case BUBBLE_SORT:
                        timeCounter.restart();
                        bubbleSortRun(window, vectorData);
                        break;
                    case INSERTION_SORT:
                        timeCounter.restart();
                        insertionSortRun(window, vectorData);
                        break;
                    default:
                        break;
                }
                timeDurantion = timeCounter.getElapsedTime();                   /* ao fim de correr o algoritmo le o tempo */
                timeFixed = timeDurantion.asSeconds();                          /* converte o tempo para um float e guarda-o */
                sortRun = false;                                                /* para o algoritmo */
            } 
                       
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

bool isUnordered(DATA vectorData[VECTOR_SIZE]){
    for(int i=0; i<VECTOR_SIZE-1; i++){
        if(vectorData[i].height > vectorData[i+1].height){
            return true;
        }
    }
    return false;
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
