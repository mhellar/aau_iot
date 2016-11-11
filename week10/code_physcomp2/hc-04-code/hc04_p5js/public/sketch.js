
var r;
var playing = false;
var fingers;
var button;

var socket = io.connect(window.location.origin);

    socket.on('data', function(data) {
        
        r = data.val;
        console.log(r);
    });


function setup() {
  // specify multiple formats for different browsers
  fingers = createVideo(['assets/fingers.mov',
                         'assets/fingers.webm']);

}

// plays or pauses the video depending on current state
function draw() {
  if (r < 5) {
    fingers.loop();
  } else {  
  fingers.pause();
  }
  playing = !playing;
}