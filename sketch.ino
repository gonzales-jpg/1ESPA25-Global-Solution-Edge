//1ESPA25-GS
//Gabriel Ardito Manes RM:567318 
//João Antônio Sarracine RM:567407
//João Pedro Gonzales Camargo RM:568166

#include <LiquidCrystal_I2C.h>
#include <Wire.h>   
#include <EEPROM.h>
#include <RTClib.h>
#define UTC_OFFSET 0

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int trigPin = 9;  
const int echoPin = 10; 
int ledR = 7;
int ledG = 6;

// EEPROM
const int maxRecords = 100;
const int recordSize = sizeof(unsigned long) + sizeof(int);
int startAddress = 0;
int endAddress = maxRecords * recordSize;
int currentAddress = 0;

// Botão
const int botao = 2;
unsigned long pressStart = 0;

// Controle por millis
unsigned long lastUpdate = 0;

//RTC
RTC_DS1307 RTC;

// ----------- EEPROM FUNCTIONS ------------------

void limparEEPROM() {
  for (int i = 0; i < endAddress; i++) EEPROM.update(i, 0xFF);
  currentAddress = 0;
  Serial.println("EEPROM limpa");
  lcd.setCursor(0, 0);
  lcd.print("EEPROM limpa!");
}

bool eepromVazia() {
  for (int addr = 0; addr < endAddress; addr++) {
    if (EEPROM.read(addr) != 0xFF) return false;
  }
  return true;
}

void salvarRegistro(unsigned long timestamp, int quantidade) {
  if (currentAddress + recordSize > endAddress) return;

  EEPROM.put(currentAddress, timestamp);
  EEPROM.put(currentAddress + sizeof(unsigned long), quantidade);
  currentAddress += recordSize;
}

void lerEEPROM() {
  if (eepromVazia()) {
    Serial.println("EEPROM vazia");
    lcd.setCursor(0, 0);
    lcd.print("EEPROM vazia!");
    return;
  }

  Serial.println("\n======= LOG EEPROM =======");

  for (int addr = 0; addr < currentAddress; addr += recordSize) {
    unsigned long ts;
    int qtd;

    EEPROM.get(addr, ts);
    EEPROM.get(addr + sizeof(unsigned long), qtd);

    if (ts == 0xFFFFFFFF || ts == 0) continue;

    DateTime dt = DateTime(ts);

    Serial.print(dt.timestamp(DateTime::TIMESTAMP_FULL));
    Serial.print(" -> Produtos: ");
    Serial.println(qtd);
  }

  Serial.println("==========================\n");
}

// ==================== CARACTERES CUSTOMIZADOS ====================
// Cada bloco representa 1 parte de uma letra feita em "pixels" no LCD
// Como o LCD só permite 8 caracteres customizados, dividimos letras
// grandes em partes (canto superior/esquerdo/direito/inferior).

// Letra "A" - canto superior esquerdo
byte A_topleft[8] = {
  B00000, B00000, B00011, B01111,
  B11000, B11000, B11000, B11000
};
// Letra "A" - canto inferior esquerdo
byte A_botleft[8] = {
  B11111, B11111, B11000, B11000,
  B11000, B11000, B00000, B00000
};
// Letra "A" - canto superior direito
byte A_topright[8] = {
  B00000, B00000, B11000, B11110,
  B00011, B00011, B00011, B00011
};
// Letra "A" - canto inferior direito
byte A_botright[8] = {
  B11111, B11111, B00011, B00011,
  B00011, B00011, B00000, B00000
};

// Letra "V" - canto superior esquerdo
byte V_topleft[8] = {
  B00000, B00000, B01100, B01100,
  B01100, B01100, B01100, B01110
};
// Letra "V" - canto inferior esquerdo
byte V_botleft[8] = {
  B00110, B00110, B00110, B00110,
  B00011, B00011, B00000, B00000
};
// Letra "V" - canto superior direito
byte V_topright[8] = {
  B00000, B00000, B00110, B00110,
  B00110, B00110, B00110, B01110
};
// Letra "V" - canto inferior direito
byte V_botright[8] = {
  B01100, B01100, B01100, B01100,
  B11000, B11000, B00000, B00000
};

