var SerialPort = require("serialport");

var serialPort = new SerialPort("/dev/cu.usbmodem14111", {
    baudrate: 9600,
    parser: SerialPort.parsers.readline('\r\n')
});

serialPort.on('open', function() {
    console.log('Port open');
});

serialPort.on('data', function(data) {
    console.log(data);
});
