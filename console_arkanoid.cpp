#include <iostream>
#include <vector>
#include <ctime>
#include <queue>

using namespace std;

struct game_board {
    int rows, columns, r, n, blocks, rnd;
    int x_paddle, y_paddle;
    vector<vector<int>> board;
    vector<queue<int>> blocks_queue;
};

game_board initialization(int columns, int r, int n, int blocks) {
    game_board state = { .rows = blocks + 5, .columns = columns, .r = r, .n = n, .blocks = blocks };
    for (int i = 0; i < state.columns; i++) {
        state.blocks_queue.push_back(queue<int>());
    }
    state.y_paddle = state.rows - 1;
    state.x_paddle = state.columns / 2;
    if (state.n)
        state.rnd = rand() % n;
    else
        state.rnd = 0;
    state.board.resize(state.rows, vector<int>(columns, 0));
    return state;
}

void menu();
void creating_board(game_board& state);
void display_board(game_board& state);
void moving_left(game_board& state);
void moving_right(game_board& state);
void shooting(game_board& state, int& moves_left, int& score);
void extra_block(game_board& state);
void game_ending(game_board& state);

int main()
{
    menu();
    return 0;
}

void menu() {
    int columns, r, n, blocks;
    srand(time(NULL));
    system("cls");
    cout << "Witaj w grze arkanoid!" << endl;
    cout << endl << "Podaj ilosc kolumn planszy: ";
    cin >> columns;
    cout << endl << "Podaj ilosc bloczkow znajdujacych sie w kazdej kolumnie: ";
    cin >> blocks;
    cout << endl << "Co r+rnd ruchow do wszystkich kolumn dodawany jest jeden bloczek zmniejszajac przestrzen dla gracza. Parametr rnd jest wartoscia losowa z przedzialu[0, n)." << endl;
    cout << endl << "Podaj parametr r: ";
    cin >> r;
    cout << endl << "Podaj parametr n: ";
    cin >> n;


    game_board state = initialization(columns, r, n, blocks);
    creating_board(state);
}

void creating_board(game_board& state) {

    int propability_of_bonus = 20;
    for (int i = 0; i < state.rows; i++) {
        for (int j = 0; j < state.columns; j++) {
            state.board[i][j] = 0;
        }
    }

    for (int i = state.blocks - 1; i >= 0; i--) { //filling blocks in
        for (int j = state.columns - 1; j >= 0; j--) {
            int rand_number = rand() % 100 + 1;
            if (rand_number > propability_of_bonus || i == 0 || i == state.blocks - 1) {
                state.blocks_queue[j].push(1);
                state.board[i][j] = 1;
            }
            else {
                int rand_bonus = rand() % 2 + 2;
                state.blocks_queue[j].push(rand_bonus); // bonus 2: '::', bonus 3: '++'
                state.board[i][j] = rand_bonus;
            }
        }
    }
    state.board[state.y_paddle][state.x_paddle] = 4; // paddle index

    display_board(state);

}


