// Get the canvas and its context
const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

// --- Game State ---

// The tank unit
const tank = {
    x: 100,
    y: 100,
    width: 30,
    height: 25,
    color: '#00ff00', // Neon green
    targetX: 100,
    targetY: 100,
    speed: 2
};

// --- Drawing Functions ---

function drawGrid() {
    const gridSize = 40;
    ctx.strokeStyle = 'rgba(0, 255, 255, 0.2)'; // Faint cyan lines
    for (let x = 0; x < canvas.width; x += gridSize) {
        ctx.beginPath();
        ctx.moveTo(x, 0);
        ctx.lineTo(x, canvas.height);
        ctx.stroke();
    }
    for (let y = 0; y < canvas.height; y += gridSize) {
        ctx.beginPath();
        ctx.moveTo(0, y);
        ctx.lineTo(canvas.width, y);
        ctx.stroke();
    }
}

function drawTank() {
    // Simple rectangle for the tank body
    ctx.fillStyle = tank.color;
    ctx.fillRect(tank.x - tank.width / 2, tank.y - tank.height / 2, tank.width, tank.height);

    // Simple rectangle for the turret
    ctx.fillStyle = 'rgba(255, 255, 255, 0.8)';
    ctx.fillRect(tank.x - 5, tank.y - 5, 20, 10); // Turret base
}

// --- Update Logic ---

function update() {
    // Calculate distance to target
    const dx = tank.targetX - tank.x;
    const dy = tank.targetY - tank.y;
    const distance = Math.sqrt(dx * dx + dy * dy);

    // Move towards target if not already there
    if (distance > tank.speed) {
        const angle = Math.atan2(dy, dx);
        tank.x += Math.cos(angle) * tank.speed;
        tank.y += Math.sin(angle) * tank.speed;
    } else {
        tank.x = tank.targetX;
        tank.y = tank.targetY;
    }
}


// --- Game Loop ---

function gameLoop() {
    // 1. Clear the canvas
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    // 2. Draw everything
    drawGrid();
    drawTank();

    // 3. Update state for the next frame
    update();

    // 4. Repeat
    requestAnimationFrame(gameLoop);
}

// --- Event Listeners ---

canvas.addEventListener('click', (event) => {
    const rect = canvas.getBoundingClientRect();
    const mouseX = event.clientX - rect.left;
    const mouseY = event.clientY - rect.top;

    // Set the tank's new target
    tank.targetX = mouseX;
    tank.targetY = mouseY;
});


// --- Start the game ---
console.log("Game starting...");
gameLoop();
