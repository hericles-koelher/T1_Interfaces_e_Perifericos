#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define EXPR_SIZE 64 // arbitrario
#define NUMBER_SIZE 32	// arbitrario

bool is_number(const char *str);
bool is_operation(const char c);

int main(void){

	char first_operand[NUMBER_SIZE] = {0};
	char op = 0;
	char second_operand[NUMBER_SIZE] = {0};
	char expression[EXPR_SIZE] = {0};
	char result[NUMBER_SIZE] = {0};

	bool success = false;

	// Leitura da entrada...
	while(!success){
		char term;

		printf("Informe o primeiro operando\n");
		scanf("%s", first_operand);
		if(!is_number(first_operand)){
			printf("Entrada incorreta!\n");
			continue;
		}

		printf("Informe uma operação\n");
		scanf(" %c", &op);
		// WTF scanf, pq sem o espaço antes do %c tu fica de palhaçada...
		if(!is_operation(op)){
			printf("Entrada incorreta!\n");
			continue;
		}

		printf("Informe o segundo operando\n");
		scanf("%s", second_operand);
		if(!is_number(second_operand)){
			printf("Entrada incorreta!\n");
			continue;
		}

		success = true;
	}
	sprintf(expression, "%s%c%s", first_operand, op, second_operand);

	int mycalc = open("/dev/mycalc",O_RDWR);

	if(mycalc >= 0){
		printf("Escrevendo dados...");
		write(mycalc, expression, strlen(expression));
		printf("Dados escritos...\n");

		printf("Lendo dados...");
        read(mycalc, result, NUMBER_SIZE);
        printf("Resultado = %s\n", result);
	}else{
		printf("Erro na abertura do arquivo\n");
		return -EIO;
	}

	return 0;
}

bool is_operation(const char c){
	if(c == '+' || c == '-' || c == '*' || c == '/'){
		return true;
	}
	return false;
}

bool is_number(const char *str){
	int i = 0;

	if(strlen(str) > 1){
		if(isdigit(str[0]) || str[0] == '+' || str[0] == '-'){
			i++;
		}else{
			return false;
		}
	}else{
		return isdigit(str[0]);
	}

	for(; str[i] != '\0'; i++){
		if(!isdigit(str[i]))
			return false;
	}

	return true;
}