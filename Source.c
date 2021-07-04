#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//элемент двусвязного списка
typedef struct list
{
	char data[10];
	list* prev;
	list* next;
} list;

//узел дерева
typedef struct node
{
	char data[10];
	node* left;
	node* right;
} node;

//функция, определяюцая приоритет операци
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

//рекурсивная функция создания дерева на основе массива токенов
node* makeTree(char tokens[][10], int first, int last)
{
	char buffer;
	int minimum_priority = 100;
	int  bracket_counter = 0;
	int the_lowest_priority_token = 0;
	int current_priority;
	node* tree = new node;

	//если массив токенов состоит только из одного элемента, то создаем 
	//концевой узел
	if (first == last)
	{
		strcpy_s(tree->data, 10, tokens[first]);
		tree->left = nullptr;
		tree->right = nullptr;
		return tree;
	}

	//если же массив токенов состоит из большего, чем единица числа строк,
	//то шагаем по массиву токенов и ищем токен с минимальным приоритетом
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

	//если первый и последний токены это скобки и между ними нет математических операторов, 
	//то избавимся от этих скобок
	if (minimum_priority == 100 && tokens[first][0] == '(' && tokens[last][0] == ')')
	{
		return makeTree(tokens, first + 1, last - 1);
	}

	//записываем информацию в узел дерева и делаем рекурсивные вызовы
	strcpy_s(tree->data, 10, tokens[the_lowest_priority_token]);
	tree->left = makeTree(tokens, first, the_lowest_priority_token - 1);
	tree->right = makeTree(tokens, the_lowest_priority_token + 1, last);

	//возвращаем узел дерева
	return tree;
}

//вывод на печать прямого обхода дерева
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

//вывод на печать симметричного обхода дерева
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

//вывод на печать обратного обхода дерева
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

//получить обратную польскую запись на основе дерева
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

//очистка память из под дерева
void deleteTree(node * root)
{
	if (root != nullptr)
	{
		deleteTree(root->left);
		deleteTree(root->right);
		delete root;
	}
}

//функция, проверяющая выражение на простейшие опечатки
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

//функция, выполняющая разбор выражения на токены и проверку количества открывающих и закрывающих скобок
bool getTokens(char user_input[], char tokens[][10], int* tokens_counter)
{
	int name_length = 0;
	int number_lenght = 0;
	int bracket_counter = 0;

	bool is_name_flag = true;
	bool is_number_flag = true;
	bool is_math_symbol_flag = true;
	bool token_is_processed_flag = false;

	//разбор выражения на токены
	for (int i = 0; i < strlen(user_input); i++)
	{
		token_is_processed_flag = false;

		//если текущий символ - одна из математических операций
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

		//если текущий символ - часть имени переменной
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

		//если текущий символ - цифра
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

		//если текущий символ не относится ни к одной из категорий
		if (token_is_processed_flag == 0)
		{
			return false;
		}
	}

	//если не совпадает количество открывающих и закрывающих скобок
	if (bracket_counter != false)
	{
		return false;
	}

	return true;
}

