
#include <iostream>
#include <vector>
#include <chrono>
#include <Windows.h>
#include <random>
#include <deque>

void printWorld(std::deque<bool>& world, int w, int h) {
	int alive = 0;

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (world[i * w + j])
				std::cout << "*";
			else
				std::cout << " ";
			
			alive += world[i * w + j];
		}
		std::cout << "\n";
	}

	std::cout << "Alive: " << alive << "\n";
}

struct Pair {
	int x, y;
};

Pair operator+(Pair p1, Pair p2) {
	Pair outcome;
	
	outcome.x = p1.x + p2.x;
	outcome.y = p1.y + p2.y;

	return outcome;
}

bool onMap(Pair pos, int w, int h) {
	if (pos.x < 0 or pos.x >= w)
		return false;

	if (pos.y < 0 or pos.y >= h)
		return false;

	return true;
}

void updateCell(std::deque<bool>& world, std::deque<bool> &new_world, int w, int h, Pair pos) {
	Pair dirs[8] = {
		{0, -1}, 
		{1, -1}, 
		{1, 0}, 
		{1, 1}, 
		{0, 1}, 
		{-1, 1}, 
		{-1, 0},  
		{-1, -1}  
	};

	int neighbours = 0;
	for (int i = 0; i < 8; i++) {
		Pair neighbour_pos = pos + dirs[i];
		if (onMap(neighbour_pos, w, h)) {
			neighbours += (int)world[neighbour_pos.y * w + neighbour_pos.x];
		}
	}

	if (world[pos.y * w + pos.x] == true) {
		if (neighbours < 2 or neighbours >= 4) {
			new_world[pos.y * w + pos.x] = false;
		}
		else {
			new_world[pos.y * w + pos.x] = world[pos.y * w + pos.x];
		}

	}
	else {
		if(neighbours == 3)
			new_world[pos.y * w + pos.x] = true;
	}

}

void update(std::deque<bool>& world, int w, int h) {
	std::deque<bool> new_world(world.size());

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			updateCell(world, new_world, w, h, {j, i});
		}
	}

	world = new_world;
}

int main() {

	int w = 30;
	int h = 30;

	std::cout << "Size of map\n";
	std::cout << "Width: ";
	std::cin >> w;
	std::cout << "Height: ";
	std::cin >> h;

	std::deque<bool> world(w * h); // Now 'world' has a name and persists

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, 1);

	for (int i = 0; i < world.size(); i++)
		world[i] = (bool)(distrib(gen));

	world[0] = 1;
	world[1] = 1;
	world[w] = 1;
	world[w + 1] = 1;

	using clock = std::chrono::steady_clock;

	auto start = clock::now();

	double delta_t = 0;
	double frame = 1000.0;
	while (true) {
		auto now = clock::now();
		delta_t = std::chrono::duration<double, std::milli>(now - start).count();

		if (delta_t > frame) {
			//std::cout << "\x1B[2J\x1B[H";
			system("cls");

			start = now;
			printWorld(world, w, h);
			update(world, w, h);
			
			
		}

	}

	return 0;
}
