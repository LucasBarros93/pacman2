#include "map.hpp"

// Construtor da classe Map
// Recebe somente o tamanho do "tile" do mapa
Map::Map(const sf::Vector2<float>& tileSize) 
    : count (1), level(1), tileSize(tileSize), // Inicializa contagem, nível e tamanho dos tiles
      pac("./assets/images/spritesheet.png", 16, 16, 0.2f), // Inicializa o PacMan
      blinky("./assets/images/spritesheet.png", 16, 16, 0.2f, 1), // Inicializa o fantasma Blinky
      pinky("./assets/images/spritesheet.png", 16, 16, 0.2f, 1), // Inicializa o fantasma Pinky
      inky("./assets/images/spritesheet.png", 16, 16, 0.2f, 1), // Inicializa o fantasma Inky
      clyde("./assets/images/spritesheet.png", 16, 16, 0.2f, 1), // Inicializa o fantasma Clyde
      bonusFruit(16,16){ // Inicializa fruta bônus

    // Configuração padrão para as paredes
    this->wall.setSize({this->tileSize.x*2, this->tileSize.y*2}); // Ajusta tamanho das paredes
    this->wall.setFillColor(sf::Color::Blue); // Define cor azul para as paredes

    // Inicializa o deslocamento do mapa (offset, para centralizar) como zero
    this->mapOffset = sf::Vector2f(0.f, 0.f);
}

// Método para carregar o mapa a partir de um arquivo de texto
bool Map::loadFromFile(const std::string& filePath){
    std::ifstream file(filePath); // Abre o arquivo em modo de leitura

    // Verifica se o arquivo foi aberto corretamente
    if(!file.is_open()){
        std::cerr << "Erro ao abrir o arquivo do mapa: " << filePath << std::endl;
        return false;
    }

    mapData.clear(); // Limpa os dados do mapa anteriores
    std::string line;

    // Lê cada linha do arquivo e armazena no vetor de dados do mapa
    while(std::getline(file, line)){
        std::vector<char> data; // Vetor temporário para armazenar os caracteres da linha
        std::copy(line.begin(), line.end(), std::back_inserter(data));
        mapData.push_back(data); // Adiciona a linha processada ao vetor principal
    }

    // Processa o mapa carregado para identificar frutas e energizadores
    for(size_t y = 0; y < mapData.size(); y++){
        for(size_t x = 0; x < mapData[y].size(); x++){

            bool isFullTile = (x%2 == 0 && y%2 ==0)? true : false; // Tiles inteiros são processados
            
            if(isFullTile){
                std::pair<int, int> position = {static_cast<int>(x), static_cast<int>(y)};

                if(mapData[y][x] == '.' || mapData[y][x+1] == '.'){  // Ponto simples
                    fruits.emplace(position, std::make_unique<Dot>(sf::Vector2<int>(x, y)));
                }
                else if(mapData[y][x] == 'o' || mapData[y][x+1] == 'o'){  // Energizer
                    fruits.emplace(position, std::make_unique<Energizer>(sf::Vector2<int>(x, y)));
                }
            }
        }
    }

    // Calcular o tamanho total do mapa em pixels
    float mapWidthPixels = mapData[0].size() * this->tileSize.x;
    float mapHeightPixels = mapData.size() * this->tileSize.y;

    // Calcula o (offset) para centralizar o mapa na janela 800x800
    float offsetX = (800.f - mapWidthPixels) / 2.f;
    float offsetY = (800.f - mapHeightPixels) / 2.f;
    
    // Caso o mapa seja maior que a janela, não deslocar para negativo
    if(offsetX < 0) offsetX = 0;
    if(offsetY < 0) offsetY = 0;

    this->mapOffset = sf::Vector2f(offsetX, offsetY); // Armazena o deslocamento calculado

    return true; // Sucesso
}

// Método para resetar o estado do mapa
void Map::reset(){
    // Recarrega o mapa
    this->loadFromFile("assets/maps/map.txt");

    // Reseta Pac-Man
    this->pac.reset();

    // Reseta fantasmas (posições e modos iniciais)
    this->blinky.reset({27, 22}); // Posição inicial de Blinky
    this->pinky.reset({29, 22}); // Posição inicial de Pinky
    this->inky.reset({25, 22}); // Posição inicial de Inky
    this->clyde.reset({31, 22}); // Posição inicial de Clyde

    // Reseta o contador
    this->count=1;
}

