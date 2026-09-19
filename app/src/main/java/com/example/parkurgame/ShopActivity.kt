package com.example.parkurgame

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.RecyclerView

class ShopActivity : AppCompatActivity() {

    private lateinit var coinsText: android.widget.TextView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Prefs.init(this)
        setContentView(R.layout.activity_shop)

        coinsText = findViewById(R.id.coinsText)
        updateCoins()

        val list = findViewById<RecyclerView>(R.id.costumeList)
        list.layoutManager = GridLayoutManager(this, 4)
        list.adapter = CostumeAdapter(CostumeCatalog.all) { updateCoins() }

        findViewById<android.widget.Button>(R.id.btnBack).setOnClickListener { finish() }
    }

    private fun updateCoins() {
        coinsText.text = "${Prefs.coins} 🪙"
    }
}
