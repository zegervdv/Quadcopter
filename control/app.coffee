winston = require 'winston'
express = require 'express'
app = express()

winston.level = process.env.COPTER_DEBUG || 'info'
winston.add winston.transports.File, { filename: process.env.COPTER_LOG || 'copter.log' }
winston.handleExceptions [new winston.transports.Console, new winston.transports.File { filename: 'copter.log' }]
winston.cli()

SerialPort = require 'serialport'

port = new SerialPort process.env.COPTER_PORT || '/dev/ttyS0', { baudRate: 115200 }

port.on 'error', (err) ->
  winston.error "SerialPort error: #{err.message}"
  winston.debug err.stack

port.on 'data', (data) ->
  winston.debug "RX data byte: #{data}"

app.listen 3000, () ->
  winston.info "Copter control server is listening on :3000"