// Método responsável por desenhar o mapa e os elementos visuais na janela
void Map::draw(sf::RenderWindow& window){
    // Variavel de controle
    bool isFullTile;

    // Itera sobre todas as linhas e colunas do mapa
    for(size_t y = 0; y < mapData.size(); y++){
        for(size_t x = 0; x < mapData[y].size(); x++){
            sf::Vector2<float> position(x * this->tileSize.x, y * this->tileSize.y);
            isFullTile = (x%2 == 0 && y%2 ==0)? true : false;

            // Desenha as paredes do mapa (caracter '#') [somente se for FullTile]
            if(isFullTile){
                if(mapData[y][x] == '#' &&
                    mapData[y+1][x] == '#' && 
                    mapData[y][x+1] == '#' && 
                    mapData[y+1][x+1] == '#'){

                        this->wall.setPosition(position + this->mapOffset);
                        window.draw(this->wall);
                }
            }
            // Desenha o Pac-Man ('P') no mapa
            if(mapData[y][x] == 'P' &&
                mapData[y+1][x] == 'P' && 
                mapData[y][x+1] == 'P' && 
                mapData[y+1][x+1] == 'P'){

                    this->pac.setPosition({static_cast<int>(x), static_cast<int>(y)}, this->tileSize);
                    // Cria uma cópia do sprite, ajusta posição, depois desenha
                    sf::Sprite pacSprite = this->pac.getSprite();
                    pacSprite.setPosition(pacSprite.getPosition() + this->mapOffset);
                    window.draw(pacSprite);
            }
            // Desenha o fantasma Blinky ('B')
            if(mapData[y][x] == 'B' &&
                mapData[y+1][x] == 'B' && 
                mapData[y][x+1] == 'B' && 
                mapData[y+1][x+1] == 'B'){

                    this->blinky.setPosition({static_cast<int>(x), static_cast<int>(y)},  this->tileSize);
                    // Cria uma cópia do sprite, ajusta posição, depois desenha
                    sf::Sprite blinkySprite = this->blinky.getSprite();
                    blinkySprite.setPosition(blinkySprite.getPosition() + this->mapOffset);
                    window.draw(blinkySprite);
            }
            // Desenha o fantasma Pinky ('R')
            if(mapData[y][x] == 'R' &&
                mapData[y+1][x] == 'R' && 
                mapData[y][x+1] == 'R' && 
                mapData[y+1][x+1] == 'R'){

                    this->pinky.setPosition({static_cast<int>(x), static_cast<int>(y)},  this->tileSize);
                    // Cria uma cópia do sprite, ajusta posição, depois desenha
                    sf::Sprite pinkySprite = this->pinky.getSprite();
                    pinkySprite.setPosition(pinkySprite.getPosition() + this->mapOffset);
                    window.draw(pinkySprite);
                }
            // Desenha o fantasma Inky ('I')
            if(mapData[y][x] == 'I' &&
                mapData[y+1][x] == 'I' && 
                mapData[y][x+1] == 'I' && 
                mapData[y+1][x+1] == 'I'){

                    this->inky.setPosition({static_cast<int>(x), static_cast<int>(y)},  this->tileSize);
                    // Cria uma cópia do sprite, ajusta posição, depois desenha
                    sf::Sprite inkySprite = this->inky.getSprite();
                    inkySprite.setPosition(inkySprite.getPosition() + this->mapOffset);
                    window.draw(inkySprite);
                }
            // Desenha o fantasma Clyde ('C')
            if(mapData[y][x] == 'C' &&
                mapData[y+1][x] == 'C' && 
                mapData[y][x+1] == 'C' && 
                mapData[y+1][x+1] == 'C'){

                    this->clyde.setPosition({static_cast<int>(x), static_cast<int>(y)},  this->tileSize);
                    // Cria uma cópia do sprite, ajusta posição, depois desenha
                    sf::Sprite clydeSprite = this->clyde.getSprite();
                    clydeSprite.setPosition(clydeSprite.getPosition() + this->mapOffset);
                    window.draw(clydeSprite);
                }
        }
    }

    // Desenha frutas (dots e energizers)
    for(const auto& fruitPair : fruits){
        fruitPair.second->draw(window, this->mapOffset);
    }

    // Desenha fruta bônus (bonusFruit)
    bonusFruit.draw(window, this->mapOffset);
}

