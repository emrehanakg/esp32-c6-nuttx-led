# ESP32-C6 NuttX Web tabanlı RGB LED Kontrolü 🚀

Bu proje, **Apache NuttX RTOS** üzerinde çalışan bir HTTP sunucusu aracılığıyla, ESP32-C6 mikrokontrolcüsüne bağlı **WS2812 RGB LED'i** web tarayıcısı üzerinden kontrol etmeyi sağlar.

## 🛠 Kullanılan Teknolojiler
- **Donanım:** ESP32-C6 DevKitC, WS2812 (NeoPixel) RGB LED
- **İşletim Sistemi:** Apache NuttX RTOS
- **Ağ Protokolleri:** Wi-Fi (WAPI), TCP/IP Sockets, HTTP
- **Programlama Dili:** C

## 📁 Proje Yapısı
- `src/`: Web sunucusu, soket programlama ve LED kontrolünü barındıran kaynak kodlar (`hello_main.c`).
- `config/`: ESP32-C6 ve NuttX için gerekli olan donanım/yazılım yapılandırma dosyası (`defconfig`).

## 🚀 Nasıl Çalıştırılır?
1. `config/defconfig` dosyasını NuttX dizinindeki `.config` olarak ayarlayın.
2. Sistemi derleyin ve karta yükleyin (`make flash`).
3. NSH terminali üzerinden Wi-Fi ağına bağlanın:
   ```bash
   wapi psk wlan0 "SIFRE" 3
   wapi essid wlan0 "AG_ADI" 1
   renew wlan0
