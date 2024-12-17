#include "entitys/ghost_base.hpp"

// Construtor da classe Ghost
Ghost::Ghost(const std::string& texturePath, int fw, int fh, float fd, int df)
    : dir(0, 0), pos(27,26), dificult(df), count(0), frameWidth(fw), frameHeight(fh), frameCount(2), frameDuration(fd),
      currentFrameIndex(0),  currentMode(OUTGAME){
    
    // Pega a textura do fantasma e seta ela
    this->texture.loadFromFile(texturePath);    
    this->sprite.setTexture(this->texture);
    this->currentFrame = sf::IntRect(0, 0, this->frameWidth, this->frameHeight); // Ajusta a textura inicial pro primeiro frame
    this->sprite.setTextureRect(this->currentFrame);
    this->sprite.setScale(1.f, 1.f); // Define a escala pro tamanho do fantasma
}


// Metodo pra reposicionar o fantasma de volta no  ponto de spawn e atualizar o mapa
MapData Ghost::spawn(MapData mapData, char self){
    if(this->count++%5 !=0){ // Controla a frequencia de movimento ao reaparecer
        return mapData;
    }

    this->dir ={0,-1};  // Define a direcao inicial do spawn (pra cima)
    char to = mapData[this->pos.y + this->dir.y][this->pos.x]; // Pega o conteudo acima da posicao atual do fantasma
    
    if(to == ' '){ // Caso a posicao acima do fantasma esteja vazia,ele sobe
        // As linhas aqui debaixo servem pra limpar a posicao atual no mapa
        mapData[this->pos.y][this->pos.x] = ' ';
        mapData[this->pos.y+1][this->pos.x] = ' ';
        mapData[this->pos.y][this->pos.x+1] = ' ';
        mapData[this->pos.y+1][this->pos.x+1] = ' ';

        this->pos += this->dir; // Move o fantasma pra cima

        // Esse conjunto aqui embaixo serve pra marcar a nova posicao no mapa com o caractere q representa o fantasma
        mapData[this->pos.y][this->pos.x]     = self;
        mapData[this->pos.y+1][this->pos.x]   = self;
        mapData[this->pos.y][this->pos.x+1]   = self;
        mapData[this->pos.y+1][this->pos.x+1] = self;

        return mapData;
    }
    else if(to == '#'){ // entra aqui se tiver uma parede em cima do fantasma (#)---> redefine o fantasma no ponto de spawn
        // Limpando a posicao atual
        mapData[this->pos.y][this->pos.x] = ' ';
        mapData[this->pos.y+1][this->pos.x] = ' ';
        mapData[this->pos.y][this->pos.x+1] = ' ';
        mapData[this->pos.y+1][this->pos.x+1] = ' ';

        this->pos ={27,22}; // Definindo a posicao de spawn

        // Atualizando o mpa com a nova posicao
        mapData[this->pos.y][this->pos.x]     = self;
        mapData[this->pos.y+1][this->pos.x]   = self;
        mapData[this->pos.y][this->pos.x+1]   = self;
        mapData[this->pos.y+1][this->pos.x+1] = self;

        this->setMode(NORMAL); // Define o modo do fantasma como NORMAL, pq ele spawna como normal

        return mapData;
    }

    return mapData;
}