// Método para retornar os dados atuais do mapa
const MapData& Map::getMapData() const{
    return this->mapData;
}

// Sobrecarga do operador '++' para incrementar o contador
void Map::operator++(int){
    this->count++;
}

// Atualiza a posição e animação do PacMan com base na direção
void Map::updatePacman(const sf::Vector2<int> direction){
    this->pac.updateAnimation(); // Atualiza a animação do Pac-Man
    this->mapData = this->pac.update(this->mapData, direction); // Atualiza o mapa com a nova posição do PacMan
}

// Método para atualizar o comportamento e dificuldade dos fantasmas
void Map::updateGhosts(){
    // Ajusta a dificuldade com base no nível atual do jogo (limite máximo: 99)
    int dificuldadeAtual = std::min(50 + (this->level - 1) * 10, 99);
    
    // Define a dificuldade dos fantasmas
    this->blinky.setDificult(dificuldadeAtual); // Blinky é o mais agressivo
    this->pinky.setDificult(std::max(dificuldadeAtual - 5, 50)); // Pinky um pouco mais fácil
    this->inky.setDificult(std::max(dificuldadeAtual - 10, 50)); // Inky menos agressivo
    this->clyde.setDificult(std::max(dificuldadeAtual - 15, 50)); // Clyde mais "aleatório"
    
    // Atualiza a animação e o movimento de cada fantasma
    this->blinky.updateAnimation();
    this->mapData = this->blinky.updateBehavior(this->mapData, 'B', this->pac.getPosition());
    this->pinky.updateAnimation();
    this->mapData = this->pinky.updateBehavior(this->mapData, 'R', this->pac.getPosition());
    this->inky.updateAnimation();
    this->mapData = this->inky.updateBehavior(this->mapData, 'I', this->pac.getPosition());
    this->clyde.updateAnimation();
    this->mapData = this->clyde.updateBehavior(this->mapData, 'C', this->pac.getPosition());

    // Configura o tempo para os fantasmas saírem de suas casas em intervalos específicos
    if(this->count%50 == 0){
        if(this->blinky.getMode() == Ghost::OUTGAME)
            this->blinky.setMode(Ghost::SPAWN);
    }
    if(this->count%80 == 0){
        if(this->pinky.getMode() == Ghost::OUTGAME)
            this->pinky.setMode(Ghost::SPAWN);
    }
    if(this->count%110 == 0){
        if(this->inky.getMode() == Ghost::OUTGAME)
            this->inky.setMode(Ghost::SPAWN);
    }
    if(this->count%130 == 0){
        if(this->clyde.getMode() == Ghost::OUTGAME)
            this->clyde.setMode(Ghost::SPAWN);
    }

    // Atualiza a fruta bônus
    this->bonusFruit.update(this->mapData);
}

// Método para definir o nível atual e ajustar a dificuldade do jogo
void Map::setLevel(int currentLevel){
    this->level = currentLevel;
}

// Atualiza a fruta bônus no mapa
void Map::updateBonusFruit(){
    bonusFruit.update(this->mapData);
}

