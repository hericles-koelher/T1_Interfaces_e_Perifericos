#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

#define INT32_SIZE 12					// um int 32 tem no maximo essa quantidade
										// de caracteres (incluindo '\0')...

#define EXPR_SIZE (INT32_SIZE*2) 		// cabem exatamente 2 inteiros e uma operação

void clear_buffer(void);
bool is_int32(const long int);
bool is_operation(const char c);
bool was_buffer_clean(void);
bool safe_scan_int32(int *);

int main(void){
	int first_operand;
	char operation;
	int second_operand;
	char expression[EXPR_SIZE] = {0};
	char result[INT32_SIZE * 2] = {0};

	bool success = false;
	int scanned = 0;

	printf("Olá, durante a utilização do programa\n");
	printf("informe números no intervalo [%d, %d].\n", INT_MIN, INT_MAX);
	printf("O não cumprimento desta regra implica em resultado inconsistente!\n\n");

	// Leitura da entrada...
	while(!success){
		printf("Informe o primeiro operando\n");

		if(!safe_scan_int32(&first_operand)) continue;

		printf("Informe uma operação\n");
		scanned = scanf("%c", &operation);
		if(!was_buffer_clean() || !scanned || !is_operation(operation)){
			printf("Entrada incorreta!\n");
			continue;
		}

		printf("Informe o segundo operando\n");
		if(!safe_scan_int32(&second_operand)) continue;

		success = true;
	}

	sprintf(expression, "%d%c%d", first_operand, operation, second_operand);

	int mycalc = open("/dev/mycalc",O_RDWR);

	if(mycalc >= 0){
		printf("Escrevendo dados...");
		write(mycalc, expression, strlen(expression));
		printf(" Dados escritos...\n");

		printf("Lendo dados...");
        read(mycalc, result, INT32_SIZE*2);
        printf("Resultado = %s\n", result);
	}else{
		printf("Erro na abertura do arquivo\n");
		return -EIO;
	}

	return 0;
}


bool  was_buffer_clean() {
	bool was_clean = true;
	char c;
    while ((c = getchar()) != '\n' && c != EOF) {
    	// Entra no while quando encontra um char não esperado
    	was_clean = false;
    };
    return was_clean;
}

bool is_operation(const char c){
	return c == '+' || c == '-' || c == '*' || c == '/';
}

bool is_int32(const long int entry){
	return entry >= INT_MIN && entry <= INT_MAX;
}

bool safe_scan_int32(int *entry){
	long int entry_aux;
	int scanned = scanf("%ld", &entry_aux);
	if(!was_buffer_clean() || !scanned || !is_int32(entry_aux)){
		printf("Entrada incorreta!\n");
		return false;
	}
	*entry = (int)entry_aux;
	return true;
}
