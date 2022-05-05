// Made by Y. Sendov. March 2022

#define _CRT_SECURE_NO_WARNINGS
#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

char commands[4][6] = { "get_c", "get_a", "lcg", "bits" };
char arguments[6][4] = { "s=", "m=", "x0=", "n=", "c=", "a=" };
unsigned long long numbers[6] = { 0, 0, 0, 0, 0, 0 };
unsigned long long sort_num[5] = { 0, 0, 0, 0, 0 };
unsigned long long devisors[128] = { 1 };
int dev_count = 0; // Количество делителей
int convert_num[64] = { 0 };
int count_bits[64] = { 0 };

// Функция поиска наибольшего общего делителя
unsigned long long find_gcd(unsigned long long first, unsigned long long second) 
{
	while (first != second)
	{
		if (first > second)
		{
			unsigned long long tmp = first;
			first = second;
			second = tmp;
		}
		second = second - first;
	}
	return first;
}

// Команда get_c
void get_c()
{
	unsigned long long s = sort_num[0];
	unsigned long long m = sort_num[1];
	if (s > 64)
	{
		FILE* outfile = fopen("output.txt", "w");
		fprintf(outfile, "incorrect command\n");
		fclose(outfile);
		return;
	}
	unsigned long long selected_numbers[10] = { 0 };
	int iter = 0;
	for (int c = pow(2, s-1); c < pow(2, s); c++)
	{
		if (find_gcd(c, m) == 1 && iter < 10)
		{
			selected_numbers[iter] = c;
			iter++;
		}
		if (iter == 10) break;
	}
	if (selected_numbers[0] == 0)
	{
		FILE* outfile = fopen("output.txt", "w");
		fprintf(outfile, "no solution");
		fclose(outfile);
	}
	else
	{
		FILE* outfile = fopen("output.txt", "w");
		for (int i = 0; i < 10; i++)
		{
			if (selected_numbers[i] == 0) break;
			fprintf(outfile, "%llu\n", selected_numbers[i]);
		}
		fclose(outfile);
	}
}

// Проверка на простоту
int check_prime(unsigned long long number)
{
	if (number > 1)
	{
		for (int i = 2; i <= sqrt(number); i++)
		{
			if (i != 2 && i % 2 == 0) continue;
			if (number % i == 0) return 0;
		}
		return 1;
	}
	else return 0;
}

// Поиск простых делителей числа
void find_dev(unsigned long long number)
{
	int count = 1;
	if (number % 4 == 0)
	{
		devisors[count] = 4;
		count++;
	}
	for (unsigned long long i = 2; i <= number; i++)
	{
		if (i != 2 && i % 2 == 0) continue;
		if (number % i == 0 && check_prime(i) == 1)
		{
			devisors[count] = i;
			printf("%llu\n", i);
			count++;
		}
	}
	dev_count = count;
}

// Команда get_a
void get_a()
{
	unsigned long long m = sort_num[0];
	if (m == 0)
	{
		FILE* outfile = fopen("output.txt", "w");
		fprintf(outfile, "incorrect command\n");
		fclose(outfile);
		return;
	}
	find_dev(m);
	unsigned long long search = 0;
	for (unsigned long long a = 2; a < m; a++)
	{
		int flag = 1;
		for (int i = 0; i < dev_count; i++)
		{
			if ((a - 1) % devisors[i] != 0) flag = 0;
		}
		if (flag == 1)
		{
			search = a;
			break;
		}
	}
	if (search == 0)
	{
		FILE* outfile = fopen("output.txt", "w");
		fprintf(outfile, "no solution");
		fclose(outfile);
	}
	else
	{
		FILE* outfile = fopen("output.txt", "w");
		fprintf(outfile, "%llu", search);
		fclose(outfile);
	}
}

// Определение максимального из двух чисел
unsigned long long max_num(unsigned long long number1, unsigned long long number2)
{
	if (number1 > number2) return number1;
	else return number2;
}

