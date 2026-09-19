package com.example.parkurgame

import android.content.Context
import android.content.SharedPreferences

/**
 * Basit SharedPreferences sarmalayıcısı: ses, grafik, jeton ve kostüm ayarlarını tutar.
 */
object Prefs {
    private const val NAME = "parkur_prefs"
    private lateinit var sp: SharedPreferences

    fun init(ctx: Context) {
        if (!::sp.isInitialized) {
            sp = ctx.applicationContext.getSharedPreferences(NAME, Context.MODE_PRIVATE)
        }
    }

    var sfxVolume: Int
        get() = sp.getInt("sfx_volume", 80)
        set(v) = sp.edit().putInt("sfx_volume", v).apply()

    var musicVolume: Int
        get() = sp.getInt("music_volume", 60)
        set(v) = sp.edit().putInt("music_volume", v).apply()

    // 0 = düşük, 1 = orta, 2 = yüksek
    var graphicsQuality: Int
        get() = sp.getInt("graphics_quality", 1)
        set(v) = sp.edit().putInt("graphics_quality", v).apply()

    var coins: Int
        get() = sp.getInt("coins", 0)
        set(v) = sp.edit().putInt("coins", v).apply()

    var bestScore: Int
        get() = sp.getInt("best_score", 0)
        set(v) = sp.edit().putInt("best_score", v).apply()

    var selectedCostume: Int
        get() = sp.getInt("selected_costume", 0)
        set(v) = sp.edit().putInt("selected_costume", v).apply()

    fun isCostumeUnlocked(id: Int): Boolean =
        id == 0 || sp.getBoolean("costume_unlocked_$id", false)

    fun unlockCostume(id: Int) {
        sp.edit().putBoolean("costume_unlocked_$id", true).apply()
    }

    fun addCoins(amount: Int) {
        coins = coins + amount
    }
}
