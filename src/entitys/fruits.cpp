#include "entitys/fruits.hpp"

// -----------------------------------
// Função auxiliar para números aleatórios
int getRandomNumber(int min, int max){
    static std::random_device rd;
    static std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}

// ===================================
// IMplementacao da classe Dot
Dot::Dot(sf::Vector2<int> position) : pos(position), points(10){} // Construtor que inicializa posicao e pontos da fruta

// Aqui retorna os pontos do ponto (dot)
int Dot::getPoints() const{
    return this->points;
}

// Metodo pra desenhar o Dot na janela, uma bolinha. Offset foi aplicado pra centralizar o Dot na tela
void Dot::draw(sf::RenderWindow& window, const sf::Vector2f& offset) const{
    sf::CircleShape dot(2.0f);
    dot.setFillColor(sf::Color(184, 197, 201));
    dot.setPosition(((this->pos.x * 10) + 10) + offset.x, ((this->pos.y * 10 )+10) + offset.y); // Aqui a aplicacao do offset, tem isso em todos os elementos graficos pra eles se alinharem no meio da tela
    window.draw(dot);
}

// ===================================
// Implementacao da classe Energizer
Energizer::Energizer(sf::Vector2<int> position) : pos(position), points(50){} // Construtor que inicializa posicao e pontos da fruta

// Metodo de retornar os pontos do energizer
int Energizer::getPoints() const {
    return this->points;
}

// Metodo pra desenhar o energizer como um ponto maior, bolinha branca maior que o Dot
void Energizer::draw(sf::RenderWindow& window, const sf::Vector2f& offset) const {
    sf::CircleShape energizer(4.0f);
    energizer.setFillColor(sf::Color(184, 197, 201));
    energizer.setPosition(((this->pos.x * 10) + 10) + offset.x, ((this->pos.y * 10 )+10) + offset.y);
    window.draw(energizer);
}

// ===================================
// Implementacao da classe Bonus
Bonus::Bonus(int fw, int fh) 
    : frameWidth(fw), frameHeight(fh), active(false){
    this->frame = sf::IntRect(0, 0, this->frameWidth, this->frameHeight);
    this->points = 0; // Inicializa pontos
} // Construtor que inicializa o tamanho so sprite e tambem define a fruta bonus como inativa

// Metodo q procura uma posicao livre no mapa pra poder colocar a fruta bonus aleatoriamente
void Bonus::spawn(const std::vector<std::vector<char>>& mapData){
    // Verifica se a fruta bonus ja esta ativa
    if(this->active){
      return;  
    } 

    // Encontra posicoes livres no mapa
    std::vector<sf::Vector2<int>> emptyPositions;
    for(size_t y = 0; y < mapData.size(); y+=2){
        for(size_t x = 0; x < mapData[y].size(); x+=2){
            if(mapData[y][x] == ' '){
                emptyPositions.push_back({static_cast<int>(x), static_cast<int>(y)});
            }
        }
    }

    if(!emptyPositions.empty()){
        // gera uma posicao aleatoria dentre aquelas q estao disponiveis
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, emptyPositions.size() - 1);
        this->pos = emptyPositions[dist(gen)];

        // Deixa a textura e os pontos da frta aleatorios
        randomize();

        this->active = true; // Ativa a fruta bonus
        this->visibilityTimer.restart(); // Reinicia o tempo de visibilidade (a fruta bonus so fica ativa durante 10seg)
    }
}

// Metodo pra atualizar o status da fruta bonus ---> ativa por 10 segundos e desativa por 15 segundos
void Bonus::update(const std::vector<std::vector<char>>& mapData){
    if(active){
        // Se a fruta estiver visivel por mais de 10seg ---> desativa fruta
        if(visibilityTimer.getElapsedTime().asSeconds() > 10.0f){
            active = false;
            respawnTimer.restart(); // Inicia o temporizador de respawn
        }
    } 
    else{
        // Se a fruta estiver inativa por 15seg ---> reaparece a fruta
        if(respawnTimer.getElapsedTime().asSeconds() > 15.0f){
            spawn(mapData); // Passa o mapa como argumento
        }
    }
}


// Metodo pra deixar o peso e a textura da fruta aleatorios
void Bonus::randomize(){
    const int pointsOptions[8] = {100, 300, 500, 700, 1000, 2000, 3000, 5000}; // Possibilidades de pontos que a frut pode dar pro pacman caso ele a pegue
    std::vector<int> weights = {40, 20, 15, 10, 7, 5, 2, 1}; // peso pra cada valor possivel de pontuacao (maior peso -> maior probabilidade)

    // Gera um indice aleatorio com base nos pesos
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::discrete_distribution<> dist(weights.begin(), weights.end());
    int textureIndex = dist(gen);

    // Configura a textura pegando uma imagem da 'base de dados' de imagens
    std::string texturePath = "./assets/images/spritesheet.png";
    texture.loadFromFile(texturePath);
    int offsetX = (textureIndex * 16) + 32;
    this->frame.left = offsetX;
    this->frame.top = 48;
    this->sprite.setTexture(texture);
    this->sprite.setTextureRect(this->frame);
    this->points = pointsOptions[textureIndex];
}

// Metodo pra retornar os pontos da fruta bonus
int Bonus::getPoints() const {
    return this->points;
}

// Metodo pra desenhar a fruta bonus apenas se ela estiver ativa
void Bonus::draw(sf::RenderWindow& window, const sf::Vector2f& offset) const{
    if(this->active){
        sf::Sprite spriteCopy = this->sprite;
        spriteCopy.setPosition((this->pos.x * 10) + offset.x, (this->pos.y * 10) + offset.y); // Novamente aqui da pra ver o offset sendo aplicado, alinhando tudo no centro da tela
        window.draw(spriteCopy);
    }
}

// Metodo pra voltar a fruta bonus pra inativa
void Bonus::reset(){
    this->active = false;            // Desativa a fruta bônus
    this->points = 0;                // Reseta os pontos
    this->pos = {0, 0};              // Reseta a posição
    this->respawnTimer.restart();    // Inicia o temporizador de respawn
}

// Metodo so pra retornar o status atual da fruta bonus: ativada/desativada
bool Bonus::isActive() const {
    return this->active;
}

// Metodo pra retornar a posicao da fruta bonus no grid
sf::Vector2<int> Bonus::getPosition() const {
    return this->pos;
}


