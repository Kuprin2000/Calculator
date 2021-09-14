#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// element of a list
typedef struct list {
	char data[10];
	struct list* prev;
	struct list* next;
} list;

// node of a tree
typedef struct node {
	char data[10];
	struct node* left;
	struct node* right;
} node;

// get priority of the operation
int priority(char input) {
	switch (input) {
	case '+':
	case '-': return 1;
	case '*':
	case '/': return 2;
	}

	return 100;
}

// creates tree based on array of tokens
node* makeTree(char tokens[][10], int first, int last) {
	char buffer;
	int minimum_priority = 100;
	int  bracket_counter = 0;
	int the_lowest_priority_token = 0;
	int current_priority;
	node* tree_ptr = (node*)malloc(sizeof(node));

	// if the array consists of only one element
	if (first == last) {
		strcpy_s(tree_ptr->data, 10, tokens[first]);
		tree_ptr->left = NULL;
		tree_ptr->right = NULL;
		return tree_ptr;
	}

	// if the array consists of more then one elemest,
	// we look for token with the lowest priority
	for (int i = first; i <= last; i++) {
		buffer = tokens[i][0];

		if (buffer == '(') {
			bracket_counter++;
			continue;
		}

		if (buffer == ')') {
			bracket_counter--;
			continue;
		}

		if (bracket_counter > 0) {
			continue;
		}

		current_priority = priority(buffer);
		if (current_priority <= minimum_priority) {
			minimum_priority = current_priority;
			the_lowest_priority_token = i;
		}
	}

	// if the first and the last tokens are brackets we delete them
	if (minimum_priority == 100 && tokens[first][0] == '(' && tokens[last][0] == ')') {
		return makeTree(tokens, first + 1, last - 1);
	}

	// create new node of the tree and make recursive calls
	strcpy_s(tree_ptr->data, 10, tokens[the_lowest_priority_token]);
	tree_ptr->left = makeTree(tokens, first, the_lowest_priority_token - 1);
	tree_ptr->right = makeTree(tokens, the_lowest_priority_token + 1, last);

	return tree_ptr;
}

// pre-order traversal of the tree 
void preOrderPrint(node * root) {
	if (root != NULL) {
		printf(" %s", root->data);
		preOrderPrint(root->left);
		preOrderPrint(root->right);
	}

	else
		printf(" ... ");
}

// in-order traversal of the tree
void inOrderPrint(node * root)
{
	if (root != NULL) {
		inOrderPrint(root->left);
		printf(" %s", root->data);
		inOrderPrint(root->right);
	}

	else
		printf(" ... ");
}

// post-order traversal of the tree
void postOrderPrint(node * root) {
	if (root != NULL) {
		postOrderPrint(root->left);
		postOrderPrint(root->right);
		printf(" %s", root->data);
	}

	else
		printf(" ... ");
}

// get reverse polish notation of the tree
void poland(node * root, char result[][10], int* counter)
{
	if (root != NULL) {
		poland(root->left, result, counter);
		poland(root->right, result, counter);

		for (int i = 0; i < 10; i++)
			result[*counter][i] = root->data[i];

		*counter += 1;
	}
}

// delete tree
void deleteTree(node * root) {
	if (root != NULL) {
		deleteTree(root->left);
		deleteTree(root->right);
		free(root);
	}
}

// check if there is no errors in the user input
char inputIsCorrect(char user_input[]) {
	if (strstr(user_input, "/0") != NULL)
		return 0;


	if (strstr(user_input, "++") != NULL || strstr(user_input, "--") != NULL || strstr(user_input, "**") != NULL || strstr(user_input, "//") != NULL)
		return 0;

	return 1;
}

