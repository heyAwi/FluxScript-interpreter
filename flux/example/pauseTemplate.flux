--[[
Pause Button Test
FluxScript v1.0.0
]]

paused = false
speed = 4
direction = 1
posX = 20

func -ready() {
    game = windowAdd("Pause Test", 600x300)

    game.context {
        windowBackground("black")
        windowResizable(false)

        movingButton = windowButton(100x40, "blue", 20x100, "MOVING")
        pauseButton = windowButton(120x40, "red", 230x210, "PAUSE")

        pauseButton.onclick {
            if paused == false {
                paused = true
                pauseButton.text = "RESUME"
            } else {
                paused = false
                pauseButton.text = "PAUSE"
            }
        }
    }
}

func -always(60) {
    if paused == false {
        posX += speed * direction

        if posX >= 480 {
            direction = -1
        }

        if posX <= 20 {
            direction = 1
        }

        pos = movingButton.position
        pos[0] = posX
        movingButton.position = pos
    }
}