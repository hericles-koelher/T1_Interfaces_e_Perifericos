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

bool is_int32(const char *str);
bool is_operation(const char c);

int main(void){

	char first_operand[INT32_SIZE] = {0};
	char op = 0;
	char second_operand[INT32_SIZE] = {0};
	char expression[EXPR_SIZE] = {0};
	char result[INT32_SIZE] = {0};

	bool success = false;

	printf("Olá, durante a utilização do programa\n");
	printf("informe números no intervalo [%d, %d].\n", INT_MIN, INT_MAX);
	printf("O não cumprimento desta regra implica em resultado incorreto!\n\n");

	// Leitura da entrada...
	while(!success){
		char term;

		printf("Informe o primeiro operando\n");
		scanf("%s", first_operand);
		if(!is_int32(first_operand)){
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
		if(!is_int32(second_operand)){
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
        read(mycalc, result, INT32_SIZE);
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

bool is_int32(const char *str){
	int i = 0;

	// O primeiro caractere pode ser um sinal (- ou +)
	// então é melhor verificar nessa parte
	if(strlen(str) > 1){
		if(isdigit(str[0]) || str[0] == '+' || str[0] == '-'){
			i++;
		}else{
			return false;
		}
	}else{
		return isdigit(str[0]);
	}

	// Verificando se tem algo que não seja um digito
	// no resto da string.
	for(; str[i] != '\0'; i++){
		if(!isdigit(str[i]))
			return false;
	}

	// Verificando se o número está no
	// range de inteiros de 32 bits
	long int test = atol(str);

	if(test >= INT_MIN && test >= INT_MAX)
		return true;
	else
		return false;
}