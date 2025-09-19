# 💡 Servidor Web para Controle de LED com Raspberry Pi Pico W

Um projeto simples e funcional que transforma a **BitDogLab (Raspberry Pi Pico W)** em um servidor web. Acesse uma página HTML a partir de qualquer dispositivo na mesma rede (celular, computador) para ligar e desligar um LED remotamente. O projeto usa as ferramentas disponibilizadas pelo Pico-SDK em C.


---

## 🌟 Recursos

-   🌐 **Servidor Web Embarcado:** Implementado usando a pilha de rede **LwIP**, otimizada para microcontroladores.
-   📱 **Interface Simples:** A página web contém dois botões claros ("Ligar LED" e "Desligar LED") para controle intuitivo.
-   ⚙️ **Código Autocontido:** Toda a lógica do servidor, conexão Wi-Fi e controle do LED está em um único arquivo `main.c` para facilitar o entendimento.
-   🛠️ **Configuração Mínima:** Basta editar o SSID e a senha do Wi-Fi diretamente no código para começar a usar.

## 🛠️ Pré-requisitos

### 🔩 Hardware
-   Raspberry Pi Pico W
-   LED (o código está configurado para o pino **GPIO 13**, compatível com o LED vermelho da placa BitDogLab)

### 💻 Software
-   [Pico C/C++ SDK](https://github.com/raspberrypi/pico-sdk) configurado.
-   [ARM GCC Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
-   CMake

## 🚀 Como Usar

### 1. Configure sua Rede
Abra o arquivo `main.c` e altere as seguintes linhas com os dados da sua rede Wi-Fi:

```c
#define WIFI_SSID "SEU_SSID_AQUI"
#define WIFI_PASSWORD "SUA_SENHA_AQUI"
```

### 2. 👨‍💻 Compile o Projeto
Crie um diretório de `build`, execute o `cmake` e o `make`.

```bash
mkdir build
cd build
cmake ..
make
```

### 3. ✨ Flashe o Pico W
-   Pressione e segure o botão **`BOOTSEL`** no seu Pico W e conecte-o ao computador.
-   Ele aparecerá como um dispositivo de armazenamento USB.
-   Arraste o arquivo `.uf2` gerado na pasta `build` para dentro do Pico W.

### 4. 🕹️ Controle o LED!
-   A placa irá reiniciar e se conectar ao Wi-Fi.
-   Abra um **monitor serial** para ver o endereço de IP que foi atribuído ao seu Pico W.

    ```
    Connecting to Wi-Fi...
    Connected.
    IP address 192.168.1.123
    ```
-   Abra um navegador no seu celular ou computador (conectado à mesma rede) e digite o endereço de IP.
-   A página de controle do LED aparecerá. Divirta-se!


Este projeto é uma base excelente para criar interfaces web para seus projetos embarcados. Explore e modifique!
