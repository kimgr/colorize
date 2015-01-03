Colorize
========

[Kim Gräsman](http://grundlig.wordpress.com)
[@kimgr](http://twitter.com/kimgr)

A naive console utility for colorizing svn diff output.

Usage:

    $ svn diff | colorize
    $ colorize 123.patch

Building requires CMake:

    $ cd .../colorize
    $ mkdir build && cd build
    $ cmake ..

Tested on Windows and FreeBSD 10.1.

See also:

 * [colordiff](http://colordiff.sourceforge.net/) -- I didn't want to depend on Perl.
 * [colorsvn](http://colorsvn.tigris.org/) -- I'm on Windows.
 * [Piping through vim](http://www.commandlinefu.com/commands/view/2420/colored-svn-diff) -- I don't have Vim installed.

License
-------
Source code and resulting binaries released under the MIT license, a.k.a use it as you see fit.

Copyright (C) 2012 Kim Gräsman.

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.