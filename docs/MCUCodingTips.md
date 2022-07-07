# MCU Coding Tips

Please factor in the following information when writing your MCU code:

- [Don’t Forget to Call `af_lib_loop()`](../MCUCodingTips#dont-forget-to-call-af_lib_loop)
- [Instead of delay(), use a function that calls `af_lib_loop()`](../MCUCodingTips#instead-of-delay-use-a-function-that-calls-af_lib_loop)
- [Adjust the Request Queue Size If Necessary](../MCUCodingTips#adjust-the-request-queue-size-if-necessary)
- [Watch Your Memory Usage](../MCUCodingTips#watch-your-memory-usage)

## Don’t Forget to Call af_lib_loop()

This is very basic, but critical: when you write MCU code using afLib, you **must** call `af_lib_loop()` to give afLib’s state machine time to execute. Without this, afLib won’t run, and your code will not interact properly with the Afero Platform components.

You should definitely make a call to `af_lib_loop()` in your code’s `loop()` method; and you can call it elsewhere, as well, if you need to ensure that afLib is executing frequently enough to support your application.

```
void loop() {
  // ALWAYS give the afLib state machine time to do its work - avoid blind delay() calls
  // of any length; instead, repeat calls to af_lib_loop() while waiting for whatever it is
  // you're waiting for to happen. Some afLib operations take multiple loop() calls to complete.
  af_lib_loop(af_lib);

  // Do your application processing here.
}
```

**Notes:**

- afLib requires *time slices* to process attribute writes.
- `af_lib_loop()` performs small amounts of work every time it’s called. A multi-threaded process is emulated on a single-threaded MCU.
- Call `af_lib_loop()` as often as possible in your code. If afLib has little to do, it returns quickly. Some complex calls, like `setAttribute`, require multiple `loop()` calls to finish.
- **IMPORTANT!** Calling `af_lib_loop()` from within the `attrEventCallback` callback is NOT supported and will likely result in undesired behavior.
- When afLib is finished with a task, you’ll get an event via `attrEventCallback()`. Within that callback, you’ll want to listen for any eventTypes that you need. See [attrEventCallback()](../afLibCallbacks#attreventcallback) for information on handling this callback.
- Make sure afLib has time to complete its work. When doing multiple `af_lib_set_attribute*()` calls, ideally wait for `attrEventCallback` before doing additional writes.
- If you write faster than afLib can process, the queue may fill up, and the return code will be:
  `AF_ERROR_QUEUE_OVERFLOW  -4 // Queue is full`
- Always check the [return code](../afLibErrors) from afLib calls. Check for return of AF_SUCCESS.

## Instead of delay(), use a function that calls af_lib_loop()

As a corollary to the above, keep in mind that if you use `delay()` in your code, you will block your main loop, and thereby block your calls to `af_lib_loop()`. This is highly undesirable. Instead, you should write code that loops for the desired pause-time and calls `af_lib_loop()`. For example, for short-duration (~100s of msec) pauses, you might do something like:

```
// Delay while making sure afLib gets attention
void safeDelay(long msec) {
    long delayTarget = af_utils_millis() + msec;
    while (af_utils_millis() <= delayTarget) {
        af_lib_loop(af_lib);
    }
}
```

## Adjust the Request Queue Size If Necessary

You may notice that the [return code](../afLibErrors) from some afLib calls in your MCU code indicates a full request queue (`AF_ERROR_QUEUE_OVERFLOW = -4`). If you observe this, you should consider increasing the size of the request queue. This value is defined as the `REQUEST_QUEUE_SIZE` in `af_lib.h`. The default value is 10, but you can safely change that value within the bounds of available memory on your platform.

## Watch Your Memory Usage

If you’ve worked much with Arduino, this tip is probably not news to you: Memory is limited, and if your MCU code exceeds available memory, your output can be quite puzzling. You may see strange behavior from a sketch running on Uno only to find that the same code runs as expected on Teensy. This is particularly useful to remember in the context of the [Useful Debugging Methods](../DebugMethods) provided elsewhere; use of those methods requires significant memory, and will be difficult using Uno.

### See Also

[Tutorial 3: Afero + Arduino](../Lesson3)
