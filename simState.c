#include <stdlib.h>
#include <stdio.h>


typedef struct stateMachine{
	char stateName;
	int stateNum;
	struct stateMachine* next0;
	struct stateMachine* next1;
}fsm;


int main(int argc, char * argv[])
{
	int counter,counter2,counter3,check, garbageNum, deletedNum = 0;
	int currentState = 0;
	int change = -1;
	char *array = (char*)malloc(sizeof(char));
	int arrSize = 0;
	char dataIn;
	char userInput[3];
	FILE *data = fopen("data.txt", "r");
	
	if (data == NULL)
		{
		fprintf(stdout, "Error could not find data file!\n");
		exit(1);
		}

	while((fscanf(data,"%c", &dataIn) == 1))
		{
			if ((dataIn != ' ') && (dataIn != '\n'))
				{
				array[arrSize] = dataIn;
				arrSize++;
				}
		}

	fclose(data);

	if ((arrSize % 3) != 0)
		{
		fprintf(stdout, "Incorrect state machine format!\n");
		exit(1);
		}

	int totalStates = arrSize/3;
	fsm states[totalStates];
	char deleted[totalStates];
	char garbage[totalStates];

	for (counter = 0; counter<arrSize; counter+=3)
		{
		states[counter/3].stateName = (char)array[counter];
		states[counter/3].stateNum = counter/3;

			for (counter2 = 0; counter2<arrSize; counter2+=3)
				{
				if (array[counter+1] == array[counter2])
					states[counter/3].next0 = states + (counter2/3);
				if (array[counter+2] == array[counter2])
					states[counter/3].next1 = states + (counter2/3);
				}

		}
	
	fprintf(stdout, "Enter your command on a single line, no spaces, followed by the enter key.\n");
	fprintf(stdout, "Enter 'e' to exit the program.\n");
	fprintf(stdout, "Starting state: %c\n", states[currentState].stateName);

	while(1)
	{

	for(counter=0;counter<3;counter++) //Clean cache
		userInput[counter] = '0';

	for(counter=0;counter<3;counter++)
		{
		scanf("%c", &userInput[counter]);
		if(userInput[counter] == '\n')
			break;
		}

	if (userInput[0] == '0')
		{
		currentState = states[currentState].next0->stateNum;
	 	fprintf(stdout, "Current State: %c\n", states[currentState].stateName);
		}

	else if (userInput[0] == '1')
		{
		currentState = states[currentState].next1->stateNum;
	 	fprintf(stdout, "Current State: %c\n", states[currentState].stateName);
		}

	else if (userInput[0] == 'c')
		{
		change = -1;
		check = 0;
		for (counter = 0; counter<totalStates; counter++)
			if (states[counter].stateName == userInput[2])
				change = states[counter].stateNum;
		
		if (change != -1)
			for (counter = 0; counter<deletedNum; counter++)
				if (states[change].stateName == deleted[counter])
					check = 1;

		if (change == -1 || check == 1)
			fprintf(stdout, "Invalid Input!\n");
		else
			{
			if (userInput[1] == '0')
				states[currentState].next0 = states + change;
			else if (userInput[1] == '1')
				states[currentState].next1 = states + change;
			else
				fprintf(stdout, "Invalid Input!\n");
			}
		}

	else if (userInput[0] == 'p')
		{
		for (counter = 0; counter<totalStates; counter++)
			{
			check = 0;
			for (counter2 = 0; counter2<deletedNum; counter2++)
				if (states[counter].stateName == deleted[counter2])
					check = 1;
			if (check == 0)
				fprintf(stdout,"%c %c %c\n", states[counter].stateName, states[counter].next0->stateName, states[counter].next1->stateName);
			}
		}

	else if (userInput[0] == 'g')
		{
		int garbageCheck = 0;
		for (counter = 0; counter<totalStates; counter++)
		{
			check = 0;
			for (counter2 = 0; counter2<totalStates; counter2++)
				if (counter2 != counter)
					if ((states[counter2].next0->stateName == states[counter].stateName) || (states[counter2].next1->stateName == states[counter].stateName))
						check = 1;
			if (check == 0)
			{
			garbageCheck = 0;
			for (counter3 = 0; counter3<garbageNum; counter3++)
				if (garbage[counter3] == states[counter].stateName)
					garbageCheck = 1;
			if (garbageCheck == 0)
				{
				garbage[garbageNum] = states[counter].stateName;
				garbageNum++;
				}
			}
		}

		if (garbageNum == 0)
			fprintf(stdout, "No garbage");
		else
			{
			fprintf(stdout, "Garbage: ");
			for (counter = 0; counter<garbageNum; counter++)
				fprintf(stdout, "%c ", garbage[counter]);
			fprintf(stdout, "\n");
			}
		}

	else if (userInput[0] == 'd'){
		int deletedCheck = 0;
		change = -1;
		check = 0;

		for (counter = 0; counter<totalStates; counter++)
			if (states[counter].stateName == userInput[1])
				change = states[counter].stateNum;

		if (change == -1 && garbageNum == 0)
			fprintf(stdout, "No states deleted.\n");

		else if (change == -1) 
			{
			
			for (counter = 0; counter<garbageNum; counter++)
				{
				deletedCheck = 0;
				for (counter3 = 0; counter3<deletedNum; counter3++)
					if (deleted[counter3] == garbage[counter])
						deletedCheck = 1;
				if (deletedCheck == 0)
					{
					deleted[deletedNum] = garbage[counter];
					deletedNum++;
					}
				}
			fprintf(stdout, "Deleted: ");
			for (counter = 0; counter<deletedNum; counter++)
				fprintf(stdout, "%c ", deleted[counter]);
			fprintf(stdout, "\n");
			}
			
			
		else
			{
			for (counter = 0; counter<totalStates; counter++)
				if (counter != change)
					if ((states[counter].next0->stateName == states[change].stateName) || (states[counter].next1->stateName == states[change].stateName))
						check = 1;

			for(counter2 = 0; counter2<deletedNum; counter2++)
				if (deleted[counter2] == userInput[1])
					check = 1;
			
			if (check == 1) //Either reachable or already deleted therefore do not delete
				fprintf(stdout, "Not deleted.\n");
			else 
				{
				deleted[deletedNum] = states[change].stateNum;
				deletedNum++;
				fprintf(stdout, "Deleted.\n");
				}
		
			}
	}

	else if (userInput[0] == 'e')
		exit(1);
	
	}
}
