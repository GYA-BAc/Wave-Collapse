#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <string>
#include <algorithm>
#include <Windows.h>

//NOTE: rules can only have apply to immediate surroundings (3x3)

struct tile {
    char id;
    std::vector<char> canBe;
};

struct border_value {
    std::vector<char> appliesTo;
    std::vector<int>  position;
};

struct rule {
    char id;
    std::vector<border_value> borders;
};

void stepWaveCollapse(std::vector<std::vector<tile>> &board,
                      const std::vector<rule> &ruleset,
                      std::vector<int>   pos) 
{
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_int_distribution<int> distr(0, (board[pos[0]][pos[1]].canBe.size()-1));
   
    char nval = board[pos[0]][pos[1]].canBe[distr(gen)];
    board[pos[0]][pos[1]].id = nval;
    board[pos[0]][pos[1]].canBe.clear();

    rule cur_rule;
    for (rule r : ruleset) {
        if (r.id == nval) {
            cur_rule = r;
            break;
        }
    }

    for (border_value check : cur_rule.borders) {
        //check if the surrounding value is out of bounds
        if (((pos[0]+check.position[0] < 0                 ) ||
             (pos[0]+check.position[0] > board.size()-1    ) ||
             (pos[1]+check.position[1] < 0                 ) ||
             (pos[1]+check.position[1] > board[0].size()-1 ))) {
            continue;
        }
       
        //make a variable to store value at board position being checked (easier to type)
        tile val = board[pos[0]+check.position[0]][pos[1]+check.position[1]];

        //update tile's list of possible value
        if (val.id != '.') continue;
        for (int i = 0; i < val.canBe.size(); i++) {
            //remove value from canBe if it is not in list of possible values define by rules
            if (check.appliesTo.size() == 1) {
               val.canBe = check.appliesTo;
                continue;
            }

            if (std::find(check.appliesTo.begin(), check.appliesTo.end(), val.canBe[i]) != check.appliesTo.end()) 
                continue;
            val.canBe.erase(val.canBe.begin() + i);
        }
        //update board value
        board[pos[0]+check.position[0]][pos[1]+check.position[1]].canBe = val.canBe;
        
    }

}

void printBoard(std::vector<std::vector<tile>> &board, bool debug) {
    std::string final = "";
    for (std::vector<tile> row : board) {
        for (tile t : row) {
            // switch (t.id) {
            //     case '0': {
            //         final += ' ';
            //         break;
            //     }
            //     case '1': {
            //         final += (char)(219); //█ this character
            //         break;
            //     }
            //     case '2': {
            //         final += 'O';
            //         break;
            //     }
            // }
            final += t.id;
            if (!debug) continue;
            final += " (";
            for (char c : t.canBe) {
                final += c;
                final += ' ';
            }
            for (int i = 0; i < 3-t.canBe.size(); i++){
                final += "  ";
            }
            final += ") ";
        }
        final += '\n';
    }
    std::cout << final;
}

int main() {

    //init rules
    rule zero = {'0', 
        std::vector<border_value>({
            border_value({std::vector<char>({'0', '1', '2'}), std::vector<int>({-1, -1})}), 
            border_value({std::vector<char>({'0', '1', '2'}), std::vector<int>({-1, 0})}), 
            border_value({std::vector<char>({'0', '1', '2'}), std::vector<int>({-1, 1})}),
            border_value({std::vector<char>({'0', '1', '2'}), std::vector<int>({0, -1})}), 
            border_value({std::vector<char>({'0', '1', '2'}), std::vector<int>({0, 1})}),
            border_value({std::vector<char>({'0', '1', '2'}), std::vector<int>({1, -1})}), 
            border_value({std::vector<char>({'0', '1', '2'}), std::vector<int>({1, 0})}), 
            border_value({std::vector<char>({'0', '1', '2'}), std::vector<int>({1, 1})}),
        }),
    };
    rule one = {'1',
        std::vector<border_value>({
            border_value({std::vector<char>({'1', '0'}), std::vector<int>({-1, -1})}), 
            border_value({std::vector<char>({'1', '0'}), std::vector<int>({-1, 0})}), 
            border_value({std::vector<char>({'1', '0'}), std::vector<int>({-1, 1})}),
            border_value({std::vector<char>({'1', '0'}), std::vector<int>({0, -1})}), 
            border_value({std::vector<char>({'1', '0'}), std::vector<int>({0, 1})}),
            border_value({std::vector<char>({'1', '0'}), std::vector<int>({1, -1})}), 
            border_value({std::vector<char>({'1', '0'}), std::vector<int>({1, 0})}), 
            border_value({std::vector<char>({'1', '0'}), std::vector<int>({1, 1})}),
        }),
    };
    rule two = {'2',
        std::vector<border_value>({
            //inner values (3x3)
            border_value({std::vector<char>({'0'}), std::vector<int>({-1, -1})}), 
            border_value({std::vector<char>({'0'}), std::vector<int>({-1, 0})}), 
            border_value({std::vector<char>({'0'}), std::vector<int>({-1, 1})}),
            border_value({std::vector<char>({'0'}), std::vector<int>({0, -1})}), 
            border_value({std::vector<char>({'0'}), std::vector<int>({0, 1})}),
            border_value({std::vector<char>({'0'}), std::vector<int>({1, -1})}), 
            border_value({std::vector<char>({'0'}), std::vector<int>({1, 0})}), 
            border_value({std::vector<char>({'0'}), std::vector<int>({1, 1})}),
            
            //outer values (5x5)
            border_value({std::vector<char>({'0'}), std::vector<int>({-2, -2})}), 
            border_value({std::vector<char>({'0'}), std::vector<int>({-2, -1})}),
            border_value({std::vector<char>({'0'}), std::vector<int>({-2, 0})}), 
            border_value({std::vector<char>({'0'}), std::vector<int>({-2, 1})}), 
            border_value({std::vector<char>({'0'}), std::vector<int>({-2, 2})}), 

            border_value({std::vector<char>({'0'}), std::vector<int>({-1, -2})}), 
            border_value({std::vector<char>({'0'}), std::vector<int>({-1, 2})}),

            border_value({std::vector<char>({'0'}), std::vector<int>({0, -2})}), 
            border_value({std::vector<char>({'0'}), std::vector<int>({0, 2})}),

            border_value({std::vector<char>({'0'}), std::vector<int>({1, -2})}), 
            border_value({std::vector<char>({'0'}), std::vector<int>({1, 2})}),

            border_value({std::vector<char>({'0'}), std::vector<int>({2, -2})}), 
            border_value({std::vector<char>({'0'}), std::vector<int>({2, -1})}),
            border_value({std::vector<char>({'0'}), std::vector<int>({2, 0})}), 
            border_value({std::vector<char>({'0'}), std::vector<int>({2, 1})}), 
            border_value({std::vector<char>({'0'}), std::vector<int>({2, 2})}), 
        }),
    };
    const std::vector<rule> ruleset = {zero, one, two};

    //init empty tile
    std::vector<char> all_options = {};
    for (rule r : ruleset) {
        all_options.push_back(r.id);
    }
    tile empty_tile = {'.', all_options};

    //init board
    const int nRows = 10, nCols = 10;

    std::vector<std::vector<tile>> board(nRows, std::vector<tile>(nCols, empty_tile));

    //stepWaveCollapse(board, ruleset, std::vector<int>({0,0}), '1');
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            //system("cls");
            stepWaveCollapse(board, ruleset, std::vector<int>({i,j}));
            //printBoard(board, true);
            //std::cin.get();
            
            //Sleep(10);
            
        }
    }
    printBoard(board, false);

    return 0;
}