// Metodo que 'seta' o movimento do fantasma como aleatorio quando ele esta 'fraco' (acontece quando o pacman come um energizer ~bolinha branca maior)
MapData Ghost::powerless(MapData mapData, char self){
        std::vector<sf::Vector2<int>> possibleDirections;

    // Sequencia de ifs para verificar as direcoes validas (baixo, cima, direita, esquerda)
    if(mapData[this->pos.y + 2][this->pos.x] != '#' && mapData[this->pos.y + 2][this->pos.x + 1] != '#')
        possibleDirections.push_back({0, 1});  // Baixo

    if(mapData[this->pos.y - 1][this->pos.x] != '#' && mapData[this->pos.y - 1][this->pos.x + 1] != '#')
        possibleDirections.push_back({0, -1}); // Cima

    if(mapData[this->pos.y][this->pos.x + 2] != '#' && mapData[this->pos.y + 1][this->pos.x + 2] != '#')
        possibleDirections.push_back({1, 0});  // Direita

    if(mapData[this->pos.y][this->pos.x - 1] != '#' && mapData[this->pos.y + 1][this->pos.x - 1] != '#')
        possibleDirections.push_back({-1, 0}); // Esquerda

    // Caso nao existam direcoes validas, nao faz nada
    if(possibleDirections.empty()){
        return mapData;
    }

    // O bloco abaixo lida com um corredor reto: existem 2 direcoes validas e uma delas eh oposta a direcao atual de movimento. O codigo tenta manter a direcao atual do fantasma
    if(possibleDirections.size()== 2){
        for(const auto& dir : possibleDirections){
            if(dir == -this->dir){ // Direcao oposta (evita ir nela)
                // Mantem a direcao atual e nao muda
                possibleDirections.erase(std::remove(possibleDirections.begin(), possibleDirections.end(), dir), possibleDirections.end());
                break;
            }
        }
        // Se ainda restar apenas 1 direcao valida, continua na mesma direcao
        if(possibleDirections.size()== 1){
            this->dir = possibleDirections[0];
        }
    }

    // Se estiver em uma encruzilhada, escolha nova direcao com probabilidade, eh aleatorio
    if(possibleDirections.size()> 1){
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // Escolhe uma direcao aleatoria
        std::uniform_int_distribution<> randomDir(0, possibleDirections.size()- 1);
        this->dir = possibleDirections[randomDir(gen)];
        
    }

    // Atualiza a posicao com a direcao atual
    sf::Vector2<int> nextPos = this->pos + this->dir;
    if(mapData[nextPos.y][nextPos.x] != '#'){
        // Limpa a posicao atual
        mapData[this->pos.y][this->pos.x] = ' ';
        mapData[this->pos.y + 1][this->pos.x] = ' ';
        mapData[this->pos.y][this->pos.x + 1] = ' ';
        mapData[this->pos.y + 1][this->pos.x + 1] = ' ';

        this->pos += this->dir;  // Move o fantasma pra nova posicao
    }

    // Trata do teleporte do fantasma nas bordas horizontais (sao 2 os lugares q tem teleporte)
    if(this->pos.x == -1){
        this->pos.x = mapData[0].size()- 2; // TEleporta pro lado direito
    } 
    else if(this->pos.x == static_cast<int>(mapData[0].size())- 1){
        this->pos.x = 0; // Teleporta pro lado esquerdo
    }

    // Atualiza a nova posicao no mapa
    mapData[this->pos.y][this->pos.x] = self;
    mapData[this->pos.y + 1][this->pos.x] = self;
    mapData[this->pos.y][this->pos.x + 1] = self;
    mapData[this->pos.y + 1][this->pos.x + 1] = self;

    return mapData;
}


// Metodo que "mata" o fantasma e joga ele no ponto de spawn (casa dos fantasmas). Acontece quando o pacman elimina o fantasma depois de ter comido um energizer
MapData Ghost::kill(MapData mapData, char self){
    int aux = 24; // Ponto inicial pra encontrar uma poiscao vazia
                
    while(mapData[28][aux] != ' ')// Procura uma posicao vazia na linha de spawn
        aux++;
    
    //Limpa a posicao atual
    mapData[this->pos.y][this->pos.x] = ' ';
    mapData[this->pos.y + 1][this->pos.x] = ' ';
    mapData[this->pos.y][this->pos.x + 1] = ' ';
    mapData[this->pos.y + 1][this->pos.x + 1] = ' ';

    // Move o fantasma pro ponto de spawn
    this->pos ={aux, 28};
    mapData[this->pos.y][this->pos.x] = self;
    mapData[this->pos.y + 1][this->pos.x] = self;
    mapData[this->pos.y][this->pos.x + 1] = self;
    mapData[this->pos.y + 1][this->pos.x + 1] = self;

    // Seta o modo do fantasma pra "DEAD" e reseta o contador 
    this->setMode(DEAD);
    this->dir ={0,0};
    this->count = 0;

    return mapData;
}

// Metodo que atualiza a animacao do fantasma de acordo com seu estado atual
// esse metodo fica alternando os frames do sprite pra dar a sensacao de movimento
void Ghost::updateAnimation(){
    // Verifica se o tempo de troca de frame foi atingido
    if(this->animationClock.getElapsedTime().asSeconds()> this->frameDuration){
        this->animationClock.restart(); // Reinicia o relogio da animacao
        this->currentFrameIndex = (this->currentFrameIndex + 1)% this->frameCount; // Avanca pro proximo frame
       
        //Deslocamentos vertical e horizontal no sprite png
        int offsetY = 0;
        int offsetX = 0;

        // switch pra escolher a animacao correta de acordo com o estado atual do fantasma
        switch(this->currentMode){
            // Os primeiro 3 cases tem a msm animacao, q eh a animacao de fantasma normal
            case OUTGAME:
                this->updateAnimationNormal();
                return;

            case SPAWN:
                this->updateAnimationNormal();
                return;

            case NORMAL:
                this->updateAnimationNormal();
                return;

            case POWERLESS: // Esse case ja eh diferente, em que o fantasma tem a animacao azul, significando q esta vulneravel
                // Frames "powerless" (linha 4, coluna 0 e coluna 1)do png global que esta sendo usado com as entidades
                offsetY = 64;  // Linha 4 (64px * 2)
                offsetX = 128 + (this->currentFrameIndex * this->frameWidth);
                break;

            case DEAD:
                // Configura o frame para o estado "morto" (linha 5 do spritesheet)
                offsetY = 80;  // Linha 5
                if(this->dir.x == -1){
                    offsetX = 160; // Esquerda
                } 
                else if(this->dir.y == -1){
                    offsetX = 176; // Cima
                }
                else if(this->dir.y == 1){
                    offsetX = 192; // Baixo
                }  
                else{
                    offsetX = 144;  // Direita
                }                    
                break;
        }
        // Atualiza a textura (frame)do sprite com os offsets calculados
        this->currentFrame.left = offsetX;
        this->currentFrame.top = offsetY;

    }
    this->sprite.setTextureRect(this->currentFrame); //Aplica o novo frame ao sprite
}

