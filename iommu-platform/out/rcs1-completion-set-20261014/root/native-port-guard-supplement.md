# Pre-execution native-port / guard precision

Original TL has one IOTLB probe and one RR slow/fast response port, with stable held ownership on a response collision. Nonblocking hit entry is limited to another bank while a normal S1 PTW is active (not new probes during choice/result pending), valid DC / unprivileged / U=1 / S1 Sv39 /4KiB /nonglobal /RW /noATS or debug. The finite model abstracts native DC/LEAF/OUT cycles, so its one hit tick is conditional rather than native cycle-accurate.

Original read_admit gates new TC read selection, not already owned R service. It retains the existing fullbuffer_job plus matchingFullBufferDID exception; all these aligned64B copies have real512beat capacity and need only8readbeats. Cache hits never generate that certificate. Input AW order uses original MaxTrans*2 ledger; do not cap all banks to one pending AW. W follows inputAW order and releases owner on accepted/enqueued WLAST, not on DDRlastW/B. PhysicalAW admission still needs privatepermission; sourcejob retires only after its ownB and consumer/owner receipt.

These are source-driven clarifications before any four-condition execution, not new resources or performance tuning. The model author applies them to all fixed rules.