// ==================== FUNÇÃO PARA DESENHAR TEXTO ====================
// Essa função recebe uma posição (coluna) e desenha as letras "VAVA"
// usando os blocos customizados. Assim conseguimos animar o texto
// passando da direita para a esquerda.
void desenhaTexto(int pos) {
  lcd.clear();

  // Linha superior
  lcd.setCursor(pos, 0);
  lcd.write(byte(4)); // V canto sup. esq
  lcd.write(byte(6)); // V canto sup. dir
  lcd.write(byte(0)); // A canto sup. esq
  lcd.write(byte(2)); // A canto sup. dir
  lcd.write(byte(4)); // P canto sup. esq (segunda letra P)
  lcd.write(byte(6)); // P canto sup. dir
  lcd.write(byte(0)); // A canto sup. esq (segunda letra A)
  lcd.write(byte(2)); // A canto sup. dir

  // Linha inferior
  lcd.setCursor(pos, 1);
  lcd.write(byte(5)); // V canto inf. esq
  lcd.write(byte(7)); // V canto inf. dir
  lcd.write(byte(1)); // A canto inf. esq
  lcd.write(byte(3)); // A canto inf. dir
  lcd.write(byte(5)); // V canto inf. esq (segunda letra P)
  lcd.write(byte(7)); // V canto inf. dir
  lcd.write(byte(1)); // A canto inf. esq (segunda letra A)
  lcd.write(byte(3)); // A canto inf. dir
}

// ----------- SETUP ------------------

const int produto = 5;
const int prateleira = 40;

void setup()
{
    Serial.begin(9600);

    pinMode(ledR, OUTPUT);
    pinMode(ledG, OUTPUT);

    Wire.begin();
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.print("Sistema iniciado");
    delay(1500);
   
    lcd.createChar(0, A_topleft);
    lcd.createChar(1, A_botleft);
    lcd.createChar(2, A_topright);
    lcd.createChar(3, A_botright);
    lcd.createChar(4, V_topleft);
    lcd.createChar(5, V_botleft);
    lcd.createChar(6, V_topright);
    lcd.createChar(7, V_botright);

  for (int pos = 16; pos >= 0; pos--) { 
    desenhaTexto(pos);
    delay(250);
  }

  delay(1500);
  lcd.clear();

    RTC.begin();

    pinMode(trigPin, OUTPUT);  
    pinMode(echoPin, INPUT);

    pinMode(botao, INPUT_PULLUP);
}

// ----------- LOOP PRINCIPAL (SEM DELAYS) ------------------

void loop()
{
  // ------ TEMPO ------  
  unsigned long agora = millis();

  // ------ BOTÃO PRIMEIRO (SEM TRAVAR) ------
  if (digitalRead(botao) == LOW) {

    delay(100); // debounce leve

    pressStart = millis();

    while (digitalRead(botao) == LOW) {
      if (millis() - pressStart > 2000) {
        limparEEPROM();
        return;
      }
    }

    lerEEPROM();
  }

  // ------ PROCESSANDO A CADA 3 SEGUNDOS ------
  if (agora - lastUpdate >= 3000) {
    lastUpdate = agora;

    DateTime now = RTC.now();
    unsigned long epoch = now.unixtime() + (UTC_OFFSET * 3600);

    long duration, cm;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH, 30000);
    cm = (duration * 0.0343) / 2;

    int nProdutos = 0;

    lcd.clear();
    lcd.print("Vizualizador");

    if(cm >= prateleira){
      lcd.setCursor(0,1);
      lcd.print("Prateleira vazia!");
      digitalWrite(ledR, HIGH);
      digitalWrite(ledG, LOW);
      nProdutos = 0;
    }
    else if(cm < produto){
      lcd.setCursor(0,1);
      lcd.print("Prateleira cheia!");
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, HIGH);
      nProdutos = 8;
    }
    else if(cm < 2*produto){
      lcd.setCursor(0,1);
      lcd.print("7 produtos!");
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, HIGH);
      nProdutos = 7;
    }
    else if(cm < 3*produto){
      lcd.setCursor(0,1);
      lcd.print("6 produtos!");
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, HIGH);
      nProdutos = 6;
    }
    else if(cm < 4*produto){
      lcd.setCursor(0,1);
      lcd.print("5 produtos!");
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, HIGH);
      nProdutos = 5;
    }
    else if(cm < 5*produto){
      lcd.setCursor(0,1);
      lcd.print("4 produtos!");
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, HIGH);
      nProdutos = 4;
    }
    else if(cm < 6*produto){
      lcd.setCursor(0,1);
      lcd.print("3 produtos!");
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, HIGH);
      nProdutos = 3;
    }
    else if(cm < 7*produto){
      lcd.setCursor(0,1);
      lcd.print("2 produtos!");
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, HIGH);
      nProdutos = 2;
    }
    else if(cm < 8*produto){
      lcd.setCursor(0,1);
      lcd.print("1 produto!");
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, HIGH);
      nProdutos = 1;
    }

    // SALVA SOMENTE SE ZERAR
    if (nProdutos == 0) {
      salvarRegistro(epoch, nProdutos);
    }

  }
}

// Mantive sua função original
long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
