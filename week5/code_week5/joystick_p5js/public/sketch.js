var x, y, b;
var socket = io.connect(window.location.origin);

socket.on('mysocket', function(data) {
    console.log(data);
    incoming = data.split(',');
    x = map(incoming[0], 0, 1023, 0, width);
    y = map(incoming[1], 0, 1023, 0, height);
    b = incoming[2];
    // console.log(incoming);
});

function setup() {
    frameRate(60);
    createCanvas(1024, 768);
    // Starts in the middle
    x = width / 2;
    y = height / 2;

}

function draw() {
    background(150 * b);
    textSize(32);
    fill(50);
    text(x, 0, 32);
    text(y, 0, 64);

    // Draw a circle
    stroke(50);
    // fill(x, x, x);
    // background(y, y, y);
    ellipse(x, y, 50, 50);
    // Jiggling randomly on the horizontal axis
    x = x + random(-1, 1);
    // Moving up at a constant speed
    y = y - 1;
    // Reset to the bottom
    if (y < 0) {
        y = height;
    }
}
