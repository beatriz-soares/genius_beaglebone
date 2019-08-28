#include <iostream>
#include "../BlackGPIO/BlackGPIO.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

using namespace BlackLib;

/*
COMO ESSE PROGRAMA FUNCIONA?

- Esperamos o player apertar (e soltar) o botão 1.

- Ao começar o jogo, geramos uma sequência aleatória de leds que piscam.
	- Como separamos a hora do player e a hora da sequência?
	- Como salvamos a sequência que vai ser mostrada?
	- Como executamos a sequência?

- Como vamos pegar a sequência do usuário?


*/

int MAXIMO_SEQUENCIA = 100;

// ENTRADAS PARA OS BOTÕES
BlackGPIO botao1(GPIO_62, input);
BlackGPIO botao2(GPIO_22, input);
//BlackGPIO botao3(GPIO_27, input);
BlackGPIO botao3(GPIO_67, input);

// SAIDAS PARA OS LEDs
BlackGPIO led1(GPIO_45, output);
BlackGPIO led2(GPIO_47, output);
BlackGPIO led3(GPIO_23, output);


int main(int argc, char* argv[]){
	int sequencia[MAXIMO_SEQUENCIA];
	int tam_sequencia = 0;

	srand(time(NULL));
	int randNum = 0;

	std::cout<<"Aperte o botão 3 para começar..."<<std::endl;

	// esperar a pessoa apertar
	while (botao3.getValue() != "1");

	// esperar a pessoa soltar
	while (botao3.getValue() != "0");

	std::cout<<"Começando!"<<std::endl;

	while(true){
		if (botao1.getValue() == "1"){
			std::cout<<"Botao 1!"<<std::endl;
			led1.setValue(high);
		}
		else
			led1.setValue(low);

		if (botao2.getValue() == "1"){
			std::cout<<"Botao 2!"<<std::endl;
			led2.setValue(high);
		}
		else
			led2.setValue(low);

		if (botao3.getValue() == "1"){
			std::cout<<"Botao 3!"<<std::endl;
			led3.setValue(high);
		}
		else
			led3.setValue(low);		
	}
	return 0;
}
