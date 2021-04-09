#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define DEFAULT_SIZE 256 // arbitrario
#define OPERAND_SIZE 32	// arbitrario

bool is_operation(const char c);

int main(void){

	int first_operand = 0;
	char op = 0;
	int second_operand = 0;
	char expression[DEFAULT_SIZE] = {0};
	char result[DEFAULT_SIZE] = {0};

	bool flag = false;

	// Leitura da entrada...
	while(!flag){
		printf("Informe o primeiro operando\n");
		scanf("%d", &first_operand);
		//fflush(stdin);

//		if(!isnumber(first_operand)){
//			printf("Entrada incorreta!\n");
//			continue;
//		}

		printf("Informe uma operação\n");
		scanf("%c", &op);
		//fflush(stdin);

		if(!is_operation(op)){
			printf("Entrada incorreta!\n");
			continue;
		}

		printf("Informe o segundo operando\n");
        scanf("%d", &second_operand);
		//fflush(stdin);

//        if(!isnumber(second_operand)){
//        	printf("Entrada incorreta!\n");
//        	continue;
//        }

        flag = true;
	}

	sprintf(expression, "%d%c%d", first_operand, op, second_operand);

	int mycalc = open("/dev/mycalc",O_RDWR);

	if(mycalc >= 0){
		printf("Escrevendo dados...");
		write(mycalc, expression, strlen(expression));
		printf("Dados escritos...");

		printf("Lendo dados...");
        read(mycalc, result, DEFAULT_SIZE);
        printf("Resultado = %s\n", result);
	}else{
		printf("Erro na abertura do arquivo\n");
		//TODO: qual macro de erro de i/o?
		return -1;
	}

//	if((mycalc = open("/dev/mycalc",O_RDWR)) < 0){
//    	printf("Lendo dados...");
//    	read(mycalc, result, DEFAULT_SIZE);
//    	printf("Resultado = %s\n", result);
//   	}else{
//    	printf("Erro na abertura do arquivo\n");
//    	//TODO: qual macro de erro de i/o?
//   		return -1;
//   	}

	return 0;
}

bool is_operation(const char c){
	if(c == '+' || c == '-' || c == '*' || c == '/'){
		return true;
	}

	return false;
}