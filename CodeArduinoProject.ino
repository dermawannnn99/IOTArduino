/*
 * Program Proyek Fisika
 * Menggunakan sensor ultrasonik dan LCD untuk menghitung gravitasi berdasarkan osilasi bola
 * Muhammad Rizky Dermawan, Andanda Fahrizal Assidiq
 */

#include <Wire.h>                     // Pustaka komunikasi I2C
#include <LiquidCrystal_I2C.h>        // Pustaka untuk LCD I2C

LiquidCrystal_I2C lcd(0x27, 16, 2);   // Inisialisasi LCD dengan alamat 0x27 dan ukuran 16x2
unsigned long waktuMulai, waktuSelesai, dataStopwatch;
int statusTombol = 0;                 // Status tombol (0: tidak ditekan, 1: ditekan)
int statusPaus = 0;                   // Status jeda stopwatch
long tombolTerakhir = 0;              // Waktu tombol terakhir ditekan untuk anti bouncing
long jedaAntiPantul = 50;             // Jeda waktu untuk menghindari pantulan tombol
long dataJeda = 0;                    // Menyimpan waktu jeda stopwatch

float jarak = 0.17;                   // Jarak tali bandul (dalam meter)

void setup() {
  Serial.begin(9600);                 // Memulai komunikasi serial pada baud rate 9600
  pinMode(A0, INPUT);                 // Mengatur pin A0 sebagai input untuk tombol
  pinMode(A1, INPUT);                 // Mengatur pin A1 sebagai input untuk tombol reset
  digitalWrite(A0, 1);                // Menarik pin A0 ke HIGH secara default
  digitalWrite(A1, 1);                // Menarik pin A1 ke HIGH secara default
  lcd.begin(16, 2);                   // Memulai LCD dengan ukuran 16x2

  lcd.backlight();                    // Mengaktifkan lampu latar LCD
  lcd.setCursor(0, 0);
  lcd.print(" SELAMAT DATANG");       // Menampilkan pesan selamat datang
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print(" PROJECT FISIKA");       // Menampilkan nama proyek
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">RIZKY DERMAWAN");       // Menampilkan nama anggota tim
  lcd.setCursor(0, 1);
  lcd.print(">ANANDA FAHRIZAL");
  delay(2000);
  lcd.clear();
  lcd.print("AYUNKAN BOLA");          // Menampilkan instruksi
  lcd.setCursor(0, 1);
  lcd.print("  SEKALI OSILASI");
}

void loop() {
  // Membaca tombol start/stop pada pin A0
  if (digitalRead(A0) == 0) {
    if ((millis() - tombolTerakhir) > jedaAntiPantul) {
      if (statusTombol == 0) {        // Jika status tombol tidak aktif
        lcd.clear();
        waktuMulai = millis();        // Menyimpan waktu saat tombol di tekan 
        statusPaus = 0;             
      } else if (statusTombol == 1) { // Jika status tombol aktif
        dataJeda = dataStopwatch;     // Menyimpan waktu jeda
        statusPaus = 1;               // Maka status jeda menjadi aktif
      }
      statusTombol = !statusTombol;   // Membalik status tombol
    }
    tombolTerakhir = millis();
  }

  // Membaca tombol reset pada pin A1
  else if (digitalRead(A1) == 0 && statusPaus == 1) {
    dataStopwatch = 0;                // Reset waktu stopwatch
    dataJeda = 0;                     // Reset data jeda
    lcd.clear();
    lcd.print("  Reset System");      // Menampilkan pesan reset
    lcd.setCursor(0, 1);
    lcd.print("");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AYUNKAN BOLA");        // Menampilkan kembali instruksi awal
    lcd.setCursor(0, 1);
    lcd.print("  SEKALI OSILASI");
  }

  // Jika tombol dalam status aktif, hitung waktu dan gravitasi
  if (statusTombol == 1) {
    waktuSelesai = millis();
    float jam, menit, detik, miliDetik;
    unsigned long sisa;

    // Menghitung waktu stopwatch
    dataStopwatch = waktuSelesai - waktuMulai;
    dataStopwatch = dataJeda + dataStopwatch;

    // Konversi waktu ke format jam, menit, detik, dan milidetik
    jam = int(dataStopwatch / 3600000);
    sisa = dataStopwatch % 3600000;
    menit = int(sisa / 60000);
    sisa = sisa % 60000;
    detik = int(sisa / 1000);
    miliDetik = sisa % 1000;

    // Menghitung gravitasi berdasarkan waktu osilasi
    float waktuOsilasi = detik + (miliDetik * 0.001);
    float gravitasi = (9.85 * jarak) / (waktuOsilasi * waktuOsilasi);

    // Menampilkan hasil pada LCD
    lcd.setCursor(0, 0);
    lcd.print("t= ");
    lcd.print(waktuOsilasi);
    lcd.print(" s    ");
    lcd.setCursor(0, 1);
    lcd.print("g= ");
    lcd.print(gravitasi);
    lcd.print(" m/s2    ");
  }
}
