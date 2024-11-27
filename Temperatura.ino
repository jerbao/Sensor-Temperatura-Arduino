#include <Wire.h> // Biblioteca para comunicação I2C
#include <LiquidCrystal_I2C.h> // Biblioteca para controle do display LCD via I2C
#include <SPI.h>  // Biblioteca para comunicação SPI
#include <Adafruit_MAX31855.h>  // Biblioteca da Adafruit para o módulo MAX31855

#define col 16 // Definição do número de colunas do display
#define lin  2 // Definição do número de linhas do display
#define ende  0x27 // Definição do endereço do display
//Porta SDA: A4
//Porta SCL: A5
LiquidCrystal_I2C lcd(ende, col, lin); // Criação do objeto para o display I2C

// Definição dos pinos utilizados pelo MAX31855
int thermoDO = 12;  // Pino de saída de dados (MISO) 
int thermoCS = 4;  // Pino de Chip Select (CS) 
int thermoCLK = 5; // Pino de clock (CLK) 

  float tempC = 0;// Inicializa variável da temperatura
  float tempC_Anterior = 0;// Inicializa variável da temperatura anterior
  bool em_erro = false;// Inicializa boolean da mensagem de erro

// Criação do objeto para comunicação com o MAX31855
Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup() {
  // Inicializa a comunicação SPI
  SPI.begin();
  
  // Inicializa o display LCD via I2C
  lcd.init(); 
  lcd.backlight();  // Liga o backlight do display
  lcd.clear(); // Limpa o LCD e exibe uma mensagem de prontidão
  lcd.setCursor(0, 0);// Move o cursor para a primeira linha
  lcd.print("INICIALIZADO!");
  lcd.setCursor(0, 1);// Move o cursor para a segunda linha
  lcd.print("PRONTO PARA USO.");
  delay(2000);  // Pausa de 2 segundos para exibir a mensagem
}

void loop() {
  tempC = thermocouple.readCelsius(); // Lê a temperatura em Celsius
  // Verifica se houve erro na leitura da temperatura
  if((isnan(tempC) == true) && (em_erro == false)){
    em_erro = true; // Muda o boolean para verdade e evita do display ficar atualizando sem necessidade (tela piscando)
    lcd.clear();  // Limpa o display
    lcd.print("ERRO DE LEITURA!");  // Exibe mensagem de erro no display
  }
  else{
    em_erro = false;
    if(tempC != tempC_Anterior){// Esse if é para evitar da tela fiscar piscando durante valores constantes, só atualiza quando o valor da temperatura mudar
      tempC_Anterior = tempC; // Recebe o novo valor de temperatura para futura comparação com novos valores
      lcd.clear();// Limpa o display
      lcd.setCursor(0, 0);// Move o cursor para a primeira linha
      lcd.print("TEMPERATURA:");  // Exibe "TEMPERATURA: "
      lcd.setCursor(0, 1);// Move o cursor para a segunda linha
      lcd.print(tempC);  // Exibe a temperatura
      lcd.write(223);    // Exibe o símbolo de grau (º)
      lcd.print("C");    // Exibe a letra "C"
    }
  }
  delay(200);// Espera 200ms para refazer a leitura
}
