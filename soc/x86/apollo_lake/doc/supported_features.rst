Supported Features
==================

In addition to the standard architecture devices (HPET, local and I/O APICs,
etc.), Zephyr supports the following Apollo Lake-specific SoC devices:

* HSUART

* GPIO

* I2C

HSUART High-Speed Serial Port Support
-------------------------------------

The Apollo Lake UARTs are NS16550-compatible, with "high-speed" capability.

Baud rates beyond 115.2kbps (up to 3.6864Mbps) are supported, with additional
configuration. The UARTs are fed a master clock which is fed into a PLL which
in turn outputs the baud master clock. The PLL is controlled by a per-UART
32-bit register called ``PRV_CLOCK_PARAMS`` (aka the ``PCP``), the format of
which is:

+--------+---------+--------+--------+
| [31]   | [30:16] | [15:1] | [0]    |
+========+=========+========+========+
| enable | ``m``   | ``n``  | toggle |
+--------+---------+--------+--------+

The resulting baud master clock frequency is ``(n/m)`` * master.

Typically, the master clock is 100MHz, and the firmware by default sets
the ``PCP`` to ``0x3d090240``, i.e., ``n = 288``, ``m =  15625``, which
results in the de-facto standard 1.8432MHz master clock and a max baud rate
of 115.2k.  Higher baud rates are enabled by changing the PCP and telling
Zephyr what the resulting master clock is.

Use devicetree to set the value of the ``PRV_CLOCK_PARAMS`` register in
the UART block of interest. Typically a devicetree overlay file would be
present in the application directory (specific to the board, such as
``up_squared.overlay`` or ``gpmrb.overlay``), with contents like this:

   .. code-block:: console

    / {
        soc {
            uart@0 {
                pcp = <0x3d090900>;
                clock-frequency = <7372800>;
                current-speed = <230400>;
            };
        };
    };

The relevant variables are ``pcp`` (the value to use for ``PRV_CLOCK_PARAMS``),
and ``clock-frequency`` (the resulting baud master clock). The meaning of
``current-speed`` is unchanged, and as usual indicates the initial baud rate.
