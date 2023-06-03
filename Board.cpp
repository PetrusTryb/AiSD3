#include "Board.h"

inline int Board::board_height() const
{
	return 2 * size - 1;
}

inline int Board::board_width(int y) const
{
	return 2 * size - 1 - abs(size - 1 - y);
}

void Board::calculate_coordinates()
{
	/*
	    a5 b6 c7 d8 e9
		a4 b5 c6 d7 e8 f8
		a3 b4 c5 d6 e7 f7 g7
		a2 b3 c4 d5 e6 f6 g6 h6
		a1 b2 c3 d4 e5 f5 g5 h5 i5
		b1 c2 d3 e4 f4 g4 h4 i4
		c1 d2 e3 f3 g3 h3 i3
		d1 e2 f2 g2 h2 i2
        e1 f1 g1 h1 i1
	*/
	int h_offset = 0;
	for (int letter = 0; letter < board_width(size-1)+2; letter++) {
		int vertical_pos = size+letter;
		if (vertical_pos > 2 * size) {
			vertical_pos = 2 * size;
			h_offset++;
		}
		int horizontal_pos = h_offset;
		int letter_count = board_width(letter - 1) + 2;
		int x = 0;
		for (int number = 0; number < letter_count; number++) {
			//cout << (char)(letter + 'a') << number + 1 << " - " << vertical_pos-number << " " << horizontal_pos+x << endl;
			string code = "";
			code += (char)(letter + 'a');
			code += (char)(number + '1');
			coordinates.insert({ code, make_pair(vertical_pos - number, horizontal_pos + x) });
			if(x+h_offset<letter)
				x++;
		}
	}
}

bool Board::check_move(string& position, pair<int, int>& direction)
{
	string code = position;
	pair<int, int> current = coordinates.at(position);
	do{
		char letter = code[0] + direction.first;
		char number = code[1] + direction.second;
		code = "";
		code += letter;
		code += number;
		current = coordinates.at(code);
		if (board[current.first][current.second] == '_') {
			position = code;
			return true;
		}
		if (current.second == size && direction.first == -1 && direction.second == 0)
			direction.second = -1;
		else if (current.first == size && direction.first == -1 && direction.second == 1)
			direction.second = 0;
		else if (current.second == size && direction.first == 1 && direction.second == 1)
			direction.second = 0;
	} while (board[current.first][current.second] != '+');
	return false;
}

void Board::commit_move(string position, pair<int, int> direction)
{
	pair<int, int> current = coordinates.at(position);
	char letter = position[0] + direction.first;
	char number = position[1] + direction.second;
	string code = "";
	code += letter;
	code += number;
	pair<int,int> next = coordinates.at(code);
	while (board[next.first][next.second] != '+') {
		board[current.first][current.second] = board[next.first][next.second];
		board[next.first][next.second] = '_';
		current = next;
		if (current.second == size && direction.first == -1 && direction.second == 0)
			direction.second = -1;
		else if (current.first == size && direction.first == -1 && direction.second == 1)
			direction.second = 0;
		else if (current.second == size && direction.first == 1 && direction.second == 1)
			direction.second = 0;
		else if (current.first == size && current.second==size && direction.first == 1 && direction.second == 0)
			direction.second = -1;
		letter = code[0] + direction.first;
		number = code[1] + direction.second;
		code = "";
		code += letter;
		code += number;
		next = coordinates.at(code);
		continue;
	}
}

Board::Board()
{
	cin>>size>>trigger>>white_count>>black_count>>white_remaining>>black_remaining>>player;
	int input_white_pawns = 0;
	int input_black_pawns = 0;
	string line;
	vector<char> row(board_width(0)+1,'+');
	board.push_back(row);
	getline(cin, line);
	for (int y = 0; y < board_height(); y++) {
		getline(cin, line);
		row.clear();
		row.push_back('+');
		for (int i = 0; i < line.size(); i++)
		{
			if(line[i] == ' ')
				continue;
			if (line[i] == 'W') {
				input_white_pawns++;
				row.push_back('W');
			}
			else if (line[i] == 'B') {
				input_black_pawns++;
				row.push_back('B');
			}
			else if (line[i] == '_') {
				row.push_back('_');
			}
		}
		if (row.size()-1 != board_width(y)) {
			board_state = "WRONG_BOARD_ROW_LENGTH";
			board.clear();
			return;
		}
		row.push_back('+');
		board.push_back(row);
	}
	if (input_black_pawns + black_remaining > black_count)
	{
		board_state = "WRONG_BLACK_PAWNS_NUMBER";
	}
	else if (input_white_pawns + white_remaining > white_count)
	{
		board_state = "WRONG_WHITE_PAWNS_NUMBER";
	}
	else {
		board_state = "BOARD_STATE_OK";
	}
	row = vector<char>(board_width(0) + 1, '+');
	board.push_back(row);
	calculate_coordinates();
	return;
}

string Board::get_board_state() const
{
	return board_state;
}

void Board::print_board()
{
	if (board_state != "BOARD_STATE_OK")
	{
		cout<<"EMPTY_BOARD"<< endl;
		return;
	}
	cout<<size<<" "<<trigger<<" "<<white_count<<" "<<black_count<<endl<<white_remaining<<" "<<black_remaining<<" "<<player<<endl;
	for (int y = 1; y <= board_height(); y++) {
		for (int i = 0; i < abs(size - y); i++)
		{
			cout << " ";
		}
		for (int x = 1; x <= board_width(y-1); x++)
		{
			cout<<board[y][x]<<" ";
		}
		cout << endl;
	}
	cout << endl;
}

string Board::do_move(string from, string to)
{
	if (coordinates.count(from) == 0)
		return "BAD_MOVE_" + from + "_IS_WRONG_INDEX";
	if (coordinates.count(to) == 0)
		return "BAD_MOVE_" + to + "_IS_WRONG_INDEX";
	if (board[coordinates[from].first][coordinates[from].second] != '+')
		return "BAD_MOVE_" + from + "_IS_WRONG_STARTING_FIELD";
	if (board[coordinates[to].first][coordinates[to].second] == '+')
		return "BAD_MOVE_" + to + "_IS_WRONG_DESTINATION_FIELD";
	pair<int,int> from_pos = coordinates[from];
	pair<int,int> to_pos = coordinates[to];
	pair<int, int> direction = {0,0};
	for (auto delta: directions) {
		char letter = from[0] + delta.first;
		char number = from[1] + delta.second;
		string code = "";
		code += letter;
		code += number;
		if (code == to)
			direction = delta;
	}
	if (direction.first==0&&direction.second==0)
		return "UNKNOWN_MOVE_DIRECTION";
	if(!check_move(from, direction))
		return "BAD_MOVE_ROW_IS_FULL";
	direction.first *= -1;
	direction.second *= -1;
	commit_move(from, direction);
	board[to_pos.first][to_pos.second] = player;
	if(player=='W')
		white_remaining--;
	else
		black_remaining--;
	player = (player == 'W') ? 'B' : 'W';
	return "MOVE_COMMITTED";
}
