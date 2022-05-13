#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <vector>
using namespace std;
//--------------------------------------------------------------------
//Externals
//--------------------------------------------------------------------
std::ofstream tfs("board2.txt");
//--------------------------------------------------------------------
//BadSuffixException
//--------------------------------------------------------------------
struct BadSuffixException {
    BadSuffixException(char* fn)
    {   cout << endl;
        cout << "Input file \"" << fn << "\" does not have a .mcr suffix.";
    }
};
//--------------------------------------------------------------------
//CommandLineException
//--------------------------------------------------------------------
struct CommandLineException {
    CommandLineException(int m,int a)
    {   cout << endl;
        cout << "Too many arguments on the command line.";
        cout << endl;
        cout << m << " argument(s) are permitted on the command line.";
        cout << endl;
        cout << a << " argument(s) appeared on the command line.";
        cout << endl;
    }
};
//--------------------------------------------------------------------
//FileException
//--------------------------------------------------------------------
struct FileException {
    FileException(const char* fn)
    {   cout << endl;
        cout << "File " << fn << " could not be opened.";
        cout << endl;
    }
};
//--------------------------------------------------------------------
int positions_evaluated = 0;
int ab_estimate = 0;
struct Node {
    int miniMaxEstimate;
    vector<char> board;
    Node(int x, vector<char> board): miniMaxEstimate(x), board(board){
    }
};

vector<char> maxMinABOpeningPhase(vector<char> board, int depth, int alpha, int beta);
vector<char> minMaxABOpeningPhase(vector<char> board, int depth, int alpha, int beta);
vector<char> swapB(vector<char> x){
    vector<char> board = x;
    for(int i=0; i< x.size(); i++){
        if(board[i] == 'W'){
            board[i] = 'B';
            continue;
        }
        if(board[i] == 'B'){
            board[i] = 'W';
        }
    }
    return board;
}

vector<int> neighbors(int i){
    switch(i){
        case 0:
            return {1, 2, 6};
        case 1:
            return {0, 3, 11};
        case 2:
            return {0, 7, 3, 4};
        case 3:
            return {1, 2 , 10};
        case 4:
            return {2, 5, 8};
        case 5:
            return {3, 4, 9};
        case 6:
            return {0, 7, 18};
        case 7:
            return {6, 8, 2, 15};
        case 8:
            return {7, 4, 12};
        case 9:
            return {10, 5, 14};
        case 10:
            return {9, 11, 3, 17};
        case 11:
            return {1, 10, 20};
        case 12:
            return {8, 13};
        case 13:
            return {12, 14, 16};
        case 14:
            return {13, 9};
        case 15:
            return {7, 16};
        case 16:
            return {15, 13, 17, 19};
        case 17:
            return {10, 16};
        case 18:
            return {6, 19};
        case 19:
            return {18, 16, 20};
        case 20:
            return {11, 19};
        default:
            return {};
    }
}

