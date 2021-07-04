#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//������� ����������� ������
typedef struct list
{
	char data[10];
	list* prev;
	list* next;
} list;

//���� ������
typedef struct node
{
	char data[10];
	node* left;
	node* right;
} node;

//�������, ������������ ��������� �������
int priority(char input)
{
	switch (input)
	{
	case '+':
	case '-': return 1;
	case '*':
	case '/': return 2;
	}

	return 100;
}

//����������� ������� �������� ������ �� ������ ������� �������
node* makeTree(char tokens[][10], int first, int last)
{
	char buffer;
	int minimum_priority = 100;
	int  bracket_counter = 0;
	int the_lowest_priority_token = 0;
	int current_priority;
	node* tree = new node;

	//���� ������ ������� ������� ������ �� ������ ��������, �� ������� 
	//�������� ����
	if (first == last)
	{
		strcpy_s(tree->data, 10, tokens[first]);
		tree->left = nullptr;
		tree->right = nullptr;
		return tree;
	}

	//���� �� ������ ������� ������� �� ��������, ��� ������� ����� �����,
	//�� ������ �� ������� ������� � ���� ����� � ����������� �����������
	for (int i = first; i <= last; i++)
	{
		buffer = tokens[i][0];

		if (buffer == '(')
		{
			bracket_counter++;
			continue;
		}

		if (buffer == ')')
		{
			bracket_counter--;
			continue;
		}

		if (bracket_counter > 0)
		{
			continue;
		}

		current_priority = priority(buffer);
		if (current_priority <= minimum_priority)
		{
			minimum_priority = current_priority;
			the_lowest_priority_token = i;
		}
	}

	//���� ������ � ��������� ������ ��� ������ � ����� ���� ��� �������������� ����������, 
	//�� ��������� �� ���� ������
	if (minimum_priority == 100 && tokens[first][0] == '(' && tokens[last][0] == ')')
	{
		return makeTree(tokens, first + 1, last - 1);
	}

	//���������� ���������� � ���� ������ � ������ ����������� ������
	strcpy_s(tree->data, 10, tokens[the_lowest_priority_token]);
	tree->left = makeTree(tokens, first, the_lowest_priority_token - 1);
	tree->right = makeTree(tokens, the_lowest_priority_token + 1, last);

	//���������� ���� ������
	return tree;
}

//����� �� ������ ������� ������ ������
void preOrderPrint(node * root)
{
	if (root != nullptr)
	{
		printf(" %s", root->data);
		preOrderPrint(root->left);
		preOrderPrint(root->right);
	}

	else
	{
		printf(" ... ");
	}
}

//����� �� ������ ������������� ������ ������
void inOrderPrint(node * root)
{
	if (root != nullptr)
	{
		inOrderPrint(root->left);
		printf(" %s", root->data);
		inOrderPrint(root->right);
	}

	else
	{
		printf(" ... ");
	}
}

//����� �� ������ ��������� ������ ������
void postOrderPrint(node * root)
{
	if (root != nullptr)
	{
		postOrderPrint(root->left);
		postOrderPrint(root->right);
		printf(" %s", root->data);
	}

	else
	{
		printf(" ... ");
	}
}

//�������� �������� �������� ������ �� ������ ������
void poland(node * root, char result[][10], int* counter)
{
	if (root != nullptr)
	{
		poland(root->left, result, counter);
		poland(root->right, result, counter);

		for (int i = 0; i < 10; i++)
		{
			result[*counter][i] = root->data[i];
		}

		*counter += 1;
	}
}

//������� ������ �� ��� ������
void deleteTree(node * root)
{
	if (root != nullptr)
	{
		deleteTree(root->left);
		deleteTree(root->right);
		delete root;
	}
}

//�������, ����������� ��������� �� ���������� ��������
bool inputIsCorrect(char user_input[])
{
	if (strstr(user_input, "/0") != nullptr)
	{
		return false;
	}

	if (strstr(user_input, "++") != nullptr || strstr(user_input, "--") != nullptr || strstr(user_input, "**") != nullptr || strstr(user_input, "//") != nullptr)
	{
		return false;
	}

	return true;
}

