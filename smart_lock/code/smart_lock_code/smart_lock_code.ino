#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 3;
#define RFID_RST_PIN 9
#define RFID_SS_PIN 10

#define buzzer A1
#define button A2

// EEPROM ayarlari
const int EEPROM_CARD_COUNT        = 0;    // Kart sayisi
const int EEPROM_CARD_DATA_START   = 1;    // Kart verileri baslangici
const int CARD_ID_LENGTH           = 4;    // UID uzunlugu (byte)
const int MAX_CARDS                = 4;    // Maks kayitli kart

const int EEPROM_PASS_ADDR         = 50;   // Sifre baslangic
const int PASS_LENGTH              = 4;    // Sifre uzunlugu

String input;
String password = "1234";
char key;
bool lock_state;

int saved_cards_number = 0;


byte card_list[MAX_CARDS][CARD_ID_LENGTH];

char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[KEYPAD_ROWS] = {8,7,6,5};
byte colPins[KEYPAD_COLS] = {4,3,2};

Keypad keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
LiquidCrystal_I2C lcd(0x27,16,2);
MFRC522 reader(RFID_SS_PIN, RFID_RST_PIN);
Servo motor;

void lock(bool state) {
  motor.write(state ? 80 : 0);
  lock_state = state;
}

// Kartın listede olup olmadığını kontrol eden fonksiyon
bool isCardValid(byte uid[]) {
  for (int i = 0; i < saved_cards_number; i++) {
    bool match = true;
    for (byte j = 0; j < CARD_ID_LENGTH; j++) {
      if (card_list[i][j] != uid[j]) {
        match = false;
        break;
      }
    }
    if (match) return true;
  }
  return false;
}

