package com.example.parkurgame

import android.graphics.Color
import android.graphics.drawable.GradientDrawable
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView

class CostumeAdapter(
    private val items: List<Costume>,
    private val onSelected: (Costume) -> Unit
) : RecyclerView.Adapter<CostumeAdapter.VH>() {

    inner class VH(view: View) : RecyclerView.ViewHolder(view) {
        val swatch: View = view.findViewById(R.id.swatch)
        val name: TextView = view.findViewById(R.id.costumeName)
        val status: TextView = view.findViewById(R.id.costumeStatus)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): VH {
        val v = LayoutInflater.from(parent.context).inflate(R.layout.item_costume, parent, false)
        return VH(v)
    }

    override fun getItemCount() = items.size

    override fun onBindViewHolder(holder: VH, position: Int) {
        val c = items[position]
        val unlocked = Prefs.isCostumeUnlocked(c.id)
        val selected = Prefs.selectedCostume == c.id

        val color = Color.rgb((c.r * 255).toInt(), (c.g * 255).toInt(), (c.b * 255).toInt())
        val gd = GradientDrawable()
        gd.shape = GradientDrawable.RECTANGLE
        gd.cornerRadius = 14f
        gd.setColor(color)
        gd.setStroke(4, if (selected) Color.parseColor("#FFC94D") else Color.parseColor("#33C7FF"))
        holder.swatch.background = gd

        holder.name.text = c.name
        holder.status.text = when {
            selected -> holder.itemView.context.getString(R.string.selected)
            unlocked -> holder.itemView.context.getString(R.string.select)
            else -> "${c.price} 🪙"
        }

        holder.itemView.setOnClickListener {
            if (unlocked) {
                Prefs.selectedCostume = c.id
                onSelected(c)
                notifyDataSetChanged()
            } else if (Prefs.coins >= c.price) {
                Prefs.coins = Prefs.coins - c.price
                Prefs.unlockCostume(c.id)
                Prefs.selectedCostume = c.id
                onSelected(c)
                notifyDataSetChanged()
            }
        }
    }
}