//�������, ����������� ������ ��������� �� ������ � �������� ���������� ����������� � ����������� ������
bool getTokens(char user_input[], char tokens[][10], int* tokens_counter)
{
	int name_length = 0;
	int number_lenght = 0;
	int bracket_counter = 0;

	bool is_name_flag = true;
	bool is_number_flag = true;
	bool is_math_symbol_flag = true;
	bool token_is_processed_flag = false;

	//������ ��������� �� ������
	for (int i = 0; i < strlen(user_input); i++)
	{
		token_is_processed_flag = false;

		//���� ������� ������ - ���� �� �������������� ��������
		if (user_input[i] == '+' || user_input[i] == '-' || user_input[i] == '*' || user_input[i] == '/' || user_input[i] == '(' || user_input[i] == ')')
		{
			if (i != 0) * tokens_counter += 1;

			is_math_symbol_flag = true;
			is_name_flag = false;
			is_number_flag = false;
			name_length = 0;
			number_lenght = 0;

			if (user_input[i] == '(') bracket_counter++;
			if (user_input[i] == ')') bracket_counter--;

			tokens[*tokens_counter][0] = user_input[i];
			tokens[*tokens_counter][1] = '\0';

			token_is_processed_flag = true;
		}

		//���� ������� ������ - ����� ����� ����������
		if (user_input[i] >= 'a' && user_input[i] <= 'z')
		{
			if (is_name_flag == 0) * tokens_counter += 1;

			is_name_flag = true;
			is_number_flag = false;
			is_math_symbol_flag = false;
			number_lenght = 0;

			if (is_number_flag != 0) * tokens_counter += 1;

			tokens[*tokens_counter][name_length] = user_input[i];
			tokens[*tokens_counter][name_length + 1] = '\0';

			name_length++;

			token_is_processed_flag = true;
		}

		//���� ������� ������ - �����
		if (user_input[i] >= '0' && user_input[i] <= '9')
		{
			if (is_number_flag == 0) * tokens_counter += 1;

			is_number_flag = true;
			is_name_flag = false;
			is_math_symbol_flag = false;
			name_length = 0;

			tokens[*tokens_counter][number_lenght] = user_input[i];
			tokens[*tokens_counter][number_lenght + 1] = '\0';

			number_lenght++;

			token_is_processed_flag = true;
		}

		//���� ������� ������ �� ��������� �� � ����� �� ���������
		if (token_is_processed_flag == 0)
		{
			return false;
		}
	}

	//���� �� ��������� ���������� ����������� � ����������� ������
	if (bracket_counter != false)
	{
		return false;
	}

	return true;
}

