// Spinner javascript
// jshint maxerr:200

/****************************************
 * Canvas Draw base functions
****************************************/

// Canvas setup
const canvas = document.getElementById("myCanvas");
var ctx = canvas.getContext("2d");


/****************************************
 * Block Types
****************************************/

// Size of blocks
const blockSize = 25;

// For functions drawing on the canvas
const draw = {
    circle: function(x, y, radius, color) {
        ctx.beginPath();
        ctx.arc(x, y, radius, 0, 2 * Math.PI);
        ctx.strokeStyle = color;
        ctx.stroke();
        ctx.fillStyle = color;
        ctx.fill();
    },
    rect: function(x, y, width, height, color) {
        ctx.beginPath();
        ctx.rect(x, y, width, height);
        ctx.strokeStyle = color;
        ctx.stroke();
        ctx.fillStyle = color;
        ctx.fill();
    },
    text: function(x, y, color, text) {
        ctx.font = "28px Arial";
        ctx.textAlign = "center";
        ctx.fillStyle = color;
        ctx.fillText(text, x, y);
    },
    erase: function(x, y) {
        draw.rect(x, y, blockSize, blockSize, "black");
    },
    open: function(x, y) {
        draw.rect(x, y, blockSize, blockSize, "white");
    },
    closed: function(x, y) {
        draw.rect(x, y, blockSize, blockSize, "black");
    },
    start: function (x, y) {
        draw.rect(x, y, blockSize, blockSize, "red");
        draw.text(x+(blockSize/2), y+22, "white", "S");
    },
    end: function (x, y) {
        draw.rect(x, y, blockSize, blockSize, "blue");
        draw.text(x+(blockSize/2), y+22, "white", "E");
    },
    visited: function(x, y) {
        draw.rect(x, y, blockSize, blockSize, "#00bcbc");
    },
    checked: function(x, y) {
        draw.rect(x, y, blockSize, blockSize, "#FF8F00");
    },
    shortest: function(x, y) {
        draw.rect(x, y, blockSize, blockSize, "green");
    },
    char: function(x, y) {
        draw.circle(x+(blockSize/2), y+(blockSize/2), 7, "purple");
    },
};

/****************************************
 * Block Class & Drawing Blocks
****************************************/

// Height and Width of grid
const blocksW = 25;
const blocksH = 16;

// Array to store each block in
var blocks = [];

// Start and end coordinates
let startCo = [];
let endCo = [];
// To ensure there's only one start and one end
let start = false;
let end = false;

// Finds the blocks index for two coordinates
function indexFor(blockX, blockY){
    return (blockY * blocksW) + blockX;
}
// These calculate x and y co from grid coordinates
function co(blockNum) {
    return (blockNum * (blockSize+1));
}
// Turn coordinate into block
function coToBlock(co) {
    return (Math.floor(co / (blockSize+1)));
}

// Check if block x y is inside the grid
function isInGrid(blockX, blockY) {
    if (((blockY === -1) || (blockY === blocksH) || (blockX === -1) || (blockX === blocksW))) {
        return false;
    }
    return true;
}

// Block base
class Block {
    constructor(blockX, blockY, style) {
        this.blockX = blockX;
        this.blockY = blockY;
        this.x = co(blockX);
        this.y = co(blockY);
        this.style = style;
        this.path = [];
        // Uncaculated values are -1, an unobtainable number
        this.gCost = -1;
        this.hCost = -1;
        this.fCost = -1;
    }
    
    draw() {
        this.erase();
        if (this.style === "open") {
            draw.open(this.x, this.y);
        }
        else if (this.style === "closed") {
            draw.closed(this.x, this.y);
        }
        else if (this.style === "start") {
            draw.start(this.x, this.y);
            start = true;
            startCo[0] = coToBlock(this.x);
            startCo[1] = coToBlock(this.y);
        }
        else if (this.style === "end") {
            draw.end(this.x, this.y);
            end = true;
            endCo[0] = coToBlock(this.x);
            endCo[1] = coToBlock(this.y);
        }
        else if (this.style === "visited") {
            draw.visited(this.x, this.y);
        }
        else if (this.style === "checked") {
            draw.checked(this.x, this.y);
        }
        else if (this.style === "shortest") {
            draw.shortest(this.x, this.y);
        }
    }
    
    erase() {
        draw.erase(this.x, this.y);
    }

