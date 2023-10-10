///
/// \file klib.h
/// \author kyle 
/// \created 2023-10-10
/// \brief klib is my collection of C++ data structures and code.
///
/// \section COPYRIGHT
/// Copyright 2023 K. Isom <kyle@imap.cc>
///
/// Permission to use, copy, modify, and/or distribute this software for
/// any purpose with or without fee is hereby granted, provided that the
/// above copyright notice and this permission notice appear in all copies.
///
/// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
/// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
/// BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
/// OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
/// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
/// ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
/// SOFTWARE.
///

#ifndef KLIB_KLIB_H
#define KLIB_KLIB_H


#include <klib/Arena.h>
#include <klib/Buffer.h>
#include <klib/Dictionary.h>
#include <klib/Exceptions.h>
#include <klib/TLV.h>
#include <klib/Test.h>


/// klib is the top-level namespace containing all the code in this library.
namespace klib {

/// \mainpage klib documentation
///
/// \section Introduction
///
/// This is a collection of data structures and subroutines that I find
/// useful in building things.
///
/// This library arose from two main use cases.
///
/// \subsection The modem
///
/// On the one hand, I was building a wireless modem for some Z80 computers I
/// have. I needed to be able to store a phonebook of SSIDs and WPA keys, as
/// well as short names to host:port descriptors. I had a limited amount of of
/// persistent NVRAM storage and no SD card or other removeable media, so
/// typical desktop-oriented serialization mechanisms weren't going to really
/// work well. Furthermore, when working with microcontrollers, I prefer not to
/// dynamically allocate memory as much as possible. This led to building out
/// Arena, TLV::Record to store the records, and finally Dictionary to make use
/// of both of them.
///
///


}


#endif // KLIB_KLIB_H
