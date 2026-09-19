package com.example.parkurgame

import android.os.Bundle
import android.widget.RadioGroup
import android.widget.SeekBar
import androidx.appcompat.app.AppCompatActivity

class SettingsActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Prefs.init(this)
        setContentView(R.layout.activity_settings)

        val seekSfx = findViewById<SeekBar>(R.id.seekSfx)
        val seekMusic = findViewById<SeekBar>(R.id.seekMusic)
        val radioGroup = findViewById<RadioGroup>(R.id.radioGraphics)

        seekSfx.progress = Prefs.sfxVolume
        seekMusic.progress = Prefs.musicVolume

        when (Prefs.graphicsQuality) {
            0 -> radioGroup.check(R.id.radioLow)
            2 -> radioGroup.check(R.id.radioHigh)
            else -> radioGroup.check(R.id.radioMedium)
        }

        seekSfx.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(sb: SeekBar?, progress: Int, fromUser: Boolean) {
                Prefs.sfxVolume = progress
            }
            override fun onStartTrackingTouch(sb: SeekBar?) {}
            override fun onStopTrackingTouch(sb: SeekBar?) {}
        })

        seekMusic.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(sb: SeekBar?, progress: Int, fromUser: Boolean) {
                Prefs.musicVolume = progress
            }
            override fun onStartTrackingTouch(sb: SeekBar?) {}
            override fun onStopTrackingTouch(sb: SeekBar?) {}
        })

        radioGroup.setOnCheckedChangeListener { _, checkedId ->
            Prefs.graphicsQuality = when (checkedId) {
                R.id.radioLow -> 0
                R.id.radioHigh -> 2
                else -> 1
            }
        }

        findViewById<android.widget.Button>(R.id.btnBack).setOnClickListener { finish() }
    }
}