// Команда lcg
void lcg()
{
	unsigned long long a = sort_num[0], x0 = sort_num[1], c = sort_num[2], m = sort_num[3], n = sort_num[4];
	unsigned long long x1 = x0;
	unsigned long long count = 0, flag = 0;
	if (n == 0 || a >= m || x0 >= m || c >= m)
	{
		FILE* outfile = fopen("output.txt", "w");
		fprintf(outfile, "no solution");
		fclose(outfile);
	}
	else
	{
		FILE* outfile = fopen("output.txt", "w");
		for (unsigned long long i = 0; i < n; i++)
		{
			x1 = (a * x0 + c) % m;
			if (x1 == flag)
			{
				count = i;
				printf("%llu\n", count);
				//break;
			}
			if (i == 0) flag = x0;
			if (x0 != 0 && (a * x0) < max_num(a, x0))
			{
				fprintf(outfile, "incorrect command (product a * x0 exceeds ULLONG_MAX)");
				return;
			}
			else
			{
				x0 = x1;
				fprintf(outfile, "%llu\n", x1);
			}
		}
		fclose(outfile);
	}
}

// Перевод числа из десятичной системы счисления в двоичную
void convert(unsigned long long number)
{
	memset(convert_num, 0, sizeof(number));
	for (int i = 63; i >= 0; i--)
	{
		convert_num[i] = (number % 2);
		number /= 2;
	}
}

// Анализ числа (Проверка - какие биты установлены у числа)
void analysis(unsigned long long number)
{
	convert(number);
	int count = 63;
	for (int i = 0; i < 64; i++)
	{
		if (convert_num[i] == 1) count_bits[i]++;
		count--;
	}
}

// Вывод из команды bits
void print_bits()
{
	FILE* outfile = fopen("output.txt", "w");
	int count = 63;
	for (int i = 0; i < 64; i++)
	{
		fprintf(outfile, "%d\n", count_bits[i]);
		count--;
	}
	fclose(outfile);
}

// Команда bits
void bits()
{
	unsigned long long a = sort_num[0], x0 = sort_num[1], c = sort_num[2], m = sort_num[3], n = sort_num[4];
	unsigned long long x1 = x0;
	if (n == 0 || a >= m || x0 >= m || c >= m)
	{
		FILE* outfile = fopen("output.txt", "w");
		fprintf(outfile, "no solution");
		fclose(outfile);
	}
	else
	{
		for (unsigned long long i = 0; i < n; i++)
		{
			x1 = (a * x0 + c) % m;
			if (x0 != 0 && (a * x0) < max_num(a, x0))
			{
				FILE* outfile = fopen("output.txt", "w");
				fprintf(outfile, "incorrect command (product a * x0 exceeds ULLONG_MAX)");
				fclose(outfile);
				return;
			}
			else
			{
				analysis(x1);
				x0 = x1;
			}
		}
		print_bits();
	}
}

int command_correct(char* command)
{
	int correct = 0;
	for (int i = 0; i < 4; i++)
	{
		if (strcmp(command, commands[i]) == 0) correct = 1;
	}
	return correct;
}

int argument_correct(char* argument)
{
	int correct = 0;
	for (int i = 0; i < 6; i++)
	{
		if (strcmp(argument, arguments[i]) == 0) correct = 1;
	}
	return correct;
}

int str_correct(char* str)
{
	int flag = 1;
	for (unsigned int i = 0; i < strlen(str); i++)
	{
		if (str[i] != ' ') flag = 0;
	}
	return flag;
}

int argument_check(char* command, char* arg1, char* arg2, char* arg3, char* arg4, char* arg5)
{
	char lcg_arg[5][4] = { "a=", "x0=", "c=", "m=", "n=" };
	if ((strcmp(arg1, arguments[0]) == 0 && strcmp(arg2, arguments[1]) == 0) || (strcmp(arg1, arguments[1]) == 0 && strcmp(arg2, arguments[0]) == 0) && strcmp(arg3, "") == 0) return 1;
	if (strcmp(arg1, arguments[1]) == 0 && strcmp(arg2, "") == 0) return 2;
	if (strcmp(command, commands[2]) == 0 || strcmp(command, commands[3]) == 0)
	{
		int value = 0;
		for (int i = 0; i < 5; i++)
		{
			if (strcmp(arg1, lcg_arg[i]) == 0)
			{
				value++;
				continue;
			}
			else if (strcmp(arg2, lcg_arg[i]) == 0)
			{
				value++;
				continue;
			}
			else if (strcmp(arg3, lcg_arg[i]) == 0)
			{
				value++;
				continue;
			}
			else if (strcmp(arg4, lcg_arg[i]) == 0)
			{
				value++;
				continue;
			}
			else if (strcmp(arg5, lcg_arg[i]) == 0)
			{
				value++;
				continue;
			}
		}
		if (strcmp(command, commands[2]) == 0 && value == 5) return 3;
		if (strcmp(command, commands[3]) == 0 && value == 5) return 4;
	}
	return 0;
}

