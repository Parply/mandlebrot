#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdio>
#include <sys/types.h>
#include <math.h>
#include <iostream>

#define ZOOMRATE 2.0

inline double maxd(double a, double b) { return a>b ? a:b; }
inline double mind(double a, double b) { return a<b ? a:b; }


const unsigned W=1920,H=1080,Size= W*H*4,max_it=1000;
const double cUpperX=1.0,cLowerX=-2.5,cLowerY=-1.0,cUpperY=1.0;
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


void mandlebrot (sf::Texture & texture,sf::Uint8 pixels[],
		double UpperX,double LowerX,double UpperY,double LowerY)
{
	#pragma omp parallel shared(pixels)
	{
	double x0,y0,x,y,xtemp;
	unsigned it,ang;
	#pragma omp for schedule(dynamic)
	for (unsigned Px=0;Px<W;Px++)
		for (unsigned Py=0; Py<H;Py++)
		{
			x0= ((UpperX-LowerX)*((double) Px )/W) + LowerX;
			y0=((UpperY-LowerY)*((double) Py)/H) +LowerY;
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


void zoom(int mousedelta,unsigned mousex,unsigned mousey,
		double & UpperX,double & LowerX,double & UpperY,double & LowerY)
{
	double midX = (UpperX-LowerX)*((double)mousex/(double) W),midY=(UpperY-LowerY)*((double) mousey/(double) H);
	double xInt,yInt,temp;
	if (mousedelta==1)//zoom
	{
		temp = 1.0/(ZOOMRATE*2.0);
	} else if (mousedelta==-1)//unzoom
	{
		temp = ZOOMRATE*2.0;
	}
	xInt = (UpperX -LowerX)*temp;
	yInt = (UpperY-LowerY)*temp;
	UpperX = mind(midX+xInt, cUpperX);
	LowerX = maxd(midX-xInt, cLowerX);
	UpperY = mind(midY+yInt,cUpperY);
	LowerY = maxd(midY-yInt,cLowerY);
}



int main()
{
	double UpperX=1.0,LowerX=-2.5,LowerY=-1.0,UpperY=1.0;
	
	sf::RenderWindow window(sf::VideoMode(W, H), "Mandlebrot!");
	window.setFramerateLimit(60);
	sf::Uint8* pixels = new sf::Uint8[Size];

    	sf::Texture texture;
	texture.create(W, H); 

	sf::Sprite sprite(texture);

	mandlebrot(texture, pixels,UpperX,LowerX,UpperY,LowerY);
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
					{
						zoom(event.mouseWheelScroll.delta,event.mouseWheelScroll.x,event.mouseWheelScroll.y,UpperX,LowerX,UpperY,LowerY);
						
						mandlebrot(texture, pixels,UpperX,LowerX,UpperY,LowerY);
				
					}
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
