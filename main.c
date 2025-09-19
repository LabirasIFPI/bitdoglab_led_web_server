#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/ip4_addr.h"

// --- CONFIGURAÇÕES ---
// Altere para o SSID e senha da sua rede Wi-Fi
#define WIFI_SSID "SEU_SSID_AQUI"
#define WIFI_PASSWORD "SUA_SENHA_AQUI"

// Pino do LED a ser controlado (GPIO 13 para o LED vermelho da BitDogLab)
#define LED_PIN 13

// --- PÁGINA HTML ---
// Esta string contém a página web que será enviada ao navegador
const char *html_page = "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n\r\n"
                        "<!DOCTYPE html><html><head><title>Pico W Controle</title></head><body>"
                        "<h1>Controle do LED do Pico W</h1>"
                        "<p>Use os bot&otilde;es abaixo para controlar o LED.</p>"
                        "<p><a href=\"/?led=on\"><button>Ligar LED</button></a></p>"
                        "<p><a href=\"/?led=off\"><button>Desligar LED</button></a></p>"
                        "</body></html>";

/**
 * @brief Manipulador de requisições HTTP recebidas.
 * Esta função é chamada pela LwIP sempre que chegam dados de um cliente.
 */
static err_t http_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        // Se não há dados (p == NULL), o cliente fechou a conexão
        tcp_close(tpcb);
        tcp_recv(tpcb, NULL);
        return ERR_OK;
    }
    
    // Converte o payload recebido para uma string
    char *data = (char*)p->payload;

    // Analisa a string da requisição para encontrar os comandos
    if (strstr(data, "GET /?led=on")) {
        printf("Comando recebido: Ligar LED\n");
        gpio_put(LED_PIN, 1);  // Liga o LED
    } else if (strstr(data, "GET /?led=off")) {
        printf("Comando recebido: Desligar LED\n");
        gpio_put(LED_PIN, 0);  // Desliga o LED
    }

    // Envia a página HTML como resposta para o cliente
    tcp_write(tpcb, html_page, strlen(html_page), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);

    // Libera a memória do buffer recebido
    pbuf_free(p);
    return ERR_OK;
}

/**
 * @brief Manipulador para aceitar novas conexões TCP.
 * Chamado pela LwIP quando um novo cliente se conecta ao servidor.
 */
static err_t http_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err) {
    // Define a função que irá tratar os dados recebidos nesta nova conexão
    tcp_recv(newpcb, http_server_recv);
    return ERR_OK;
}

/**
 * @brief Inicializa o servidor HTTP na porta 80.
 */
void init_http_server() {
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Falha ao criar PCB do TCP.\n");
        return;
    }

    // Vincula o servidor a qualquer endereço de IP na porta 80
    tcp_bind(pcb, IP_ADDR_ANY, 80);

    // Coloca o servidor em modo de escuta para aguardar conexões
    pcb = tcp_listen(pcb);

    // Define a função que será chamada para aceitar novas conexões
    tcp_accept(pcb, http_server_accept);

    printf("Servidor HTTP inicializado na porta 80.\n");
}

/**
 * @brief Função principal do programa.
 */
int main() {
    // Inicializa a interface de entrada/saída padrão (para o printf)
    stdio_init_all();

    // Configura o pino do LED como saída
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Inicializa o hardware Wi-Fi
    if (cyw43_arch_init()) {
        printf("Falha ao inicializar o Wi-Fi.\n");
        return -1;
    }

    // Habilita o modo Station (cliente Wi-Fi)
    cyw43_arch_enable_sta_mode();

    printf("Conectando ao Wi-Fi '%s'...\n", WIFI_SSID);

    // Tenta conectar ao Wi-Fi com um timeout de 10 segundos
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Falha ao conectar.\n");
        return 1;
    }
    
    printf("Conectado com sucesso!\n");
    sleep_ms(1000);

    // Obtém o endereço IP do dispositivo
    uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
    printf("IP do servidor %d.%d.%d.%d\n", ip_address[0], ip_address[1], ip_address[2], ip_address[3]);

    // Inicializa o servidor HTTP
    init_http_server();

    // Loop principal infinito para manter o programa e a rede ativos
    while (true) {
        cyw43_arch_poll();  // Processa eventos de rede pendentes
        sleep_ms(100);      // Pequena pausa para evitar uso excessivo da CPU
    }
}