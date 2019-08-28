#include <iostream>
#include <iostream>
#include "../BlackGPIO/BlackGPIO.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

using namespace BlackLib;

const int MAXIMO_SEQUENCIA = 100; // tamanho de sequencia maxima que o programa pode armazenar
const int TEMPO_ACESO = 1;        // quanto tempo fica o led aceso
const int TEMPO_APAGADO = 1;      // quanto tempo fica o led apagado

// ENTRADAS PARA OS BOTÕES
BlackGPIO botao1(GPIO_69, input); // 9
BlackGPIO botao2(GPIO_22, input); // 19
BlackGPIO botao3(GPIO_67, input); // 8

// SAIDAS PARA OS LEDs
BlackGPIO led1(GPIO_45, output); // 11
BlackGPIO led2(GPIO_47, output); // 15
BlackGPIO led3(GPIO_23, output); // 13

// SEQUENCIA QUE O PLAYER DEVE REPRODUZIR
int sequencia[MAXIMO_SEQUENCIA];
int tam_sequencia = 0;

void tocar(){
  /*
    Reproduz a sequência até o momento.
    Ou seja, vai acendendo do primeiro ao ultimo led,
    ficando um tempo aceso em cada.
  */
  for (int i = 0; i < tam_sequencia; i++){
    // apaga tudo, depois só um acende
    led1.setValue(low);
    led2.setValue(low);
    led3.setValue(low);

    sleep(TEMPO_APAGADO);

    switch(sequencia[i]){
      case 1:
        led1.setValue(high);
        break;
      case 2:
        led2.setValue(high);
        break;
      case 3:
        led3.setValue(high);
        break;
      default:
        break;
    }

    sleep(TEMPO_ACESO);
  }
}
int main(int argc, char* argv[]){
  // FLAGS DE CONTROLE
  bool turno_player = false;  // se é hora do player jogar ou de tocar a sequencia
  bool acabou = false;

  // CONTADORES
  int pontuacao = 0;

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