void argument_sort(char* command, char* arg1, char* arg2, char* arg3, char* arg4, char* arg5)
{
	// get_c
	if (strcmp(command, commands[0]) == 0)
	{
		if (strcmp(arg1, arguments[0]) == 0)
		{
			sort_num[0] = numbers[0];
			sort_num[1] = numbers[1];
		}
		else if (strcmp(arg1, arguments[1]) == 0)
		{
			sort_num[0] = numbers[1];
			sort_num[1] = numbers[0];
		}
	}
	// get_a
	if (strcmp(command, commands[1]) == 0)
	{
		sort_num[0] = numbers[0];
	}
	// lcg and bits
	if (strcmp(command, commands[2]) == 0 || strcmp(command, commands[3]) == 0)
	{
		char args[5][4] = { "a=", "x0=", "c=", "m=", "n=" };
		for (int i = 0; i < 5; i++)
		{
			if (strcmp(arg1, args[i]) == 0)
			{
				sort_num[i] = numbers[0];
				continue;
			}
			else if (strcmp(arg2, args[i]) == 0)
			{
				sort_num[i] = numbers[1];
				continue;
			}
			else if (strcmp(arg3, args[i]) == 0)
			{
				sort_num[i] = numbers[2];
				continue;
			}
			else if (strcmp(arg4, args[i]) == 0)
			{
				sort_num[i] = numbers[3];
				continue;
			}
			else if (strcmp(arg5, args[i]) == 0)
			{
				sort_num[i] = numbers[4];
				continue;
			}
		}
	}
}

