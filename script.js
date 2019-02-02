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
/*
// tricky map 1
updateBlock(2, 11, "start");
updateBlock(17, 8, "end");

updateBlock(5, 8, "closed");
updateBlock(5, 7, "closed");
updateBlock(5, 6, "closed");
updateBlock(5, 5, "closed");

updateBlock(11, 8, "closed");
updateBlock(11, 9, "closed");
updateBlock(11, 10, "closed");
updateBlock(11, 11, "closed");

updateBlock(15, 8, "closed");
updateBlock(15, 7, "closed");
updateBlock(15, 6, "closed");
updateBlock(15, 5, "closed");*/

// tricky map 2
updateBlock(4, 10, "start");
updateBlock(16, 5, "end");

updateBlock(5, 10, "closed");
updateBlock(5, 9, "closed");
updateBlock(4, 8, "closed");
updateBlock(3, 8, "closed");
updateBlock(2, 9, "closed");
updateBlock(2, 10, "closed");
updateBlock(2, 11, "closed");
updateBlock(2, 12, "closed");

updateBlock(14, 4, "closed");
updateBlock(14, 5, "closed");
updateBlock(15, 6, "closed");
updateBlock(16, 7, "closed");

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
        this.fCosts = [];
        this.fCostsLowest = 100;
    }
    
    draw() {
        this.center().erase();
        this.center().draw();
        draw.char(co(this.blockX), co(this.blockY));
        if (this.blockX === endCo[0] && this.blockY === endCo[1]) {
            console.log("at end!");
            for (let i = 1; i < (this.center().path.length - 2); i+= 2) {
                let tempX = i - 1;
                let tempY = i;
                updateBlock(this.center().path[tempX], this.center().path[tempY], "shortest");
            }
        }
    }
    
    /*
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
    */
    
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
    
    addToOpen(direct) {
        if (this.fCosts.length === 0) {
            this.fCosts[0] = this.around(direct).hCost;
            this.openPaths[0] = this.around(direct).blockX;
            this.openPaths[1] = this.around(direct).blockY;
        }
        else {
            // Find index for fCost
            let desiredCost = this.around(direct).hCost;
            for (let i = 0; i < this.fCosts.length; i++) {
                if (this.fCosts[i] >= desiredCost) {
                    this.fCosts.splice(i, 0, desiredCost);
                    this.openPaths.splice((i*2), 0, this.around(direct).blockX);
                    this.openPaths.splice((i*2)+1, 0, this.around(direct).blockY);
                    break;
                }
                if (i === (this.fCosts.length - 1)) {
                    this.fCosts.splice(i, 0, desiredCost);
                    this.openPaths.splice((i*2), 0, this.around(direct).blockX);
                    this.openPaths.splice((i*2)+1, 0, this.around(direct).blockY);
                    break;
                }
            }
        }
        //console.log("paths "+this.openPaths);
        this.fCostsLowest = this.fCosts[0];
    }
    
    // Adds open blocks to openPaths array before moving
    lightCheck() {
        for (let i = 0; i < 4; i++) {
            if (this.isMoveable(i) && this.around(i).style !== "checked") {
                    this.around(i).getFCost();
                    if (this.around(i).style !== "end") {
                        updateBlock(this.blockX+near[0][i], this.blockY+near[1][i], "checked");
                    }
                    this.addToOpen(i);
            }
        }
    }
    
    // Moves to the lowest block in the openPaths array
    checkAndMoveSimplified(){
        if (!(this.blockX === endCo[0] && this.blockY === endCo[1])) {
            // Mark blocks before moving
            if (this.center().style !== "start") {
                updateBlock(this.blockX, this.blockY, "visited");
            }
            this.lightCheck();
            this.center().draw();
            // Change checker xy blocks
            this.blockX = this.openPaths[0];
            this.blockY = this.openPaths[1];
            //console.log(this.blockX, this.blockY);
            // Remove block from open path arrays
            this.fCosts.splice(0, 1);
            this.openPaths.splice(0, 2);
            this.fCostsLowest = this.fCosts[0];
            // Move
            if (this.center().style !== "end") {
                updateBlock(this.blockX, this.blockY, "visited");
            }
            this.draw();
        }
    }
    
    /*
    // Checks four blocks around, sets block values, picks one to move forward
    // If a dead end is found, robot resumes search from the array of open ends
    checkAround() {
        // If fCost is getting too high, pick a diff open path
        if (this.center().fCost > this.fCostsLowest) {
            // Mark blocks before moving
            updateBlock(this.blockX, this.blockY, "visited");
            //this.lightCheck();
            this.center().draw();
            // Change checker xy blocks
            this.blockX = this.openPaths[0];
            this.blockY = this.openPaths[1];
            //console.log(this.blockX, this.blockY);
            // Remove block from open paths
            this.fCosts.splice(0, 1);
            this.openPaths.splice(0, 2);
            this.fCostsLowest = this.fCosts[0];
            // Move
            this.draw();
            // is not marking new checked tile as visited, revisits
        }
        
        // if not at the end, check around and move the checker on block
        if (!(this.blockX === endCo[0] && this.blockY === endCo[1])) {
            // Uses an array to find & remember the lowest f-cost
            let fCosts = [];
            let lowest = (this.center().getFCost() * 3);    // unobtainable
            
            // First find the lowest fCost
            for (let i = 0; i < 4; i++) {
                if (this.isMoveable(i)) {
                    //this.around(i).getFCost();
                    fCosts[i] = this.around(i).getFCost();
                    if (fCosts[i] < lowest) {
                        lowest = fCosts[i];
                    }
                }
            }
            
            // Will not change anything if no lowest fCost (dead end was found)
            if (lowest === -1) {
                // Find new block from open array, change path
                console.log("dead end!");
            }
            else {
                // Direction is where the robot will move next
                let direction = [];
                
                // Finds direction in here
                for (let i = 0; i < 4; i++) {
                    if (this.isMoveable(i)) {
                        if (fCosts[i] === lowest) {
                            if (this.around(i).style !== "end") {
                                updateBlock(this.blockX+near[0][i], this.blockY+near[1][i], "visited");
                            }
                            direction.push(i);
                        }
                        else {
                            updateBlock(this.blockX+near[0][i], this.blockY+near[1][i], "checked");
                            this.addToOpen(i);
                        }
                    }
                }
                
                // Determine the lower HCost between the two directions if needed
                if (direction.length > 1) {
                    if (this.around(direction[0]).hCost < this.around(direction[1]).hCost) {
                        // Mark second block as checked
                        updateBlock(this.blockX+near[0][direction[1]], this.blockY+near[1][direction[1]], "checked");
                        this.addToOpen(direction[1]);
                    }
                    else {
                        // Mark first block as checked
                        updateBlock(this.blockX+near[0][direction[0]], this.blockY+near[1][direction[0]], "checked");
                        this.addToOpen(direction[0]);
                        // Mark direction as second block
                        direction[0] = direction[1];
                    }
                }
                return (direction[0]);
            }
        }
    }
    
    // Move direction seperate from check
    move(direct) {
        if (direct === -1) {
            // Pick from openPaths
            console.log("dead end need new path");
        }
        else {
            // Erase block and move checker
            this.center().draw();
            switch (direct) {
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
            //console.log("bot "+this.blockX, this.blockY);
        }
    }
    */
}
// Declared before later use
var checker;

// Will remove this
document.onkeydown = function (e) {
    switch (e.key) {
        case 'c':
            checker.checkAndMoveSimplified();
            //checker.checkAround();
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