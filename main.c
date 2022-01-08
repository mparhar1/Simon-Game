/*
 * File:    main.c
 * Purpose: To create a playable 'Simon Game' on the DAQ simulator
 * Date:    December 2020
 */

#define _CRT_SECURE_NO_WARNINGS

/* Preprocessor Directives */
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <DAQlib.h>
#include <time.h>

/* Definitions */
/* Status Indicators */
#define TRUE 1
#define FALSE 0
#define ON 1
#define OFF 0

/* I/O Channels */
#define BUTTON0 0
#define BUTTON1 1
#define BUTTON2 2
#define BUTTON3 3

/* LEDs */
#define LED0 0 
#define LED1 1
#define LED2 2
#define LED3 3

#define NUM_LEDS 4
#define LENGTH 5

/* Function Prototypes */
void main_simon();
int random_int(int lower, int upper);
void generate_sequence(int length, int data[]);
int get_button();
void flash_LED(int LED);

/* Main Function for DAQ */
int main(void)
{
	int setupVal;

	srand((unsigned)time(NULL));

	printf("Enter DAQ configuration type (0 for device or 6 for simulator): ");
	scanf("%d", &setupVal);

	if (setupDAQ(setupVal) == TRUE)
		runSimon();
	else
		printf("ERROR: Cannot initialise system...\n");

	system("PAUSE");
	return 0;
}

/* Main Function to Run the Simon Game */
void main_simon()
{
	int data[LENGTH];
	int plays = 1;

	generate_sequence(LENGTH, data);

	Sleep(1000);

	while (continueSuperLoop() == TRUE)
	{
		/* Checks if Player is Correct */
		if (plays == LENGTH + 1) {
			/* Green LED Flashes 3 times if Correct */
			for (int wins = 0; wins < 3; wins++) {
				flash_LED(LED0);
			}
			/* Generates New Sequence */
			generate_sequence(LENGTH, data);
			plays = 0;
		}

		/* Displays LED Sequence */
		for (int display_count = 0; display_count < plays; display_count++) {
			flash_LED(data[display_count]);
		}

		/* Reads User's Input */
		for (int reads = 0; reads < plays; reads++) {
			/* Checks if Player is Incorrect */
			if (get_button() != data[reads]) {
				/* Red LED Flashes 3 times if Incorrect */
				for (int loses = 0; loses < 3; loses++) {
					flash_LED(LED1);
				}
				/* Generates New Sequence */
				generate_sequence(LENGTH, data);
				plays = 0;
			}
		}

		Sleep(1000);

		plays++;
	}
}

/* Returns a Random Integer Within Specified Range */
int random_int(int lower, int upper)
{
	return rand() % upper;
}

/* Generates the Sequence for the Round */
void generate_sequence(int length, int data[])
{
	for (int i = 0; i < length; i++) {
		data[i] = random_int(0, NUM_LEDS);
	}

}

/* Reads the Users Inputs */
int get_button()
{
	/* Wait for User Input */
	while (digitalRead(BUTTON0) == FALSE || digitalRead(BUTTON1) == FALSE || digitalRead(BUTTON2) == FALSE || digitalRead(BUTTON3) == FALSE) {

		if (continueSuperLoop() == FALSE)
			return;

		/* Check Button Input */
		if (digitalRead(BUTTON0) == TRUE) {
			while (digitalRead(BUTTON0) == TRUE) { }
			return 0;
		}
		else if (digitalRead(BUTTON1) == TRUE) {
			while (digitalRead(BUTTON1) == TRUE) { }
			return 1;
		}
		else if (digitalRead(BUTTON2) == TRUE) {
			while (digitalRead(BUTTON2) == TRUE) { }
			return 2;
		}
		else if (digitalRead(BUTTON3) == TRUE) {
			while (digitalRead(BUTTON3) == TRUE) { }
			return 3;
		}
	}
}

/* Flashes Specified LED */
void flash_LED(int LED)
{
	Sleep(100);
	digitalWrite(LED, ON);
	Sleep(100);
	digitalWrite(LED, OFF);
}
