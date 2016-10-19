//the child_process library let's us execute command line commands, https://nodejs.org/api/child_process.html
var exec = require('child_process').exec;
//This variable stores the command we want to execute, we are going to use the say command
var say = 'say ';

//create a bart oobject that queries the API every 5 seconds
var bart = require('bart').createClient({"interval":20000});

//create new johnny-five object and board
var five = require("johnny-five"),
  board, lcd;
board = new five.Board();

//When the board is ready
board.on("ready", function() {
//create a new LCD object
  lcd = new five.LCD({
    // LCD pin name  RS  EN  DB4 DB5 DB6 DB7
    // Arduino pin # 12  11   5   4   3   2
    pins: [12, 11, 5, 4, 3, 2],
    backlight: 6,
    rows: 2,
    cols: 16
    // Options:
    // bitMode: 4 or 8, defaults to 4
    // lines: number of lines, defaults to 2
    // dots: matrix dimensions, defaults to "5x8"
  });

//let's make a function that speaks
function speak(whatosay){
  //speak the string
  exec(say + whatosay);
  //log it to the console
  console.log(whatosay)
}


function queryBart(){
//choose which bart staion to to monitor, station abbreviations are here: http://api.bart.gov/docs/overview/abbrev.aspx
bart.on('powl', function(estimates){
   //log the results to the console
   console.log(estimates); 
   // store the results in some variables
   var nextTrainSouth = "next train in " + estimates[0].minutes;
   var destSouth = "Dest: " + estimates[0].destination;

   var nextTrainNorth = "next train in " + estimates[5].minutes;
   var destNorth = "Dest: " + estimates[5].destination;

   // print results to the lcd
   lcd.cursor(0, 0).clear().print(nextTrainSouth + "M");
   lcd.cursor(1, 0).print("Dest: " + estimates[0].destination);
   // call the speak function
   speak(nextTrainSouth + " minutes" + " destination is " + estimates[0].destination + " Direction is " + estimates[0].direction);
   
   //wait 5 seconds and then do the same with southbound data
   setTimeout(function() {
   lcd.cursor(0, 0).clear().print(nextTrainNorth + "M");
   lcd.cursor(1, 0).print("Dest: " + estimates[5].destination);
   // call the function
   speak(nextTrainNorth + " minutes" + " destination is " + estimates[5].destination + " Direction is " + estimates[5].direction);
   }, 5000);
})
}

queryBart();

});