// Método para detectar colisões entre PacMan, fantasmas e frutas
int Map::colision(){
    int pointsEarned = 0; // Pontos ganhos na colisão

    // Vetores auxiliares para verificar posições adjacentes ao Pac-Man
    sf::Vector2<int> aux1 = {this->pac.getPosition().x+1, this->pac.getPosition().y};
    sf::Vector2<int> aux2 = {this->pac.getPosition().x, this->pac.getPosition().y+1};
    sf::Vector2<int> aux3 = {this->pac.getPosition().x, this->pac.getPosition().y-1};
    sf::Vector2<int> aux4 = {this->pac.getPosition().x-1, this->pac.getPosition().y};

    // Colisão com Blinky
    if(this->pac.getPosition() == this->blinky.getPosition() ||
        aux1 == this->blinky.getPosition() ||
        aux2 == this->blinky.getPosition() ||
        aux3 == this->blinky.getPosition() ||
        aux4 == this->blinky.getPosition()){
        if(this->blinky.getMode() == Ghost::Mode::POWERLESS){ // Blinky está vulnerável
            this->mapData = this->blinky.kill(this->mapData, 'B');
            pointsEarned += 400;   
        }

        else if(this->blinky.getMode() == Ghost::Mode::NORMAL)
            return -1; // PacMan morreu
    }

    // Colisão com Pinky
    if(this->pac.getPosition() == this->pinky.getPosition() ||
        aux1 == this->pinky.getPosition() ||
        aux2 == this->pinky.getPosition() ||
        aux3 == this->pinky.getPosition() ||
        aux4 == this->pinky.getPosition()){
        if(this->pinky.getMode() == Ghost::Mode::POWERLESS){
            this->mapData = this->pinky.kill(this->mapData, 'R');
            pointsEarned += 400;
        }
        
        else if(this->pinky.getMode() == Ghost::Mode::NORMAL)
            return -1;
    }

    // Colisão com Inky
    if(this->pac.getPosition() == this->inky.getPosition() ||
        aux1 == this->inky.getPosition() ||
        aux2 == this->inky.getPosition() ||
        aux3 == this->inky.getPosition() ||
        aux4 == this->inky.getPosition()){
        if(this->inky.getMode() == Ghost::Mode::POWERLESS){
            this->mapData = this->inky.kill(this->mapData, 'I');
            pointsEarned += 400;
        }

        else if(this->inky.getMode() == Ghost::Mode::NORMAL)
            return -1;
    }

    // Colisão com Clyde
    if(this->pac.getPosition() == this->clyde.getPosition() ||
        aux1 == this->clyde.getPosition() ||
        aux2 == this->clyde.getPosition() ||
        aux3 == this->clyde.getPosition() ||
        aux4 == this->clyde.getPosition()){
        if(this->clyde.getMode() == Ghost::Mode::POWERLESS){
            this->mapData = this->clyde.kill(this->mapData, 'C');
            pointsEarned += 400;
        }

        else if(this->clyde.getMode() == Ghost::Mode::NORMAL)
            return -1;
    }

    // Verifica se PacMan coletou uma fruta (dot ou energizer)
    auto it = this->fruits.find({pac.getPosition().x, pac.getPosition().y});
    if(it != this->fruits.end()){
        int pointsFruit = it->second->getPoints(); // Pontos da fruta
        this->fruits.erase(it); // Remove a fruta

        // Se for um energizer, deixa os fantasmas vulneráveis
        if(pointsFruit== 50){
            if(this->blinky.getMode() == Ghost::NORMAL){
                this->blinky.setMode(Ghost::POWERLESS);
                this->blinky.setCount(0);
            }

            if(this->pinky.getMode() == Ghost::NORMAL){
                this->pinky.setMode(Ghost::POWERLESS);
                this->pinky.setCount(0);
            }

            if(this->inky.getMode() == Ghost::NORMAL){
                this->inky.setMode(Ghost::POWERLESS);
                this->inky.setCount(0);
            }

            if(this->clyde.getMode() == Ghost::NORMAL){
                this->clyde.setMode(Ghost::POWERLESS);
                this->clyde.setCount(0);
            }
        }
        pointsEarned += pointsFruit; // Soma pontuação total acumulada
    }

    // Colisão com a fruta bônus
    if(bonusFruit.isActive() && pac.getPosition() == bonusFruit.getPosition()){
        pointsEarned += bonusFruit.getPoints();
        bonusFruit.reset(); // Reseta a fruta bônus
    }

    return pointsEarned; // Retorna a pontuação total acumulada
}

// Método para retornar a quantidade de frutas restantes no mapa
int Map::getFruitsRemaining() const{
    return fruits.size();
}

