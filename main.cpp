/*
 * main.cpp
 *
 *  Created on: 27/04/2014
 *      Author: Windsdon
 *  
 *   dungeon
 *
 *    The MIT License (MIT)
 *
 *    Copyright (c) 2014 Windsdon
 *
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 *
 */

#include "Dungeon.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;

int main(int argc, char **argv) {
	srand(time(0));
	Dungeon d;

	VideoMode vm(800, 800);

	RenderWindow window(vm, "Dungeon", Style::Resize | Style::Titlebar | Style::Close);

	Dungeon::DungeonSettings settings;
	settings.sizeX = 50;
	settings.sizeY = 50;

	bool running = true;
	bool stepping = false;
	while (running) {
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				running = false;
				break;
			}
			if (e.type == Event::MouseButtonPressed) {
				if (e.mouseButton.button == 0) {
					d.create(settings, false);
					stepping = false;
				} else {
					if (stepping) {
						if (d.step()) {
							stepping = false;
						}
					} else {
						d.create(settings, true);
						stepping = true;
					}
				}
			}
			if (e.type == Event::Resized) {
				View v = window.getView();
				v.setSize(Vector2f(e.size.width, e.size.height));
				v.setCenter(e.size.width / 2, e.size.height / 2);
				window.setView(v);
			}
		}

		if (!running) {
			break;
		}

		window.clear(sf::Color::Black);

		d.draw(window);

		window.display();
	}
}
