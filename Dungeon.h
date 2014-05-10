/*
 * Dungeon.h
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

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <set>

using namespace std;
using namespace sf;

class DungeonTile {
	public:
		int x;
		int y;

		DungeonTile(int x, int y)
				: x(x), y(y) {
		}

		virtual ~DungeonTile();

		virtual sf::Color getDrawColor() const = 0;
		virtual void draw(RenderTarget &target, const Vector2i &origin, int size) const;
		virtual int getID() const = 0;
};

class TileAir: public DungeonTile {
	public:
		static const int ID;
		TileAir(int x, int y)
				: DungeonTile(x, y) {
		}

		virtual sf::Color getDrawColor() const {
			return sf::Color::Black;
		}

		virtual int getID() const {
			return ID;
		}
};

class TileWall: public DungeonTile {
	public:
		static const int ID;
		TileWall(int x, int y)
				: DungeonTile(x, y) {
		}

		virtual sf::Color getDrawColor() const {
			return sf::Color(153, 117, 0, 255);
		}

		virtual int getID() const {
			return ID;
		}
};

class TileFloor: public DungeonTile {
	public:
		static const int ID;

		TileFloor(int x, int y)
				: DungeonTile(x, y) {
		}

		virtual sf::Color getDrawColor() const {
			return sf::Color(201, 175, 89, 255);
		}

		virtual int getID() const {
			return ID;
		}
};

class Dungeon {
	public:

		struct DungeonSettings {
				int sizeX;
				int sizeY;
				float chestFrequency;

				void operator=(const DungeonSettings& other) {
					sizeX = other.sizeX;
					sizeY = other.sizeY;
					chestFrequency = other.chestFrequency;
				}
		};

		void create(DungeonSettings settings, bool step);
		bool step();

		void draw(RenderTarget& target);

	private:
		vector<vector<DungeonTile*> > tiles;
		DungeonSettings settings;

		bool createRoom(int ex, int ey, int sx, int sy, int w, int h);

		void setTileAt(int x, int y, DungeonTile* tile);

		int getTileIDat(int x, int y) const;
		bool tileAssert(int sx, int sy, int ex, int ey, set<int> &ls, bool only) const;
};
