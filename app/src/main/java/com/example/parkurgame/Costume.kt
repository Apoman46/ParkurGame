package com.example.parkurgame

/** Mağazadaki kostümler: id, isim, RGB rengi (native tarafa gönderilir) ve fiyatı. */
data class Costume(
    val id: Int,
    val name: String,
    val r: Float,
    val g: Float,
    val b: Float,
    val price: Int
)

object CostumeCatalog {
    val all = listOf(
        Costume(0, "Klasik Mavi", 0.20f, 0.78f, 1.00f, 0),
        Costume(1, "Ateş Kırmızı", 0.95f, 0.25f, 0.20f, 150),
        Costume(2, "Zehir Yeşili", 0.25f, 0.90f, 0.35f, 150),
        Costume(3, "Altın", 1.00f, 0.80f, 0.20f, 300),
        Costume(4, "Gölge Mor", 0.55f, 0.20f, 0.85f, 300),
        Costume(5, "Karbon Siyah", 0.12f, 0.12f, 0.14f, 500),
        Costume(6, "Buz Beyazı", 0.92f, 0.96f, 1.00f, 500),
        Costume(7, "Neon Pembe", 1.00f, 0.20f, 0.65f, 750)
    )
}
