#include <WiFi.h>
// Замените на свой идентификатор и пароль
const char* ssid = "XXXXXXX";
const char* password = "XXXXXXXX";
WiFiServer server(80); // Номер порта для сервера
String header; // HTTP-запрос
String output26State = "off"; // текущее состояние кнопки
const int output26 = 4;

void setup() {
  Serial.begin(115200);
  pinMode(output26, OUTPUT);
  digitalWrite(output26, LOW);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available(); // прослушка входящих клиентов
  if (client) { // Если подключается новый клиент,
    Serial.println("New Client."); // выводим сообщение
    String currentLine = "";
    while (client.connected()) { // цикл, пока есть соединение клиента
      if (client.available()) { // если от клиента поступают данные,
        char c = client.read(); // читаем байт, затем
        Serial.write(c); // выводим на экран
        header += c;
        if (c == '\n') { // если байт является переводом строки
          // если пустая строка, мы получили два символа перевода строки
          // значит это конец HTTP-запроса, формируем ответ сервера:
          if (currentLine.length() == 0) {
            // HTTP заголовки начинаются с кода ответа (напр., HTTP / 1.1 200 OK)
            // и content-type, затем пустая строка:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            // Включаем или выключаем светодиоды
            if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            }
            // Формируем веб-страницу на сервере
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // можете менять под свои нужды
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border:  none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color:#555555;}</style></head>");
            client.println("<body><h1>ESP32 Web Server</h1>");
            // Выводим текущее состояние кнопки
            client.println("<p>LED - State " + output26State + "</p>");
            // Если output26State сейчас off, то выводим надпись ON
            if (output26State == "off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            // HTTP-ответ завершается пустой строкой
            client.println();
            break;
          } else { // если получили новую строку, очищаем currentLine
            currentLine = "";
          }
        } else if (c != '\r') { // Если получили что-то ещё кроме возврата строки,
          currentLine += c; // добавляем в конец currentLine
        }
      }
    }
    // Очистим переменную
    header = "";
    // Закрываем соединение
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
