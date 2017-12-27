# Copter controller class
# Encodes and decodes packets to copter

SYNC = String.fromCharCode 85

class Copter
  constructor: (@logger) ->
    @logger.debug "Initialised copter control"
    @buffer = []

  store: (data) ->
    @buffer.push data
    if @buffer.length is 22
      out = @buffer
      @clear()
      @logger.info "Received full packet"
      @logger.debug "Packet data: #{@buffer}"
      try
        return @decode out
      catch err
        @logger.error err
        return null  
    else
      return null

  clear: () ->
    @logger.debug "Cleared receive buffer"
    @buffer = []

  parse_floats: (arr, start) ->
    buffer = new ArrayBuffer 4
    intView = new Int32Array buffer
    floatView = new Float32Array buffer

    bytes = arr.slice start, start + 4
    value = 0
    bytes.forEach (el, idx) ->
      value += (2**idx) * el.charCodeAt(0)
    intView[0] = value
    return floatView[0] 

  decode: (data) ->
    throw new Error('Invalid sync header') if data[0] isnt SYNC and data[1] isnt SYNC

    decode_data =
      roll: 0.0
      pitch: 0.0
      yaw: 0.0
      altitude: 0.0
      battery: 0.0

    decode_data.roll = @parse_floats data, 2
    decode_data.pitch = @parse_floats data, 6
    decode_data.yaw = @parse_floats data, 10
    decode_data.altitude = @parse_floats data, 14
    decode_data.battery = @parse_floats data, 18

    @logger.debug "Decoded packet to #{decode_data}"
    return decode_data
    

module.exports = (logger) ->
  return new Copter(logger)