bool closeMill(int pos, vector<char> board){
    switch(pos){
        case 0:
            if((board[0] == board[2] && board[2] == board[4]) || (board[6] == board[0] && board[18] == board[0])){
                return true;
            } else return false;
        case 1:
            if((board[1] == board[3] && board[5] == board[1]) || (board[1] == board[11] && board[20] == board[1])){
                return true;
            } else return false;
        case 2:
            if((board[0] == board[2] && board[2] == board[4]) || (board[2] == board[7] && board[15] == board[7])) {
                return true;
            } else return false;
        case 3:
            if((board[1] == board[3] && board[5] == board[1]) || (board[3] == board[10] && board[17] == board[3])){
                return true;
            } else return false;
        case 4:
            if((board[0] == board[2] && board[2] == board[4]) || (board[4] == board[8] && board[12] == board[4])){
                return true;
            } else return false;
        case 5:
            if((board[1] == board[3] && board[5] == board[1]) || (board[5] == board[9] && board[14] == board[5])){
                return true;
            } else return false;
        case 6:
            if((board[0] == board[6] && board[18] == board[0]) || (board[6] == board[7] && board[8] == board[6])){
                return true;
            } else return false;
        case 7:
            if((board[2] == board[7] && board[15] == board[2]) || (board[6] == board[7] && board[8] == board[6])){
                return true;
            } else return false;
        case 8:
            if((board[6] == board[7] && board[8] == board[6]) || (board[4] == board[8] && board[12] == board[4])){
                return true;
            } else return false;
        case 9:
            if((board[5] == board[9] && board[14] == board[5]) || (board[9] == board[10] && board[11] == board[9])){
                return true;
            } else return false;
        case 10:
            if((board[3] == board[10] && board[17] == board[3]) || (board[9] == board[10] && board[11] == board[9])){
                return true;
            } else return false;
        case 11:
            if((board[1] == board[11] && board[20] == board[1]) || (board[9] == board[10] && board[11] == board[9])){
                return true;
            } else return false;
        case 12:
            if((board[4] == board[8] && board[12] == board[4]) || (board[12] == board[13] && board[14] == board[12])){
                return true;
            } else return false;
        case 13:
            if((board[12] == board[13] && board[14] == board[13]) || (board[13] == board[16] && board[19] == board[13])){
                return true;
            } else return false;
        case 14:
            if((board[12] == board[13] && board[14] == board[12]) || (board[5] == board[9] && board[14] == board[5])) {
                return true;
            } else return false;
        case 15:
            if((board[2] == board[7] && board[15] == board[2]) || (board[15] == board[16] && board[17] == board[15])){
                return true;
            } else return false;
        case 16:
            if((board[15] == board[16] && board[17] == board[15]) || (board[13] == board[16] && board[19] == board[13])){
                return true;
            } else return false;
        case 17:
            if((board[15] == board[16] && board[17] == board[15]) || (board[3] == board[10] && board[17] == board[3])){
                return true;
            } else return false;
        case 18:
            if((board[0] == board[6] && board[18] == board[0]) || (board[18] == board[19] && board[20] == board[19])){
                return true;
            } else return false;
        case 19:
            if((board[18] == board[19] && board[20] == board[18]) || (board[13] == board[16] && board[19] == board[13])){
                return true;
            } else return false;
        case 20:
            if((board[18] == board[19] && board[20] == board[18]) || (board[1] == board[11] && board[20] == board[1])){
                return true;
            } else return false;
        default:
            return false;
    }
}


void generateRemove(vector<char>& board, vector<vector<char>> nextStates){
    int size = nextStates.size();
    for(int i=0; i < board.size(); i++){
        if(board[i] == 'B'){
            if(!(closeMill(i, board))){
                vector<char> b = board;
                b[i] = 'x';
                nextStates.push_back(b);
            }
        }
    }
    if(size == nextStates.size()){
        for(int j =0; j < board.size(); j++){
            if(board[j] == 'B'){
                vector<char> b = board;
                b[j] = 'x';
                nextStates.push_back(b);
            }
        }
    }

}
vector<vector<char>> generateAdd(vector<char>& board){
    vector<vector<char>> nextStates;
    for(int i=0; i < board.size(); i++){
        if(board[i] == 'x'){ 
            vector<char> b = board;
            b[i] = 'W';
            if(closeMill(i, b)) {
                generateRemove(b, nextStates);
            } else{
                nextStates.push_back(b);
            }
        }
    }
    return nextStates;
}

vector<vector<char>> generateMove(vector<char>& board){
    vector<vector<char>> nextMoves;
    for(int i=0; i < board.size(); i++){
        if(board[i] == 'W'){
            vector<int> n = neighbors(i);
            for(int j : n){
                if(board[j] == 'x'){
                    vector<char> b = board;
                    b[j] = 'W';
                    if(closeMill(j, b)) generateRemove(b, nextMoves);
                    else{
                        nextMoves.push_back(b);
                    }
                }
            }
            
        }
    }
    return nextMoves;
}

vector<vector<char>> generateBlackMoves(vector<char> board){
    vector<char> b(board.begin(), board.end());
    for(int i=0; i < b.size(); i++){
        if(b[i] == 'W'){
            b[i] = 'B';
            continue;
        }
        if(b[i] == 'B'){
            b[i] = 'W';
        }
    }
    vector<vector<char>>gbmswap;

    vector<vector<char>> gbm = generateAdd(b);
    
    for(vector<char> y: gbm){
        vector<char> b1(y.begin(), y.end());
        for(int i=0; i < b1.size(); i++){
            if(b1[i] == 'W'){
                b1[i] = 'B';
                continue;
            }
            if(b1[i] == 'B'){
                b1[i] = 'W';
            }
        }
        gbmswap.push_back(y);
    }
    
    return gbmswap;
}

int staticEstimationOpeningPhase(vector<char> board){
    int numBlackPieces =0;
    int numWhitePieces =0;
    for(int i =0; i < board.size(); i++){
        if(board[i] == 'W'){
            ++numWhitePieces;
        } else if(board[i] == 'B'){
            ++numBlackPieces;
        }
    }
    return numWhitePieces - numBlackPieces;
}


