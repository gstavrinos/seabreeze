[README.TXT]

== OVERVIEW ==

It turns out that 8-channel function generators are kind of expensive when you 
need to do some testing.  Happily, you can turn most Ocean Optics spectrometers
into simple square-edge TTL pulse generators with just a bit of programming the 
DIO (Digital I/O) / GPIO (General-Purpose I/O) pins.

The provided sample implementation is NOT particularly deterministic, and lacks
numerous nice-to-have features, but gives a quick idea of what can be done (and
solved an actual problem in my test lab this evening).  

== TODO ==

- precision event-based timing instead of Thread.Sleep() (use wakeup timers)

- automatically infer the supported number of GPIO pins for a given spectrometer
  (really requires a new SeaBreeze function, getNumberOfGPIOPins())

- better handling of FormClosing and other events

- ability to save / load function complex function definitions for repeatable
  regression testing

- ...etc

Note that this becomes way more convenient when coupled with an HR4-BREAKOUT:

    http://oceanoptics.com/product/hr4-breakout-breakout-box/

== HISTORY == 

1.0.4 - added explicit "Single Threaded" mode, for more-deterministic execution
        of non-overlapping use cases
1.0.3 - made multi-threaded (each pulse running in its own thread), for simple
        support for overlapping pulses
