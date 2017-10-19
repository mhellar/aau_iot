//Import Twitter and J5 modules
var Twitter = require('twitter')
var five = require("johnny-five");

// Load secret settings from .env file
require('dotenv').config();

// Authenticates with Twitter using the (unofficial) twitter
// package on npm. This is required for using the interesting parts
// of the API, e.g. streaming or posting tweets.
var client = new Twitter({
  consumer_key: process.env.TWITTER_CONSUMER_KEY,
  consumer_secret: process.env.TWITTER_CONSUMER_SECRET,
  access_token_key: process.env.TWITTER_ACCESS_TOKEN_KEY,
  access_token_secret: process.env.TWITTER_ACCESS_TOKEN_SECRET
});

//nice date format function
function getDate() {
  var date = new Date();
  var month = date.getMonth() + 1;
  var day = date.getDate();
  var hour = date.getHours();
  var min = date.getMinutes();
  var sec = date.getSeconds();

  month = (month < 10 ? "0" : "") + month;
  day = (day < 10 ? "0" : "") + day;
  hour = (hour < 10 ? "0" : "") + hour;
  min = (min < 10 ? "0" : "") + min;
  sec = (sec < 10 ? "0" : "") + sec;

  var str = date.getFullYear() + "-" + month + "-" + day + "_" + hour + ":" + min + ":" + sec;

  return str;
}

//set up a new j5 board
var board = new five.Board();


board.on("ready", function() {
  var button = new five.Button({
    pin: 2,
  });

  button.on("press", function() {
    var time = Date.now();
    client.post('statuses/update', {
      status: 'Hello Internet! ' + getDate()
    }, function(err, tweets) {
      if (err) throw new Error(err[0].message)
      console.log('Tweeted successfully!')
    });
  });
});
