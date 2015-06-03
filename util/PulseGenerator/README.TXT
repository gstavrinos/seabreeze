[README.TXT]

It turns out that 8-channel function generators are kind of expensive when you 
need to do some testing.  Happily, you can turn most Ocean Optics spectrometers
into simple square-edge TTL pulse generators with just a bit of programming the 
DIO (Digital I/O) / GPIO (General-Purpose I/O) pins.

The provided sample implementation is NOT particularly deterministic, and lacks
numerous nice-to-have features, but gives a quick idea of what can be done (and
solves a real problem in my test lab this evening).  Some obvious points to
improve:

- in addition to "initial offset", add "post-pulse delay" to each channel so that
  each channel can loop at its own rate

- provide real-time GUI update of all 8 pulse levels, like an oscilloscope

- better handling of FormClosing and other events

- precision event-based timing

- ability to save / load function complex function definitions for repeatable
  regression testing

- automatically infer the supported number of GPIO pins for a given spectrometer
  (really requires a new SeaBreeze function, getNumberOfGPIOPins())

- support parallel pulses (current implementation fires them sequentially, not
  overlapping)

- ...etc
