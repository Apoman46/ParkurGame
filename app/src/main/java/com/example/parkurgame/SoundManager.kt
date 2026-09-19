package com.example.parkurgame

import android.content.Context
import android.media.AudioAttributes
import android.media.SoundPool

/**
 * SoundPool tabanlı efekt ses yöneticisi. Ayarlar ekranındaki ses seviyesini uygular.
 */
class SoundManager(ctx: Context) {

    private val pool: SoundPool
    private val ids = HashMap<String, Int>()
    private val appCtx = ctx.applicationContext

    init {
        val attrs = AudioAttributes.Builder()
            .setUsage(AudioAttributes.USAGE_GAME)
            .setContentType(AudioAttributes.CONTENT_TYPE_SONIFICATION)
            .build()

        pool = SoundPool.Builder()
            .setMaxStreams(6)
            .setAudioAttributes(attrs)
            .build()

        load("jump", R.raw.jump)
        load("land", R.raw.land)
        load("fail", R.raw.fail)
        load("coin", R.raw.coin)
        load("click", R.raw.click)
    }

    private fun load(name: String, resId: Int) {
        ids[name] = pool.load(appCtx, resId, 1)
    }

    fun play(name: String) {
        val id = ids[name] ?: return
        val vol = Prefs.sfxVolume / 100f
        pool.play(id, vol, vol, 1, 0, 1f)
    }

    fun release() {
        pool.release()
    }
}
