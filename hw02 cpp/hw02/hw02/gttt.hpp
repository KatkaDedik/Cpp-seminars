#pragma once
#include <map>
#include <tuple>
#include <vector>

/* This class, ‹gttt› (short for generalized tic-tac-toe), provides
* the entire interface of the game. */
class gttt
{
	int currentPlayer = 0;
	int to_win;
	std::map<std::tuple<int, int, int>, int> playground;
	std::vector<std::vector<std::tuple<int, int, int>>> players;

public:
	/* Start a new game with a given number of players. The
	* ‹to_win› argument specifies how many consecutive symbols need
	* to be placed by the same player in order to win the game. The
	* number of players must be between 2 and 1000000 (inclusive).
	* Same limits apply to the length of the winning sequence. */
	gttt(int _players, int _to_win)
	{
		if (_players > 1 && _players <= 1000000) {
			players = std::vector<std::vector<std::tuple<int, int, int>>>(_players);
		}
		else {
			return;
		}
		if (_to_win > 1 && _to_win <= 1000000) {
			to_win = _to_win;
		}
		else {
			return;
		}
	}

	/* Which player plays next. Players are numbered from 0 and play
	* in ascending order (player 0 goes first). */
	int next() const;

	/* Play the active player's round by placing a mark on the cell
	 * given by (x, y, z). Returns true if the move was valid.
	 * Returns false if the cell was already occupied (and does not
	 * change the state in this case and the player's round is not
	 * completed).  */
	bool play(int x, int y, int z);

	/* Return true if the specified cell is empty, false otherwise. */
	bool empty(int x, int y, int z) const;

	/* Return the identifier of the player who owns (placed their
	 * mark on) the given cell. The behaviour is unspecified if the
	 * cell is empty. */
	int owner(int x, int y, int z) const;

	/* True iff there is a winning sequence on the grid. */
	bool finished() const;


	/* The player who won. Undefined unless finished() is true. */
	int winner() const;

	/* Returns true if the next player can immediately win the game
	 * by placing their mark strategically. */
	bool may_win() const;


private:

	bool is_winning(int x, int y, int z, bool may, int player) const;

	bool check_direction(std::tuple<int, int, int> direction, int x, int y, int z,bool may, int player) const;
};