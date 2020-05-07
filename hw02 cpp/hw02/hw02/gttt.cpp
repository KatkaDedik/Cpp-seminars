#include "gttt.hpp"
#include <iostream>

	int gttt::next() const
	{
		return currentPlayer;
	}

	bool gttt::play(int x, int y, int z)
	{
		if (!empty(x, y, z)) {
			return false;
		}
		playground.insert(std::make_pair(std::make_tuple(x, y, z), currentPlayer));
		players[currentPlayer].push_back(std::make_tuple(x, y, z));
		currentPlayer++;
		if (currentPlayer == players.size()) {
			currentPlayer = 0;
		}
		return true;
	}

	bool gttt::empty(int x, int y, int z) const
	{
		return playground.find({ x, y, z }) == playground.end();
	}

	int gttt::owner(int x, int y, int z) const
	{
		auto iter = playground.find({ x, y, z });
		if (iter == playground.end()) {
			return -1;
		}
		return playground.at({ x, y, z });
	}

	bool gttt::finished() const
	{
		for (size_t p = 0; p < players.size(); p++)
		{
			for (std::tuple<int, int, int> place : players[p])
			{
				if (is_winning(std::get<0>(place), std::get<1>(place), std::get<2>(place), false, p)) {
					return true;
				}
			}
		}
		return false;
	}

	int gttt::winner() const
	{
		for (size_t p = 0; p < players.size(); p++)
		{
			for (std::tuple<int, int, int> place : players[p])
			{
				if (is_winning(std::get<0>(place), std::get<1>(place), std::get<2>(place), false, p)) {
					return (int)p;
				}
			}
		}
		return -1;
	}

	bool gttt::may_win() const
	{
		for (std::tuple<int, int, int> place : players[currentPlayer])
		{
			if (is_winning(std::get<0>(place), std::get<1>(place), std::get<2>(place), true, currentPlayer)) {
				return true;
			}
		}
		return false;
	}

	bool gttt::is_winning(int x, int y, int z, bool may, int player) const
	{
		const std::tuple<int, int, int> directions[13] =
		{
			{0,0,1},
			{0,1,0},
			{1,0,0},
			{1,1,1},
			{-1,1,1},
			{1,-1,1},
			{1,1,-1},
			{0,1,1},
			{0,-1,1},
			{1,0,1},
			{-1,0,1},
			{1,1,0},
			{-1,1,0}
		};

		for (int i = 0; i < 13; i++) {
			if (check_direction(directions[i], x, y, z, may, player)) {
				return true;
			}
		}
		return false;
	}

	bool gttt::check_direction(std::tuple<int, int, int> direction, int x, int y, int z, bool may, int player) const
	{
		int counter = 1;
		const int xd = std::get<0>(direction);
		const int yd = std::get<1>(direction);
		const int zd = std::get<2>(direction);

		while (counter < to_win) {
			if (owner(x + (counter * xd), y + (counter * yd), z + (counter * zd)) != player) {
				if (!may) { return false; }
				break;
			}
			counter++;
		}

		if (counter == to_win - 1) {
			return (empty(x + (-xd), y + (-yd), z + (-zd)) 
				|| empty(x + (counter * xd), y + (counter * yd), z + (counter * zd)));
		}


		if (empty(x + (counter * xd), y + (counter * yd), z + (counter * zd))) {
			counter++;
			while (counter < to_win) {
				if (owner(x + (counter * xd), y + (counter * yd), z + (counter * zd)) != player) {
					return false;
				}
				counter++;
			}
			return true;
		}
		return false;
	}