// parse user input, get tokens and check if brackets are correct
char getTokens(char user_input[], char tokens[][10], int* tokens_counter) {
	int name_length = 0;
	int number_lenght = 0;
	int bracket_counter = 0;

	char is_name_flag = 1;
	char is_number_flag = 1;
	char is_math_symbol_flag = 1;
	char token_is_processed_flag = 0;

	// parse tokens
	for (int i = 0; i < strlen(user_input); i++) {
		token_is_processed_flag = 0;

		// if it is mathematical operation
		if (user_input[i] == '+' || user_input[i] == '-' || user_input[i] == '*' || user_input[i] == '/' || user_input[i] == '(' || user_input[i] == ')') {
			if (i != 0) * tokens_counter += 1;

			is_math_symbol_flag = 1;
			is_name_flag = 0;
			is_number_flag = 0;
			name_length = 0;
			number_lenght = 0;

			if (user_input[i] == '(') bracket_counter++;
			if (user_input[i] == ')') bracket_counter--;

			tokens[*tokens_counter][0] = user_input[i];
			tokens[*tokens_counter][1] = '\0';

			token_is_processed_flag = 1;
		}

		// if it is a name of variable
		if (user_input[i] >= 'a' && user_input[i] <= 'z') {
			if (is_name_flag == 0) * tokens_counter += 1;

			is_name_flag = 1;
			is_number_flag = 0;
			is_math_symbol_flag = 0;
			number_lenght = 0;

			if (is_number_flag != 0) * tokens_counter += 1;

			tokens[*tokens_counter][name_length] = user_input[i];
			tokens[*tokens_counter][name_length + 1] = '\0';

			name_length++;

			token_is_processed_flag = 1;
		}

		// if it is a digit
		if (user_input[i] >= '0' && user_input[i] <= '9') {
			if (is_number_flag == 0) * tokens_counter += 1;

			is_number_flag = 1;
			is_name_flag = 0;
			is_math_symbol_flag = 0;
			name_length = 0;

			tokens[*tokens_counter][number_lenght] = user_input[i];
			tokens[*tokens_counter][number_lenght + 1] = '\0';

			number_lenght++;

			token_is_processed_flag = 1;
		}

		// if it is an unknown symbol
		if (token_is_processed_flag == 0)
			return 0;
	}

	// if the number brackets is incorrect
	if (bracket_counter != 0)
		return 0;

	return 1;
}

// define values of variables
void defineVariables(char tokens[][10], char variable_names[][10], char variable_values[][10], int* tokens_counter) {
	char variable_is_found_flag = 0;
	int current_variable_value;
	int variable_value_input = 0;

	for (int i = 0; i < *tokens_counter; i++) {
		// if current token is a variable name
		if (tokens[i][0] >= 'a' && tokens[i][0] <= 'z') {
			variable_is_found_flag = 0;

			// check if user has already defined value
			for (int j = 0; j < 50; j++) {
				// if user has already defined
				if (strcmp(variable_names[j], tokens[i]) == 0) {
					current_variable_value = atoi(variable_values[j]);

					// if value is positive
					if (current_variable_value >= 0)
						_itoa_s(current_variable_value, tokens[i], 10, 10);

					// if value is negative, we paste it as (-...)
					else {
						current_variable_value *= -1;
						for (int k = *tokens_counter + 4; k > i; k--) {
							for (int m = 0; m < 10; m++)
								tokens[k][m] = tokens[k - 4][m];
						}
						tokens[i][0] = '(';
						tokens[i][1] = '\0';
						tokens[i + 1][0] = '0';
						tokens[i + 1][1] = '\0';
						tokens[i + 2][0] = '-';
						tokens[i + 2][1] = '\0';
						tokens[i + 4][0] = ')';
						tokens[i + 4][1] = '\0';
						_itoa_s(current_variable_value, tokens[i + 3], 10, 10);
						*tokens_counter += 4;
					}

					variable_is_found_flag = 1;
				}
			}

			// if user has not already defined
			if (variable_is_found_flag == 0) {
				printf("\nDefine variable value %s (you can input 0 to generate random unumber from 0 to 100): ", tokens[i]);
				scanf_s("%d", &variable_value_input);

				// if value is 0, generate a random number
				if (variable_value_input == 0) {
					variable_value_input = rand() % 100;
					printf("\nGenerated value %d \n", variable_value_input);
				}

				// write name of the variable and the value to the arrays
				strcpy_s(variable_names[i], 10, tokens[i]);
				_itoa_s(variable_value_input, variable_values[i], 10, 10);

				// if the value is positive 
				if (variable_value_input >= 0)
					_itoa_s(variable_value_input, tokens[i], 10, 10);

				// if value is negative, we paste it as (-...)
				else {
					variable_value_input *= -1;

					for (int k = *tokens_counter + 4; k > i; k--) {
						for (int m = 0; m < 10; m++)
							tokens[k][m] = tokens[k - 4][m];
					}

					tokens[i][0] = '(';
					tokens[i][1] = '\0';
					tokens[i + 1][0] = '0';
					tokens[i + 1][1] = '\0';
					tokens[i + 2][0] = '-';
					tokens[i + 2][1] = '\0';
					tokens[i + 4][0] = ')';
					tokens[i + 4][1] = '\0';

					_itoa_s(variable_value_input, tokens[i + 3], 10, 10);

					*tokens_counter += 4;
				}
			}
		}
	}
}