int main()
{
	// Парсер файла
	FILE* file = fopen("input.txt", "r");
	if (file == NULL)
	{
		printf("Error! The source file was not found. Expected: input.txt\n");
		printf("Check for the file and run the program again.\n");
		return -1;
	}

	char symbol = 0; // переменная для считываемого символа
	char command[6] = { 0 }; // переменная для команды

	int is_space = TRUE, ready = FALSE, mark = FALSE, readynum = FALSE; // необходимые флаги

	char argument[6][4] = { 0, 0, 0, 0, 0, 0 }; // массив, содержащий все аргументы
	int a_count = 0; // переменная для индексации аргументов

	int n_count = 0; // переменная для индексации чисел

	int index = 1, last_index = 0, last_n_index = 0; // переменные для индексации всех символов, хранения индекса последнего символа в числе
	int c_index = 0, a_index = 0; // переменные для индексации символов в команде и аргументе

	char strspace[128] = " "; // переменная для хранения строки между командой и аргументом (аргументом и аргументом)
	int str_index = 0; // переменная для индексации данной строки

	char number[128] = { 0 }; // переменная для считывания числа
	int num_index = 0; // переменная для индексации строки числа

	int correct = 1; // маркер корректности всей команды

	while (!feof(file))
	{
		symbol = fgetc(file);

		// Если строка после команды (до аргумента) некорректна - команда некорректна
		if (command_correct(command) == TRUE && str_correct(strspace) == FALSE)
		{
			correct = 0;
			if (strcmp(command, commands[0]) == 0 && a_count == 2) correct = 1;
			if (strcmp(command, commands[1]) == 0 && a_count == 1) correct = 1;
			if (strcmp(command, commands[2]) == 0 && a_count == 5) correct = 1;
			if (strcmp(command, commands[3]) == 0 && a_count == 5) correct = 1;
			break;
		}

		// Парсинг аргумента (Если название команды корректно считано)
		if (ready == 1 && argument[a_count][0] == 0)
		{
			if (is_space == TRUE && (symbol == arguments[0][a_index] || symbol == arguments[1][a_index] || symbol == arguments[2][a_index] || symbol == arguments[3][a_index] || symbol == arguments[4][a_index] || symbol == arguments[5][a_index]) && index - last_index == 1)
			{
				mark = TRUE;
				argument[a_count][a_index] = symbol;
				a_index++;
			}
		}
		else if (ready == TRUE && (symbol == arguments[0][a_index] || symbol == arguments[1][a_index] || symbol == arguments[2][a_index] || symbol == arguments[3][a_index] || symbol == arguments[4][a_index] || symbol == arguments[5][a_index]) && index - last_index == 1)
		{
			argument[a_count][a_index] = symbol;
			a_index++;
		}
		else a_index = 0;
		
		if ((symbol == ' ' || symbol == EOF) && index - last_n_index == 1)
		{
			readynum = TRUE;
			mark = FALSE;
			number[21] = '\0';
			numbers[n_count] = strtoull(number, NULL, 0);
			n_count++;
			if (errno == ERANGE)
			{
				correct = 0;
				break;
			}
			strcpy(strspace, " ");
			str_index = 0;
			a_index = 0;
			a_count++;
			if (a_count > 5) break;
			memset(number, 0, sizeof(number));
			num_index = 0;
		}

		// Проверка на отсутствие лишних символов в числе
		if ((command_correct(command) == TRUE && argument_correct(argument[a_count]) == TRUE && symbol == '-') || (readynum == FALSE && isdigit(number[0]) != 0 && isdigit(symbol) == 0))
		{
			correct = 0;
			break;
		}

		// Парсинг числа
		if (command_correct(command) == TRUE && argument_correct(argument[a_count]) == TRUE && isdigit(symbol) != 0)
		{
			mark = TRUE;
			number[num_index] = symbol;
			num_index++;
			last_n_index = index;
		}

		if (ready == TRUE && mark == FALSE)
		{
			strspace[str_index] = symbol;
			str_index++;
		}

		// Парсинг команд
		if (c_index == 4 && (symbol == commands[0][c_index] || symbol == commands[1][c_index]))
		{
			command[c_index] = symbol;
			c_index++;
		}
		if (command[0] == 0)
		{
			if (ready == FALSE && is_space == TRUE && (symbol == commands[0][c_index] || symbol == commands[2][c_index] || symbol == commands[3][c_index]) && index - last_index == 1)
			{
				command[c_index] = symbol;
				c_index++;
			}
		}
		else if (ready == FALSE && (symbol == commands[0][c_index] || symbol == commands[2][c_index] || symbol == commands[3][c_index]) && index - last_index == 1)
		{
			command[c_index] = symbol;
			c_index++;
		}
		else c_index = 0;

		// Считывание пробела
		if (symbol == ' ') is_space = TRUE;
		else is_space = FALSE;
		
		// Если название команды считано корректно, то она готова к дальнейшему парсингу
		if (command_correct(command) == TRUE) ready = TRUE;

		// Индекс
		last_index = index;
		index++;
	}

	for (int i = 0; i < 5; i++)
	{
		argument[i][3] = '\0';
	}
	command[5] = '\0';
	fclose(file);

	if (correct == 1)
	{
		// Распределение вызовов команд
		if (strcmp(command, commands[0]) == 0 && argument_check(command, argument[0], argument[1], argument[2], argument[3], argument[4]) == 1)
		{
			argument_sort(command, argument[0], argument[1], argument[2], argument[3], argument[4]);
			get_c();
		}
		else if (strcmp(command, commands[1]) == 0 && argument_check(command, argument[0], argument[1], argument[2], argument[3], argument[4]) == 2)
		{
			argument_sort(command, argument[0], argument[1], argument[2], argument[3], argument[4]);
			get_a();
		}
		else if (strcmp(command, commands[2]) == 0 && argument_check(command, argument[0], argument[1], argument[2], argument[3], argument[4]) == 3)
		{
			argument_sort(command, argument[0], argument[1], argument[2], argument[3], argument[4]);
			lcg();
		}
		else if (strcmp(command, commands[3]) == 0 && argument_check(command, argument[0], argument[1], argument[2], argument[3], argument[4]) == 4)
		{
			argument_sort(command, argument[0], argument[1], argument[2], argument[3], argument[4]);
			bits();
		}
		else
		{
			FILE* outfile = fopen("output.txt", "w");
			fprintf(outfile, "incorrect command\n");
			fclose(outfile);
			return 0;
		}
	}
	else
	{
		FILE* outfile = fopen("output.txt", "w");
		fprintf(outfile, "incorrect command\n");
		fclose(outfile);
	}
}

// 18446744073709551615 - 21