#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

#include "olcConsoleGameEngine.h"

class aaronAsteroids : public olcConsoleGameEngine
{
public:
	aaronAsteroids()
	{
		m_sAppName = L"Asteroids";
	}

private:
	struct spaceObject
	{
		// x and y are positions
		// dx and dy are speeds
		// size is size of asteroids
		float x;
		float y;
		float dx;
		float dy;
		int size;
		float angle;
	};

	vector<spaceObject> vecAsteroids;
	vector<spaceObject> vecBullets;
	spaceObject player;
	bool bDead = false;
	int nScore = 0;

	vector<pair<float, float>> vecModelShip;
	vector<pair<float, float>> vecModelAsteroid;

protected:
	// Called by olcConsoleGameEngine
	virtual bool OnUserCreate()
	{

		vecModelShip =
		{
			{0.0f, -5.0f},
			{-2.5f, 2.5f},
			{2.5f, 2.5f}
		}; // player model ship; isoceles triangle

		int verts = 20;

		for (int i = 0; i < verts; i++)
		{
			float radius = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f;
			float a = ((float)i / (float)verts) * 6.28318f;
			vecModelAsteroid.push_back(make_pair(radius * sinf(a), radius * cosf(a)));
		}

		resetGame();

		return true;
	}

	void resetGame()
	{
		// Initialise Player Position
		player.x = ScreenWidth() / 2.0f;
		player.y = ScreenHeight() / 2.0f;
		player.dx = 0.0f;
		player.dy = 0.0f;
		player.angle = 0.0f;

		vecBullets.clear();
		vecAsteroids.clear();

		// Put in two asteroids
		vecAsteroids.push_back({ 20.0f, 20.0f, 8.0f, -6.0f, (int)16, 0.0f });
		vecAsteroids.push_back({ 100.0f, 20.0f, -5.0f, 3.0f, (int)16, 0.0f });

		// Reset game
		bDead = false;
		nScore = 0;
	}

