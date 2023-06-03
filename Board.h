#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <map>
using namespace std;
class Board
{
private:
	const pair<int,int> directions[6] = { {-1,0},{-1,1},{0,-1},{1,0},{1,1},{0,1} };
	int size;
	int trigger;
	int white_count;
	int black_count;
	int white_remaining;
	int black_remaining;
	char player;
	vector<vector<char>> board;
	map<string, pair<int, int>> coordinates;
	string board_state;
	inline int board_height() const;
	inline int board_width(int y) const;
	void calculate_coordinates();
	bool check_move(string& position, pair<int,int>& direction);
	void commit_move(string position, pair<int, int> direction);
public:
	Board();
	string get_board_state() const;
	void print_board();
	string do_move(string from, string to);
};

