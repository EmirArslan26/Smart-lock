# 🔐 Akıllı Oda Kilit Sistemi (Arduino Tabanlı)

Bu proje, **odanızın güvenliğini sağlamak** ve **izinsiz girişleri engellemek** amacıyla geliştirilmiş bir **Arduino tabanlı akıllı kilit sistemidir**. Projede, 3D baskı parçalar, LCD ekran, tuş takımı (keypad) ve RFID gibi bileşenler kullanılarak fiziksel güvenlik desteklenmiştir.

🎥 Tanıtım Videosu: [YouTube'da İzle](https://youtu.be/GzReClGKmlc?si=mIgXRUeBFSM3Q335)

---

## 📦 Kullanılan Malzemeler

| Malzeme                     | Açıklama                                      |
|----------------------------|-----------------------------------------------|
| Arduino Uno                | Sistemin ana kontrolcüsüdür                   |
| 3D Baskı Parçalar          | Kilit mekanizması için tasarlanmış parçalar   |
| I2C LCD Ekran (16x2)       | Giriş bilgilerini ve sistem durumunu gösterir |
| Buzzer                     | Uyarı sesleri için                            |
| 3x4 Keypad                 | Şifre girişi                                  |
| RFID Kart Okuyucu (RC522) | Opsiyonel – Kullanıcı tanımlı giriş           |
| Jumper Kablolar            | Bağlantılar için                              |
| Anahtar / Adaptör / Pil    | 5–9V güç kaynağı                              |
| 9V Pil + Pil Yatağı        | (Eğer adaptör yerine pil kullanılacaksa)      |

---

## ⚙️ Sistem Özellikleri

- Şifreli giriş sistemi (3x4 Keypad ile)
- LCD ekranda bilgilendirme
- Hatalı girişlerde buzzer ile uyarı
- RFID kart ile giriş (isteğe bağlı)
- 3D yazıcı ile hazırlanmış kilit mekanizması
- Arduino UNO tabanlı sistem
- Enerji kaynağı olarak adaptör veya pil kullanılabilir

---

## 🧠 Opsiyonel Bileşenler

RFID kart okuyucu zorunlu **değildir**. Eğer bu bileşeni sisteminize eklemek istemiyorsanız, kodu **hiçbir değişiklik yapmadan** doğrudan yükleyebilir ve kullanabilirsiniz.

---

## 🚀 Nasıl Kullanılır?

1. Gerekli malzemeleri temin edin.
2. Devre bağlantılarını yapın (devre şeması için proje klasörüne göz atın).
3. Arduino IDE üzerinden `akilli_kilit.ino` dosyasını açın ve yükleyin.
4. Sistemi test edin ve kişisel şifrenizi tanımlayın.
5. Dilerseniz RFID kart okuyucu modülünü entegre edin.

---

## 🧾 Lisans

Bu proje açık kaynaklıdır ve kişisel/öğrenci projelerinde serbestçe kullanılabilir. Her türlü katkıya açıktır!

---

## 💡 Katkıda Bulun

Pull request'ler ve issue bildirimleri memnuniyetle karşılanır. Projeyi geliştirmek istersen katkıda bulunabilirsin. 🙌

---

