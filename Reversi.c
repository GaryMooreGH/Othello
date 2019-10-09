#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define MAX 20
// Definition of a struct in which we can return the necessary values.
struct input{
	int x;
	int y;
};
bool checkDirection2(int gamedata[][8],int P,int row,int column, int r, int c);
bool checkPosition(int x, int y);
bool checkDirection(int gamedata[][8],int P,int row,int column, int r, int c );
void beginGame(int gamedata[][8]); //Function to initialise the gamedata array
void printBoard(int gamedata[][8]); // Print out the current state of the gameboard
struct input insert(struct input data, int player); // Input of the chosen move for each player
bool checkValidMove(int gamedata[][8],struct input data, int player); // Check if the chosen move by the player is an acceptable one
void addData(int gamedata[][8], struct input data, int player);
int countSquares(int gamedata[][8], int player);
bool checkMoveAvailable(int gamedata[][8], int P); // Check if there is a move available for the player
void listMoveAvailable(int gamedata[][8], struct input possiblemoves[10], int P);
bool checkIfFound(int tempx, int tempy, struct input possiblemoves[10]);
bool checkDirection3(int gamedata[][8],int P,int row,int column, int r, int c);

int main(void)
{
	int length;
	char Player1[MAX];
	char Player2[MAX];

	int remain = 60;
	int white = 2, black = 1;
	int gamedata[8][8], player = 2;
	struct input data;
	int whitecounter = 0;
	int blackcounter = 0;
	struct input possiblemoves[10]; // Create an array of structs;

	printf("Please enter Player 1: \n");
    fgets(Player1,MAX, stdin);
    length = strlen(Player1);
    Player1[length-1] = '\0';

    printf("Please enter Player 2: \n");
    fgets(Player2,MAX, stdin);
    length = strlen(Player2);
    Player2[length-1] = '\0';

    printf("\n%s is playing as White!\n%s is Black and will go first\n\n",Player2,Player1);

    printf("Black is represented as '2' and\nWhite is represented by '1' on the board\nEmpty spaces appear as '0'\n\n");

	beginGame(gamedata);
	printBoard(gamedata);

	//If either player can make a move continues game
	while((checkMoveAvailable(gamedata,white) == true || checkMoveAvailable(gamedata,black) == true))
	{
		//Skips current players turn if they can't make a move
		if((checkMoveAvailable(gamedata,player)) == false)
		{
			printf("You have no moves available!\n Your move will be passed to the other player!\n");
			if(player == 1)
			{
				player = 2;
			}
			else
			{
				player = 1;
			}
		}

		listMoveAvailable(gamedata,possiblemoves,player);

		data = insert(data, player);
		addData(gamedata,data,player);
		remain--;

		//Swaps players at end of turn
		printBoard(gamedata);
		if(player == 1)
		{
			player = 2;
		}
		else
		{
			player = 1;
		}

	// Count the number of squares of each colour
	whitecounter = countSquares(gamedata, white);
	blackcounter = countSquares(gamedata, black);

	printf("%s has %d counters; %s has %d\n", Player1, blackcounter,Player2, whitecounter);
	printf("There are %d counters left to be placed\n", remain);
	}

	printf("The game has ended!\n");
	if(blackcounter>whitecounter)
	{
		printf("%s has won!\n", Player1);
	}
	else if(whitecounter>blackcounter)
	{
		printf("%s has won!\n",Player2 );
	}
	else
	{
		printf("The game was a Draw!\n");
	}

	return 0;
}

