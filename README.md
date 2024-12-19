# Pong Game ESP32 Project - Embedded System

## **Deskripsi Sistem**
Proyek ini bertujuan untuk mengembangkan permainan **Pong Game** digital berbasis ESP32 yang mengintegrasikan teknologi tampilan visual Dot Matrix MAX7219 untuk menciptakan pengalaman permainan yang interaktif dan menyenangkan. Sistem ini memungkinkan pemain mengontrol paddle dengan **Potensiometer Slide**, memberikan kendali presisi, serta melakukan aksi smash menggunakan **Micro Limit Switch** untuk menambah dinamika permainan.

---

## **Tujuan Sistem**
1. Mengembangkan permainan yang interaktif dan menyenangkan, di mana pemain dapat berpartisipasi secara aktif menggunakan kontrol fisik.
2. Memastikan sistem dapat merespons input dari pemain dengan cepat dan akurat untuk mendukung gameplay yang mulus.
3. Menampilkan elemen permainan, skor, dan animasi secara dinamis menggunakan Dot Matrix MAX7219.

---

## **Hardware yang Dibutuhkan**
1. **ESP32**
2. **Dot Matrix MAX7219 (4 modules)**
3. **Potensiometer Slide** (2 buah untuk paddle kontrol)
4. **Micro Limit Switch** (2 buah untuk aksi smash)
5. **Buzzer Pasif**
6. Kabel Tembaga
7. PCB Matrix 12x18cm

---

## **Fitur Permainan**
### **1. Pergerakan Bola**
- Bola bergerak otomatis dengan kecepatan dan arah tertentu.
- Bola memantul saat mengenai paddle atau dinding.
- Jika bola melewati paddle, poin diberikan ke lawan.

### **2. Kontrol Paddle**
- Paddle bergerak berdasarkan nilai potensiometer yang diterjemahkan menjadi posisi paddle pada layar Dot Matrix.

### **3. Aksi Smash**
- Tombol smash dapat digunakan untuk meningkatkan kecepatan bola secara instan.
- Smash memiliki cooldown 10 detik sebelum bisa digunakan kembali.
- Indikator smash ditampilkan di **pojok kanan atas module 3** (Player 1) dan **pojok kiri atas module 0** (Player 2).

### **4. Skor dan Mode "Golden Point"**
- Skor dicatat setiap bola melewati paddle lawan.
- Ketika skor mencapai 10-10, sistem masuk ke mode "Golden Point" yang memerlukan selisih 2 poin untuk menentukan pemenang set.

### **5. Sistem Best-of-N**
- Permainan mendukung format **Best of 3/5/7**.
- Pemenang pertandingan adalah pemain pertama yang memenangkan jumlah set yang ditentukan.

### **6. Animasi dan Suara**
- **Countdown** sebelum permainan dimulai.
- Animasi "PONG" dan "GAME" ditampilkan sebelum permainan dimulai.
- Suara buzzer disinkronkan dengan berbagai aksi:
  - Pantulan bola pada paddle.
  - Tombol smash ditekan.
  - Gol tercetak.
  - Animasi kemenangan.

---

## **Logika Permainan**
### **Pergerakan Bola**
1. Bola bergerak otomatis di layar dengan arah tertentu.
2. Memantul saat mengenai paddle atau dinding.
3. Jika bola melewati paddle, poin diberikan kepada lawan.

### **Kontrol Paddle**
- Paddle dikendalikan menggunakan potensiometer yang memetakan nilai analog ke posisi vertikal paddle di layar.

### **Aksi Smash**
- Tombol smash meningkatkan kecepatan bola.
- Smash memiliki cooldown 10 detik.
- Indikator menunjukkan kapan smash siap digunakan.

### **Skor dan Golden Point**
- Skor ditampilkan secara real-time di Dot Matrix MAX7219.
- Jika skor mencapai 10-10, pemain harus mencetak 2 poin berturut-turut untuk memenangkan set.

### **Animasi dan Akhir Permainan**
- Setelah salah satu pemain memenangkan pertandingan, animasi "1 WIN" atau "2 WIN" ditampilkan bersama suara buzzer.
- Animasi "PONG" dan "GAME" muncul sebelum permainan dimulai.

---

## **Struktur File**
### **1. Main Program**
<!-- - `pong_game_18dec.ino`: Implementasi logika permainan dan kontrol utama. -->

### **2. Header Files**
- `Numbers.h`: Data byte untuk menampilkan angka di Dot Matrix.
- `WinnerBuzzer.h`: Suara buzzer untuk animasi kemenangan.
- `Countdown.h`: Countdown sebelum permainan dimulai.
- `Goal.h`: Suara dan logika saat gol tercetak.
- `PaddleSwitchBuzzer.h`: Suara buzzer untuk pantulan paddle dan smash.
- `PongGameBuzzer.h`: Animasi "PONG" dan "GAME" beserta suara buzzer.

---

## **Cara Kerja**
1. **Setup Awal**:
   - ESP32 dihubungkan dengan Dot Matrix MAX7219.
   - Potensiometer dan tombol limit switch dihubungkan ke input analog/digital ESP32.
   - Buzzer pasif dihubungkan ke salah satu pin digital ESP32.

2. **Proses**:
   - **Animasi "PONG" dan "GAME"** muncul sebelum permainan dimulai.
   - Countdown 3 detik dilakukan sebelum bola bergerak.
   - Pemain mengontrol paddle dengan potensiometer dan menggunakan tombol smash jika tersedia.
   - Skor dan set tercatat secara otomatis di Dot Matrix MAX7219.
   - Sistem menentukan pemenang berdasarkan format Best-of-N.

3. **Output**:
   - Dot Matrix menampilkan permainan secara real-time.
   - Buzzer memberikan suara saat bola memantul, smash dilakukan, gol tercetak, atau animasi kemenangan ditampilkan.

---

## **Pengaturan Pin**
| Komponen                | Pin ESP32  |
|-------------------------|------------|
| Dot Matrix DATA         | 23         |
| Dot Matrix CLOCK        | 18         |
| Dot Matrix LOAD         | 5          |
| Potensiometer Player 1  | 25         |
| Potensiometer Player 2  | 4          |
| Switch Player 1         | 32         |
| Switch Player 2         | 19         |
| Buzzer                  | 33         |

---

## **Instruksi Kompilasi dan Upload**
1. Pastikan Anda memiliki **Arduino IDE** atau editor lain yang kompatibel dengan ESP32.
2. Instal library berikut melalui Library Manager:
   - `LedControl.h`
3. Hubungkan ESP32 ke komputer.
4. Kompilasi dan upload kode ke ESP32.