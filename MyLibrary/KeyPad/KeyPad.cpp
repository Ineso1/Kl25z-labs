#include "KeyPad.h"

/**
 * @file KeyPad.cpp
 *
 * @brief Class for numeric matrix keypad initialization and key detection.
 */


KeyPad::KeyPad(){}

void KeyPad::rstRows(){
    PTB->PDDR &= ~ ( D1 | D2 | D3 | D4 );
}

void KeyPad::clrRows(){
    PTB->PCOR = ( D1 | D2 | D3 | D4 );
}

void KeyPad::setRows(){
    PTB->PDDR |= ( D1 | D2 | D3 | D4 );
}

void KeyPad::rstColumns(){
    PTE->PDDR &= ~( D5 | D6 | D7 | D8 );
}

void KeyPad::setColumns(){
    PTE->PDDR |= ( D5 | D6 | D7 | D8 );
}

/**
 * @brief Initializes the keypad by configuring the necessary pins and registers.
*/
void KeyPad::init(){

    SIM->SCGC5 |= ( SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK );
    PORTB->PCR[8] = 0x103; //Row (PTB1) D1
    PORTB->PCR[9] = 0x103; //Row (PTB2) D2
    PORTB->PCR[10] = 0x103; //Row (PTB4) D3
    PORTB->PCR[11] = 0x103; //Row (PTB12) D4
    PORTE->PCR[2] = 0x103; //Column (PTE4) D5
    PORTE->PCR[3] = 0x103; //Column (PTE5) D6
    PORTE->PCR[4] = 0x103; //Column (PTE8) D7
    PORTE->PCR[5] = 0x103; //Column (PTE9) D8

    //Make all rows as input
    setRows();

    //Make all columns output
    rstColumns();

}

/**
 * @brief Reads the pressed key from the keypad.
 *
 * @return The character corresponding to the pressed key. Returns '0' if no key is pressed.
*/
char KeyPad::getKey(){

    delay_ms(2);
    int row, col;
    int rowNum;
    int colNum;

    /* one row is active */
    /* check to see any key pressed */

    setRows(); /* enable all rows */
    clrRows();

    delay_us(2); /* wait for signal return */

    col = (PTE->PDIR & (D5 | D6 | D7 | D8)) >> 2; /* read all columns */

    rstRows();
    rstColumns();

    if (col == 0xF)
    {
        return 0; /* No pressed */
    }

    int32_t rows[] = {D1,D2,D3,D4};
    /* If a key is pressed, we need find out which key.*/ 
    for (row = 0; row < 4; row++)
    {   
        rstRows();
        rstColumns();

        PTB->PDDR |= rows[row];
        PTB->PCOR = rows[row];

        delay_us(2); /* wait for signal to settle */
        col = (PTE->PDIR & (D5 | D6 | D7 | D8)) >> 2; /* read all columns */

        if (col != 0xF) break; 
        /* if one of the input is low, some key is pressed. */
    }
    rstRows();
    rstColumns();

    if (row == 4) return '0'; /* if we get here, no key is pressed */

    // Debouncing
    int debounceDelay = 20; 
    char prevKey = 0;
    char currentKey = 0;

    // Read the keypad multiple times and wait for stable input
    for (int i = 0; i < debounceDelay; i++) {
        delay_ms(1);  
        setRows();
        PTB->PCOR = rows[row];
        delay_us(2); /* wait for signal to settle */
        col = (PTE->PDIR & (D5 | D6 | D7 | D8)) >> 2; /* read all columns */
        rstRows();
        rstColumns();

        if (col == 0xE) {
            currentKey = keypadValues[0][row]; /* key in column 0 */
        } else if (col == 0xD) {
            currentKey = keypadValues[1][row]; /* key in column 1 */
        } else if (col == 0xB) {
            currentKey = keypadValues[2][row]; /* key in column 2 */
        } else if (col == 0x7) {
            currentKey = keypadValues[3][row]; /* key in column 3 */
        } else {
            currentKey = 0; /* just to be safe */
        }

        // Check if the current key is stable for debounceDelay consecutive reads
        if (currentKey == prevKey) {
            // Key reading is stable
            break;
        }
        
        prevKey = currentKey;
    }

    return currentKey;

}