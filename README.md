# UART_STM32
Kod do przesyłu i odbioru danych przez UART. Zawiera komendy do sterowania/zapalania diodami
Włączanie diody - wlacz all, wlacz1, wlacz2, wlacz3...
Wyłączanie diody - wylacz all, wylacz1, wylacz2, wylacz3...

-------------------------------------------------------
Inicjalizacja 
stty -F /dev/ttyACM0 115200 cs8 -cstopb -parenb -echo raw

stty -F /dev/ttyACM0 -cstopb  - baud bit stopu(paramet "-" włącza lub wyłącza)
stty -F /dev/ttyACM0 -parenb  - bity parzystości
stty -F /dev/ttyACM0 cs8      - liczba bitów
stty -F /dev/ttyACM0 115200   - baud rate