void save_card(byte uid[]) {
  if (saved_cards_number < MAX_CARDS) {
    int addres = EEPROM_CARD_DATA_START + saved_cards_number * CARD_ID_LENGTH;
    for (byte i = 0; i < CARD_ID_LENGTH; i++) {
      EEPROM.write(addres + i, uid[i]);
    }
    // EKLENDİ: Yeni kartı card_list'e ekle
    for (byte i = 0; i < CARD_ID_LENGTH; i++) {
      card_list[saved_cards_number][i] = uid[i];
    }
    saved_cards_number++;
    // EKLENDİ: Kart sayısını EEPROM'a kaydet
    EEPROM.write(EEPROM_CARD_COUNT, saved_cards_number);
  } else {
    lcd.clear(); 
    lcd.setCursor(0,0); lcd.print("Hata:");
    lcd.setCursor(0,1); lcd.print("Fazla kart");
    delay(2000); // EKLENDİ: Mesajın görünmesi için bekleme
  }
}
void beep(int beep_duration,int range_duration,byte beep_count){
  for (int i = 0;i < beep_count;i++){
    digitalWrite(buzzer,1);
    delay(beep_duration);
    digitalWrite(buzzer,0);
    delay(range_duration);
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  SPI.begin();
  reader.PCD_Init();
  Serial.begin(9600);
  motor.attach(A0);
  lock(0);
  pinMode(buzzer,OUTPUT);
  pinMode(button,INPUT);

  // EEPROM'dan sifre yukle veya ilk defa yaz
  if (EEPROM.read(EEPROM_PASS_ADDR) == 0xFF) {
    for (int i = 0; i < PASS_LENGTH; i++)
      EEPROM.write(EEPROM_PASS_ADDR + i, password.charAt(i));
  } else {
    String sp = "";
    for (int i = 0; i < PASS_LENGTH; i++)
      sp += char(EEPROM.read(EEPROM_PASS_ADDR + i));
    password = sp;
  }

  // EEPROM'dan kayıtlı kart sayısını güvenli oku
  saved_cards_number = EEPROM.read(EEPROM_CARD_COUNT);
  if (saved_cards_number == 0xFF || saved_cards_number > MAX_CARDS) {
    saved_cards_number = 0;
    EEPROM.write(EEPROM_CARD_COUNT, saved_cards_number);
  }

  // EEPROM'dan id'leri listeye toplama
  for (int i = 0; i < saved_cards_number; i++) {
    int adres = EEPROM_CARD_DATA_START + i * CARD_ID_LENGTH;
    for (int b = 0; b < CARD_ID_LENGTH; b++) {
      card_list[i][b] = EEPROM.read(adres + b);
    }
  }
}


void loop() {
  // Ana ekran
  lcd.setCursor(2,0); lcd.print("Merhaba,");
  lcd.setCursor(2,1); lcd.print("Hosgeldiniz");

  // buton iç kapı kilit açma
  if (digitalRead(button) == 1){
    delay(200); // buton sıçramasını önler
    lock(lock_state ? 0 : 1);
    while (digitalRead(button) == 1); // buton bırakılana kadar bekle
  }
  // RFID kart kontrolü ve kilit açma
  if (reader.PICC_IsNewCardPresent() && reader.PICC_ReadCardSerial()) {
    byte uid[CARD_ID_LENGTH];
    Serial.print("Kart ID: ");
    for (byte i = 0; i < CARD_ID_LENGTH; i++) {
      uid[i] = reader.uid.uidByte[i];
      Serial.print(uid[i], HEX);
      if (i < CARD_ID_LENGTH - 1) Serial.print(":");
    }
    Serial.println();
    
    // Kartın geçerliliğini kontrol et ve kilit aç
    if (isCardValid(uid)) {
      lock(1);
      lcd.clear();
      lcd.setCursor(0,0); lcd.print("Kilit acildi");
      beep(100,100,1);
      delay(3000);
      lcd.clear();
    } 
    else {
      lcd.clear();
      lcd.setCursor(0,0); lcd.print("Gecersiz kart");
      beep(100,60,6);
      delay(2000);
      lcd.clear();
    }
    
    reader.PICC_HaltA(); // Kart okumayı durdur
    //reader.PCD_StopCrypto1(); // Şifreleme modunu kapat, böylece yeni kart okuma işlemleri sorunsuz gerçekleşir.
    // EKLENDİĞİ YERDEN KALDIRILDI: reader.PCD_Stop();
  }

  // Keypad girişi
  key = keypad.getKey();
  if (key) {
    input += key;
    Serial.println(input);
  }

  // Şifre giriş modu **
  if (input.endsWith("**")) {
    input = "";
    lcd.clear(); lcd.setCursor(0,0); lcd.print("Sifre:");
    while (input.length() < PASS_LENGTH) {
      if ((key = keypad.getKey())) {
        input += key;
        beep(100,100,1);
        lcd.setCursor(0,1);
        for (int i = 0; i < input.length(); i++) lcd.print('*');
      }
    }
    if (input == password) {
      lock(1);
      lcd.clear(); lcd.setCursor(0,0); lcd.print("Kilit acildi");
      delay(3000);
    } else {
      lcd.clear(); lcd.setCursor(0,0); lcd.print("Sifre hatali");
      beep(100,60,6);
      delay(2000);
    }
    input = ""; 
    lcd.clear();
  }

  // Kilit kapatma modu :"##"
  if (input.endsWith("##")) {
    lock(0);
    input = "";
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("Kilit kapandi");
    delay(2000);
    lcd.clear();
  }

  // Şifre değiştirme: "99"
  if (input.endsWith("99")) {
    lcd.clear(); lcd.setCursor(0,0); lcd.print("Eski sifre:");
    input = "";
    while (input.length() < PASS_LENGTH) {
      if ((key = keypad.getKey())) {
        input += key;
        beep(100,100,1);
        lcd.setCursor(0,1);
        for (int i = 0; i < input.length(); i++) lcd.print('*');
      }
    }
    if (input == password) {
      lcd.clear(); lcd.setCursor(0,0); lcd.print("Yeni sifre:");
      input = "";
      while (input.length() < PASS_LENGTH) {
        if ((key = keypad.getKey())) {
          input += key;
          beep(100,100,1);
          lcd.setCursor(0,1); lcd.print(input);
        }
      }
      password = input;
      for (int i = 0; i < PASS_LENGTH; i++)
        EEPROM.write(EEPROM_PASS_ADDR + i, password.charAt(i));
      lcd.clear();
      lcd.setCursor(0,0); lcd.print("Sifre degisti");
      delay(2000);
    } 
    else {
      lcd.clear(); lcd.setCursor(0,0); lcd.print("Sifre hatali");
      beep(100,60,6);
      delay(2000);
    }
    input = "";
    lcd.clear();
  }

  // Kart tanımlama modu: "88"
  if (input.endsWith("88")) {
    input = ""; // Girilen "88" temizlenir
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("Sifre girin:"); // Şifre girme ekranı
    String passInput = "";
    
    // Şifre girişini bekle (örneğin 4 haneli bir şifre)
    while (passInput.length() < 4) { 
      char key = keypad.getKey();
      if (key) {
        passInput += key;
        beep(100,100,1);
        lcd.setCursor(0, 1);
        for (int i = 0; i < passInput.length(); i++) lcd.print('*'); // Şifreyi yıldızla göster
      }
    }

    // Şifre doğrulama
    if (passInput == password) { // Örnek şifre "1234", bunu kendi şifrenle değiştir
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("Yeni kartinizi"); // 13 karakter
      lcd.setCursor(0, 1); lcd.print("okutun");        // 6 karakter
      
      bool cardSaved = false;
      // Kart okutulana kadar bekle
      while (!cardSaved) {
        if (reader.PICC_IsNewCardPresent() && reader.PICC_ReadCardSerial()) {
          byte uid[CARD_ID_LENGTH];
          Serial.print("Kart ID: ");
          for (byte i = 0; i < CARD_ID_LENGTH; i++) {
            uid[i] = reader.uid.uidByte[i];
            Serial.print(uid[i], HEX);
            if (i < CARD_ID_LENGTH - 1) Serial.print(":");
          }
          Serial.println();

          // Kartın daha önce kaydedilip kaydedilmediğini kontrol et
          if (!isCardValid(uid)) {
            save_card(uid); // Kartı kaydet
            cardSaved = true;
            lcd.clear();
            lcd.setCursor(0, 0); lcd.print("Yeni kart");     // 9 karakter
            lcd.setCursor(0, 1); lcd.print("kaydedildi");   // 10 karakter
            delay(2000); // Mesajı 2 saniye göster
          } else {
            lcd.clear();
            lcd.setCursor(0, 0); lcd.print("Kart zaten var");
            delay(2000);
            break;
          }
          reader.PICC_HaltA(); // Kart okumasını sonlandır
        }
      }
    } else {
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("Sifre hatali");
      beep(100,60,6);
      delay(2000); // Hata mesajını 2 saniye göster
    }
    
    // İşlem sonunda ana ekrana dön
    lcd.clear();
  }

  // EEPROM sıfırlamak için (kayıtlı kart idleri,şifre): "00"
  if (input.endsWith("00")){
    input = ""; // Girilen "88" temizlenir
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.print("Sifre girin:"); // Şifre girme ekranı
    String passInput = "";
    
    // Şifre girişini bekle (örneğin 4 haneli bir şifre)
    while (passInput.length() < 4) { 
      char key = keypad.getKey();
      if (key) {
        passInput += key;
        beep(100,100,1);
        lcd.setCursor(0, 1);
        for (int i = 0; i < passInput.length(); i++) lcd.print('*'); // Şifreyi yıldızla göster
      }
    }

    // Şifre doğrulama
    if (passInput == password) {// Kartları sıfırla
      for (int i = 0; i < MAX_CARDS * CARD_ID_LENGTH; i++) {
        EEPROM.write(EEPROM_CARD_DATA_START + i, 0xFF);
      }
      EEPROM.write(EEPROM_CARD_COUNT, 0);

      // Şifreyi sıfırla
      for (int i = 0; i < PASS_LENGTH; i++) {
        EEPROM.write(EEPROM_PASS_ADDR + i, 0xFF);
      }
    }
    else{
      lcd.clear(); lcd.setCursor(0,0); lcd.print("Sifre hatali");
      beep(100,60,6);
      delay(2000);
    }
    input = "";
    lcd.clear();
  }
}

// orange-red  purple-orange  brown-brown