void beginGame(int gamedata[][8]) //Function to initialise the gamedata array
{
	int i, j;

	// For loops run at start of the game in order to give initial values to each array position.
	// Each place can have three values, 0 for a square which has not been used yet, 1 for white
	// 2 for black. j = along the y axis of board. i = along the x axis of board
	for(j = 0; j < 8;j++)
	{
		for(i = 0; i < 8; i++)
		{
			// If and else if statements to set up the middle 4 squares
			if((j == 3 && i == 3) || (j == 4 && i == 4))
			{
				gamedata[j][i] = 1;
			}
			else if((j == 3 && i == 4) || (j == 4 && i == 3))
			{
				gamedata[j][i] = 2;
			}

			// Else statement to set other squares to value 0;
			else
			{
				gamedata[j][i] = 0;
			}
		}
	}
}
//Function to print board and coordinates of axis
void printBoard(int gamedata[][8])
{
	int i, j;
	int number = 1;

	printf(" |1|2|3|4|5|6|7|8|\n");

	for(j = 0; j < 8; j++)
	{
		printf("%d|", number);
		for(i = 0; i < 8; i++)
		{
			printf("%d|",gamedata[j][i]);
		}
		printf("%d\n", number);
		number++;
	}

	printf(" |1|2|3|4|5|6|7|8|\n");

}
//Function to get players next move
struct input insert(struct input data, int player)
{
	printf("Input a move for Player %d please :\n", player);

	printf("Select a number across x axis: ");
	scanf("%d", &data.x);
	printf("Select a number down y axis: ");
	scanf("%d", &data.y);

	data.x--;
	data.y--;

	return data;
}
//Function to  check if a move from player input is valid
bool checkValidMove(int gamedata[][8],struct input data, int player)
{
	int x = data.x, y = data.y;
	bool temp;

	// If the square inputted by the square contains no colour
	if(gamedata[y][x] == 0)
	{
		//Checks around the position chosen for counters of opposite colour
		 for(int Row=-1;Row<=1;Row++)
         {
            for(int Col=-1;Col<=1;Col++)
            {
           		if(Row==0 && Col==0);

                else if(checkPosition(x,y) && (gamedata[y + Row][x +Col] != player) && (gamedata[y + Row][x +Col] != 0))
                {
                	//If counter of opposite colour is found uses checkDirection to see if a move can be made
                    if(checkDirection2(gamedata,player,(y +Row),(x + Col),Row, Col) == true)
                    {
                    	return true;
                    }
                }
            }
		}
	}
	return false;
}
//Checks if there is any move a player can make
void listMoveAvailable(int gamedata[][8], struct input possiblemoves[10], int P)
{
	bool temp, result = false;
	int counter = 0,OppRow,OppCol; // Counter for struct array position;
	//Goes through the array till the opposite colour is found
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(gamedata[i][j] != P && gamedata[i][j] != 0)
            {
							printf("OppColour\n" );
            	//Checks around opposite colour for an empty space
                for(int Row=-1;Row<=1;Row++)
                {
                    for(int Col=-1;Col<=1;Col++)
                    {
                    	OppRow = Row*(-1);
                    	OppCol = Col*(-1);
                        if(gamedata[i+Row][j+Col] == 0)
                        {
                        	//Uses check direction to follow the opposite way to see if moves can be made
                        	if(checkDirection3(gamedata,P,(i+OppRow),(j+OppCol), OppRow, OppCol) == true)
                        	{
															possiblemoves[counter].x = (j+Row);
															possiblemoves[counter].y = (i+Col);
															counter++;
                        	}
                        }
                    }
                }
            }
        }
    }
		int i = 0;
		while(i<counter)
		{
			printf("%d.(%d,%d)\n", i+1, possiblemoves[i].x+1, possiblemoves[i].y+1);
			i++;
		}
}

