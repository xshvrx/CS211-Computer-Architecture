#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct Gate {
	int Count;
	int Decode;
	int *InputArray;
	int *OutputArray;
	int Outputs;
	int Selectors;
	int *SelectorsArray;
	char directivetype[20];
} gate;

typedef struct Node {
	int i;
	char n[10];
	struct Node* next;
} Node;

struct Node* newNode(int i, char *n) {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->i = i;
	newNode->next = NULL;
	strcpy(newNode->n, n);
	return newNode;
}

void InsertNode(struct Node** head, struct Node* newNode) {
	if(*head == NULL) {
		newNode->next = *head;
		*head = newNode;
		return;
	}
	Node* current = *head;
	while(current->next != NULL) {
		current = current->next;
	}
	current->next = newNode;
	return;
}

int SearchNode(struct Node* head, char *n) {
	struct Node* node = head;
	int result = 999;
	while (node != NULL) {
		int var = strcmp(n, node->n);
		if(var == 0) {
			result = node->i;
			return result;
		}
		node = node->next;
	}
	return result;
}

void DELETE(struct Node* head) {
    while(head != NULL) {
        Node* current = head;
        head = head->next;
        free(current);
    }
}

int OPERATOR(int *result, int inputA, int inputB, int output) {
	int OPERATOR = result[inputA] * result[inputB];
	return OPERATOR;
}

int NEGATE(int n) {
	if(n == 1 || n == 0) {
		if (n == 1)
			return 0;
		if (n == 0)
			return 1;
	}
	return 0;
}

void NOT(int *result, int input, int output) {
	int NOT = NEGATE(result[input]);
	result[output] = NOT;
	return;
}

void AND(int *result, int inputA, int inputB, int output) {
	int AND = result[inputA] * result[inputB];
	result[output] = AND;
	return;
}

void NAND(int *result, int inputA, int inputB, int output) {
	int NAND = OPERATOR(result, inputA, inputB, output);
	NAND = NEGATE(NAND);
	result[output] = NAND;
	return;
}	

void OR(int *result, int inputA, int inputB, int output) {
	int OR;
	if(result[inputA] == 1 || result[inputB] == 1) {
		OR = 1;
	} else {
		OR = 0;
	}
	result[output] = OR;
	return;
}

int OROPERATOR(int *result, int inputA, int inputB, int output) {
	int OROPERATOR;
	if(result[inputA] == 1 || result[inputB] == 1) {
		OROPERATOR = 1;
	} else {
		OROPERATOR = 0;
	}
	return OROPERATOR;	
}

void NOR(int *result, int inputA, int inputB, int output) {
	int NOR = OROPERATOR(result, inputA, inputB, output);
	NOR = NEGATE(NOR);
	result[output] = NOR;
	return;
}	

void XOR(int *result, int inputA, int inputB, int output) {
	int XOR;
	if((result[inputA] == 0 && result[inputB] == 1) || (result[inputA] == 1 && result[inputB] == 0)) {
		XOR = 1;
	} else {
		XOR = 0;
	}
	result[output] = XOR;
	return;
}

void NEXT(int *result, int input, int output) {
	int NEXT = result[input];
	result[output] = NEXT;
	return;
}

int DECODE(int input) {
	int DECODE = 0;
	DECODE = pow(2, input);
	return DECODE;
}

void DECODER(int *result, int input, int *arrayA, int *arrayB) {
	int outputA;
	int DECODER = DECODE(input);
	for(int i = 0; i < DECODER; i++) {
		outputA = arrayB[i];
		result[outputA] = 0;
		outputA = 0;
	}
	int prod = 0;
	int sel = 0;
	for(int i = 0; i < input; i++) {
		int offset1 = input - i - 1;
		DECODER = DECODE(offset1);
		outputA = arrayA[i];
		prod = result[outputA] * DECODER;
		sel = sel + prod;
	}
	outputA = arrayB[sel];
	result[outputA] = 1;
	return;
}

void MULTIPLEXER(int *result, int input, int *arrayA, int *arrayB, int output) {
	int out;
	int sel = 0;
	int prod = 0;
	for(int i = 0; i < input; i++) {
		int offset1 = input - i - 1;
		int decoder = DECODE(offset1);
		out = arrayB[i];
		prod = result[out] * decoder;
		sel = sel + prod;
	}
	out = arrayA[sel];
	result[output] = result[out];
	return;
}	