//�������, ���������� �� ���������� �������� ����������
void defineVariables(char tokens[][10], char variable_names[][10], char variable_values[][10], int* tokens_counter)
{
	bool variable_is_found_flag = false;
	int current_variable_value;
	int variable_value_input = 0;

	for (int i = 0; i < *tokens_counter; i++)
	{
		//���� ��������� ����� ��� ��� ����������
		if (tokens[i][0] >= 'a' && tokens[i][0] <= 'z')
		{
			variable_is_found_flag = false;

			//���������, �� ���� �� ��� ������������ �������� ���� ����������
			for (int j = 0; j < 50; j++)
			{
				//���� ��
				if (strcmp(variable_names[j], tokens[i]) == 0)
				{
					current_variable_value = atoi(variable_values[j]);

					//���� �������� ���������� ���������������, �� ������ �������������� �����
					if (current_variable_value >= 0)
					{
						_itoa_s(current_variable_value, tokens[i], 10, 10);
					}

					//� ����� �������� �������� ���� ���������� � ������ ������� � ���� (-...)
					else
					{
						current_variable_value *= -1;
						for (int k = *tokens_counter + 4; k > i; k--)
						{
							for (int m = 0; m < 10; m++)
							{
								tokens[k][m] = tokens[k - 4][m];
							}
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

					variable_is_found_flag = true;
				}
			}

			//���� �� ������������ ��� �� ������ �������� ���� ����������
			if (variable_is_found_flag == false)
			{
				printf("\n��������� �������� ���������� %s (������� 0, ����� ������������� ��������� ����� �� 0 �� 100): ", tokens[i]);

				//�� ���� ��������� �� ����� ��� ����
				scanf_s("%d", &variable_value_input);

				//���� ���������� ��������� �����
				if (variable_value_input == 0)
				{
					variable_value_input = rand() % 100;
					printf("���������� ��������� �������� %d \n", variable_value_input);
				}

				//���������� ��� � �������� ���������� � �������������� �������
				strcpy_s(variable_names[i], 10, tokens[i]);
				_itoa_s(variable_value_input, variable_values[i], 10, 10);

				//���� �������� ���������� ���������������, �� ������ �������������� �����
				if (variable_value_input >= 0)
				{
					_itoa_s(variable_value_input, tokens[i], 10, 10);
				}

				//� ����� �������� �������� ���� ���������� � ������ ������� � ���� (-...)
				else
				{
					variable_value_input *= -1;

					for (int k = *tokens_counter + 4; k > i; k--)
					{
						for (int m = 0; m < 10; m++)
						{
							tokens[k][m] = tokens[k - 4][m];
						}
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

//�������, ����������������� ���������, ���� � ��� ������������ ������� �������� + � -
void unaryOperationsConvertions(char tokens[][10], int* tokens_counter)
{
	for (int k = 0; k < *tokens_counter; k++)
	{
		//���� ������ �������� � ��������� ����� �����
		if (tokens[k][0] == '-' && k == 0)
		{
			for (int i = *tokens_counter; i > 0; i--)
			{
				for (int j = 0; j < 10; j++)
				{
					tokens[i][j] = tokens[i - 1][j];
				}
			}
			
			tokens[0][0] = '0';
			tokens[0][1] = '\0';
			tokens[1][0] = '-';
			tokens[1][1] = '\0';
			
			*tokens_counter += 1;
		}

		//���� ������ �������� � ��������� ����� ����
		if (tokens[k][0] == '+' && k == 0)
		{
			for (int i = *tokens_counter; i > 0; i--)
			{
				for (int j = 0; j < 10; j++)
				{
					tokens[i][j] = tokens[i - 1][j];
				}
			}
			
			tokens[0][0] = '0';
			tokens[0][1] = '\0';
			tokens[1][0] = '+';
			tokens[1][1] = '\0';
			
			*tokens_counter += 1;
		}

		//���� ������ �������� � �����-�� �� ������ ����� �����
		if (tokens[k][0] == '-' && k != 0 && tokens[k - 1][0] == '(')
		{
			for (int i = *tokens_counter + k; i > k; i--)
			{
				for (int j = 0; j < 10; j++)
				{
					tokens[i][j] = tokens[i - 1][j];
				}
			}
			
			tokens[k][0] = '0';
			tokens[k][1] = '\0';
			tokens[k + 1][0] = '-';
			tokens[k + 1][1] = '\0';
			
			*tokens_counter += 1;
		}

		//���� ������ �������� � �����-�� �� ������ ����� ����
		if (tokens[k][0] == '+' && k != 0 && tokens[k - 1][0] == '(')
		{
			for (int i = *tokens_counter + k; i > k; i--)
			{
				for (int j = 0; j < 10; j++)
				{
					tokens[i][j] = tokens[i - 1][j];
				}
			}
			
			tokens[k][0] = '0';
			tokens[k][1] = '\0';
			tokens[k + 1][0] = '+';
			tokens[k + 1][1] = '\0';
			
			*tokens_counter += 1;
		}
	}
}

//�������, ������������ ��������� � �������������� �����
int calculate(list * head, char tokens[][10], int tokens_counter)
{
	int first_operand, second_operand, result;
	char current_token[10];

	for (int i = 0; i < tokens_counter; i++)
	{
		strcpy_s(current_token, 10, tokens[i]);

		//���� ��������� ����� ��� �����, �� ������ ���������� ��� � ����
		if (current_token[0] >= '0' && current_token[0] < '9')
		{
			list* add = new list;
			strcpy_s(add->data, 10, current_token);
			add->prev = nullptr;
			add->next = head;
			head = add;
		}

		//���� �� ��������� ����� ��� �������������� ��������, �� ����� �� ����� ���
		//������� ��������, ��������� ����������� �������� � ������ ��������� �� ������� �����
		if (current_token[0] == '+')
		{
			first_operand = atoi(head->next->data);
			second_operand = atoi(head->data);

			head = head->next;
			delete head->prev;
			head->prev = nullptr;
			
			_itoa_s(first_operand + second_operand, head->data, 10, 10);
		}
		if (current_token[0] == '-')
		{
			first_operand = atoi(head->next->data);
			second_operand = atoi(head->data);

			head = head->next;
			delete head->prev;
			head->prev = nullptr;
			
			_itoa_s(first_operand - second_operand, head->data, 10, 10);
		}
		if (current_token[0] == '*')
		{
			first_operand = atoi(head->next->data);
			second_operand = atoi(head->data);

			head = head->next;
			delete head->prev;
			head->prev = nullptr;
			
			_itoa_s(first_operand * second_operand, head->data, 10, 10);
		}
		if (current_token[0] == '/')
		{
			first_operand = atoi(head->next->data);
			second_operand = atoi(head->data);

			//���� ��������� ������� �� ����
			if (second_operand == 0)
			{
				return INT_MIN;
			}

			head = head->next;
			delete head->prev;
			head->prev = nullptr;
			
			_itoa_s(first_operand / second_operand, head->data, 10, 10);
		}
	}

	return atoi(head->data);
}

int main()
{
	srand(time(NULL));
	setlocale(0, "russian");

	char user_input[50], tokens[50][10], variable_names[50][10], variable_values[50][10];
	int  tokens_counter = 0;
	int tree_nodes_counter = 0;
	int result;

	node* tree;
	list* head;

	//������������ ������ ���������
	printf("�����������\n\n�������������� ������ ����� �����, �������� +, -, *, / � ������.\n����� ����� ������������ ���������� � ����������, ����������� ���������.\n");
	printf("� ���� ������ ����� ����� ��������� ��������� �������� � ��� �������� ���� ����������.\n��������� �������� ������ � � ���� ������, � �� ������ ���� ������ 50 ��������.\n\n");
	printf("������� ���������: ");

	gets_s(user_input, 50);

	//�������� ��������� �� ���������� ��������
	if (!inputIsCorrect(user_input))
	{
		printf("\n��������� ������� �� ���� ���� ��������� �������� ��������");
		_getch();
		return 0;
	}

	//������ ��������� �� ������ � �������� ���������� ����������� � ����������� ������
	if (!getTokens(user_input, tokens, &tokens_counter))
	{
		printf("\n��������� �������� ������������ ������, ���� �� ��������� ���������� ����������� � ����������� ������");
		_getch();
		return 0;
	}

	tokens_counter++;

	//������� ������ ���� ����������
	for (int i = 0; i < 50; ++i)
	{
		variable_names[i][0] = '\0';
	}

	//���������� �������� ����������
	defineVariables(tokens, variable_names, variable_values, &tokens_counter);

	//�������������� ���������, ���� � ��� ������������ ������� ����� + � -
	unaryOperationsConvertions(tokens, &tokens_counter);

	//������� ������ �� ������
	printf("\n�������� ��������� �� ������ � ������ ���������� � ������� ������� � �������:\n");
	for (int i = 0; i < tokens_counter; i++)
	{
		printf("%s ", tokens[i]);
	}

	//�������� ������
	printf("\n\n�������� �� ������� �������� ������, �������:");
	tree = makeTree(tokens, 0, tokens_counter - 1);

	//���������������� ��������� ������ ����� ������
	printf("\n1)������ �����: ");
	preOrderPrint(tree);

	printf("\n2)������������ �����: ");
	inOrderPrint(tree);

	printf("\n3)�������� �����: ");
	postOrderPrint(tree);

	//������� ������ � ��������
	for (int i = 0; i < tokens_counter; i++)
	{
		tokens[i][0] = '\0';
	}

	//���������� � ������, ��� ���� ������, �������� ��������� ������ ���������
	poland(tree, tokens, &tree_nodes_counter);
	tokens_counter = tree_nodes_counter;

	//������� �������� �������� ������ �� ������
	printf("\n\n�������� �������� �������� ������: ");
	for (int i = 0; i < tokens_counter; i++)
	{
		printf("%s ", tokens[i]);
	}

	//������� ����
	head = new list;
	head->prev = nullptr;
	head->next = nullptr;

	//��������� ��������� � �������������� �����
	result = calculate(head, tokens, tokens_counter);

	if (result == INT_MIN)
	{
		printf("\n\n ��������� ������� �� ����");
		delete head;
		deleteTree(tree);
		_getch();
		return 0;
	}

	//����� �� ������ ����������
	printf("\n\n���������: %d", result);

	//������� ������
	delete head;
	deleteTree(tree);

	_getch();
	return 0;
}