	// Called by oldConsoleGameEngine
	// fElapsedTime = variable time of a frame, timing is variable because CPU load could change timings.
	virtual bool OnUserUpdate(float fElapsedTime)
	{

		// Reset game if player collides with asteroid
		if (bDead)
			resetGame();

		// Clear console screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, 0);

		// Steer
		if (m_keys[VK_LEFT].bHeld)
			player.angle -= 5.0f * fElapsedTime;
		if (m_keys[VK_RIGHT].bHeld)
			player.angle += 5.0f * fElapsedTime;

		// Thrust
		if (m_keys[VK_UP].bHeld)
		{

			// Acceleration changes velocity with respect to time, or fElapsedTime
			player.dx += sin(player.angle) * 20.0f * fElapsedTime;
			player.dy += -cos(player.angle) * 20.0f * fElapsedTime;
		}

		// Velocity changes position with respect to time, or fElapsedTime
		player.x += player.dx * fElapsedTime;
		player.y += player.dy * fElapsedTime;

		// Keep ship in gamespace using wrapCoordinated()
		wrapCoordinates(player.x, player.y, player.x, player.y);

		// Check ship collision with asteroids

		for(auto &a : vecAsteroids)
			if (isPointInsideCircle(a.x, a.y, a.size, player.x, player.y))
				bDead = true; // Death)

		// Fire bullets in direction of player ship
		if (m_keys[VK_SPACE].bReleased)
			vecBullets.push_back({player.x, player.y, 50.0f * sinf(player.angle), -50.0f * cosf(player.angle), 0, 0 });


		// Update and draw asteroids
		for (auto& a : vecAsteroids)
		{
			a.x += a.dx * fElapsedTime;
			a.y += a.dy * fElapsedTime;
			a.angle += 0.5f * fElapsedTime;
			wrapCoordinates(a.x, a.y, a.x, a.y);
			DrawWireFrameModel(vecModelAsteroid, a.x, a.y, a.angle, a.size, FG_RED);
		}

		// Temporary new children asteroids
		vector<spaceObject> newAsteroids;

		// Draw and update bullets
		for (auto& b : vecBullets)
		{
			b.x += b.dx * fElapsedTime;
			b.y += b.dy * fElapsedTime;
			wrapCoordinates(b.x, b.y, b.x, b.y);
			Draw(b.x, b.y);

			for (auto& a : vecAsteroids)
			{
				if (isPointInsideCircle(a.x, a.y, a.size, b.x, b.y))
				{
					// Collision occurred
					b.x = -100;

					if (a.size > 4)
					{
						// Split asteroids into children
						float angle1 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
						float angle2 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
						newAsteroids.push_back({ a.x, a.y, 10.0f * sinf(angle1), 10.0f * cosf(angle1), (int)a.size >> 1, 0.0f });
						newAsteroids.push_back({ a.x, a.y, 10.0f * sinf(angle2), 10.0f * cosf(angle2), (int)a.size >> 1, 0.0f });
					}

					a.x = -100;
					nScore += 100;
				}
			}
		}

		// Append new asteroids to existing vector
		for (auto a : newAsteroids)
			vecAsteroids.push_back(a);

		// Remove asteroids that have been blown up
		if (vecAsteroids.empty())
		{
			auto i = remove_if(vecAsteroids.begin(), vecAsteroids.end(), [&](spaceObject o) { return (o.x < 0); });
			if (i != vecAsteroids.end())
				vecAsteroids.erase(i);
		}

		if (vecAsteroids.empty()) // If no asteroids, level complete! :) - you win MORE asteroids!
		{
			// Level Clear
			nScore += 1000; // Large score for level progression
			vecAsteroids.clear();
			vecBullets.clear();

			// Add two new asteroids, but in a place where the player is not, we'll simply
			// add them 90 degrees left and right to the player, their coordinates will
			// be wrapped by th enext asteroid update
			vecAsteroids.push_back({30.0f * sinf(player.angle - 3.14159f / 2.0f) + player.x,
									30.0f * cosf(player.angle - 3.14159f / 2.0f) + player.y,
									10.0f * sinf(player.angle), 
									10.0f * cosf(player.angle), 
									(int)16, 0.0f });

			vecAsteroids.push_back({30.0f * sinf(player.angle + 3.14159f / 2.0f) + player.x,
									30.0f * cosf(player.angle + 3.14159f / 2.0f) + player.y,
									10.0f * sinf(-player.angle),
									10.0f * cosf(-player.angle),
									(int)16, 0.0f });
		}

		// Remove offscreen bullets
		if (vecBullets.size() > 0)
		{
			auto i = remove_if(vecBullets.begin(), vecBullets.end(), 
				[&](spaceObject o) {return (o.x < 1 || o.y < 1 || o.x >= ScreenWidth() || o.y >= ScreenHeight() - 1); });
			if (i != vecBullets.end())
				vecBullets.erase(i);
		}

		if (vecAsteroids.size() > 0)
		{
			auto i = remove_if(vecAsteroids.begin(), vecAsteroids.end(), [&](spaceObject o) {return (o.x < 0); });
			if (i != vecAsteroids.end())
				vecAsteroids.erase(i);
		}

		if (vecAsteroids.empty())
		{
			nScore += 1000;

		}
		// Draw ship
		DrawWireFrameModel(vecModelShip, player.x, player.y, player.angle);

		//Draw score
		DrawString(2, 2, L"SCRORE: " + to_wstring(nScore));
		return true;
	}

	void wrapCoordinates(float ix, float iy, float& ox, float& oy)
	{
		ox = ix;
		oy = iy;
		if (ix < 0.0f) ox = ix + (float)ScreenWidth();
		if (ix >= (float)ScreenWidth()) ox = ix - (float)ScreenWidth();
		if (iy < 0.0f) oy = iy + (float)ScreenHeight();
		if (iy >= (float)ScreenHeight()) oy = iy - (float)ScreenHeight();
	}

	virtual void Draw(int x, int y, short c = 0x2588, short col = 0x000F)
	{
		float fx, fy;
		wrapCoordinates(x, y, fx, fy);
		olcConsoleGameEngine::Draw(fx, fy, c, col);
	}

	// Collision check for asteroids. If distance is less than radius, it is inside the asteroid
	bool isPointInsideCircle(float cx, float cy, float radius, float x, float y)
	{
		return sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) < radius;
	}

	void DrawWireFrameModel(const vector<pair<float, float>>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, short col = FG_WHITE)
	{
		// pair.first = x coordinate
		// pair.second = y coordinate

		// Create translated model vector of coordinate pairs
		vector<pair<float, float>> vecTransformedCoordinates;
		int verts = vecModelCoordinates.size();
		vecTransformedCoordinates.resize(verts);

		// Rotate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
			vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
		}

		// Scale
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
		}

		// Translate
		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
		}

		// Draw Closed Polygon
		for (int i = 0; i < verts + 1; i++)
		{
			int j = (i + 1);
			DrawLine(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second,
				vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second, PIXEL_SOLID, col);
		}
	}
};


int main()
{
	// Use olcConsoleGameEngine derived app
	aaronAsteroids game;
	game.ConstructConsole(160, 100, 8, 8);
	game.Start();
	return 0;
}