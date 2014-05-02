/*
 * Dungeon.cpp
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
#include <iostream>

void Dungeon::create(DungeonSettings settings, bool step) {
	vector<vector<DungeonTile*> >::iterator it1;

	this->settings = settings;

	for (it1 = tiles.begin(); it1 != tiles.end(); ++it1) {
		vector<DungeonTile*>::iterator it2;
		for (it2 = it1->begin(); it2 != it1->end(); ++it2) {
			if ((*it2) != 0) {
				delete (*it2);
			}
		}
		it1->clear();
	}

	tiles.clear();

	for (int i = 0; i < settings.sizeY; i++) {
		tiles.push_back(vector<DungeonTile*>());
		for (int j = 0; j < settings.sizeX; j++) {
			if(rand()%2){
				tiles.back().push_back(new TileWall(j, i));
			}else{
				tiles.back().push_back(new TileAir(j, i));
			}
		}
	}
}

void Dungeon::draw(RenderTarget& target) {

	Vector2i size(target.getSize().x - 20, target.getSize().y - 20);

	int sizeX = size.x / settings.sizeX;
	int sizeY = size.y / settings.sizeY;

	if (sizeX <= 0 || sizeY <= 0) {
		return;
	}

	int minSize = min(sizeX, sizeY);

	int px = (size.x - settings.sizeX * minSize) / 2 + 10;
	int py = (size.y - settings.sizeY * minSize) / 2 + 10;

	Vector2i origin(px, py);

	vector<vector<DungeonTile*> >::iterator it1;
	for (it1 = tiles.begin(); it1 != tiles.end(); ++it1) {
		vector<DungeonTile*>::iterator it2;

		for (it2 = it1->begin(); it2 != it1->end(); ++it2) {
			if (*it2 == 0) {
				continue;
			}
			(*it2)->draw(target, origin, minSize);
		}
	}
}

bool Dungeon::step() {
	return false;
}

const void DungeonTile::draw(RenderTarget &target, const Vector2i &origin, int size) {
	RectangleShape rect(Vector2f(size, size));
	rect.setFillColor(getDrawColor());

	Vector2i o = origin + Vector2i(x, y) * size;

	rect.setPosition(o.x, o.y);
	target.draw(rect);
}

DungeonTile::~DungeonTile() {

}
