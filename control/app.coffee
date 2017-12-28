winston = require 'winston'
express = require 'express'
app = express()
http = require('http')(app)
io = require('socket.io')(http)

# Logger configuration
winston.level = process.env.COPTER_DEBUG || 'info'
winston.add winston.transports.File, { filename: process.env.COPTER_LOG || 'copter.log' }
winston.handleExceptions [new winston.transports.Console, new winston.transports.File { filename: 'copter.log' }]
winston.cli()

copter = require('./src/copter')(winston)

SerialPort = require 'serialport'

port = new SerialPort process.env.COPTER_PORT || '/dev/ttyAMA0', { baudRate: 115200 }

port.on 'error', (err) ->
  winston.error "SerialPort error: #{err.message}"
  winston.debug err.stack

port.on 'readable', () ->
  data = port.read()
  winston.debug "Readable data: #{data}"
  done = copter.store data
  if done
    winston.debug "Full packet decoded"

io.on 'connection', (socket) ->
   winston.info 'Connected to frontend interface'

app.get '/', (req, res) ->
  res.sendFile __dirname + '/views/index.html'

app.listen 3000, () ->
  winston.info "Copter control server is listening on :3000"
