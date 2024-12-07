# pacman2
Aqui criaremos nossa segunda versão do Pacman, dessas vez em C++. O jogo será criado para o trabalho de POO e segue o planejamento

## Classes do jogo:

* Tabuleiro:
    * classe mais simples (?)
    * pai do placar se pa
    * guarda o cenario e checa colisões com as paredes

* Pacman:
    * é o pacman
    * come, anda, fica forte e tal

* Fantasmas:
    * classe pai dos fantasmas
    * anda, fica fraco, vai atrá do pacman

    * Blink, Pink, Inky, Clyde:
        * classes filhos de fantasma
        * toma as decisões de acordo com a personalidade de cada fantasma

* Placar:
    * conta os pontos
    * sobrecarga de operador ++. --. etc.
    * escreve os resultados em arquivos no final.

* Frutas:
    * classe completamente porlimorfisada em:

        * Fruta padrão "."
        * Fruta mais forte "o"
        * Frutas q dão mais pontos "fruta"

## Sobre o trabalho:

As classes descritas a cima cobrem a maior parte dos resquisitos do trabalho. A lógica que eu pensei pra funcionamento é basicamente a do nosso tabalho de ICC, na qual o tabuleiro é o mapa, ao tentar mover alguma das coisas q se movem ele diz quais são os espaços livres e tal.

O joog funcionaria com uma "taxa de quadros" na qual tudo se move a cada tik rate desse, ou seja, as coisas n ficam paradas, deixando mais próximo do jogo original, assim tudo que se move tem direções, podendo ou não mudar de acordo com o input e a "permição do tabubeliro".

Contudo tudo pode mudar dependendo do motor gráfico do jogo.

## Coisas a se pensar:

### Motor gráfico:
Qual biblioteca gráfica a gente vai usar? ela vai mudar mto a lógica doo jogo? Sugestões do biblioteca para olha:

* SDL2
* SFML (*)

### Tópico de inovação:
Na descrição do trabalho fala q ele tem q ser inovador, não acho q o professor vai pegar no pé com isso, mas se der tempo seria legal fazer isso. Umas ideias q eu tive são

* jogar com umou mais fantasmas em um estilo meio "fogo e agua" de gameplay
* ter outros méetodos de jogo em que o objetivo n é pontuar mas sim comer todos os fantasmas ou sla