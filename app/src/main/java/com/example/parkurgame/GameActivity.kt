package com.example.parkurgame

import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.MotionEvent
import android.widget.Button
import android.widget.FrameLayout
import android.widget.LinearLayout
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity

class GameActivity : AppCompatActivity() {

    private lateinit var glView: GameGLSurfaceView
    private lateinit var sound: SoundManager
    private lateinit var scoreText: TextView
    private lateinit var gameOverPanel: LinearLayout
    private lateinit var finalScoreText: TextView

    private val uiHandler = Handler(Looper.getMainLooper())
    private var running = true
    private var paused = false
    private var lastDifficulty = 0

    private val scoreUpdater = object : Runnable {
        override fun run() {
            if (running && !paused) {
                val h = NativeLib.getHeightScore()
                scoreText.text = "${h.toInt()} m"

                val diff = NativeLib.getDifficultyLevel()
                if (diff != lastDifficulty) {
                    lastDifficulty = diff
                    sound.play("coin")
                }
            }
            uiHandler.postDelayed(this, 150)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Prefs.init(this)
        setContentView(R.layout.activity_game)

        sound = SoundManager(this)

        glView = GameGLSurfaceView(this)
        findViewById<FrameLayout>(R.id.glContainer).addView(glView)

        scoreText = findViewById(R.id.scoreText)
        gameOverPanel = findViewById(R.id.gameOverPanel)
        finalScoreText = findViewById(R.id.finalScoreText)

        glView.onPlayerFell = { onGameOver() }

        setupControls()
        uiHandler.post(scoreUpdater)
    }

    private fun setupControls() {
        val btnLeft = findViewById<Button>(R.id.btnLeft)
        val btnRight = findViewById<Button>(R.id.btnRight)
        val btnJump = findViewById<Button>(R.id.btnJump)
        val btnPause = findViewById<Button>(R.id.btnPause)
        val btnRetry = findViewById<Button>(R.id.btnRetry)
        val btnToMenu = findViewById<Button>(R.id.btnToMenu)

        btnLeft.setOnTouchListener { _, event ->
            when (event.action) {
                MotionEvent.ACTION_DOWN -> NativeLib.setTurnInput(-1)
                MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> NativeLib.setTurnInput(0)
            }
            true
        }
        btnRight.setOnTouchListener { _, event ->
            when (event.action) {
                MotionEvent.ACTION_DOWN -> NativeLib.setTurnInput(1)
                MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> NativeLib.setTurnInput(0)
            }
            true
        }
        btnJump.setOnClickListener {
            NativeLib.requestJump()
            sound.play("jump")
        }
        btnPause.setOnClickListener {
            paused = !paused
            if (paused) glView.onPause() else glView.onResume()
        }
        btnRetry.setOnClickListener {
            gameOverPanel.visibility = android.view.View.GONE
            NativeLib.resetGame()
            running = true
            sound.play("click")
        }
        btnToMenu.setOnClickListener { finish() }

        val costume = CostumeCatalog.all.firstOrNull { it.id == Prefs.selectedCostume }
            ?: CostumeCatalog.all[0]
        // Native taraf sadece renderer hazır olduğunda ayarlanabilir; GL thread'e postla.
        glView.queueEvent {
            NativeLib.setCostumeColor(costume.r, costume.g, costume.b)
        }
    }

    private fun onGameOver() {
        if (!running) return
        running = false
        sound.play("fail")

        val finalH = NativeLib.getHeightScore().toInt()
        if (finalH > Prefs.bestScore) Prefs.bestScore = finalH
        Prefs.addCoins(finalH / 2)

        finalScoreText.text = "${getString(R.string.score)}: $finalH m"
        gameOverPanel.visibility = android.view.View.VISIBLE
    }

    override fun onPause() {
        super.onPause()
        glView.onPause()
    }

    override fun onResume() {
        super.onResume()
        if (!paused) glView.onResume()
    }

    override fun onDestroy() {
        super.onDestroy()
        sound.release()
        uiHandler.removeCallbacks(scoreUpdater)
    }
}
