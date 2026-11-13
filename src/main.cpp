
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Prototipos
void conexaoWiFi();
void reconexaoWiFi();
String consultaAPiCep(String cep);

// Dados WiFi
const char *SSID = "SENAI EDUCACIONAL";
const char *SENHA = "info@134";

const unsigned long tempoConexao = 10000;
const unsigned long tempoReconexao = 5000;

unsigned long tempoUltimaConexao = 0;

const String enderecoAPI = "https://brasilapi.com.br/api/cep/v2/";

// void setup()
// {
//   Serial.begin(115200);

//   conexaoWiFi();

//   String cep = "09510200";

//   String respostaAPI = consultaAPiCep(cep);

//   if (respostaAPI != "") {

//     JsonDocument doc;

//     DeserializationError error = deserializeJson(doc, respostaAPI);

//     if (!error) {

//       String rua = doc["street"];
//       String bairro = doc["neighborhood"];
//       String cidade = doc["city"];
//       String estado = doc["state"];
//       String cepRetornado = doc["cep"];

//       Serial.println("Logradouro: " + rua);
//       Serial.println("Bairro: " + bairro);
//       Serial.println("Cidade: " + cidade);
//       Serial.println("Estado: " + estado);
//       Serial.println("CEP: " + cepRetornado);

//     }
//     else {
//       Serial.println("Erro ao desserializar o JSON");
//     }
//   }
// }

// void loop()
// {
//   reconexaoWiFi();
// }

// void conexaoWiFi()
// {
//   Serial.printf("Conectando ao WIFI: %s", SSID);

//   WiFi.begin(SSID, SENHA);

//   unsigned long tempoWifi = millis();

//   while (WiFi.status() != WL_CONNECTED &&
//          millis() - tempoWifi < tempoConexao)
//   {
//     Serial.print(".");
//     delay(500);
//   }

//   if (WiFi.status() == WL_CONNECTED) {
//     Serial.println("\nWifi conectado com sucesso");
//     Serial.print("Endereco IP: ");
//     Serial.println(WiFi.localIP());
//   }
//   else {
//     Serial.println("\nFalha ao se conectar");
//   }
// }

// void reconexaoWiFi()
// {
//   unsigned long tempoAtual = millis();

//   if (tempoAtual - tempoUltimaConexao > tempoReconexao) {

//     if (WiFi.status() != WL_CONNECTED) {
//       Serial.println("Conexao Wifi perdida");
//       conexaoWiFi();
//     }

//     tempoUltimaConexao = tempoAtual;
//   }
// }

// String consultaAPiCep(String cep)
// {
//   if (WiFi.status() != WL_CONNECTED) {
//     Serial.println("WiFi desconectado");
//     return "";
//   }

//   HTTPClient http;

//   http.begin(enderecoAPI + cep);

//   int codigoHttp = http.GET();

//   String resposta = "";

//   if (codigoHttp == 200) {
//     resposta = http.getString();
//   }
//   else {
//     Serial.println("Erro na requisicao HTTP: " + String(codigoHttp));
//   }

//   http.end();

//   return resposta;
// }




void setup()
{
  Serial.begin(115200);

  Serial.setTimeout(5000);

  conexaoWiFi();

  Serial.println("Digite um CEP:");
}

void loop()
{
  reconexaoWiFi();

  if (Serial.available() > 0)
  {
    String cep = Serial.readStringUntil('\n');
    cep.trim();

    if (cep.length() == 8)
    {
      bool valido = true;

      for (int i = 0; i < cep.length(); i++)
      {
        if (!isDigit(cep[i]))
        {
          valido = false;
        }
      }

      if (valido)
      {
        String respostaAPI = consultaAPiCep(cep);

        if (respostaAPI != "")
        {
          JsonDocument doc;

          DeserializationError error = deserializeJson(doc, respostaAPI);

          if (!error)
          {
            String rua = doc["street"];
            String bairro = doc["neighborhood"];
            String cidade = doc["city"];
            String estado = doc["state"];
            String cepRetornado = doc["cep"];

            Serial.println("Logradouro: " + rua);
            Serial.println("Bairro: " + bairro);
            Serial.println("Cidade: " + cidade);
            Serial.println("Estado: " + estado);
            Serial.println("CEP: " + cepRetornado);
          }
          else
          {
            Serial.println("Erro ao desserializar JSON");
          }
        }
      }
      else
      {
        Serial.println("CEP invalido! Digite apenas numeros.");
      }
    }
    else
    {
      Serial.println("CEP invalido! Deve conter 8 caracteres.");
    }

    Serial.println("\nDigite outro CEP:");
  }
}

void conexaoWiFi()
{
  Serial.printf("Conectando ao WIFI: %s", SSID);

  WiFi.begin(SSID, SENHA);

  unsigned long tempoWifi = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - tempoWifi < tempoConexao)
  {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nWiFi conectado com sucesso");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("\nFalha ao se conectar");
  }
}

void reconexaoWiFi()
{
  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoUltimaConexao > tempoReconexao)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Conexao WiFi perdida");
      conexaoWiFi();
    }

    tempoUltimaConexao = tempoAtual;
  }
}

String consultaAPiCep(String cep)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi desconectado");
    return "";
  }

  HTTPClient http;

  http.begin(enderecoAPI + cep);

  int codigoHttp = http.GET();

  String resposta = "";

  if (codigoHttp == 200)
  {
    resposta = http.getString();
  }
  else if (codigoHttp == 404)
  {
    Serial.println("CEP nao encontrado");
  }
  else
  {
    Serial.println("Erro HTTP: " + String(codigoHttp));
  }

  http.end();

  return resposta;
}