// Metodo virtual que sera sobrescrito pelos fantasmas especificos. Tem apenas um cout que n serve diretamente aqui
void Ghost::updateAnimationNormal(){
    std::cout << "POO eng comp 2024" << std::endl;
}

// Metodo que atualiza o comportamento do fantasma com base no estado atual
MapData Ghost::updateBehavior(MapData mapData, char self, sf::Vector2<int>pacmanPos){
    // Comportamento base dos fantasmas
    // Cada fantasma específico pode sobrescrever este comportamento
    switch (this->currentMode){
        case OUTGAME:
            // fora do jogo n se mexe
            return mapData;
        case SPAWN:
            // Spanwnando
            mapData = this->spawn(mapData, self);
            return mapData;

        case NORMAL:
            // Movimento normal -> padrao de perseguir o pacman
            mapData = this->updateBehaviorNormal(mapData, pacmanPos);
            return mapData;
            
        case POWERLESS:
            // Movimento aleatorio
            mapData = this->powerless(mapData, self);
            // Apos um 'tempo' de 100 ciclos o fantasma volta pro estado NORMAL
            if(this->count++ > 100){
                this->setMode(NORMAL);
                this->count = 0;
            }

            return mapData;
        
        case DEAD:
            // Apos um 'tempo' de 80 ciclos o fantasma volta pro modo SPAWN e sai de casa
            if(this->count++ > 80){
                this->setMode(SPAWN);
                this->count = 0;
            }
            break;
            
    }

    return mapData;
}


// Metodo que reseta o fantasma pra posicao inicial e define o estado OUTGAME
void Ghost::reset(const sf::Vector2<int>& position){
    this->pos = position; // Posicao inicial do fantasma
    this->dir ={0, 0}; // Direcao inicial eh neutra
    this->currentMode = OUTGAME; // Define o modo inicial como fora do jogo
    this->currentFrameIndex = 0; // Reseta o indice do frame
    this->sprite.setPosition(this->pos.x * 10, this->pos.y * 10); // Define a posicao visual inicial no mapa
}

// Metodo que lida com comportamento padrao do fantasma (pra ser sobrescrito por classes derivadas)
MapData Ghost::updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos){
    pacmanPos ={0,0};
    return mapData;
}

// Metodo que define a direcao de movimento do fantasma
void Ghost::setDirection(const sf::Vector2<int>& direction){
    this->dir = direction;
}

// Metodo que define a posicao do fantasma no mapa
void Ghost::setPosition(const sf::Vector2<int>& position, const sf::Vector2<float>& tileSize){
    this->pos = position; // define a nova posicao
    this->sprite.setPosition(this->pos.x * 1.005*tileSize.x, this->pos.y * 1.005*tileSize.y); // atualiza a posicao visual no sprite com base no tamanho da celula (tilesize)
}

// Metodo que define o estado atual do fantasma
void Ghost::setMode(Mode mode){
    this->currentMode = mode;
}

// Metodo que define o nivel de dificuldade do fantasma
void Ghost::setDificult(int df){
    this->dificult = df;
}

// Metodo que define o valor do contador de ciclos (usado em estados como POWERLESS e DEAD)
void Ghost::setCount(int i){
    this->count = i;
}

// Metodo que retorna o sprite atual do fantasma (pra desenhar ele na tela)
const sf::Sprite Ghost::getSprite()const{
    return this->sprite;
}

// Metodo que retorna a posicao atual do fantasma no grid
const sf::Vector2<int>& Ghost::getPosition()const{
    return this->pos;
}

// Metodo que retorna o estado atual do fantasma
Ghost::Mode Ghost::getMode()const{
    return this->currentMode;
}