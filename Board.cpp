#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include "Box.cpp"
#include "Box_structure.cpp"
#include "Row.cpp";
#include "Column.cpp";
#include "Container.cpp";
#include <iostream>
#include <bitset>;


using std::vector;

class Board {
public:
	vector<vector<Box*>> boxes;
	vector<vector<int>> board;

	vector<Box_Structure*> rows;
	vector<Box_Structure*> columns;
	vector<Box_Structure*> containers;

	int container_size;
	int board_size;

	Board(vector<vector<int>> b) {
		board = b;
		board_size = b.size();
		container_size = sqrt(b.size());

		rows.reserve(board_size);
		make_rows();
		make_columns();
		make_containers();

		fill_row_structure_boxes_and_board_boxes();

		fill_column_structure_boxes();
		fill_container_structure_boxes();

		for (int row = 0; row < board_size; row++) {
			fill_boxes_per_value(rows[row]);
		}

		//Test rows
		std::cout << "Test box structures " << std::endl;
		std::cout << std::to_string(rows.size()) << std::endl;
		for (int i = 0; i < rows.size(); i++) {
			std::cout << "row " + std::to_string(i) << std::endl;
			for (int j = 1; j < rows.size() + 1; j++) {
				std::cout << "value " + std::to_string(j) + " ";
				std::cout << std::bitset<25>(rows[i]->boxes_per_value.at(j)) << std::endl;
			}
		}
		std::cout << " " << std::endl;

		for (int column = 0; column < board_size; column++) {
			fill_boxes_per_value(columns[column]);
		}

	}

	void fill_row_structure_boxes_and_board_boxes() {
		for (int row = 0; row < board_size; row++) {
			vector<Box*> current;

			for (int column = 0; column < board_size; column++) {
				current.push_back(create_box(row, column, board[row][column]));
			}

			rows[row]->structure_boxes = current;
			boxes.push_back(current);
		}
	}

	void fill_container_structure_boxes() {
		/*for (int row = 0; row < board_size; row++) {
		for (int column = 0; column < board_size; column++) {
		int* container_coords = find_container_starting_box(row, column);

		}
		}*/
	}

	void make_rows() {
		for (int row = 0; row < board_size; row++) {
			Box_Structure* row_structure = new Row();

			for (int value = 1; value < board_size + 1; value++) {
				row_structure->boxes_per_value.insert({ value,  0b1111111111111111111111111 });
			}

			rows.push_back(row_structure);
		}
		
	}

	void make_columns() {
		for (int column = 0; column < board_size; column++) {
			Box_Structure* column_structure = new Column();

			for (int value = 1; value < board_size + 1; value++) {
				column_structure->boxes_per_value.insert({ value, 0b1111111111111111111111111 });
			}

			columns.push_back(column_structure);
		}
	}

	void make_containers() {
		for (int container = 0; container < board_size; container++) {
			Box_Structure* container_structure = new Container();

			for (int value = 1; value < board_size + 1; value++) {
				container_structure->boxes_per_value.insert({ value,  0b1111111111111111111111111 });
			}

			containers.push_back(container_structure);
		}
	}

	void fill_column_structure_boxes() {
		for (int column = 0; column < board_size; column++) {
			vector<Box*> current;

			for (int row = 0; row < board_size; row++) {
				current.push_back(rows[row]->structure_boxes[column]);
			}

			columns[column]->structure_boxes = current;
		}
	}

	void fill_boxes_per_value(Box_Structure* structure) {
		for (int box = board_size-1; box >= 0; box--) {
			int box_value = structure->structure_boxes[box]->value;

			if (box_value == 0) {
				structure->boxes_per_value[box_value] = 0b0000000000000000000000000;
			}

			for (int value = 1; value < board_size + 1; value++) {
				if (box_value == 0) {
					long available_values = structure->structure_boxes[box]->available_values;
					long value_to_long = 1 << value;
					long intersection = value_to_long & available_values;
					bool value_is_not_available = (intersection != 0);

					if (value_is_not_available) {
						remove_box(structure, value, box);
					}
					
				}
				else {
					remove_box(structure, value, box);
				}
			}
			
		}
		
	}

	void remove_box(Box_Structure* structure, int value, int box) {
		long av_boxes = structure->boxes_per_value.at(value);
		long box_to_long = 1 << box;

		//std::cout << std::bitset<26>(av_boxes & ~box_to_long) << std::endl;
		structure->boxes_per_value[value] = av_boxes & ~box_to_long;
	}

