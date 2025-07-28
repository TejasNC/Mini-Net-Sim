To Fix:

1) Potential memory issues with simulator context and packet when it is dropped

2) Each packet, and therefore the simulator context can have only one event in the event queue.

3) Clean up the sendPacketWithContext methods in interface and make sending packets with context default

4) Compute transmission delay and compute propogatino delay should be methods of link / interface. But we are using them as method of simulator context.

Simulator Context Ownership:

1) Node, Link and Interface do not won Simulator Context => For them, it should be weak_ptr

2) Should packet own context?

3) Eveents partially own SImulator Context, just like the own packet. Therefore for events, it should be
    a shared_ptr
