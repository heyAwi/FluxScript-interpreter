
get random

--[[ Snake - FluxScript v1.1.0
W A S D = Move
P = Pause / Resume
R = Restart
]]
cols = 16
rows = 16
cellSize = 20

tiles = []
snake = []

direction = "right"
nextDirection = "right"

foodX = 0
foodY = 0
score = 0

paused = false
gameOver = false
won = false

boardColor = "#171C25"
bodyColor = "#28B463"
headColor = "#7DFF70"
foodColor = "#FF5252"

func paintCell(x, y, color) {
    index = y * cols + x
    tile = tiles[index]
    tile.color = color
}

func spawnFood() {
    if len(snake) == cols * rows {
        won = true
        gameOver = true
        statusLabel.text = "YOU WIN!"
        return
    }

    valid = false

    while not valid {
        foodX = random.randint(0, cols - 1)
        foodY = random.randint(0, rows - 1)

        valid = true

        for part in snake {
            if part[0] == foodX and part[1] == foodY {
                valid = false
            }
        }
    }

    paintCell(foodX, foodY, foodColor)
}

func updateScore() {
    scoreLabel.text = "SCORE: " + str(score)
}

func resetGame() {
    snake = [[8, 8], [7, 8], [6, 8]]

    direction = "right"
    nextDirection = "right"

    score = 0
    paused = false
    gameOver = false
    won = false

    for tile in tiles {
        tile.color = boardColor
    }

    for part in snake {
        paintCell(part[0], part[1], bodyColor)
    }

    paintCell(8, 8, headColor)

    spawnFood()
    updateScore()

    statusLabel.text = "PLAYING"
    pauseButton.text = "PAUSE"
}

func changeDirection(newDirection) {
    if paused or gameOver {
        return
    }

    if newDirection == "up" and direction != "down" {
        nextDirection = "up"
    } elif newDirection == "down" and direction != "up" {
        nextDirection = "down"
    } elif newDirection == "left" and direction != "right" {
        nextDirection = "left"
    } elif newDirection == "right" and direction != "left" {
        nextDirection = "right"
    }
}

func togglePause() {
    if gameOver {
        return
    }

    paused = not paused

    if paused {
        statusLabel.text = "PAUSED"
        pauseButton.text = "RESUME"
    } else {
        statusLabel.text = "PLAYING"
        pauseButton.text = "PAUSE"
    }
}

func endGame() {
    gameOver = true

    if won {
        statusLabel.text = "YOU WIN!"
    } else {
        statusLabel.text = "GAME OVER"
    }

    pauseButton.text = "PAUSE"
}

func updateGame() {
    if paused or gameOver {
        return
    }

    direction = nextDirection

    head = snake[0]

    newX = head[0]
    newY = head[1]

    if direction == "up" {
        newY -= 1
    } elif direction == "down" {
        newY += 1
    } elif direction == "left" {
        newX -= 1
    } elif direction == "right" {
        newX += 1
    }

    if newX < 0 or newX >= cols or newY < 0 or newY >= rows {
        endGame()
        return
    }

    eating = newX == foodX and newY == foodY

    checkLength = len(snake)

    if not eating {
        checkLength -= 1
    }

    for i(checkLength) {
        part = snake[i]

        if part[0] == newX and part[1] == newY {
            endGame()
            return
        }
    }

    oldHead = snake[0]

    newSnake = [[newX, newY]]

    for part in snake {
        newSnake.add part
    }

    if not eating {
        tail = newSnake.pop
        paintCell(tail[0], tail[1], boardColor)
    }

    snake = newSnake

    paintCell(oldHead[0], oldHead[1], bodyColor)
    paintCell(newX, newY, headColor)

    if eating {
        score += 1
        updateScore()
        spawnFood()
    }
}

func -ready() {
    main = windowAdd("FluxScript Snake", 360x440)

    main.context {
        windowBackground("#10141C")
        windowResizable(false)

        for y(rows) {
            for x(cols) {
                px = 10 + x * cellSize
                py = 10 + y * cellSize
                square = windowDrawSquare(19x19, [px, py], boardColor, 0)
                tiles.add square
            }
        }
        scoreLabel = windowLabel(140x30, "white", 10x345, "SCORE: 0")
        statusLabel = windowLabel(150x30, "cyan", 180x345, "PLAYING")
        pauseButton = windowButton(145x35, "blue", 10x375, "PAUSE")
        restartButton = windowButton(145x35, "green", 185x375, "RESTART")
        pauseButton.onclick {
            togglePause()
        }

        restartButton.onclick {
            resetGame()
        }
    }

    resetGame()
}
func -keypress("w") {
    changeDirection("up")
}

func -keypress("s") {
    changeDirection("down")
}

func -keypress("a") {
    changeDirection("left")
}

func -keypress("d") {
    changeDirection("right")
}

func -keypress("p") {
    togglePause()
}

func -keypress("r") {
    resetGame()
}

func -always(10) {
    updateGame()
}