vector<char> minMaxABOpeningPhase(vector<char> board, int depth, int alpha, int beta){
    if(depth > 0){
        cout << "Current depth at MinMax is" << depth;
        depth--;
        vector<char>maxBoard;
        vector<char> minBoardChoice(50);
        vector<vector<char>> bchildren = generateBlackMoves(board);
        for(vector<char> n: bchildren){
            std::string s(n.begin(), n.end());
            cout << "the posible moves for black are: " << s << endl;
        }
        int v = 999999;
        for(int i=0; i < bchildren.size(); i++){
            maxBoard = maxMinABOpeningPhase(bchildren[i], depth, alpha, beta);
            if(v > staticEstimationOpeningPhase(maxBoard)){
                v = staticEstimationOpeningPhase(maxBoard);
                minBoardChoice = bchildren[i];
            }
            if(v <= alpha){
                return minBoardChoice;
            }
            else {
                beta = min(v, beta);
            }
        }
        return minBoardChoice;
    } else if(depth == 0){
        positions_evaluated++;
    }
    return board;
}

vector<char> maxMinABOpeningPhase(vector<char> board, int depth, int alpha, int beta){
    if(depth > 0){
        cout << "current depth at maxMin is " << depth << endl;
        depth--;
        vector<char> minBoard;
        vector<char> maxBoardChoice(50);
        vector<vector<char>> children = generateAdd(board);
        for(vector<char> c: children){
            std::string s(c.begin(), c.end());
            cout << "the possible moves for white are: " << s  << endl;
        }
        int v = -999999;
        for(int i=0; i < children.size(); i++){
            minBoard = minMaxABOpeningPhase(children[i], depth, alpha, beta);
            if(v < staticEstimationOpeningPhase(minBoard)){
                v = staticEstimationOpeningPhase(minBoard);
                ab_estimate = v;
                //tfs << "v" << v << ab_estimate;
                maxBoardChoice = children[i];
            }
            if(v >= beta){
                return maxBoardChoice;
            } else{
                alpha = max(v, alpha);
            }
        }
        return maxBoardChoice;
    } else if(depth == 0){
        positions_evaluated++;
    }
    return board;
}

void StateMgr(ifstream& i, int depth)
{   
    char c;
    int x = -999999, y = 999999;
    string s;
    while(i){
        std::getline(i,s);
        if( i.eof() ) break;
        std::vector<char> v(s.begin(), s.end());
        vector<char> d = maxMinABOpeningPhase(v, depth, x, y);
        cout << positions_evaluated << endl;
        std::string str(d.begin(), d.end());
        tfs << "Input state: " << s << endl;
        tfs << "Output state: "  << str << endl;
        tfs << "States evaluated by static estimation: " << positions_evaluated << endl;
        tfs << "MINIMAX estimate: " << ab_estimate << endl;
    }
}

//--------------------------------------------------------------------
//Function main processes command line arguments
//--------------------------------------------------------------------
int main(int argc,char* argv[])
{   try {
        char ifn[255];
        char ofn[255];
        int depth;
        switch (argc) {

            case 1: //Prompt for the input file name
                cout << "Enter the input file name: ";
                cin >> ifn;
                cout << "Enter the output file name: ";
                cin >> ofn;
                cout << "Enter the depth to search: ";
                cin >> depth;
                break;
            case 2: //Read the input file name
                strcpy(ifn,argv[1]);
                cout << "Enter the output file name: ";
                cin >> ofn;
                cout << "Enter the depth to search: ";
                cin >> depth;
                break;
            case 3: //Read the input file name
                strcpy(ifn,argv[1]);
                strcpy(ofn,argv[2]);
                cout << "Enter the depth to search: ";
                cin >> depth;
                break;
            case 4:
                strcpy(ifn,argv[1]);
                strcpy(ofn,argv[2]);
                depth = atoi(argv[3]);
                break;
            default:
                throw CommandLineException(3,argc-1);
                break;
        }
        char tfn[255];
        //FILE* i=fopen(ifn,"r"); //Open the input file
        ifstream file;
        file.open(ifn);
        if (!file) throw FileException(ifn);
        //tfs.open(tfn); if (!tfs) throw FileException(tfn);
        StateMgr(file, depth);
        tfs << endl; //Put a new line in the trace file
        tfs.close(); //Close the trace file
        file.close(); //Close the input file
    } catch (...) {
        cout << endl;
        cout << "Program terminated!" << endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}