//функция, отвечающая за присвоение значений переменным
void defineVariables(char tokens[][10], char variable_names[][10], char variable_values[][10], int* tokens_counter)
{
	bool variable_is_found_flag = false;
	int current_variable_value;
	int variable_value_input = 0;

	for (int i = 0; i < *tokens_counter; i++)
	{
		//если очередной токен это имя переменной
		if (tokens[i][0] >= 'a' && tokens[i][0] <= 'z')
		{
			variable_is_found_flag = false;

			//проверяем, не ввел ли уже пользователь значение этой переменной
			for (int j = 0; j < 50; j++)
			{
				//если да
				if (strcmp(variable_names[j], tokens[i]) == 0)
				{
					current_variable_value = atoi(variable_values[j]);

					//если значение переменной неотрицательное, то просто перезаписываем токен
					if (current_variable_value >= 0)
					{
						_itoa_s(current_variable_value, tokens[i], 10, 10);
					}

					//а иначе всталяем значение этой переменной в массив токенов в виде (-...)
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

			//если же пользователь еще не вводил значение этой переменной
			if (variable_is_found_flag == false)
			{
				printf("\nОпределим значение переменной %s (вводите 0, чтобы сгенерировать случайное число от 0 до 100): ", tokens[i]);

				//то либо оставляем ее какой она есть
				scanf_s("%d", &variable_value_input);

				//либо генерируем случайное число
				if (variable_value_input == 0)
				{
					variable_value_input = rand() % 100;
					printf("Переменной присвоено значение %d \n", variable_value_input);
				}

				//записываем имя и значение переменной в соотвествующие массивы
				strcpy_s(variable_names[i], 10, tokens[i]);
				_itoa_s(variable_value_input, variable_values[i], 10, 10);

				//если значение переменной неотрицательное, то просто перезаписываем токен
				if (variable_value_input >= 0)
				{
					_itoa_s(variable_value_input, tokens[i], 10, 10);
				}

				//а иначе всталяем значение этой переменной в массив токенов в виде (-...)
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

//функция, преобразовывающая выражение, если в нем присутствуют унарные операции + и -
void unaryOperationsConvertions(char tokens[][10], int* tokens_counter)
{
	for (int k = 0; k < *tokens_counter; k++)
	{
		//если первым символом в выражении стоит минус
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

		//если первым символом в выражении стоит плюс
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

		//если первым символом в какой-то из скобок стоит минус
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

		//если первым символом в какой-то из скобок стоит плюс
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

//функция, высчисляющая выражение с использованием стека
int calculate(list * head, char tokens[][10], int tokens_counter)
{
	int first_operand, second_operand, result;
	char current_token[10];

	for (int i = 0; i < tokens_counter; i++)
	{
		strcpy_s(current_token, 10, tokens[i]);

		//если очередной токен это число, то просто записываем его в стек
		if (current_token[0] >= '0' && current_token[0] < '9')
		{
			list* add = new list;
			strcpy_s(add->data, 10, current_token);
			add->prev = nullptr;
			add->next = head;
			head = add;
		}

		//если же очередной токен это математический оператор, то берем из стека два
		//верхних элемента, выполняем необходимое действие и кладем результат на вершину стека
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

			//если произошло деление на ноль
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

	//пользователь вводит выражение
	printf("КАЛЬКУЛЯТОР\n\nПоддерживаются только целые числа, операции +, -, *, / и скобки.\nМожно также использовать переменные с названиями, написанными латиницей.\n");
	printf("В этом случае после ввода выражения программа запросит у вас значения этих переменных.\nВыражение вводится слитно и в одну строку, и не должно быть длинее 50 символов.\n\n");
	printf("Введите выражение: ");

	gets_s(user_input, 50);

	//проверка выражения на простейшие опечатки
	if (!inputIsCorrect(user_input))
	{
		printf("\nПроизошло деление на ноль либо выражение содержит опечатку");
		_getch();
		return 0;
	}

	//разбор выражения на токены и проверка количества открывающих и закрывающих скобок
	if (!getTokens(user_input, tokens, &tokens_counter))
	{
		printf("\nВыражение содержит недопустимый символ, либо не совпадает количество открывающих и закрывающих скобок");
		_getch();
		return 0;
	}

	tokens_counter++;

	//очистим массив имен переменных
	for (int i = 0; i < 50; ++i)
	{
		variable_names[i][0] = '\0';
	}

	//определяем значения переменных
	defineVariables(tokens, variable_names, variable_values, &tokens_counter);

	//преобразование выражения, если в нем присутствуют унарные знаки + и -
	unaryOperationsConvertions(tokens, &tokens_counter);

	//выведем токены на печать
	printf("\nРазобьем выражение на токены с учетом переменных и унарных минусов и получим:\n");
	for (int i = 0; i < tokens_counter; i++)
	{
		printf("%s ", tokens[i]);
	}

	//создадим дерево
	printf("\n\nПостроим по токенам бинарное дерево, получим:");
	tree = makeTree(tokens, 0, tokens_counter - 1);

	//продемонстрируем различные обходы этого дерева
	printf("\n1)прямой обход: ");
	preOrderPrint(tree);

	printf("\n2)симметричный обход: ");
	inOrderPrint(tree);

	printf("\n3)обратный обход: ");
	postOrderPrint(tree);

	//очищаем массив с токенами
	for (int i = 0; i < tokens_counter; i++)
	{
		tokens[i][0] = '\0';
	}

	//записываем в массив, где были токены, обратную пользскую запись выражения
	poland(tree, tokens, &tree_nodes_counter);
	tokens_counter = tree_nodes_counter;

	//выводим обратную польскую запись на печать
	printf("\n\nПолучаем обратную польскую запись: ");
	for (int i = 0; i < tokens_counter; i++)
	{
		printf("%s ", tokens[i]);
	}

	//создаем стек
	head = new list;
	head->prev = nullptr;
	head->next = nullptr;

	//вычисляем выражение с использованием стека
	result = calculate(head, tokens, tokens_counter);

	if (result == INT_MIN)
	{
		printf("\n\n Произошло деление на ноль");
		delete head;
		deleteTree(tree);
		_getch();
		return 0;
	}

	//вывод на печать результата
	printf("\n\nРезультат: %d", result);

	//очистка памяти
	delete head;
	deleteTree(tree);

	_getch();
	return 0;
}