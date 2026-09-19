package com.example.parkurgame

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity

class MainMenuActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Prefs.init(this)
        setContentView(R.layout.activity_main_menu)

        findViewById<android.widget.Button>(R.id.btnPlay).setOnClickListener {
            startActivity(Intent(this, GameActivity::class.java))
        }
        findViewById<android.widget.Button>(R.id.btnShop).setOnClickListener {
            startActivity(Intent(this, ShopActivity::class.java))
        }
        findViewById<android.widget.Button>(R.id.btnSettings).setOnClickListener {
            startActivity(Intent(this, SettingsActivity::class.java))
        }
    }
}
