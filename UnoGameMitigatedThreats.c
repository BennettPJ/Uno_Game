
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct card_s {
    char color[8];
    int value;
    char action[15];
    struct card_s* pt;
} card;

void ReadDeck(FILE* inp, card* deck); //Reads the deck from a text file if the user desires

int CheckFileContents(const char *filename);

void initDeck(card* deck); // Initializes the deck as an array

void shuffleDeck(card* deck); //Shuffles the deck

void playerHandinit(card** player1, card** player2, card* deck, int numPlayers, int* curCard, int* discardIndex); //Initializes the player's hand as a linked list

void playerAddCard(int* discardPileIndex, int* curCard, card* deck, card** head); //Adds a node to the player's hand/linked list

void playerSubtractCard(card** head, card* deck, int listIndex, int* discardIndex); //Removes a node from the player's hand/linked list

int cardCount(card* player); //Counts the amount of cards in a player's hand

void mainGame(card* deck, int* curCard, int* discardPileIndex, card** player, card** playerother, int playerNumber, int* drawCards); //The main game

//All of the finction calls above are initializing the functions so that they can be used in the main. By having the main function about the rest this makes the code easier to read since the user may only care about the main function


int main(void) {
    srand((unsigned int)time(0)); //This seeds the rand to the current time so that there is alway a new random deck when shuffled
    FILE* playerDeck; //This is the user inputed file
    int curCardIndex = 0; //This is the index of where the cards are being pulled from in the deck
    int discardPileIndex = 0; //This is the index of ther discard pile which is the same as the deck they are just going back into the freeed deck space
    int drawCards = 0; //This is a variable to see how many cards a player has to draw
    int shuffleOrNo = 0; // This is the variable that checks to see if the user wants to use a preloaded deck or their own
    int numberPlayers = 2; //This is the variable that tells the number of players
    char userFile[50]; //This is where the users file name is stored
    int dealCards = 0; //This is the variable that waits for the user input if they want to deal the cards
    int result = 0;
    char contPlay = 'y';
    card* player1 = NULL; //This creates player 1 that can store cards in their hand
    card* player2 = NULL; //This creates player 2 that can store cards in their hand
    card deck[108]; //This creates the deck


    printf("Lets play a game of Uno!\n");


    
    printf("Press 1 to shuffle the Uno deck or 2 to load deck from file: ");
    result = scanf("%d", &shuffleOrNo);
    while (result != 1 || (shuffleOrNo != 1 && shuffleOrNo != 2)) {  // Check if scanf successfully read the number
        while (getchar() != '\n');  // Clear the input buffer until a newline is encountered
        printf("Invalid input. Please press 1 to shuffle the Uno deck or 2 to load deck from file: ");
        result = scanf("%d", &shuffleOrNo);
    }//This makes sure that the user enters a valid number so that they can get the correct input
    //SFWE 401 threat 9 mitigated

    if (shuffleOrNo == 1) { //If the user wants to shuffle the deck they press 1
        initDeck(deck); //This initializes the deck if the players don't want to uplaod their own deck
        shuffleDeck(deck); //This calls the shuffle deck function to shuffle the ordered deck
        printf("The deck is shuffled! "); //Lets the user know that the deck was successfuly shuffled
    }
    else {//This is if the user wants to load in their own deck from their own file
        printf("Please enter a file name: "); //Prompts user to enter their file name
        scanf("%s", userFile); //This scans the user files name
        playerDeck = fopen(userFile, "r"); //This opens their file in read mode
        result = CheckFileContents(userFile);
        if (result == 1) {
            printf("File contents are valid reading file...\n");
        } else if (result == 0) {
            printf("File contents are invalid. Closing game\n");
            exit(1);
        } 
        while (playerDeck == NULL) {
            printf("No file found please enter a valid file name: ");
            scanf("%s", userFile); //This scans the user files name
            int result = CheckFileContents(userFile);
            if (result == 1) {
                printf("File contents are valid reading file...\n");
            } else if (result == 0) {
                printf("File contents are invalid. Closing game\n");
                exit(1);
            } 
            playerDeck = fopen(userFile, "r"); //This opens their file in read mode
        }
        ReadDeck(playerDeck, deck); //This sends their file to the ReadDeck function
    }


    printf("Press 1 to deal cards!\n"); //SFWE401 Threat 1 mitiagted and SFWE401 Threat 9 mitigated
    result = scanf(" %d", &dealCards);  // Attempt to read an integer
    while (result != 1 || dealCards != 1) {  // Check if scanf successfully read the number
        while (getchar() != '\n');  // Clear the input buffer until a newline is encountered
        printf("Invalid input. Please press 1 to deal cards: ");
        result = scanf("%d", &dealCards);
    }
    playerHandinit(&player1, &player2, deck, numberPlayers, &curCardIndex, &discardPileIndex);


    if (dealCards != '\0') { //This checks to see if the user pressed any key and then if they did a hand will be dealt to both players
        playerHandinit(&player1, &player2, deck, numberPlayers, &curCardIndex, &discardPileIndex);
    }//This passes each player and the deck to a function that initializes each players hand


    while (contPlay == 'y') { //This will be the main game loop to see if the players want to continue playing
        if (deck[discardPileIndex].value >= 10) {
            shuffleDeck(deck);
            playerHandinit(&player1, &player2, deck, numberPlayers, &curCardIndex, &discardPileIndex);
        }//This makes sure that the game won't start on a special card like in the rules and then re distributes cards to each player
        while (cardCount(player1) && cardCount(player2) != 0) {
            mainGame(deck, &curCardIndex, &discardPileIndex, &player1, &player2, 1, &drawCards); //This calls the main game function where all of the players moves happen
            mainGame(deck, &curCardIndex, &discardPileIndex, &player2, &player1, 2, &drawCards); //This calls the main game function where all of the players moves happen
        }//This is the second loop of the game this goes as long as each player still has cards in their hand. When this loop ends this means a player won the game


        if (cardCount(player1) == 0) {
            printf("Player 1 won!\n");
        } //If player 1 has no cards left in their hand then they won
        else {
            printf("Player 2 won!\n");
        } //If player 2 has no cards left in their hand then they won


        printf("Would you like to play again (y/n)? ");
        scanf(" %c", &contPlay);
        //This checks to see if the players want to play the game again. If they press n then the program will terminate if they press y the game will continue

        shuffleDeck(deck); //This calls the shuffle deck function to shuffle the ordered deck
        playerHandinit(&player1, &player2, deck, numberPlayers, &curCardIndex, &discardPileIndex); //This will give the players new hands to play with for the next round
    }

    printf("Bye bye!\n"); //This is the very end of the program

    return 0;
}

