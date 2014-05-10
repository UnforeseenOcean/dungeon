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
#include <set>

const int TileAir::ID = 0;
const int TileWall::ID = 1;
const int TileFloor::ID = 2;

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
			tiles[i].push_back(0);
		}
	}

	int rooms = 0;

	for (int i = 0; rooms < 10; i++) {
		cout << "Room spawn try: " << i << endl;
		int sx = rand() % settings.sizeX;
		int sy = rand() % settings.sizeY;
		int w = rand() % 10 + 5;
		int h = rand() % 10 + 5;
		int ex, ey;
		if (rand() % 2) {
			if (rand() % 2) {
				ex = sx;
			} else {
				ex = sx + w - 1;
			}
			ey = sy + h / 2;
		} else {
			if (rand() % 2) {
				ey = sy;
			} else {
				ey = sy + h - 1;
			}
			ex = sx + w / 2;
		}
		if (createRoom(ex, ey, sx, sy, w, h)) {
			cout << "Success" << endl;
			rooms++;
		} else {
			cout << "Failed" << endl;
		}
	}
}

void Dungeon::draw(RenderTarget& target) {
	int margin = 5;

	Vector2i size(target.getSize().x - 2 * margin, target.getSize().y - 2 * margin);

	int sizeX = size.x / settings.sizeX;
	int sizeY = size.y / settings.sizeY;

	if (sizeX <= 0 || sizeY <= 0) {
		return;
	}

	int minSize = min(sizeX, sizeY);

	int px = (size.x - settings.sizeX * minSize) / 2 + margin;
	int py = (size.y - settings.sizeY * minSize) / 2 + margin;

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

int Dungeon::getTileIDat(int x, int y) const {
	if (((unsigned) y) < tiles.size()) {
		if (((unsigned) x) < tiles[y].size()) {
			if (tiles[y][x] == 0) {
				return 0;
			} else {
				return tiles[y][x]->getID();
			}
		}
	}
	return -2;
}

bool Dungeon::tileAssert(int sx, int sy, int w, int h, set<int> &ls, bool only) const {
	for (int i = sy; i < sy + h; i++) {
		for (int j = sx; j <= sx + w; j++) {
			int tile = getTileIDat(j, i);
			if (ls.find(tile) == ls.end()) {
				if (only) {
					return false;
				}
			} else {
				if (!only) {
					return false;
				}
			}
		}
	}

	return true;
}

bool Dungeon::createRoom(int ex, int ey, int sx, int sy, int w, int h) {
	set<int> allowed;
	allowed.insert(TileAir::ID);
	allowed.insert(TileWall::ID);

	if (tileAssert(sx, sy, w, h, allowed, true)) {
		for (int i = 0; i < w; i++) {
			int bx = sx + i;
			for (int j = 0; j < h; j++) {
				int by = sy + j;
				if (bx == ex && by == ey) {
					cout << "Skipping door tile! ";
					continue;
				}
				if (i == 0 || i == (w - 1) || j == 0 || j == (h - 1)) {
					setTileAt(bx, by, new TileWall(bx, by));
				} else {
					setTileAt(bx, by, new TileFloor(bx, by));
				}
			}
		}
		return true;
	} else {
		return false;
	}
}

void Dungeon::setTileAt(int x, int y, DungeonTile* tile) {
	if (((unsigned) y) >= tiles.size()) {
		return;
	}
	if (((unsigned) x) >= tiles[y].size()) {
		return;
	}
	if (tiles[y][x] != 0) {
		delete tiles[y][x];
	}

	tiles[y][x] = tile;
}

void DungeonTile::draw(RenderTarget &target, const Vector2i &origin, int size) const {
	RectangleShape rect(Vector2f(size, size));
	rect.setFillColor(getDrawColor());

	Vector2i o = origin + Vector2i(x, y) * size;

	rect.setPosition(o.x, o.y);
	target.draw(rect);
}

DungeonTile::~DungeonTile() {

}
