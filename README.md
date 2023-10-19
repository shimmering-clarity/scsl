# scsl : The Shimmering Clarity Standard C++ Library

[![CircleCI](https://dl.circleci.com/status-badge/img/gh/shimmering-clarity/emsha/tree/master.svg?style=svg)](https://dl.circleci.com/status-badge/redirect/gh/shimmering-clarity/emsha/tree/master)

[![image](https://scan.coverity.com/projects/29251/badge.svg)](https://scan.coverity.com/projects/shimmering-clarity-scsl)

scsl is a collection of software I found myself needing to use repeatedly.

Full [Doxygen documentation](https://docs.shimmering-clarity.net/scsl/)
is available.

## Introduction

This is a collection of C++ code that I find useful in building things. 
It arose from two main use cases.

 ### The modem

On the one hand, I was building a wireless modem for some Z80 computers I
have. I needed to be able to store a phonebook of SSIDs and WPA keys, as
well as short names to host:port descriptors. I had a limited amount of
persistent NVRAM storage and no SD card or other removeable media, so
typical desktop-oriented serialization mechanisms weren't going to really
work well. Furthermore, when working with microcontrollers, I prefer not to
dynamically allocate memory as much as possible. This led to building out
Arena, TLV::Record to store the records, and finally Dictionary to make use
of both of them.

Closely related to this, I've been working on building an ARM-based handheld
computer, for which I would also need a memory arena.

### The text editors

Some time ago, I wrote a console text editor of my own; then later, started
working on a graphical editor. For this, I needed some data structures to
manage memory in the editor. Thus, Buffer was born.

### Finally

I'd been writing Go professionally for a while, but C was my first love. I
recently started a job that is mostly in C++, and the best way for me to
learn is to build a bunch of stuff with it. So, I took a bunch of micro-
controller stuff I'd been writing and started building out some other stuff.


## License

Copyright 2023 K. Isom <kyle@imap.cc>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