    // Distance from start
    getGCost() {
        // This is the shortest count of block from the start
        // Saves the block coordinates in an array
        if (this.gCost < 0) {
            // getGCost must only be called when the robot is on the block
            if (this.path.length === 0) {
                this.path = checker.center().path.slice(0);
                this.path.push(this.blockX);
                this.path.push(this.blockY);
            }
            
            this.gCost = Math.floor((this.path.length - 1) / 2);
        }
        return (this.gCost);
    }
    
    // Distance from end
    getHCost() {
        // An estimate based on how far away the end is
        if (this.hCost < 0 ) {
            let xDistance = Math.abs(endCo[0] - coToBlock(this.x));
            let yDistance = Math.abs(endCo[1] - coToBlock(this.y));
            this.hCost = xDistance + yDistance;
        }
        return(this.hCost);
    }
    
    // Both G-Cost and H-Cost
    getFCost() {
        if (this.fCost < 0) {
            this.fCost = this.getGCost() + this.getHCost();
        }
        return (this.fCost);
    }
}

// Changes map and redraws the block
function updateBlock(blockX, blockY, newStyle) {
    blocks[indexFor(blockX, blockY)].erase();
    if (getStyle(blockX, blockY) === "start") {
        startCo = [];
        start = false;
    }
    else if (getStyle(blockX, blockY) === "end") {
        endCo = [];
        end = false;
    }
    blocks[indexFor(blockX, blockY)].style = newStyle;
    blocks[indexFor(blockX, blockY)].draw();
}

// Fills blocks array and canvas
function populateBlocks() {
    for (let r = 0; r < blocksW; r++) {
        for (let c = 0; c < blocksH; c++) {
            blocks[indexFor(r, c)] = new Block(r, c, "open");
            blocks[indexFor(r, c)].draw();
        }
    }
}
// intial draw for map
populateBlocks();
updateBlock(3, 3, "start");
updateBlock(7, 5, "end");

// returns the style of the block
function getStyle(blockX, blockY) {
    return (blocks[indexFor(blockX, blockY)].style);
}

/****************************************
 * Moving Robot
****************************************/

const near = [
    [1, 0, -1, 0],
    [0, 1, 0, -1],
];

// Char is sort of like Care, like a carer of the fields
class Char {
    constructor (blockX, blockY) {
        this.blockX = blockX;
        this.blockY = blockY;
        // Keeps track of the currently open-end paths in an array (not visited yet)
        this.openPaths = [];
        // Blocks hold their own shortest path from the start, so robot can pick up from any block
        // When one route doesn't pan out, the robot picks one from the start so there's a chance of backtracking
        // use splice(index, 1) to remove element from the middle of an array
    }
    
    draw() {
        this.center().erase();
        this.center().draw();
        draw.char(co(this.blockX), co(this.blockY));
        if (this.blockX === endCo[0] && this.blockY === endCo[1]) {
            console.log("at end!");
            for (let i = 3; i < (this.center().path.length - 2); i+= 2) {
                let tempX = i - 1;
                let tempY = i;
                updateBlock(this.center().path[tempX], this.center().path[tempY], "shortest");
            }
        }
    }
    
    // Moves the char itself
    moveUp() {
        this.blockY--;
        this.draw();
    }
    moveDown() {
        this.blockY++;
        this.draw();
    }
    moveLeft() {
        this.blockX--;
        this.draw();
    }
    moveRight() {
        this.blockX++;
        this.draw();
    }
    
    // Returns the current block's info
    center() {
        return(blocks[indexFor(this.blockX, this.blockY)]);
    }
    
    // 0=Right 1=Down 2=Left 3=Up
    around(direct) {
        return (blocks[indexFor(this.blockX+near[0][direct], this.blockY+near[1][direct])]);
    }
    // 0=Right 1=Down 2=Left 3=Up
    isMoveable(direct) {
        if (isInGrid(this.blockX+near[0][direct], this.blockY+near[1][direct])) {
            if (this.around(direct).style !== "closed" && this.around(direct).style !== "visited" && this.around(direct).style !== "start") {
                return true;
            }
        }
        return false;
    }
    
