# Spiral Parkur (OpenGL ES 3.0 + C++ / Kotlin)

## İçerik
- **Java/Kotlin (UI katmanı):** Ana Menü (Oyna / Ayarlar / Mağaza), Ayarlar (efekt/müzik sesi,
  grafik kalitesi), Mağaza (kostüm/renk satın alma, jeton sistemi), Oyun ekranı (GLSurfaceView +
  dokunmatik kontroller + skor + oyun sonu paneli).
- **C++ (native, JNI ile):** OpenGL ES 3.0 render motoru, GLSL ES 300 shader'lar (Lambert +
  Blinn-Phong parlaklık + kenar vurgusu + mesafe sisi), prosedürel **spiral** parkur üretici
  (yukarı doğru dönerek yükselen, giderek zorlaşan platformlar), basit fizik (yerçekimi, zıplama,
  şerit değiştirme) ve squash/stretch tarzı basit animasyon.
- **Ses:** jump/land/fail/coin/click için kodla sentezlenmiş kısa efektler (`res/raw/*.wav`).

## Nasıl Açılır / Derlenir
1. Android Studio (Hedgehog veya üstü) ile bu klasörü **"Open"** ile aç (klasörün kökünde
   `settings.gradle` var).
2. Gradle senkronizasyonu NDK ve CMake bileşenlerini isteyebilir; SDK Manager'dan
   **NDK (Side by side)** ve **CMake**'i kur.
3. Bir cihaz/emülatör seç (OpenGL ES 3.0 destekleyen, gerçek cihaz önerilir) ve **Run** de.
4. İmzalı APK için: **Build > Generate Signed Bundle / APK > APK** yolunu izleyip kendi
   keystore'unu oluştur/seç.

## Kontroller
- Sol / Sağ yuvarlak butonlar: iç / dış şeride geç (3 şeritli spiral parkur).
- ZIPLA butonu: boşluklardan atla.
- Sağ üstteki `II` butonu: duraklat.

## Oynanış Mantığı (özet)
- Oyuncu otomatik olarak ileri (spiral boyunca açısal olarak) ilerler ve yükseklik kazanır.
- Parkur, belirli aralıklarla (her ~18 segment) zorluk seviyesini artırır: platform boşluğu
  olasılığı ve ileri hız kademeli olarak artar.
- Bir şeritte platform yoksa ve oyuncu o şeritteyken zemine ulaşırsa düşmeye başlar; belirli bir
  mesafe düşünce oyun biter.
- Skor = ulaşılan yükseklik (metre). Oyun sonunda skorun yarısı kadar jeton kazanılır ve
  mağazada kostüm açmakta kullanılabilir.

## Bilinçli Basitleştirmeler (dürüstçe belirtmek isterim)
- Karakter gerçek iskeletli (rig) bir 3D model değildir; kod içinde küp tabanlı, squash/stretch
  animasyonlu basit bir gövdedir. Gerçek bir karakter modeli/animasyonu (örn. Blender'da
  hazırlanmış .fbx/.gltf + iskelet animasyonu) eklemek istersen, modeli assets/ klasörüne koyup
  bir glTF loader (örn. cgltf) entegre etmek gerekir — bu depoya dahil değildir.
- Kostümler şu an renk/materyal varyasyonu olarak uygulanmıştır (mağaza altyapısı gerçek farklı
  3D modelleri de destekleyecek şekilde genişletilebilir).
- Bu ortamda Android SDK/NDK bulunmadığından proje derlenerek test edilemedi; yapı ve API
  kullanımı doğru olacak şekilde yazıldı, olası küçük derleme hatalarını Android Studio anında
  gösterip düzeltmene yardımcı olacaktır.

## Termux'tan APK Üretme (GitHub Actions ile)

Bu depoda `.github/workflows/build.yml` hazır: kodu GitHub'a attığında Google'ın x86_64 sunucusunda
gerçek NDK ile derlenir ve sana indirilebilir bir debug APK üretir. Telefonda ağır bir kurulum
gerekmez, sadece bir GitHub hesabı ve internet.

**1) Termux'ta hazırlık**
```
pkg update && pkg upgrade -y
pkg install git -y
git config --global user.name "Adın"
git config --global user.email "eposta@example.com"
```

**2) Bu zip'i telefonda bir klasöre çıkar** (örn. Termux'un erişebildiği bir yere kopyala):
```
cd ~
cp /sdcard/Download/SpiralParkur_AndroidStudioProjesi.zip .
pkg install unzip -y
unzip SpiralParkur_AndroidStudioProjesi.zip
cd ParkurGame
```
(Termux'un `~/storage` üzerinden `/sdcard`'a erişebilmesi için daha önce `termux-setup-storage`
çalıştırmış olman gerekir.)

**3) GitHub'da boş bir repo oluştur** (github.com üzerinden, README eklemeden, örn. adı
`spiral-parkur`).

**4) Kişisel erişim jetonu (PAT) oluştur:** GitHub artık şifreyle push'a izin vermiyor.
GitHub → Settings → Developer settings → Personal access tokens → "repo" yetkisiyle bir jeton
oluştur ve kopyala.

**5) Termux'tan gönder:**
```
git init
git add .
git commit -m "ilk commit"
git branch -M main
git remote add origin https://github.com/KULLANICI_ADIN/spiral-parkur.git
git push -u origin main
```
Push sırasında kullanıcı adı sorulunca GitHub kullanıcı adını, şifre sorulunca **PAT jetonunu**
yapıştır.

**6) Derlemeyi izle:** GitHub'da repo sayfasına git → **Actions** sekmesi → push'un tetiklediği
"Build Debug APK" işini aç, birkaç dakika sürer (yeşil tik = başarılı).

**7) APK'yı indir:** Aynı sayfanın altında **Artifacts** bölümünde `SpiralParkur-debug-apk`
adında bir .zip göreceksin — indir, telefonda aç (dosya yöneticisi veya `unzip` ile) içinden
`.apk` çıkacak. "Bilinmeyen kaynaklardan yükleme"ye izin verip apk'ya dokunarak kur.

Bir sonraki değişiklikte tekrar `git add . && git commit -m "..." && git push` yeterli — Actions
otomatik tekrar derler ve yeni bir APK üretir.

## Dosya Haritası
```
app/src/main/cpp/         -> native-lib.cpp (JNI), GameRenderer, Player, ParkourGenerator,
                              Mesh, Shader, MathUtil, Shaders.h (GLSL kaynakları)
app/src/main/java/.../    -> MainMenuActivity, SettingsActivity, ShopActivity, GameActivity,
                              GameGLSurfaceView, NativeLib (JNI köprüsü), Prefs, SoundManager,
                              Costume/CostumeAdapter
app/src/main/res/         -> layout, values, drawable, mipmap (ikon), raw (ses efektleri)
```
