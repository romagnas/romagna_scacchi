#include <iostream>
using namespace std;

/*
    turno:
    - pari  = Nero
    - dispari = Bianco
*/
int turno = 1;

/*
    Scacchiera 8x8
    Riga 0 = lato Nero
    Riga 7 = lato Bianco
*/
string board[8][8] =
{
    {"♜","♞","♝","♛","♚","♝","♞","♜"},
    {"♟","♟","♟","♟","♟","♟","♟","♟"},
    {" "," "," "," "," "," "," "," "},
    {" "," "," "," "," "," "," "," "},
    {" "," "," "," "," "," "," "," "},
    {" "," "," "," "," "," "," "," "},
    {"♙","♙","♙","♙","♙","♙","♙","♙"},
    {"♖","♘","♗","♕","♔","♗","♘","♖"}
};

/* ---------------------------------------------------
   Stampa la scacchiera con colori alternati
--------------------------------------------------- */
void printBoard()
{
    for (int row = 0; row < 8; row++)
    {
        cout << 8 - row << "|";

        for (int col = 0; col < 8; col++)
        {
            if ((row + col) % 2 == 0)
                cout << "\033[100m";
            else
                cout << "\033[40m";

            cout << board[row][col] << " ";
        }

        cout << "\033[0m\n";
    }

    cout << "  A|B|C|D|E|F|G|H|\n";
}

// controlla se la pedina appartiene al giocatore corrente
bool isMyPiece(int col, int row)
{
    string piece = board[row][col];

    // Turno del Bianco
    if (turno % 2 == 1)
    {
        return (piece >= "♔" && piece <= "♙");
    }
    // Turno del Nero
    else
    {
        return (piece >= "♚" && piece <= "♟");
    }
}

// controlla se il percorso è libero
bool pathFree(int startCol, int startRow, int deltaCol, int deltaRow)
{
    int stepCol = (deltaCol > 0) ? 1 : (deltaCol < 0) ? -1 : 0;
    int stepRow = (deltaRow > 0) ? 1 : (deltaRow < 0) ? -1 : 0;

    int currentCol = stepCol;
    int currentRow = stepRow;

    while (currentCol != deltaCol || currentRow != deltaRow)
    {
        if (board[startRow + currentRow][startCol + currentCol] != " ")
            return false;

        currentCol += stepCol;
        currentRow += stepRow;
    }

    return true;
}

// controlla se la mossa è valida
bool validMove(int fromCol, int fromRow, int toCol, int toRow)
{
    string piece = board[fromRow][fromCol];

    int dCol = toCol - fromCol;
    int dRow = toRow - fromRow;

    int absCol = abs(dCol);
    int absRow = abs(dRow);

    if (absCol == 0 && absRow == 0)
        return false;
	//RE
    if (piece == "♔" || piece == "♚")
        return absCol <= 1 && absRow <= 1;
	//REGINA
    if (piece == "♕" || piece == "♛")
        return ((absCol == absRow) || (absCol == 0 || absRow == 0))
               && pathFree(fromCol, fromRow, dCol, dRow);
	//TORRE
    if (piece == "♖" || piece == "♜")
        return (absCol == 0 || absRow == 0)
               && pathFree(fromCol, fromRow, dCol, dRow);
	//ALFIERE
    if (piece == "♗" || piece == "♝")
        return absCol == absRow
               && pathFree(fromCol, fromRow, dCol, dRow);
	//CAVALLUCCIO
    if (piece == "♘" || piece == "♞")
        return (absCol == 2 && absRow == 1) ||
               (absCol == 1 && absRow == 2);
	//PEDONE
    if (piece == "♙")
    {
        if (dRow == -1 && absCol == 0 && board[toRow][toCol] == " ")
            return true;

        if (fromRow == 6 && dRow == -2 && absCol == 0 &&
            pathFree(fromCol, fromRow, dCol, dRow))
            return true;

        if (dRow == -1 && absCol == 1 && board[toRow][toCol] != " " && !isMyPiece(toCol, toRow))
	    return true;
    }
	//PEDONE 2
    if (piece == "♟")
    {
        if (dRow == 1 && absCol == 0 && board[toRow][toCol] == " ")
            return true;

        if (fromRow == 1 && dRow == 2 && absCol == 0 &&
            pathFree(fromCol, fromRow, dCol, dRow))
            return true;

        if (dRow == 1 && absCol == 1 && board[toRow][toCol] != " " && !isMyPiece(toCol, toRow))
            return true;
    }

    return false;
}

// gestisce la mossa dell'utente
bool makeMove()
{
    string move;
    cin >> move;
    string scelta;

	// Opzione uscita
    if (move == "q" || move == "quit")
    {
        cout << "Uscita dal gioco.\n";
        exit(0);
    }

    system("clear");

    int fromCol = move[0] - 'a';
    int fromRow = 7 - (move[1] - '1');
    int toCol   = move[2] - 'a';
    int toRow   = 7 - (move[3] - '1');

    if (board[fromRow][fromCol] == " ")
    {
        cout << "\033[31mNessuna pedina selezionata!\033[0m\n";
        return false;
    }

    if (!isMyPiece(fromCol, fromRow))
    {
        cout << "\033[31mPedina avversaria!\033[0m\n";
        return false;
    }

    if (!validMove(fromCol, fromRow, toCol, toRow))
    {
        cout << "\033[31mMossa non valida!\033[0m\n";
        return false;
    }

    board[toRow][toCol] = board[fromRow][fromCol];
    board[fromRow][fromCol] = " ";
    
    for (int i = 0; i < 8; i++)
    {
    	if (board[0][i] == "♙")
    	{
    		cout << "Scegli la pedina in cui trasformare il pedone:" << endl;
    		cin >> scelta;
    		
    		if (scelta == "regina")
    		{
    			board[0][i] = "♕";
			}
			if (scelta == "torre")
    		{
    			board[0][i] = "♖";
			}
			if (scelta == "cavallo")
    		{
    			board[0][i] = "♘";
			}
			if (scelta == "alfiere")
    		{
    			board[0][i] = "♗";
			}
		}
		if (board[7][i] == "♟")
    	{
    		cout << "Scegli la pedina in cui trasformare il pedone:" << endl;
    		cin >> scelta;
    		
    		if (scelta == "regina")
    		{
    			board[0][i] = "♛";
			}
			if (scelta == "torre")
    		{
    			board[0][i] = "♜";
			}
			if (scelta == "cavallo")
    		{
    			board[0][i] = "♞";
			}
			if (scelta == "alfiere")
    		{
    			board[0][i] = "♝";
			}
		}
	}

    return true;
}

// MAIN
int main()
{
    system("chcp 65001"); //unicode

    while (true)
    {
        printBoard();

        cout << "Turno: "
             << ((turno % 2) ? "\033[31mBianco" : "\033[34mNero")
             << "\033[0m\n";

        cout << "Inserisci mossa (es: e2e4): ";

        if (makeMove())
            turno++;
    }
}

