#include <nuttx/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>

static char buffer[1024];

void set_color(int fd, uint8_t r, uint8_t g, uint8_t b) {
    /* * DENEY SONUCLARINA GORE SIRALAMA:
     * 1. Kanal -> Mavi (B)
     * 2. Kanal -> Yeşil (G)
     * 3. Kanal -> Kırmızı (R)
     * 4. Kanal -> Boş/Parlaklık (X)
     */
    
    uint8_t final_packet[4];
    
    final_packet[0] = b; // Mavi butona basınca 3. kanala giden veri aslında 1. kanala gitmeli
    final_packet[1] = g; // Yeşil kanal orta vagon
    final_packet[2] = r; // Kırmızı butona basınca burası yanmalı
    final_packet[3] = 0; // 4. kanal genelde beyaz veya parlaklıktır, 0 tutuyoruz

    lseek(fd, 0, SEEK_SET);
    write(fd, final_packet, 4);
    
    printf("--> RENK BASILDI: R:%d G:%d B:%d\n", r, g, b);
}

int main(int argc, FAR char *argv[])
{
    printf("Basit IoT Sunucusu Baslatiliyor...\n");
    
    /* leds0 komutu surucuyu acik birakmis olabilir,
       bu yuzden sadece O_WRONLY deniyoruz. */
    int led_fd = open("/dev/leds0", O_WRONLY);
    if (led_fd < 0) {
        printf("HATA: /dev/leds0 acilamadi!\n");
        return -1;
    }

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(80);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    
    printf("Port 80 dinleniyor...\n");

    while(1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) continue;

        memset(buffer, 0, sizeof(buffer));
        read(new_socket, buffer, sizeof(buffer) - 1);
        
        if (strstr(buffer, "GET /red"))   set_color(led_fd, 255, 0, 0);
        if (strstr(buffer, "GET /green")) set_color(led_fd, 0, 255, 0);
        if (strstr(buffer, "GET /blue"))  set_color(led_fd, 0, 0, 255);
        if (strstr(buffer, "GET /off"))   set_color(led_fd, 0, 0, 0);

        char *html = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"
                     "<html><body style='text-align:center; background:#222; color:white;'>"
                     "<h1>ESP32-C6 LED</h1>"
                     "<a href='/red'><button style='padding:20px; background:red; color:white;'>KIRMIZI</button></a><br><br>"
                     "<a href='/green'><button style='padding:20px; background:green; color:white;'>YESIL</button></a><br><br>"
                     "<a href='/blue'><button style='padding:20px; background:blue; color:white;'>MAVI</button></a>"
                     "</body></html>";
        
        write(new_socket, html, strlen(html));
        close(new_socket);
    }
    
    close(led_fd);
    return 0;
}

