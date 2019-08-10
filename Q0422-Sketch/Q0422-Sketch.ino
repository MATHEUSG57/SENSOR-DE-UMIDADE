#include <SPI.h>
#include <Ethernet.h>
String readString;
 
int pino_sol = 12;
boolean ligado = true;

 
//Informacoes de endereco IP, gateway, mascara de rede
byte mac[] = { 0xA4, 0x28, 0x72, 0xCA, 0x55, 0x2F };
byte ip[] = { 192, 168, 0, 110 };
byte gateway[] = { 192, 168, 0, 1 };
byte subnet[] = { 255, 255, 255, 0 };
 
EthernetServer server(80);


void setup() {
  
  //Sensor
  pinMode(8, INPUT);

  //manual
  boolean ligado = true;

  //Atuador
  pinMode(12, OUTPUT);
  
  //LEDs
  pinMode(5, OUTPUT);  //vermelho
  pinMode(6, OUTPUT);  //amarelo
  pinMode(7, OUTPUT);  //verde
}

void loop() {

  leituraSensor = digitalRead(8);

  if (leituraSensor == HIGH) {
     //No estado seco
     digitalWrite(5, HIGH);  //vermelho
     digitalWrite(7, LOW);   //verde
  } else {
     //No estado úmido
     digitalWrite(5, LOW);   //vermelho
     digitalWrite(7, HIGH);  //verde
  }

  //Ao entrar no estado seco  
  if (leituraSensor && !leituraAnterior) {
     delay(5000);
     digitalWrite(5, LOW);   //vermelho
     digitalWrite(6, HIGH);  //amarelo

     while (digitalRead(8)) {
        digitalWrite(12, HIGH);   //rele / válvula / solenoide / bomba
        delay(500);
        digitalWrite(12, LOW);   //rele / válvula / solenoide / bomba

        delay(10000);          
     }
     digitalWrite(6, LOW);  //amarelo
  }
  
  leituraAnterior = leituraSensor;
}
 {
  Serial.begin(9600);
  pinMode(pino_sol, OUTPUT);
 
  //Inicializa Ethernet Shield
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
 
  Serial.println("FILIPEFLOP - Automacao Residencial"); 
 
  //Desliga solenoide
  digitalWrite(pino_sol, HIGH);
}
 
void loop()
{
  EthernetClient client = server.available();
  if (client) {
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        if (readString.length() < 100) {
          readString += c;
        }
        if (c == 'n')
        {
          //Controle do solenoide
          Serial.println(readString);
          //Liga a solenoide
          if (readString.indexOf("?ligar") > 0)
          {
            digitalWrite(pino_sol, LOW);
            Serial.println("solenoide Ligada");
            ligado = false;
          }
          else
          {
            //Desliga o solenoide
            if (readString.indexOf("?desligar") > 0)
            {
              digitalWrite(pino_sol, HIGH);
              Serial.println("solenooide Desligado");
              ligado = true;
            }
          }
          readString = "";
 
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<html>");
          client.println("<head>");
          client.println("<title>FILIPEFLOP - Automacao Residencial</title>");
          client.println("<meta http-equiv='Content-Type' content='text/html; charset=ISO-8859-1'>");
          client.println("<meta name='viewport' content='width=720, initial-scale=0.5' />");
          client.println("<link rel='stylesheet' type='text/css' href='http://img.filipeflop.com/files/download/automacao/automacao_residencial.css' />");
          client.println("<script type='text/javascript' src='http://img.filipeflop.com/files/download/automacao/automacao_residencial.js'></script>");
          client.println("</head>");
          client.println("<body>");
          client.println("<div id='wrapper'><img alt='FILIPEFLOP' src='http://img.filipeflop.com/files/download/automacao/logoFF.jpg'/><br/>");
          client.println("<div id='div1'>Rele 1</div>");
          client.println("<div id='div2'>Rele 2</div>");
          client.print("<div id='rele'></div><div id='estado' style='visibility: hidden;'>");
          client.print(ligado);
          client.println("</div>");
          client.println("<div id='botao'></div>");
          client.println("<div id='botao_2'></div>");
          client.print("<div id='rele_2'></div><div id='estado_2' style='visibility: hidden;'>");
          client.print(ligado_2);
          client.println("</div>");
          client.println("</div>");
          client.println("<script>AlteraRele1()</script>");
          client.println("<script>AlteraRele2()</script>");
          client.println("</div>");
          client.println("</body>");
          client.println("</head>");
 
          delay(1);
          client.stop();
        }
      }
    }
  }
