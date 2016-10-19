//import weather-js
var weather = require('weather-js');

//The child process lib allows us to execute command line options
var exec = require('child_process').exec;

//This variable stores the command we want to execute
var say = 'say ';

//create new J5 board and LCD
var five = require("johnny-five"),
  board, lcd;

//create a new five board
board = new five.Board();

//when the board is ready
board.on("ready", function() {

  //configure the j5 LCD
  lcd = new five.LCD({
    // LCD pin name  RS  EN  DB4 DB5 DB6 DB7
    // Arduino pin # 12  11   5   4   3   2
    pins: [12, 11, 5, 4, 3, 2],
    backlight: 6,
    rows: 2,
    cols: 16
  });

//Function to get weather and display on LCD
function getWeather(){
  lcd.clear;
  
  //make a call to the MSN weather API fand get waether for SF
  weather.find({search: 'San Francisco, CA', degreeType: 'F'}, function(err, result) {
  //catch any errors
  if(err) console.log(err);
 
  //store the results 
  var location = result[0].location.name;
  var temp = "is " + result[0].current.temperature + " Fahrenheit";
  
  //log the results
  console.log(location);
  console.log(temp);

  //Speak the results and print them on the LCD
  exec(say + "Current Temperature in San Francisco " + temp);
  lcd.cursor(0, 0).print(location);
  lcd.cursor(1, 0).print(temp);
});
}

//call getWeather initially when we start the program
getWeather();

//call function every 30 seconds using the setInterval function.
setInterval(function() { getWeather(); }, 30000);
});