// transforms expression for unary operations to work properly
void unaryOperationsConvertions(char tokens[][10], int* tokens_counter) {
	for (int k = 0; k < *tokens_counter; k++) {
		// unary -
		if (tokens[k][0] == '-' && k == 0) {
			for (int i = *tokens_counter; i > 0; i--) {
				for (int j = 0; j < 10; j++)
					tokens[i][j] = tokens[i - 1][j];
			}

			tokens[0][0] = '0';
			tokens[0][1] = '\0';
			tokens[1][0] = '-';
			tokens[1][1] = '\0';

			*tokens_counter += 1;
		}

		// unary +
		if (tokens[k][0] == '+' && k == 0) {
			for (int i = *tokens_counter; i > 0; i--) {
				for (int j = 0; j < 10; j++)
					tokens[i][j] = tokens[i - 1][j];
			}

			tokens[0][0] = '0';
			tokens[0][1] = '\0';
			tokens[1][0] = '+';
			tokens[1][1] = '\0';

			*tokens_counter += 1;
		}

		// unary - after bracket
		if (tokens[k][0] == '-' && k != 0 && tokens[k - 1][0] == '(') {
			for (int i = *tokens_counter + k; i > k; i--) {
				for (int j = 0; j < 10; j++)
					tokens[i][j] = tokens[i - 1][j];
			}

			tokens[k][0] = '0';
			tokens[k][1] = '\0';
			tokens[k + 1][0] = '-';
			tokens[k + 1][1] = '\0';

			*tokens_counter += 1;
		}

		// unary + after bracket
		if (tokens[k][0] == '+' && k != 0 && tokens[k - 1][0] == '(') {
			for (int i = *tokens_counter + k; i > k; i--) {
				for (int j = 0; j < 10; j++)
					tokens[i][j] = tokens[i - 1][j];
			}

			tokens[k][0] = '0';
			tokens[k][1] = '\0';
			tokens[k + 1][0] = '+';
			tokens[k + 1][1] = '\0';

			*tokens_counter += 1;
		}
	}
}

