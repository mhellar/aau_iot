//Import Twitter and J5 modules
var Twitter = require('twitter')
var five = require("johnny-five");
var exec = require('child_process').exec;

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

//set up a new j5 board
var board = new five.Board();
var led;

//make a blink function
function blink() {
  led.pulse(40);

  board.wait(4000, function() {
    led.stop();
  });
}

var execProcess = null;
board.on("ready", function() {
  // Create assign a j5 led on pin 6 to led
  led = new five.Led(6);

  // Creates a realtime streaming connection to the Twitter
  // API, letting you "track" a particular keyword or hashtag
  // and recieve a notification instantly as soon as a tweet is posted.
  //
  // Documentation, including additional parameters you can use, may
  // be found here:
  // https://dev.twitter.com/streaming/reference/post/statuses/filter
  //
  // Note that you can also stream tweets from particular users, or
  // tweets posted from around a particular location

  client.stream('statuses/filter', {
    locations: '-122.75,36.8,-121.75,37.8'
  }, function(tweetStream) {


    // `tweetStream` will emit a "data" event whenever
    // a new tweet is posted. These will be in the same format
    // as seen in the first example.
    tweetStream.on('data', function(tweet) {
      console.log(tweet.text);
      blink(led);
      if (!execProcess) {
        execProcess = exec('say "' + tweet.text + '"', function(err, stdout, stderr) {
          console.log(stdout);
          execProcess = null;
        });
      }
    })
  })
});
