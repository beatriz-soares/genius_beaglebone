#include <iostream>
#include "../BlackGPIO/BlackGPIO.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

using namespace BlackLib;

const int MAXIMO_SEQUENCIA = 100; // tamanho de sequencia maxima que o programa pode armazenar
const int TEMPO_ACESO = 700000;        // quanto tempo fica o led aceso (ms)
const int TEMPO_APAGADO = 400000;      // quanto tempo fica o led apagado (ms)

struct timeval tempo_inicial, tempo_final;

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


void testar_botoes(){
    if (botao1.getValue() == "1"){
      led1.setValue(high);
    }
    else
      led1.setValue(low);

    if (botao2.getValue() == "1"){
      led2.setValue(high);
    }
    else
      led2.setValue(low);

    if (botao3.getValue() == "1"){
      led3.setValue(high);
    }
    else
      led3.setValue(low);  
}


void tocar_espera(){
    led1.setValue(high);
    led2.setValue(low);
    led3.setValue(low);
    usleep(300000);

    led1.setValue(low);
    led2.setValue(high);
    led3.setValue(low);
    usleep(300000);

    led1.setValue(low);
    led2.setValue(low);
    led3.setValue(high);
    usleep(300000);
} 


void tocar_finalizou(){
    led1.setValue(high);
    led2.setValue(high);
    led3.setValue(high);
    usleep(300000);

    led1.setValue(low);
    led2.setValue(low);
    led3.setValue(low);
    usleep(300000);
}


void aumentar_sequencia(){
    int led = rand() % 3 + 1;  // numero entre 1 e 3
    sequencia[tam_sequencia] = led;
    tam_sequencia = tam_sequencia + 1;
}


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

    usleep(TEMPO_APAGADO);

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

    usleep(TEMPO_ACESO);
  }
  
  // apaga tudo
    led1.setValue(low);
    led2.setValue(low);
    led3.setValue(low);
}


int botao_apertado(){
    int  botao = 0;
    int tmili = 0;

    while (botao1.getValue() != "1" || botao2.getValue() != "1" || botao3.getValue() != "1"){
        testar_botoes();
        gettimeofday(&tempo_final, NULL);
        tmili = (int) (tempo_final.tv_sec - tempo_inicial.tv_sec);
        if (tmili > (3*tam_sequencia)+5){
            botao = 0;
            break;
        }

        if (tmili%5==0)
            printf("tempo decorrido: %d segundos\n", tmili);

        if (botao1.getValue() == "1"){
            botao = 1;
            break;
        }
        else if (botao2.getValue() == "1"){
            botao = 2;
            break;
        }
        else if (botao3.getValue() == "1"){
            botao = 3;
            break;
        }
        
    }
    
    while (botao1.getValue() == "1" || botao2.getValue() == "1" || botao3.getValue() == "1"){
        testar_botoes();
        gettimeofday(&tempo_final, NULL);
        tmili = (int) (tempo_final.tv_sec - tempo_inicial.tv_sec);

        if (tmili > (3*tam_sequencia)+5){
            botao = 0;
            break;
        }

        if (tmili%5==0)
            printf("tempo decorrido: %d segundos\n", tmili);
    }
    
    return botao;
}


bool ler(){
    int botao;
    for (int i = 0; i < tam_sequencia; i++){
        botao = botao_apertado();
        std::cout<<"apertou: "<<botao<<std::endl;
        if (botao == 0){
            std::cout<<"TEMPO ACABOU!"<<std::endl;
            return false;
        }
        if (botao != sequencia[i]){
std::cout<<"erou! Apertou "<<botao<<", e era "<<sequencia[i]<<std::endl;
            return false;
        }
    }
    
    return true;
}


int main(int argc, char* argv[]){
  inicio:
  // FLAGS DE CONTROLE
  bool turno_player = false;  // se é hora do player jogar ou de tocar a sequencia
  bool acabou = false;

  // CONTADORES
  int pontuacao = 0;
  
  srand(time(NULL));

  std::cout<<"Aperte o botão 3 para começar..."<<std::endl;

  // esperar a pessoa apertar
  while (botao3.getValue() != "1"){tocar_espera();}

  // esperar a pessoa soltar
  while (botao3.getValue() != "0"){
    led1.setValue(low);
    led2.setValue(low);
    led3.setValue(low);
  }

  std::cout<<"Começando!"<<std::endl;
  sleep(1);

  bool ganhou = false;

  while(!acabou){
    if (tam_sequencia >= MAXIMO_SEQUENCIA){
        ganhou = true;
        break;
    }
    aumentar_sequencia();
    std::cout<<"Novo tamanho de sequencia: "<<tam_sequencia<<std::endl;
    tocar();
    gettimeofday(&tempo_inicial, NULL);
    
    if (!ler()){
        acabou = true;
    }
  }

  if (!ganhou)
    std::cout<<"Fim de jogo! Pontuacao final: "<<tam_sequencia - 1<<std::endl;
  else
    std::cout<<"GANHOUUUUUUU! Pontuacao final: "<<tam_sequencia<<std::endl;

  std::cout<<"Aperte o botão 1 para reiniciar e 2 pra sair..."<<std::endl;

  // esperar a pessoa apertar
  while (botao1.getValue() != "1" || botao2.getValue() != "1"){
    tocar_finalizou();
    if (botao1.getValue() == "1"){
        tam_sequencia = 0;
        goto inicio;
        break;
    }
    else if (botao2.getValue() == "1"){
        break;
    }
  }

  return 0;
}