void mainGame(card* deck, int* curCard, int* discardPileIndex, card** player, card** playerOther, int playerNumber, int* drawCards) {
    int discLoop = 0; //This is the variable for our discard loop
    card* temp = NULL; //This is the variable to iterate through each players hand
    int playerAct = 0; //This is the input from the user when asked to play a card
    char newColor[8]; //These are the colors of the cards
    int playerHasPlusTwo = 0; //This checks to see if the next player has a plus two if the previous player place one down
    int i = 0;
    int result;
    char userInput; //This gets user input for the extra credit option
    int card = 0;

mainGameBeginning: //This is a marking so we can use the goto function later in our code
    if (*drawCards != 0 && strcmp(deck[*discardPileIndex].action, "plus two") == 0) { //This if statment makes sure that the action card is +2 and out draw cards variable drawCards is not equal to 0 this means a +2 was played and it sees if the other player has another +2 to stack.
        temp = *player; //This sets temp equal to whichever player is passed into the finction
        while (temp != NULL) { //This is while the pointer is not null or the last card in the list
            if (strcmp(temp->action, "plus two") == 0) {
                playerHasPlusTwo = 1; // Set this boolean to true
            }
            temp = temp->pt; // Go to the next card in the hand
        }
        if (playerHasPlusTwo != 0) { //This means that if the player has a +2 card in their hand
            printf("\nDiscard pile: ");
            for (discLoop = 0; discLoop <= *discardPileIndex; discLoop++) {
                if (discLoop == *discardPileIndex) {//This will print only the top most card in the discard pile
                    if (deck[*discardPileIndex].value <= 9) {
                        printf("%d %s ", deck[*discardPileIndex].value, deck[*discardPileIndex].color);
                    } //If the top most card is a regular card it prints the value and color
                    else {
                        printf("%s %s ", deck[*discardPileIndex].action, deck[*discardPileIndex].color);
                    } //If the top most card is a special card it prints the action and color
                }
            }//This prints the top most card on the discard pile and it checks to see if there is a wildcard played. If there is then the user is prompted to enter a card color
            printf("\n\n");


            printf("Player %d hand: ", playerNumber);
            temp = *player;
            while (temp != NULL) { // Iterate over player hand.
                if (temp->value >= 10) { // If it's a special card
                    printf(" %s %s |", temp->action, temp->color);
                }
                else {
                    printf(" %d %s |", temp->value, temp->color);
                }//This prints the regular card in the
                temp = temp->pt; // Next card in hand
            }//This prints player's hand
            printf("\n");
            printf("\nPlease input the plus two you would like to play or press 0 to lose a turn and draw 2 cards: ");
            scanf("%d", &playerAct);
            playerAct--; // Set it back to 0 based numbers.
            if (playerAct == -1) { //If the player wants to draw a card
                for (i = 0; i < *drawCards; i++) {
                    playerAddCard(discardPileIndex, curCard, deck, player); //Draw cards until drawCards is reached, the amount the player has to draw after a plus two/four is played
                }
                *drawCards = 0;
                return;
            }
            else {
              // Iterate to the card then play it.
                temp = *player;
                card = playerAct;
                while (card != 0) {
                    temp = temp->pt;
                    card--;
                }
            }
            while (temp->value != 12) { //If the player does not play a plus two, keep looping and ask them to play a plus two or draw cards
                printf("\nPlease input the plus two you would like to play or press 0 to lose a turn and draw 2 cards: ");
                scanf("%d", &playerAct);
                playerAct--;
                if (playerAct == -1) {
                    for (i = 0; i < *drawCards; i++) {
                        playerAddCard(discardPileIndex, curCard, deck, player); //If the player chooses to draw cards, draw drawCards amount of cards
                    }
                    *drawCards = 0;
                    return;
                }
                else {
                    temp = *player;
                    card = playerAct;
                    while (card != 0) {
                        temp = temp->pt;
                        card--;
                    }
                }
            }
            *drawCards += 2;
            playerSubtractCard(player, deck, playerAct, discardPileIndex);
          //If the player plays a plus two on top of another, add 2 to the amount of cards the next player might need to draw and remove the card from their hand
            return;
        }
        else {
            for (i = 0; i < *drawCards; i++) {
                playerAddCard(discardPileIndex, curCard, deck, player);
            }
            *drawCards = 0;
            return;
          //If the player does not have a plus two they must draw cards
        }
    }
    else if (*drawCards != 0) { //If the draw cards is not 0 and there are no more +2 cards to be played then the unlucky player has to draw that ammount of cards
        for (i = 0; i < *drawCards; i++) {
            playerAddCard(discardPileIndex, curCard, deck, player); //This makes the player add that amount of cards
        }
        *drawCards = 0; //This resets the draw cards variable
        return; //This skips the players turn
    }

    printf("\nDiscard pile: ");
    for (discLoop = 0; discLoop <= *discardPileIndex; discLoop++) {
        if (discLoop == *discardPileIndex) {
            if (deck[*discardPileIndex].value <= 9) {
                printf("%d %s ", deck[*discardPileIndex].value, deck[*discardPileIndex].color);
            }
            else {
                printf("%s %s ", deck[*discardPileIndex].action, deck[*discardPileIndex].color);
            }
        }
    }//This prints the top most card on the discard pile and it checks to see if there is a wildcard played. If there is then the user is prompted to enter a card color
    printf("\n\n");


    printf("Player %d hand: ", playerNumber);
    temp = *player;
    while (temp != NULL) {
        if (temp->value >= 10) {
            printf(" %s %s |", temp->action, temp->color);
        }
        else {
            printf(" %d %s |", temp->value, temp->color);
        }
        temp = temp->pt;
    }//This prints player's hand
    printf("\n");

    //cardCount() checks how many cards a player has and returns that number
    printf("\nPress 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", cardCount(*player));
    result = scanf("%d", &playerAct);
    while (result != 1) {  // Check if scanf successfully read the number
        while (getchar() != '\n');  // Clear the input buffer until a newline is encountered
        printf("Invalid input. Please press 1-%d to play card from hand: ", cardCount(*player));
        result = scanf("%d", &playerAct);
    }//SFWE 401 threat 9 mitigated
    
    playerAct--;
    if (playerAct == -1) {
        playerAddCard(discardPileIndex, curCard, deck, player);
    }//This is for if the player wants to draw a card
    else {
        temp = *player;
        int card = playerAct;
        while (card) {
            temp = temp->pt;
            card--;
        }//This iterates through the players hand
        //-1 draws a card for the player, anything else lets them play the card
    }
    if (playerAct != -1) {
        while ((strcmp(temp->color, deck[*discardPileIndex].color) != 0) && (temp->value != deck[*discardPileIndex].value) && temp->value != 13) { //This checks the colors to see if they are not matching and that the values of the card and discard pile are different and that its not a wild or a wild +4
            printf("%d %s cannot be placed on a %d %s", temp->value, temp->color, deck[*discardPileIndex].value, deck[*discardPileIndex].color); //Tells the user that they cant place this card
            printf("\n");
            printf("Please select another card: "); //It continuosly asks for a player to chose another card until they chose the correct card
            printf("\nPress 1-%d to play any card from your hand, or press zero to draw a card from the deck: ", cardCount(*player)); //This tells them which numbers they can press
            result = scanf("%d", &playerAct);
            while (result != 1) {  
                while (getchar() != '\n');  
                printf("Invalid input. Please press 1-%d to play card from hand: ", cardCount(*player));
                result = scanf("%d", &playerAct);
            } //SFWE 401 threat 9 mitigated
            playerAct--;
            if (playerAct == -1) {
                playerAddCard(discardPileIndex, curCard, deck, player);
                break;
            }//This will add a card to players hand if they decide to draw a card
            else {
                temp = *player; //This sets the temp = to whichever player was passed to the function
                card = playerAct;
                while (card != 0) {
                    temp = temp->pt;
                    card--;
                }//This will iterate though and find whichever card the player wants to play
            }
            //If the player chose the wrong card, let them draw from the deck or choose another card
        }
    }


    if (playerAct != -1) { //This is if the player wants to play a card
        if (temp->value == 7) { //This is for extra credit
            userInput = ' '; //User input is initialized as a space
            printf("Would you like to switch hands with the other player? (y/n): ");
            scanf("%c", &userInput); //This asks if the player wants to switch hands
            while(userInput != 'y' && userInput != 'n') {//This makes sure that userInput is not y or n
              printf("Would you like to switch hands with the other player? (y/n): ");
              scanf("%c", &userInput); //This asks if the player wants to switch hands
            }
            printf("\nPlayer %d placed: %s %s\n", playerNumber, temp->action, temp->color); //This says that the player plased the card
            playerSubtractCard(player, deck, playerAct, discardPileIndex); //This removes the card from their hand
            if(userInput == 'y') { //If the user says y
              temp = *player;
              *player = *playerOther;
              *playerOther = temp;
            }//This switched the playes pointers so that ended up switching their decks
           
        }
        else if (strcmp(temp->action, "plus two") == 0) {
            *drawCards += 2;
            playerSubtractCard(player, deck, playerAct, discardPileIndex);
            printf("\nPlayer %d placed: %s %s\n", playerNumber, temp->action, temp->color);
            //Adds 2 to *drawCards, the amount of cards that need to be drawn which depends based on how many plus two's are played
        }
        else if (strcmp(temp->action, "wild") == 0) {
            printf("Please choose a color (Capitalize first letter and type in full word): ");
            scanf("%7s", newColor);
            do {
                printf("Invalid color please try again: ");
                scanf("%7s", newColor); 
            } while (strcmp(newColor, "Red") != 0 && strcmp(newColor, "Yellow") != 0 &&
                    strcmp(newColor, "Green") != 0 && strcmp(newColor, "Blue") != 0);

            strncpy(temp->color, newColor, 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
            printf("%s %s", temp->action, temp->color);
            printf("\nPlayer %d placed: %s %s\n", playerNumber, temp->action, temp->color);
            playerSubtractCard(player, deck, playerAct, discardPileIndex);
            //Lets the player choose a new color
        }
        else if (strcmp(temp->action, "wild plus four") == 0) {
            printf("Please choose a color (Capitalize first letter and type in full word): ");
            scanf("%7s", newColor);
            do {
                printf("Invalid color please try again: ");
                scanf("%7s", newColor); 
            } while (strcmp(newColor, "Red") != 0 && strcmp(newColor, "Yellow") != 0 &&
                    strcmp(newColor, "Green") != 0 && strcmp(newColor, "Blue") != 0);
            strncpy(temp->color, newColor, 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
            printf("\nPlayer %d placed: %s %s\n", playerNumber, temp->action, temp->color);
            playerSubtractCard(player, deck, playerAct, discardPileIndex);
            *drawCards += 4;
            //Makes the next player draw four cards and lets the player choose a color
        }
        else if (strcmp(temp->action, "skip") == 0) {
            printf("\nPlayer %d placed: %s %s\n", playerNumber, temp->action, temp->color);
            playerSubtractCard(player, deck, playerAct, discardPileIndex);
            printf("\n-----------------------------------------------------------------------------------\n");
            goto mainGameBeginning; //If a player plays the skip card then it goes back to their turn
        }
        else if (strcmp(temp->action, "reverse") == 0) {
            printf("\nPlayer %d placed: %s %s\n", playerNumber, temp->action, temp->color);
            playerSubtractCard(player, deck, playerAct, discardPileIndex);
            printf("\n-----------------------------------------------------------------------------------\n");
            goto mainGameBeginning; //If a player plays the reverse card then it goes back to their turn since there are only 2 players playing
        }
        else if (temp->value <= 9) {
            printf("\nPlayer %d placed: %d %s\n", playerNumber, temp->value, temp->color);
            playerSubtractCard(player, deck, playerAct, discardPileIndex);
        }//This just plays the normal card that the player selected and then says which card they played


    }
    if ((playerNumber == 1 || playerNumber == 2) && cardCount(*player) == 1) { //Checks if either player has 1 card left
        printf("\nPlayer %d has UNO!", playerNumber);
    }
    printf("\n-----------------------------------------------------------------------------------\n");
}//The overall goal of this function is that this is where the main game happens. This is where each player can decide which card to play and this tracks how each card interacts with one another.


void ReadDeck(FILE* inp, card* deck) { //This function is used to read the ordered deck from the file into the deck variable
    int i = 0; //This is used to cycle through the deck array
    char tcolor[7];
    int tvalue;
    char taction[19];


    while (!feof(inp)) {
        fscanf(inp, "%s %d %[^\n]", tcolor, &tvalue, taction); //This scans each value into each index of deck

        if (tvalue != 12 && tvalue != 13) {
            strncpy(deck[i].color, tcolor, 8);
            deck[i].value = tvalue;
            strncpy(deck[i].action, taction, 15);
            i++; //incriment i
        }

        else if(tvalue == 12) {
            strncpy(deck[i].color, tcolor, 8);
            deck[i].value = tvalue;
            strncpy(deck[i].action, "plus two", 15);
            i++; //incriment i
        }

        else if (tvalue == 13) {
            strncpy(deck[i].color, "", 8);
            deck[i].value = tvalue;
            strncpy(deck[i].action, taction, 15);
            i++; //incriment i
        }
    }
    fclose(inp); //This closes the input file
}//The purpose of this function is if the players want to load in their own deck that is already shuffled 
//or created they can do that. This function reads the whole file and then it scans each card and its attributes 
//into an array. Then the function closes the file to ensure that the file isn't staying open and using memory.

//SFWE 401 Threat 8 mitigated
int CheckFileContents(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1; // Error opening file
    }

    char line[100]; // Assuming maximum line length of 100 characters

    // Loop through each line in the file
    while (fgets(line, sizeof(line), file) != NULL) {
        char firstString[10], secondString[20];
        int integerValue;

        // Parse the line into three parts
        if (sscanf(line, "%9s %d %19[^\n]", firstString, &integerValue, secondString) != 3) {
            fclose(file);
            return 0; // Incorrect format
        }

        // Check first string
        if (strcmp(firstString, "Red") != 0 &&
            strcmp(firstString, "Yellow") != 0 &&
            strcmp(firstString, "Blue") != 0 &&
            strcmp(firstString, "Green") != 0) {
            fclose(file);
            return 0; // Incorrect first string
        }

        // Check integer value
        if (integerValue < 0 || integerValue > 13) {
            fclose(file);
            return 0; // Integer out of range
        }

        // Check second string based on integer value
        if (integerValue >= 1 && integerValue <= 9) {
            if (strcmp(secondString, "none") != 0) {
                fclose(file);
                return 0; // Incorrect second string
            }
        }
        else if (integerValue == 10) {
            if (strcmp(secondString, "skip") != 0) {
                fclose(file);
                return 0; // Incorrect second string
            }
        }
        else if (integerValue == 11) {
            if (strcmp(secondString, "reverse") != 0) {
                fclose(file);
                return 0; // Incorrect second string
            }
        }
        else if (integerValue == 12) {
            if (strcmp(secondString, "plus two") != 0) {
                fclose(file);
                return 0; // Incorrect second string
            }
        }
        else if (integerValue == 13) {
            if (strcmp(secondString, "wild") != 0 && strcmp(secondString, "wild plus four") != 0) {
                fclose(file);
                return 0; // Incorrect second string
            }
        }
        else {
            fclose(file);
            return 0; // Invalid integer value
        }
    }

    fclose(file);
    return 1; // All lines checked successfully
}


void initDeck(card* deck) { //if players don't want to load deck then this deck will be initialized
    char color[4][7] = { "Red" , "Yellow" , "Green" , "Blue" }; //These are colors of each card
    int i = 0;
    int j = 0;
    int count = 0;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 14; j++) {
            if (j < 10) {
                strncpy(deck[count].color, color[i], 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
                deck[count].value = j;
                strncpy(deck[count].action, "none", 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
            }
            else if (j == 10) {
                strncpy(deck[count].color, color[i], 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
                deck[count].value = j;
                strncpy(deck[count].action, "skip", 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
            }
            else if (j == 11) {
                strncpy(deck[count].color, color[i], 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
                deck[count].value = j;
                strncpy(deck[count].action, "reverse", 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
            }
            else if (j == 12) {
                strncpy(deck[count].color, color[i], 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
                deck[count].value = j;
                strncpy(deck[count].action, "plus two", 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's
            }
            else {
                strncpy(deck[count].color, "",8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
                deck[count].value = j;
                strncpy(deck[count].action, "wild", 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
            }
            count++;
        }
    } //This creates the first half of the deck with 4 colors and cards 0 - 13 (10-13 are special cards)

    for (i = 0; i < 4; i++) {
        for (j = 1; j < 14; j++) {
            if (j < 10) {
                strncpy(deck[count].color, color[i], 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's
                deck[count].value = j;
                strncpy(deck[count].action, "none", 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's
            }
            else if (j == 10) {
                strncpy(deck[count].color, color[i], 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's
                deck[count].value = j;
                strncpy(deck[count].action, "skip", 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's
            }
            else if (j == 11) {
                strncpy(deck[count].color, color[i], 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's
                deck[count].value = j;
                strncpy(deck[count].action, "reverse", 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's
            }
            else if (j == 12) {
                strncpy(deck[count].color, color[i], 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's
                deck[count].value = j;
                strncpy(deck[count].action, "plus two", 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's
            }
            else {
                strncpy(deck[count].color, "", 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's
                deck[count].value = j;
                strncpy(deck[count].action, "wild plus four", 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's
            }
            count++;
        }
    }//This creates the second half of the deck with 4 colors and cards 1 - 13 (10-13 are special cards)
}//The goal of this function is to create an array of 108 cards which will be the deck. These cards are all unshuffled so another function is called later on to shuffle the deck before the game is played. Since there are 2 halves to the deck and they are both different you need 2 seperate for loops to initialize each half of the deck.


void shuffleDeck(card* deck) { //This is used to shuffle the deck
    int i;
    int j;
    int a;
    card temp;

    for (a = 0; a < 10000; a++) {
        i = rand() % 108; //This will be used to get a random number of the array and it will swap it with j
        j = rand() % 108; //This will be used to get a random number of the array and it will swap it with i
        temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }//This swaps each I and J value card to make teh deck more random
}


void playerHandinit(card** player1, card** player2, card* deck, int numPlayers, int* curCard, int* discardIndex) { //This is the player hand initialize function
    *player1 = (card*)malloc(sizeof(card)); //This creates the head pointer for player 1
    *player2 = (card*)malloc(sizeof(card));  //This creates the head pointer for player 2
    card* temp1 = *player1; //This is the tail for player 1
    card* temp2 = *player2; //This is the tail for player 2

    if(temp1 == NULL){
        printf("Memory allocation failed for player 1\n");
        exit(1);
    } //SFWE 401 Threat 2 mitigated
    strncpy(temp1->color, deck[*curCard].color, 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
    temp1->value = deck[*curCard].value;
    strncpy(temp1->action, deck[*curCard].action, 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
    //The above 3 lines give player 1 their first card

    (*curCard)++;

    if(temp2 == NULL){
        printf("Memory allocation failed for player 1\n");
        exit(1);
    } //SFWE 401 Threat 3 mitigated
    strncpy(temp2->color, deck[*curCard].color, 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
    temp2->value = deck[*curCard].value;
    strncpy(temp2->action, deck[*curCard].action, 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
    //The above 3 lines give player 1 their first card

    for (*curCard = 2; *curCard < 14; (*curCard)++) {
        if (*curCard % 2 != 0) {
            temp1->pt = (card*)malloc(sizeof(card)); //This creates a node for each player
            if (temp1->pt == NULL){
                printf("Memory allocation failed for player 1\n");
                exit(1);
            } //SFWE 401 Threat 4 mitigated
            temp1 = temp1->pt;
            strncpy(temp1->color, deck[*curCard].color, 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
            temp1->value = deck[*curCard].value;
            strncpy(temp1->action, deck[*curCard].action, 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
            temp1->pt = 0;
        }//This goes through a loop adding 6 more cards to the players hand in while making it a linked list in an alternating order
        else {
            temp2->pt = (card*)malloc(sizeof(card)); //This creates a node for each player
            if (temp2->pt == NULL){
                printf("Memory allocation failed for player 2\n");
                exit(1);
            } //SFWE 401 Threat 4 mitigated
            temp2 = temp2->pt;
            strncpy(temp2->color, deck[*curCard].color, 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
            temp2->value = deck[*curCard].value;
            strncpy(temp2->action, deck[*curCard].action, 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
            temp2->pt = 0;
            //This goes through a loop adding 6 more cards to the players hand in while making it a linked list in an alternating order
        }
    }
    (*curCard)++;

    strncpy(deck[*discardIndex].color, deck[*curCard].color, 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
    deck[*discardIndex].value = deck[*curCard].value;
    strncpy(deck[*discardIndex].action, deck[*curCard].action, 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
    (*discardIndex)++;
    (*curCard)++;
}//This function is used to initialize each players hands in an alternating order and it also initializes the first card of the discard pile so that the game can begin.


void playerAddCard(int* discardPileIndex, int* curCard, card* deck, card** head) {
    //Adds a card to the player's hand by getting the new card's info from the deck and making it the new head node
    if (*curCard == 107) {//Has to be 107 since card indexing starts at 0
        shuffleDeck(deck);
        *curCard = 0;
        *discardPileIndex = 0;
    }//This checks to make sure that the deck of cards hasn't ran out. If it has ran out then it will reshuffle the discard pile and use that as the new deck.
    card* newCard = (card*)malloc(sizeof(card));
    if (newCard == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    } //SFWE 401 Threat 5 mitigated
    newCard->value = deck[*curCard].value;
    strncpy(newCard->color, deck[*curCard].color, 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
    strncpy(newCard->action, deck[*curCard].action, 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
    newCard->pt = *head;
    (*curCard)++;

    *head = newCard;
    //free(newCard); //SFWE 401 Threat 6 mitigated
}//This function is used to add a single card to a players hand when this function is called. It gets the next card in the deck and it adds it to the linked list of whichever player the card was called for.


void playerSubtractCard(card** head, card* deck, int listIndex, int* discardIndex) {
    //Removes a card from the player's hand
    card* tail = *head;
    card* removed;
    if (listIndex == 0) {
        removed = *head;
        *head = (*head)->pt;
        (*discardIndex)++;
        deck[*discardIndex].value = removed->value;
        strncpy(deck[*discardIndex].color, removed->color, 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
        strncpy(deck[*discardIndex].action, removed->action, 15); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
        free(removed);
        return;
    }//This above function removes the head card from the players linked list if that is the card they desire to play.
    while (listIndex != 1) {
        //If the removed card is elsewhere
        tail = tail->pt;
        listIndex--;
    }//This finds out which node the player wants to remove
    //Make the previous node point to the next node and free the removed card and add it to the discard pile
    removed = tail->pt;
    tail->pt = tail->pt->pt;
    (*discardIndex)++;
    deck[*discardIndex].value = removed->value;
    strncpy(deck[*discardIndex].color, removed->color, 8); //SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
    strncpy(deck[*discardIndex].action, removed->action, 15);//SFWE401 Threat 7 mitigated (all strcpy's are now strncpy's)
    free(removed);
}//This function is used to remove a card from each players linked list when the function is called. This function also frees that card after it is played to save memory.


int cardCount(card* player) {
    int out = 0;

    while (player != NULL) {
        out++;
        player = player->pt;
    }
    return out;
}//This function returns the ammount of cards each player has in their hands. This allows us to determine the range of cards they can chose from to play.

