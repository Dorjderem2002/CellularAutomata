#include "App.hpp"

App::App()
{

}

void App::start(int w,int h,const char* title)
{
    renderWindow = new sf::RenderWindow(sf::VideoMode(w,h),title);
    windowWidth = w; windowHeight = h;

    renderWindow->setFramerateLimit(60);
    init();
    running = true;
    while (running && renderWindow->isOpen())
    {
        sf::Event sfEvent;
        handleEvent(sfEvent);
        update();
        render();
    }
    
}

void App::init()
{
    arrLen = windowWidth * windowHeight / pixelSize / pixelSize;
    currentState = new int[arrLen];
    outputState = new int[arrLen];

    memset(outputState,0,sizeof(int) * arrLen);
    memset(currentState,0,sizeof(int) * arrLen);

    srand(time(NULL));
    for(int i = 0;i < 5000; i++)
    {
        int x = rand() % (windowWidth / 4);
        int y = rand() % (windowHeight / 4);

        currentState[y * windowWidth / 4 + x] = 1;
    }
    if(stateRenderVertex)
    {
        sPixels.resize(arrLen);
    }
    else
    {
        rects.resize(arrLen);
    }
    
    
    for(int i = 0;i < arrLen; i++)
    {
        int x = i % (windowWidth / pixelSize);
        int y = i / (windowWidth / pixelSize);
        if(stateRenderVertex)
        {
            sPixels[i] = sf::VertexArray(sf::Quads, pixelSize);
            sPixels[i][0].position = sf::Vector2f(x * pixelSize, y * pixelSize);
            sPixels[i][1].position = sf::Vector2f(x * pixelSize + pixelSize, y * pixelSize);
            sPixels[i][2].position = sf::Vector2f(x * pixelSize + pixelSize, y * pixelSize + pixelSize);
            sPixels[i][3].position = sf::Vector2f(x * pixelSize, pixelSize);
            sPixels[i][0].color = sf::Color::Black;
            sPixels[i][1].color = sf::Color::Black;
            sPixels[i][2].color = sf::Color::Black;
            sPixels[i][3].color = sf::Color::Black;
        }
        else
        {
            rects[i] = sf::RectangleShape(sf::Vector2f(pixelSize,pixelSize));
            rects[i].setPosition(x * pixelSize, y * pixelSize);
            rects[i].setFillColor(sf::Color::Black);
        }
        
    }

    /* for(int i = arrLen / 3;i < arrLen / 2; i++)
    {
        currentState[i] = 1;
    } */
}

void App::update()
{
    for(int i = 0; i < arrLen; i++)
    {
        int numberOfNeighbors = getNeighbor(i);
        if (currentState[i] == 1 && (numberOfNeighbors < 2 || numberOfNeighbors > 3))
        {
            outputState[i] = 0;
        }
        else if(currentState[i] == 0 && numberOfNeighbors == 3)
        {
            outputState[i] = 1;
        }
    }

    for(int i = 0; i < arrLen; i++)
    {
        if(outputState[i] == 1)
        {
            if(stateRenderVertex)
            {
                for(int j = 0; j < 4; j++)
                {
                    sPixels[i][j].color = sf::Color::White;
                }
            }
            else
            {
                rects[i].setFillColor(sf::Color::White);
            }
            
        }
        else
        {
            if(stateRenderVertex)
            {
                for(int j = 0; j < 4; j++)
                {
                    sPixels[i][j].color = sf::Color::Black;
                }
            }
            else
            {
                rects[i].setFillColor(sf::Color::Black);
            }
        }
        currentState[i] = outputState[i];
    } 
}

void App::render()
{
    renderWindow->clear();

    if(stateRenderVertex)
    {
        for(auto i : sPixels)
        {
            renderWindow->draw(i);
        }  
    }
    else
    {
        for(auto i : rects)
        {
            renderWindow->draw(i);
        }
    }
    
      

    renderWindow->display();
}

void App::handleEvent(sf::Event ev)
{
    while(renderWindow->pollEvent(ev))
    {
        if(ev.type == sf::Event::Closed)
        {
            renderWindow->close();
            running = false;
        }
    }
}

int App::getNeighbor(int index)
{
    int res = 0;
    int x = index % (windowWidth / pixelSize);
    int y = index / (windowWidth / pixelSize);
    for(int i = y - 1; i <= y + 1; i++)
    {
        for(int j = x - 1; j <= x + 1; j++)
        {
            if(i >= 0 && i < windowHeight / pixelSize && j >= 0 && j < windowWidth / pixelSize)
            {
                res += currentState[i * (windowWidth / pixelSize) + j];
            }
            
        }
    }
    return res - currentState[index];
}

App::~App()
{
    delete currentState;
    delete outputState;
    delete renderWindow;
}