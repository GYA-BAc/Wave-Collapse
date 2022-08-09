#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <string>
#include <algorithm>
#include <Windows.h>


struct tile {
    char id;
    std::vector<char> canBe;
};

struct rule {
    char id;
    std::vector<char> canBorder;
};

void stepWaveCollapse(std::vector<std::vector<tile>> &board,
                      const std::vector<rule> &ruleset,
                      std::vector<int>   pos) 
{
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_int_distribution<int> distr(0, (board[pos[0]][pos[1]].canBe.size()-1));

    const std::vector<std::vector<int>> surr = {
        {-1,-1}, {0,-1}, {1,-1}, {-1,0}, {1,0}, {-1,1}, {0,1}, {1,1}
    };
    
   
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
    // std::cout << cur_rule.id << ' ';
    // for (char c : cur_rule.canBorder) {
    //     std::cout << c << ' ';
    // }
    // std::cout << '\n';

    for (std::vector<int> check : surr) {
        //check if the surrounding value is out of bounds
        if (((pos[0] == 0                 && check[0] == -1) ||
             (pos[0] == board.size()-1    && check[0] == 1 ) ||
             (pos[1] == 0                 && check[1] == -1) ||
             (pos[1] == board[0].size()-1 && check[1] == 1 ))) {
            continue;
        }
       
        
        tile val = board[pos[0]+check[0]][pos[1]+check[1]];

        if (val.id != '.') continue;
        for (int i = 0; i < val.canBe.size(); i++) {
            //remove value from canBe if it is not in canBorder
            if (cur_rule.canBorder.size() == 1) {
                val.canBe = cur_rule.canBorder;
                continue;
            }

            if (std::find(cur_rule.canBorder.begin(), cur_rule.canBorder.end(), val.canBe[i]) != cur_rule.canBorder.end()) 
                continue;
            val.canBe.erase(val.canBe.begin() + i);
        }
        board[pos[0]+check[0]][pos[1]+check[1]].canBe = val.canBe;
        
    }

}

void printBoard(std::vector<std::vector<tile>> &board, bool debug) {
    std::string final = "";
    for (std::vector<tile> row : board) {
        for (tile t : row) {
            switch (t.id) {
                case '0': {
                    final += ' ';
                    break;
                }
                case '1': {
                    final += (char)(219); //█ this character
                    break;
                }
                case '2': {
                    final += 'O';
                    break;
                }
            }
            //final += t.id;
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

    rule zero = {'0', std::vector<char>({'0', '1', '2'})};
    rule one  = {'1', std::vector<char>({'1', '0'})};
    rule two  = {'2', std::vector<char>({'0'})};

    tile empty_tile = {'.', std::vector<char>({'0', '1', '2'})};

    const std::vector<rule> ruleset = {zero, one, two};

    //init board
    const int nRows = 20, nCols = 50;

    std::vector<std::vector<tile>> board(nRows, std::vector<tile>(nCols, empty_tile));
    //NOTE : check if occupied tiles have restricted list

    //stepWaveCollapse(board, ruleset, std::vector<int>({0,0}), '1');
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            //system("cls");
            stepWaveCollapse(board, ruleset, std::vector<int>({i,j}));
            //printBoard(board, true);
            //Sleep(10);
            //std::cin.get();
            
        }
    }
    printBoard(board, false);


    //std::vector<char> a = {'1', '2', '3'};
    //if (std::find(a.begin(), a.end(), '1') != a.end()) std::cout << '\a';

   
    return 0;
}