bool checkMoveAvailable(int gamedata[][8], int P)
{
	bool temp, result = false;
	int counter = 0,OppRow,OppCol; // Counter for struct array position;
	//Goes through the array till the opposite colour is found
	for(int i=0;i<8;i++)
	{
			for(int j=0;j<8;j++)
			{
					if(gamedata[i][j] != P && gamedata[i][j] != 0)
					{
						//Checks around opposite colour for an empty space
							for(int Row=-1;Row<=1;Row++)
							{
									for(int Col=-1;Col<=1;Col++)
									{
										OppRow = Row*(-1);
										OppCol = Col*(-1);
											if(gamedata[i+Row][j+Col] == 0)
											{
												//Uses check direction to follow the opposite way to see if moves can be made
												if(checkDirection3(gamedata,P,(i+OppRow),(j+OppCol), OppRow, OppCol) == true)
												{
													return true;
												}

											}

									}
							}
					}
			}
	}
	return false;
}
bool checkDirection3(int gamedata[][8],int P,int row,int column, int r, int c)
{
	int count=0;

	while(checkPosition((row + (count*r)),(column + (count * c))) && gamedata[row + (count*r)][column + (count * c)] != 0)
	{
		if(checkPosition((row + (count*r)),(column + (count * c))) && gamedata[row + (count*r)][column + (count * c)] == P)
			{
				printf("Row: %d Col: %d\n",row + (count*r),column + (count * c) );
				return true;
			}
		count++;
	}
	//printf("NoMove\n");
	return false;
}
//Function checks if there is an empty space a players counter could be placed after a series of opposite colours
bool checkDirection(int gamedata[][8],int P,int row,int column, int r, int c)
{
	int count=0;

	while(checkPosition((row + (count*r)),(column + (count * c))) && gamedata[row + (count*r)][column + (count * c)] != P && gamedata[row + (count*r)][column + (count * c)] != 0)
	{
		count++;
		if(checkPosition((row + (count*r)),(column + (count * c))) && gamedata[row + (count*r)][column + (count * c)] == 0) // Need to add something to the check position function
			{
				return true;
			}
	}
	printf("NoMove\n");
	return false;
}
//Function checks if there is another counter the players colour after a number of the opposite colour counters
bool checkDirection2(int gamedata[][8],int P,int row,int column, int r, int c)
{
	int count=0;
	printf("Entered Check212\n");
	printf("Row: %d, Column: %d\n", row, column );

	while(checkPosition((row + (count*r)),(column + (count * c))) && gamedata[row + (count*r)][column + (count * c)] != 0 && gamedata[row + (count*r)][column + (count * c)] != P)
	{
		count++;
		if(checkPosition((row + (count*r)),(column + (count * c))) && gamedata[row + (count*r)][column + (count * c)] == P) // Need to add something to the check position function
			{
				return true;
			}
	}
	return false;

}
//Function checks if the postion is within the board
bool checkPosition(int x,int y)
{
	if(x<=8 && x>0 && y<=8 && y>0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Adds counter to table and swaps necessary colours
void addData(int gamedata[][8], struct input data, int player)
{
	bool temp;
				//Checks around placed counterfor opposite colour
                for(int Row=-1;Row<=1;Row++)
                {
                    for(int Col=-1;Col<=1;Col++)
                    {
                        if(Row==0 && Col==0);

                        else if(gamedata[data.y + Row][data.x + Col] != player && gamedata[data.y + Row][data.x + Col] != 0)
                        {
                        	//Follows opposite colour and if reaches another chip of the players colour swaps inbetween
                        	if(checkDirection2(gamedata,player,(data.y + Row),(data.x + Col), Row, Col))
                        	{
                        		int count = 1;
                        		while(gamedata[data.y + Row*count][data.x + Col*count] != player )
                        		{
                        			gamedata[data.y + Row*count][data.x + Col*count] = player;
                        			count++;
                        		}
                        	}
                        }
                    }
                }

	gamedata[data.y][data.x] = player;
}
//function used to count squares on the board of a particular colour
int countSquares(int gamedata[][8], int player)
{
	int i, j, count = 0;
	//Goes through array and increases count if players "colour" is contained
	for(j = 0; j < 8; j++)
	{
		for(i = 0; i <8; i++)
		{
			if(gamedata[j][i] == player )
			{
				count++;
			}
		}
	}

	return count;
}

// Function to check if co-ordinates are already in the possible array.
bool checkIfFound(int tempx, int tempy, struct input possiblemoves[10])
{
	int i, j;

	for(i = 0; i < 10; i++)
	{
		if(tempx == possiblemoves[i].x)
		{
			for(j = 0; j < 10; j++)
			{
				if(tempy == possiblemoves[j].y)
				{
					//printf("ran %d %d\n", tempx, tempy);
					return true;
				}
			}
		}
	}
	return false;
}