// caclulation of the expression using stack
int calculate(list * head, char tokens[][10], int tokens_counter) {
	int first_operand, second_operand, result;
	char current_token[10];

	for (int i = 0; i < tokens_counter; i++) {
		strcpy_s(current_token, 10, tokens[i]);

		// if current token is a digit
		if (current_token[0] >= '0' && current_token[0] < '9') {
			list* add_ptr = (list*)malloc(sizeof(list));
			strcpy_s(add_ptr->data, 10, current_token);
			add_ptr->prev = NULL;
			add_ptr->next = head;
			head = add_ptr;
		}

		// if current token is a math operator
		if (current_token[0] == '+') {
			first_operand = atoi(head->next->data);
			second_operand = atoi(head->data);

			head = head->next;
			free(head->prev);
			head->prev = NULL;

			_itoa_s(first_operand + second_operand, head->data, 10, 10);
		}
		if (current_token[0] == '-') {
			first_operand = atoi(head->next->data);
			second_operand = atoi(head->data);

			head = head->next;
			free(head->prev);
			head->prev = NULL;

			_itoa_s(first_operand - second_operand, head->data, 10, 10);
		}
		if (current_token[0] == '*') {
			first_operand = atoi(head->next->data);
			second_operand = atoi(head->data);

			head = head->next;
			free(head->prev);
			head->prev = NULL;

			_itoa_s(first_operand * second_operand, head->data, 10, 10);
		}
		if (current_token[0] == '/') {
			first_operand = atoi(head->next->data);
			second_operand = atoi(head->data);

			// zero division
			if (second_operand == 0)
				return INT_MIN;

			head = head->next;
			free(head->prev);
			head->prev = NULL;

			_itoa_s(first_operand / second_operand, head->data, 10, 10);
		}
	}

	return atoi(head->data);
}

int main() {
	srand(time(NULL));
	setlocale(0, "russian");

	char user_input[50], tokens[50][10], variable_names[50][10], variable_values[50][10];
	int  tokens_counter = 0;
	int tree_nodes_counter = 0;
	int result;


	node* tree_ptr;
	list* head_ptr;

	// user inputs an expression
	printf("Calculator\n\nYou can use whole numbers, operations +, -, *, \ and brackets. You can also use variables. For example a+b/2-5.\n");
	printf("The program will ask you to input values of these variables later. Your expression should not contan ' ' and\nshould not be longer than 50 characters.\n\n");
	printf("Input your expression: ");

	gets_s(user_input, 50);

	// check if the expression is correct
	if (!inputIsCorrect(user_input)) {
		printf("Zero division or input is incorrect\n");
		_getch();
		return 0;
	}

	// get tokens from the expression and check number of brackets
	if (!getTokens(user_input, tokens, &tokens_counter)) {
		printf("\nYour expression is incorrect or number of brackets is incorrect");
		_getch();
		return 0;
	}

	tokens_counter++;

	// initialize veriable names array
	for (int i = 0; i < 50; ++i) {
		variable_names[i][0] = '\0';
	}

	// define values if the variables
	defineVariables(tokens, variable_names, variable_values, &tokens_counter);

	// transforms expression for unary operations to work properly
	unaryOperationsConvertions(tokens, &tokens_counter);

	// print tokens
	printf("\nTokens we got from the expression:\n");
	for (int i = 0; i < tokens_counter; i++) {
		printf("%s ", tokens[i]);
	}

	// Create a tree
	printf("\n\nLet's create a binary tree:");
	tree_ptr = makeTree(tokens, 0, tokens_counter - 1);

	// Print different traversals of the tree
	printf("\n1) pre-order print: ");
	preOrderPrint(tree_ptr);

	printf("\n2) in-order print: ");
	inOrderPrint(tree_ptr);

	printf("\n3) past-order print: ");
	postOrderPrint(tree_ptr);

	// delete tokens array
	for (int i = 0; i < tokens_counter; i++) {
		tokens[i][0] = '\0';
	}

	// get reverse polish notation of the tree
	poland(tree_ptr, tokens, &tree_nodes_counter);
	tokens_counter = tree_nodes_counter;

	// print reverse polish notation of the tree
	printf("\n\nReverse polish notation of the tree: ");
	for (int i = 0; i < tokens_counter; i++) {
		printf("%s ", tokens[i]);
	}

	// create stack
	head_ptr = (list*)malloc(sizeof(list));
	head_ptr->prev = NULL;
	head_ptr->next = NULL;

	// calculate the expression
	result = calculate(head_ptr, tokens, tokens_counter);

	if (result == INT_MIN) {
		printf("\n\nZero dividision");
		free(head_ptr);
		deleteTree(tree_ptr);
		_getch();
		return 0;
	}

	//Print the result
	printf("\n\nResult: %d", result);

	//Free memory
	free(head_ptr);
	deleteTree(tree_ptr);

	_getch();
	return 0;
}