    // Checks four blocks around, sets block values, picks one to move forward
    // If a dead end is found, robot resumes search from the array of open ends
    checkAround() {
        // Uses an array to find & remember the lowest f-cost
        let fCosts = [];
        let lowest = (this.center().getFCost() * 3);    // unobtainable
        // Direction is where the robot will move next
        let direction = -1;
        
        // First find the lowest fCost
        for (let i = 0; i < 4; i++) {
            if (this.isMoveable(i)) {
                fCosts[i] = this.around(i).getFCost();
                if (fCosts[i] < lowest) {
                    lowest = fCosts[i];
                }
            }
        }
        
        // Will not change anything if no lowest fCost (dead end was found)
        if (lowest === -1) {
            // Find new block from open array, change path
        }
        else {
            // Uses coordinates saved in an array for this section
            for (let i = 0; i < 4; i++) {
                if (this.isMoveable(i)) {
                    if (fCosts[i] === lowest && (direction < 0)) {
                        if (this.around(i).style !== "end") {
                            updateBlock(this.blockX+near[0][i], this.blockY+near[1][i], "visited");
                        }
                        direction = i;
                    }
                    else {
                        updateBlock(this.blockX+near[0][i], this.blockY+near[1][i], "checked");
                        this.openPaths.push(this.blockX+near[0][i]);
                        this.openPaths.push(this.blockY+near[1][i]);
                    }
                }
            // then need to move robot, add new block to path, add other blocks to open array
            }
            this.center().draw();
            switch (direction) {
                case 0: 
                    checker.moveRight();
                    break;
                case 1:
                    checker.moveDown();
                    break;
                case 2:
                    checker.moveLeft();
                    break;
                case 3:
                    checker.moveUp();
                    break;
            }
        }
    }
}
// Declared before later use
var checker;

// Will remove this
document.onkeydown = function (e) {
    switch (e.key) {
        case 'c':
            checker.checkAround();
            //console.log(checker.blockX, checker.blockY);
            break;
    }
};

/****************************************
 * Clicking the Grid
****************************************/

// For getting mouse coordinates
let rect = canvas.getBoundingClientRect();
function getMouseX(e) {
    return (e.clientX - rect.left);
}
function getMouseY(e) {
    return (e.clientY - rect.top);
}

// On mouse click in canvas
canvas.addEventListener('click', function(e) {
    let blockX = coToBlock(getMouseX(e));
    let blockY = coToBlock(getMouseY(e));
    if (stylus.styling === true) {
        if (getStyle(blockX, blockY) !== stylus.stat) {
            //console.log(stylus.stat);
            if (stylus.stat === "start") {
                if (start === false) {
                    updateBlock(blockX, blockY, stylus.stat);
                }
            } 
            else if (stylus.stat === "end") {
                if (end === false) {
                    updateBlock(blockX, blockY, stylus.stat);
                }
            }
            else {
                updateBlock(blockX, blockY, stylus.stat);
            }
        }
        else {
            updateBlock(blockX, blockY, "open");
        }
        // Don't draw more than one start or end
        if (stylus.stat === "start" || stylus.stat === "end") {
            stylus.stat = "closed";
        }
        printStyle(blockX, blockY);
    }
    else {
        printStyle(blockX, blockY);
    }
}, false);

/****************************************
 * Block-type Buttons
****************************************/

// 
var stylus = {
    stat: "closed",
    styling: true,
    // Top buttons use these to change stylus value
    closed: function() {
        stylus.stat = "closed";
    },
    open: function() {
        stylus.stat = "open";
    },
    start: function() {
        stylus.stat = "start";
    },
    end: function() {
        stylus.stat = "end";
    }
};

// Start robot
function run() {
    stylus.styling = false;
    if (start === true && end === true) {
        checker = new Char(startCo[0], startCo[1]);
        checker.draw();
    }
}
// Restart styling, reset long and short styles to open
function edit() {
    stylus.styling = true;
}
  
/****************************************
 * Info Span
****************************************/

const span = document.getElementById("infoSpan");

function getCoPrint(blockX, blockY) {
    return ("(" + blockX + ", " + blockY + ") ");
}

function getStylePrint(blockX, blockY) {
    return("Status = "+ getStyle(blockX, blockY));
}

function printStyle(blockX, blockY) {
    span.textContent = (getCoPrint(blockX, blockY) + getStylePrint(blockX, blockY));
    if (blocks[indexFor(blockX, blockY)].fCost !== -1) {
        span.textContent += (", F-Cost = " + blocks[indexFor(blockX, blockY)].fCost);
    }
    if (blocks[indexFor(blockX, blockY)].gCost !== -1) {
        span.textContent += (", G-Cost = " + blocks[indexFor(blockX, blockY)].gCost);
    }
    if (blocks[indexFor(blockX, blockY)].hCost !== -1) {
        span.textContent += (", H-Cost = " + blocks[indexFor(blockX, blockY)].hCost);
    }
}