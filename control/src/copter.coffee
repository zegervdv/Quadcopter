# Copter controller class
# Encodes and decodes packets to copter

SYNC = 85

class Copter
  constructor: (@logger) ->
    @logger.debug "Initialised copter control"
    @buffer = []

  store: (data) ->
    Array::push.apply @buffer, data
    @logger.debug @buffer.length
    if @buffer.length >= 22
      out = @buffer
      @clear()
      @logger.info "Received full packet" 
      @logger.debug "Packet data: #{out}"
      try
        return @decode out
      catch err
        @logger.error err.message
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
      value += (256**idx) * el
    intView[0] = value
    return floatView[0] 

  decode: (data) ->
    throw new Error("Invalid sync header: #{data[0]}, #{data[1]}") if data[0] isnt SYNC and data[1] isnt SYNC

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

    @logger.info "Decoded packet to #{JSON.stringify decode_data}"
    return decode_data
    

module.exports = (logger) ->
  return new Copter(logger)