int main(int argc, char **argv) {
	if(argc > 2 || argc < 1) {
		return EXIT_FAILURE;
	}
	char tempA[20];
	char tempB[20];
	char buff[20];
	FILE *fp = fopen(argv[1], "r");
	fscanf(fp, " %15s", tempA);
	fscanf(fp, " %15s", buff);
	int input = atoi(buff);
	int offset1 = 2;
	int count = 0;
	int bp = 5;
	int length = 2 + input;
	Node *NODEHEAD = NULL;
	InsertNode(&NODEHEAD, newNode(0, "0"));
	InsertNode(&NODEHEAD, newNode(1, "1"));
	gate **LogicGate = malloc(sizeof(gate*)*100);;
	for(int i = 2; i < input + 2; i++) {
		fscanf(fp, " %15s", tempA);
		InsertNode(&NODEHEAD, newNode(i, tempA));
		offset1++;
	}	
	fscanf(fp, " %15s", tempB);
	fscanf(fp, " %15s", buff);
	int out = atoi(buff);
	for(int i = length; i < out + length; i++) {
		fscanf(fp, " %15s", tempB);
		InsertNode(&NODEHEAD, newNode(i, tempB));
	}
    	length = length + out;
	while (1) {
        	char temp[20];
		if(fscanf(fp, " %15s", temp) == EOF) {
			break;
		}
		int inputs;
		int outputs;
		int var;
		int selectors;
		char buff2[20];
        	LogicGate[count] = malloc(sizeof(gate));
		count = count + 1;
		LogicGate[count-1]->Decode = 0;
		LogicGate[count-1]->Selectors = 0;
		LogicGate[count-1]->Outputs = 0;
		LogicGate[count-1]->InputArray = NULL;
		LogicGate[count-1]->OutputArray = NULL;
		LogicGate[count-1]->SelectorsArray = NULL;
        	strncpy(LogicGate[count-1]->directivetype, temp, 20);
		var = strcmp("NEXT", LogicGate[count-1]->directivetype);
		if (var == 0) {
			inputs = 1;
			outputs = 1;
			selectors = 0;
			LogicGate[count-1]->Decode = inputs;
			LogicGate[count-1]->Outputs = outputs;
			LogicGate[count-1]->InputArray = malloc(inputs * sizeof(int));
            		LogicGate[count-1]->OutputArray = malloc(outputs * sizeof(int));
            		LogicGate[count-1]->SelectorsArray = malloc(selectors * sizeof(int));
		}
		var = strcmp("NOT", LogicGate[count-1]->directivetype);
		if (var == 0) {
			inputs = 1;
			outputs = 1;
			selectors = 0;
			LogicGate[count-1]->Decode = inputs;
			LogicGate[count-1]->Outputs = outputs;
			LogicGate[count-1]->InputArray = malloc(inputs * sizeof(int));
			LogicGate[count-1]->OutputArray = malloc(outputs * sizeof(int));
			LogicGate[count-1]->SelectorsArray = malloc(selectors * sizeof(int));
		}
		var = strcmp("AND", LogicGate[count-1]->directivetype);
		if (var == 0) {
			inputs = 2;
			outputs = 1;
			selectors = 0;
			LogicGate[count-1]->Decode = inputs;
			LogicGate[count-1]->Outputs = outputs;
			LogicGate[count-1]->InputArray = malloc(inputs * sizeof(int));
			LogicGate[count-1]->OutputArray = malloc(outputs * sizeof(int));
			LogicGate[count-1]->SelectorsArray = malloc(selectors * sizeof(int));
		}
		var = strcmp("OR", LogicGate[count-1]->directivetype);
		if (var == 0) {
			inputs = 2;
			outputs = 1;
			selectors = 0;
			LogicGate[count-1]->Decode = inputs;
			LogicGate[count-1]->Outputs = outputs;
			LogicGate[count-1]->InputArray = malloc(inputs * sizeof(int));
			LogicGate[count-1]->OutputArray = malloc(outputs * sizeof(int));
			LogicGate[count-1]->SelectorsArray = malloc(selectors * sizeof(int));
		}	
		var = strcmp("NAND", LogicGate[count-1]->directivetype);
		if (var == 0) {
			inputs = 2;
			outputs = 1;
			selectors = 0;
			LogicGate[count-1]->Decode = inputs;
			LogicGate[count-1]->Outputs = outputs;
			LogicGate[count-1]->InputArray = malloc(inputs * sizeof(int));
			LogicGate[count-1]->OutputArray = malloc(outputs * sizeof(int));
			LogicGate[count-1]->SelectorsArray = malloc(selectors * sizeof(int));
		}
		var = strcmp("NOR",LogicGate[count-1]->directivetype);
		if (var == 0) {
			inputs = 2;
			outputs = 1;
			selectors = 0;
			LogicGate[count-1]->Decode = inputs;
			LogicGate[count-1]->Outputs = outputs;
			LogicGate[count-1]->InputArray = malloc(inputs * sizeof(int));
			LogicGate[count-1]->OutputArray = malloc(outputs * sizeof(int));
			LogicGate[count-1]->SelectorsArray = malloc(selectors * sizeof(int));
		}
		var = strcmp("XOR", LogicGate[count-1]->directivetype);
		if (var == 0) {
			inputs = 2;
			outputs = 1;
			selectors = 0;
			LogicGate[count-1]->Decode = inputs;
			LogicGate[count-1]->Outputs = outputs;
			LogicGate[count-1]->InputArray = malloc(inputs * sizeof(int));
			LogicGate[count-1]->OutputArray = malloc(outputs * sizeof(int));
			LogicGate[count-1]->SelectorsArray = malloc(selectors * sizeof(int));
		}
		var = strcmp("DECODER", LogicGate[count-1]->directivetype);
		if (var == 0) {
			selectors = 0;
			LogicGate[count-1]->Decode = 0;
			fscanf(fp, " %15s", buff2);
			inputs = atoi(buff2);
			LogicGate[count-1]->Decode = inputs;
			int decoder = DECODE(inputs);
			outputs = decoder;
			LogicGate[count-1]->Outputs = outputs;
			LogicGate[count-1]->InputArray = malloc(inputs * sizeof(int));
			LogicGate[count-1]->OutputArray = malloc(outputs * sizeof(int));
			LogicGate[count-1]->SelectorsArray = malloc(selectors * sizeof(int));
		}
		var = strcmp("MULTIPLEXER", LogicGate[count-1]->directivetype);
		if (var == 0) {
			outputs = 1;
			LogicGate[count-1]->Selectors = 0;
			fscanf(fp, " %15s", buff2);
			inputs = atoi(buff2);
			LogicGate[count-1]->Selectors = inputs;
			int decoder = DECODE(inputs);
			inputs = decoder;
			selectors = LogicGate[count-1]->Selectors;
			LogicGate[count-1]->InputArray = malloc(inputs * sizeof(int));
            LogicGate[count-1]->OutputArray = malloc(outputs * sizeof(int));
			LogicGate[count-1]->SelectorsArray = malloc(selectors * sizeof(int));
		}
		char tempC[20];
		for (int i = 0; i < inputs; i++) {
			int point;
			fscanf(fp, " %15s", tempC);
			point = SearchNode(NODEHEAD, tempC);
			if (point == 999) {
				InsertNode(&NODEHEAD, newNode(length, tempC));
                LogicGate[count-1]->InputArray[i] = length;
                length = length + 1;
                continue;
			}
			if (point != 999) {
				LogicGate[count-1]->InputArray[i] = point;
			}
		}
	if(selectors != 0) {
		char tempD[20];
		for (int i = 0; i < selectors; i++) {	
			int point;
			fscanf(fp, " %15s", tempD);
			point = SearchNode(NODEHEAD, tempD);
			LogicGate[count-1]->SelectorsArray[i] = point;
		}
	}
		char tempE[20];
		for (int i = 0; i < outputs; i++) {
			int point;
			fscanf(fp, " %15s", tempE);
			point = SearchNode(NODEHEAD, tempE);
			if (point == 999) {
				InsertNode(&NODEHEAD, newNode(length, tempE));			 
				LogicGate[count-1]->OutputArray[i] = length;
				length = length + 1;
				continue;
			}
			else if (point != 999) {
				LogicGate[count-1]->OutputArray[i] = point;
			}

		}
	}	
		int *result = malloc(length * sizeof(int));
		for (int i = 0; i < length; i++) {
			result[i] = 0;
		}
		result[1] = 1;		
	while(bp != 0) {
		int offset2 = 2;
		for(int i = 0; i < input; i++) {
			printf("%d ", result[offset2]);
			offset2 = offset2 + 1;
		}
		printf("|");
		for(int i = 0; i < count; i++) {
			int var = strcmp("NEXT", LogicGate[i]->directivetype);
			if (var == 0) {
				int inputA = LogicGate[i]->InputArray[0];
				int outputA = LogicGate[i]->OutputArray[0];
				NEXT(result, inputA, outputA);
			}
			var = strcmp("DECODER", LogicGate[i]->directivetype);
			if (var == 0) {
				int input = LogicGate[i]->Decode;
				int *arrayA = LogicGate[i]->InputArray;
				int *arrayB = LogicGate[i]->OutputArray;
				DECODER(result, input, arrayA, arrayB);
			}
			var = strcmp("MULTIPLEXER", LogicGate[i]->directivetype);
			if (var == 0) {
				int selectors = LogicGate[i]->Selectors;
				int *arrayA = LogicGate[i]->InputArray;
				int *arrayB = LogicGate[i]->SelectorsArray;
				int outputA = LogicGate[i]->OutputArray[0];
				MULTIPLEXER(result, selectors, arrayA, arrayB, outputA);
			}
		var = strcmp("NOT", LogicGate[i]->directivetype);
	    if (var == 0) {
		int inputA = LogicGate[i]->InputArray[0];
                int outputA = LogicGate[i]->OutputArray[0];
		NOT(result, inputA, outputA);
	   }
		var = strcmp("AND", LogicGate[i]->directivetype);
            if (var == 0) {
                int inputA = LogicGate[i]->InputArray[0];
                int inputB = LogicGate[i]->InputArray[1];
                int outputA = LogicGate[i]->OutputArray[0];
                AND(result, inputA, inputB, outputA);
            }
		var = strcmp("NAND", LogicGate[i]->directivetype);
            if (var == 0) {
                int inputA = LogicGate[i]->InputArray[0];
                int inputB = LogicGate[i]->InputArray[1];
                int outputA = LogicGate[i]->OutputArray[0];
                NAND(result, inputA, inputB, outputA);
            }
		var = strcmp("OR", LogicGate[i]->directivetype);
            if (var == 0) {
                int inputA = LogicGate[i]->InputArray[0];
                int inputB = LogicGate[i]->InputArray[1];
                int outputA = LogicGate[i]->OutputArray[0];
                OR(result, inputA, inputB, outputA);
            }
		var = strcmp("NOR", LogicGate[i]->directivetype);
            if (var == 0) {
                int inputA = LogicGate[i]->InputArray[0];
                int inputB = LogicGate[i]->InputArray[1];
                int outputA = LogicGate[i]->OutputArray[0];
                NOR(result, inputA, inputB, outputA);
            }
		var = strcmp("XOR", LogicGate[i]->directivetype);
            if (var == 0) {
                int inputA = LogicGate[i]->InputArray[0];
                int inputB = LogicGate[i]->InputArray[1];
                int outputA = LogicGate[i]->OutputArray[0];
                XOR(result, inputA, inputB, outputA);
            }
		}
		int offset3 = offset1;
		for(int i = 0; i < out; i++) {
			printf(" %d", result[offset3]);
			offset3 = offset3 + 1;
		}
		printf("\n");
		for(int i = input + 1; i > 1; i--) {
			bp = 0;
			if(result[i] == 1) {
				result[i] = 0;
			}
			else if (result[i] == 0) {
				result[i] = 1;
				bp = 1;
				break;
			}
		}
		for(int i = input + 2; i < length; i++) {
			result[i] = 0;
		}									
	}
    for(int i = 0; i < count; i++) {
        if(LogicGate[i]->InputArray!= NULL) {
            free(LogicGate[i]->InputArray);
            LogicGate[i]->InputArray= NULL;
        }
        if(LogicGate[i]->SelectorsArray!= NULL) {
            free(LogicGate[i]->SelectorsArray);
            LogicGate[i]->SelectorsArray= NULL;
        }
        if(LogicGate[i]->OutputArray!= NULL) {
            free(LogicGate[i]->OutputArray);
            LogicGate[i]->OutputArray= NULL;
        }
        free(LogicGate[i]);
    }
    free(LogicGate);
    DELETE(NODEHEAD);
    free(result);
    return EXIT_SUCCESS;
}