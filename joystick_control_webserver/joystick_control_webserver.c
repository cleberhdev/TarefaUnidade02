#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/cyw43_arch.h"
#include <stdio.h>
#include <string.h>
#include "lwip/tcp.h"
#include <math.h>

#define WIFI_SSID "VIRTEX_A"
#define WIFI_PASSWORD "lamar338"


#define JOYSTICK_X_PIN 26 
#define JOYSTICK_Y_PIN 27  
#define JOYSTICK_BUTTON_PIN 22

typedef struct {
    float x;
    float y;
    bool button;
    const char* direction;
    float angle; 
} JoystickData;

JoystickData joystick;
bool debounce_button() {
    static uint32_t last_time = 0;
    bool state = !gpio_get(JOYSTICK_BUTTON_PIN);
    if (state && (to_ms_since_boot(get_absolute_time())) - last_time > 50){
        last_time = to_ms_since_boot(get_absolute_time());
        return true;
    }
    return false;
}

void read_joystick() {
    adc_select_input(0);
    joystick.x = (adc_read() / 2047.5f) - 1.0f;
    adc_select_input(1);
    joystick.y = 1.0f - (adc_read() / 2047.5f);
    
    joystick.button = debounce_button();

    if (fabsf(joystick.x) < 0.1f && fabsf(joystick.y) < 0.1f) {
        joystick.direction = "Centro";
        joystick.angle = 0;
        return;
    }

    joystick.angle = atan2f(joystick.y, joystick.x) * 180.0f / M_PI;
    joystick.angle = fmodf(joystick.angle + 360.0f, 360.0f);

    if (joystick.angle >= 337.5f || joystick.angle < 22.5f) {
        joystick.direction = "Norte";
    } else if (joystick.angle >= 22.5f && joystick.angle < 67.5f) {
        joystick.direction = "Nordeste";
    } else if (joystick.angle >= 67.5f && joystick.angle < 112.5f) {
        joystick.direction = "Leste";
    } else if (joystick.angle >= 112.5f && joystick.angle < 157.5f) {
        joystick.direction = "Sudeste";
    } else if (joystick.angle >= 157.5f && joystick.angle < 202.5f) {
        joystick.direction = "Sul";
    } else if (joystick.angle >= 202.5f && joystick.angle < 247.5f) {
        joystick.direction = "Sudoeste";
    } else if (joystick.angle >= 247.5f && joystick.angle < 292.5f) {
        joystick.direction = "Oeste";
    } else {
        joystick.direction = "Noroeste";
    }
}

static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) {
        tcp_close(tpcb);
        return ERR_OK;
    }

    read_joystick();

    char *request = calloc(p->tot_len + 1, sizeof(char));
    pbuf_copy_partial(p, request, p->tot_len, 0);

    char response[2048];

    if (strncmp(request, "GET /data", 9) == 0) {
        snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "\r\n"
            "{"
            "\"x\": %.2f,"
            "\"y\": %.2f,"
            "\"button\": \"%s\","
            "\"direction\": \"%s\","
            "\"angle\": %.1f"
            "}",
            joystick.x,
            joystick.y,
            joystick.button ? "Pressionado" : "Solto",
            joystick.direction,
            joystick.angle
        );
    } else {
        char transform_style[128];
        if (strcmp(joystick.direction, "Centro") == 0) {
            strcpy(transform_style, "transform: translate(-50%, -50%);");
        } else {
            snprintf(transform_style, sizeof(transform_style),
                "transform: translate(-50%%, -50%%) scaleX(-1) rotate(%.1fdeg) translateY(-115px);",
                joystick.angle
            );
        }

    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<!DOCTYPE html>"
        "<html lang='pt-BR'>"
        "<head><meta charset='UTF-8'>"
        "<title>Joystick Status</title>"
        "<style>"
        "body { font-family: Arial; text-align: center; background-color: #f0f8ff; }"
        ".container { max-width: 600px; margin: auto; background: white; padding: 20px; "
                    "border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }"
        ".compass { width:250px; height:250px; border-radius:50%%; margin:20px auto; "
                "background:url('https://lh4.googleusercontent.com/-CVs_02oIm3g/TYwFeJFzB-I/AAAAAAAAAcQ/shj5fbaKagA/s1600/ROSA+DOS+VENTOS.jpg') "
                "center no-repeat; background-size:contain; border:3px solid #333; "
                "box-shadow:0 0 15px rgba(0,0,0,0.2); position:relative; }"
        ".direction-indicator { width:20px; height:20px; background:red; border-radius:50%%; "
                            "position:absolute; top:50%%; left:50%%; border:2px solid white; "
                            "box-shadow:0 0 5px rgba(0,0,0,0.5); }"
        ".data-panel { margin-top:20px; padding:15px; background:#f8f8f8; border-radius:8px; }"
        "h1 { color:#004080; } strong { color:#006400; }"
        ".direction-value { color:#004080; font-size:1.2em; font-weight:bold; }"
        "</style></head><body>"
        "<div class='container'><h1>Status do Joystick</h1>"
        "<div class='compass'>"
        "<div class='direction-indicator' style=\"%s\"></div>"
        "</div>"
        "<div class='data-panel'>"
        "<p>Eixo X: <strong>%.2f</strong></p>"
        "<p>Eixo Y: <strong>%.2f</strong></p>"
        "<p>Botão: <strong>%s</strong></p>"
        "<p>Direção: <span class='direction-value'>%s (%.1f°)</span></p>"
        "</div></div></body></html>",
        transform_style,      
        joystick.x,
        joystick.y,
        joystick.button ? "Pressionado" : "Solto",
        joystick.direction,
        joystick.angle
    );
    }
    tcp_write(tpcb, response, strlen(response), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);
    pbuf_free(p);
    free(request);
    return ERR_OK;
}

static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, tcp_server_recv);
    return ERR_OK;
}

int main() {
    stdio_init_all();
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);
    
    gpio_init(JOYSTICK_BUTTON_PIN);
    gpio_set_dir(JOYSTICK_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON_PIN);
    if (cyw43_arch_init()) {
        printf("Erro ao iniciar Wi-Fi\n");
        return -1;
    }
    
    cyw43_arch_enable_sta_mode();
    printf("Conectando a %s...\n", WIFI_SSID);
    
    while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Falha na conexão. Tentando novamente...\n");
        sleep_ms(2000);
    }
    printf("Conectado!\n");
    if (netif_default) {
        printf("Servidor acessível em: http://%s\n", ipaddr_ntoa(&netif_default->ip_addr));
    }

    struct tcp_pcb *server = tcp_new();
    tcp_bind(server, IP_ADDR_ANY, 80);
    server = tcp_listen(server);
    tcp_accept(server, tcp_server_accept);
    printf("Servidor pronto!\n");

    while (true) {
        cyw43_arch_poll();
        sleep_ms(10);
    }

    cyw43_arch_deinit();
    return 0;
}