void display_board(game_board& state) {
    char move;
    int moves_left = state.r + state.rnd;
    int score = 0;
    while (true) {
        system("cls");
        cout << "                                   A R K A N O I D\n\n\n";

        cout << "            -------------------------------------------------------\n";
        cout << "            |     S T E R O W A N I E    |       B O N U S Y      |\n";
        cout << "            |                            |                        |\n";
        cout << "            |      A - ruch w lewo       |     #::# - zbicie      |\n";
        cout << "            |      D - ruch w prawo      |     dwoch bloczkow     |\n";
        cout << "            |      W - strzal            |     #++# - dodatkowe   |\n";
        cout << "            |      Q - wyjscie z gry     |       5 ruchow         |\n";
        cout << "            |                            |                        |\n";
        cout << "            -------------------------------------------------------\n";
        cout << "                                     WYNIK: " << score << "                    \n";
        cout << "            -------------------------------------------------------\n\n";


        cout << "       ";
        for (int i = 0; i < (5 * state.columns - 1); i++) { //creating borders
            cout << '-';
        }
        cout << '\n';

        for (int i = 0; i < state.rows; i++) {
            cout << "       ";
            for (int j = 0; j < state.columns; j++) {
                if (i <= state.blocks_queue[j].size() - 1 && state.blocks_queue[j].size() > 0) {
                    switch (state.board[i][j]) {
                    case 1:
                        cout << "#### ";
                        break;
                    case 2: // bonus 2: '::'
                        cout << "#::# ";
                        break;
                    case 3: // bonus 2: '++'
                        cout << "#++# ";
                        break;
                    }
                }
                else if (state.board[i][j] == 4) {
                    cout << "=[]= ";
                }
                else {
                    cout << "     ";
                }
            }
            if (i == 0) {
                cout << "              Za " << moves_left << " ruchow pojawi sie";
            }
            else if (i == 1) {
                cout << "               " << " dodatkowy bloczek   ";
            }
            cout << endl;
        }
        cout << "       ";
        for (int i = 0; i < (5 * state.columns - 1); i++) { //creating borders
            cout << '-';
        }

        game_ending(state);

        cout << "\n\nWykonaj ruch: ";
        cin >> move;

        switch (move) {
        case 'A': moving_left(state); break;
        case 'D': moving_right(state); break;
        case 'W': shooting(state, moves_left, score); break;
        case 'Q': menu(); break;
        }
        moves_left--;
        if (!moves_left) {
            extra_block(state);
            moves_left = state.r + state.rnd;
        }
    }
}

void moving_left(game_board& state) {
    if (state.x_paddle > 0) {
        state.board[state.y_paddle][state.x_paddle] = 0;
        state.x_paddle--;
        state.board[state.y_paddle][state.x_paddle] = 4;
    }
}

void moving_right(game_board& state) {
    if (state.x_paddle < state.columns - 1) {
        state.board[state.y_paddle][state.x_paddle] = 0;
        state.x_paddle++;
        state.board[state.y_paddle][state.x_paddle] = 4;
    }
}

void shooting(game_board& state, int& moves_left, int& score) {
    if (!state.blocks_queue[state.x_paddle].empty()) {
        int queue_size = state.blocks_queue[state.x_paddle].size();
        int newest_element = state.blocks_queue[state.x_paddle].front();
        switch (newest_element) {
        case 1: //simple shot
            state.blocks_queue[state.x_paddle].pop();
            state.board[queue_size - 1][state.x_paddle] = 0;
            score++;
            break;
        case 2: // bonus 2: '::' 
            state.blocks_queue[state.x_paddle].pop();
            state.board[queue_size - 1][state.x_paddle] = 0;
            if (!state.blocks_queue[state.x_paddle].empty()) {
                state.board[queue_size - 2][state.x_paddle] = 0;
                shooting(state, moves_left, score);
            }
            score += 2;
            break;
        case 3: // bonus 3: '++'
            moves_left += 5;
            state.blocks_queue[state.x_paddle].pop();
            state.board[queue_size - 1][state.x_paddle] = 0;
            score += 2;
            break;
        }
    }
}

void extra_block(game_board& state) {
    int rand_column = rand() % state.columns;
    state.blocks_queue[rand_column].push(1);
    int size_queue = state.blocks_queue[rand_column].size();
    state.board[size_queue - 1][rand_column] = 1;
}

void game_ending(game_board& state) {
    char input;
    int lost = 0;
    int win = 0;
    for (int i = 0; i < state.columns; i++) {
        if (state.blocks_queue[i].size() >= state.rows) {
            lost = 1;
        }
        if (!state.blocks_queue[i].empty()) {
            win = 1;
        }
    }
    if (lost == 1) {
        cout << "\n\nP R Z E G R A L E S\n\n";
        cout << "Czy chcesz sprobwac jeszcze raz? (T - tak, N - nie) ";
        cin >> input;
        switch (input) {
        case 'T': menu(); break;
        case 'N': exit(0); break;
        }
    }
    if (win == 0) {
        cout << "\n\nW Y G R A L E S\n\n";
        cout << "Czy chcesz sprobwac jeszcze raz? (T - tak, N - nie)";
        cin >> input;
        switch (input) {
        case 'T': menu(); break;
        case 'N': exit(0); break;
        }
    }
}