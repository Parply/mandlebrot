#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Window/Event.hpp>
#include <cstdio>
#include <sys/types.h>
#include <math.h>
#include <iostream>

const unsigned W=1920,H=1080,Size= W*H*4,max_it=1000;

const uint8_t lights[360]={
  0,   0,   0,   0,   0,   1,   1,   2, 
  2,   3,   4,   5,   6,   7,   8,   9, 
 11,  12,  13,  15,  17,  18,  20,  22, 
 24,  26,  28,  30,  32,  35,  37,  39, 
 42,  44,  47,  49,  52,  55,  58,  60, 
 63,  66,  69,  72,  75,  78,  81,  85, 
 88,  91,  94,  97, 101, 104, 107, 111, 
114, 117, 121, 124, 127, 131, 134, 137, 
141, 144, 147, 150, 154, 157, 160, 163, 
167, 170, 173, 176, 179, 182, 185, 188, 
191, 194, 197, 200, 202, 205, 208, 210, 
213, 215, 217, 220, 222, 224, 226, 229, 
231, 232, 234, 236, 238, 239, 241, 242, 
244, 245, 246, 248, 249, 250, 251, 251, 
252, 253, 253, 254, 254, 255, 255, 255, 
255, 255, 255, 255, 254, 254, 253, 253, 
252, 251, 251, 250, 249, 248, 246, 245, 
244, 242, 241, 239, 238, 236, 234, 232, 
231, 229, 226, 224, 222, 220, 217, 215, 
213, 210, 208, 205, 202, 200, 197, 194, 
191, 188, 185, 182, 179, 176, 173, 170, 
167, 163, 160, 157, 154, 150, 147, 144, 
141, 137, 134, 131, 127, 124, 121, 117, 
114, 111, 107, 104, 101,  97,  94,  91, 
 88,  85,  81,  78,  75,  72,  69,  66, 
 63,  60,  58,  55,  52,  49,  47,  44, 
 42,  39,  37,  35,  32,  30,  28,  26, 
 24,  22,  20,  18,  17,  15,  13,  12, 
 11,   9,   8,   7,   6,   5,   4,   3, 
  2,   2,   1,   1,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0};


void mandlebrot (sf::Texture & texture,sf::Uint8 pixels[])
{
	#pragma omp parallel shared(pixels)
	{
	double x0,y0,x,y,xtemp;
	unsigned it,ang;
	#pragma omp for schedule(dynamic)
	for (unsigned Px=0;Px<W;Px++)
		for (unsigned Py=0; Py<H;Py++)
		{
			x0= (3.5*((double) Px )/W) -2.5;
			y0=(2*((double) Py)/H) -1;
			x=0.0;
			y=0.0;
			it=0;
			while (it<max_it && x*x+y*y<=4)
			{
				xtemp = x*x-y*y+x0;
				y = 2*x*y+y0;
				x=xtemp;
				it++;
			}
			ang=(int) 360*sqrt((double)it/(double)max_it);
			pixels[4*(Px+Py*W)] = lights[(ang+120)%360];
			pixels[4*(Px+Py*W)+1] =lights[ang];
			pixels[4*(Px+Py*W)+2] = lights[(ang+240)%360];
;		
			if (it==max_it)
				pixels[4*(Px+Py*W)+3] = 0;
			else
				pixels[4*(Px+Py*W)+3] = 127;



		}
	}

	texture.update(pixels);
}


int main()
{
	double UpperX=2.5,LowerX=-1.0,LowerY=-1.0,UpperY=1.0;
	unsigned mousex,mousey;
	sf::RenderWindow window(sf::VideoMode(W, H), "Mandlebrot!");
	window.setFramerateLimit(60);
	sf::Uint8* pixels = new sf::Uint8[Size];

    	sf::Texture texture;
	texture.create(W, H); 

	sf::Sprite sprite(texture);

	mandlebrot(texture, pixels);
	while (window.isOpen())
    {
        	sf::Event event;
        	while (window.pollEvent(event))
        	{
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::MouseWheelScrolled:
					if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
        std::cout << "wheel type: vertical" << std::endl;
    else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
        std::cout << "wheel type: horizontal" << std::endl;
    else
        std::cout << "wheel type: unknown" << std::endl;
    std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
    std::cout << "mouse x: " << event.mouseWheelScroll.x << std::endl;
    std::cout << "mouse y: " << event.mouseWheelScroll.y << std::endl;
    break;
				default:
					break;

			}
        	}

        	window.clear();
        	window.draw(sprite);
        	window.display();
    	}

    	return 0;
}