	//void create_row_structures() {
	//	
	//	for (int row = 0; row < board_size; row++) {
	//		Box_Structure* row_structure = new Row();
	//		
	//		for (int value = 1; value < board_size + 1; value++) {
	//			row_structure->boxes_per_value.insert({ value, 0 });
	//		}
	//		
	//		for (int col = 0; col < board_size; col++) {
	//			long box_av_values = boxes[col][row]->available_values;

	//			for (int value = 1; value < board_size + 1; value++) {
	//				long value_to_binary_long = 1 << value;
	//				/*std::cout << "inside " << std::endl;
	//				std::cout << std::bitset<26>(box_av_values) << std::endl;
	//				std::cout << std::bitset<26>(value_to_binary_long) << std::endl;
	//				std::cout<< std::bitset<26>(box_av_values & value_to_binary_long) << std::endl;
	//				std::cout << std::to_string(box_av_values & value_to_binary_long != 0);*/
	//				
	//				if ((box_av_values & value_to_binary_long) != 0) {
	//					
	//					long boxes = row_structure->boxes_per_value.at(value);
	//					long box_column = 1 << col;
	//					long updated_boxes = boxes | box_column;
	//					//std::cout << std::bitset<26>(box_av_values & value_to_binary_long) << std::endl;
	//					row_structure->boxes_per_value[value] = updated_boxes;
	//				}
	//			}
	//			
	//		}

	//		rows.push_back(row_structure);
	//	}
	//}

	/*void create_column_structures() {

		for (int column = 0; column < board_size; column++) {
			Box_Structure* column_structure = new Column();

			for (int val = 1; val < board_size + 1; val++) {
				column_structure->boxes_per_value.insert(val, 0);
			}

			for (int row = 0; row < board_size; row++) {
				int value = board[row][column];
				long boxes = column_structure->boxes_per_value.at(value);
				long value_to_binary_long = 1 << value;
				long updated_boxes = boxes | value_to_binary_long;

				column_structure->boxes_per_value[value] = updated_boxes;
			}

			columns[column] = column_structure;
		}
	}*/

	/*void create_container_structures() {

		for (int container = 0; container < board_size; container++) {
			Box_Structure* container_structure = new Container();

			for (int val = 1; val < board_size + 1; val++) {
				container_structure->boxes_per_value.insert(val, 0);
			}

			int start_row = container / container_size;
			int start_col = container / container_size;
			int end_row = start_row + container_size;
			int end_col = start_col + container_size;

			for (int row = start_row; row < end_row; row++) {
				for (int column = start_col; column < end_col; column++) {
					int value = board[row][column];
					long boxes = container_structure->boxes_per_value.at(value);
					long value_to_binary_long = 1 << value;
					long updated_boxes = boxes | value_to_binary_long;

					container_structure->boxes_per_value[value] = updated_boxes;
				}
			}

			containers[container] = container_structure;
		}
	}*/

	void update_boxes(Box_Structure* structure, int value) {
		long boxes = structure->boxes_per_value.at(value);
		long value_to_binary_long = 1 << value;
		long updated_boxes = boxes | value_to_binary_long;

		structure->boxes_per_value[value] = updated_boxes;
	}


	Box* create_box(int row, int column, int value) {
		Box* box = new Box(row, column);

		if (value == 0) {
			box->value = 0;
			update_available_values(box);
		}
		else {
			box->value = value;
			box->available_values = 0;
		}

		return box;
	}

	void update_available_values(Box* box) {
		int row = box->row;
		int column = box->column;

		long values = 0b0000000000000000000000000;

		for (int col = 0; col < board_size; col++) {
			if (board[row][col] != 0) {
				long v = 1 << board[row][col];
				values |= v;
			}
		}

		for (int r = 0; r < board_size; r++) {
			if (board[r][column] != 0) {
				long v = 1 << board[r][column];
				values |= v;
			}
		}

		int* container_coords = find_container_starting_box(row, column);
		int start_row = container_coords[0];
		int start_column = container_coords[1];
		int end_row = container_coords[0] + container_size;
		int end_column = container_coords[1] + container_size;

		for (int r = start_row; r < end_row; r++) {
			for (int col = start_column; col < end_column; col++) {
				if (board[r][col] != 0) {
					long v = 1 << board[r][col];
					values |= v;
				}
			}
		}

		box->available_values = values;
	}

	int* find_container_starting_box(int row, int column) {
		int container_x = row / container_size;
		int container_y = column / container_size;

		int starting_box_x = container_x * container_size;
		int starting_box_y = container_y * container_size;

		int coords[] = { starting_box_x, starting_box_y };
		return coords;
	}
};

#endif