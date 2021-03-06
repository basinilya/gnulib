#! /bin/sh
# Test suite for argp.
# Copyright (C) 2006-2007 Free Software Foundation, Inc.
# This file is part of the GNUlib Library.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

TMP=argp.$$

unset ARGP_HELP_FMT
ERR=0

func_compare() {
# If argp was compiled without base_name, it will display full program name.
# If run on mingw, it will display the program name with a .exe suffix.
  sed '1{
         s,: [^ ]*/test-argp,: test-argp,
         s,: test-argp\.exe,: test-argp,
        }' | diff -c $TMP -
}  

####
# Test --usage output
cat > $TMP <<EOT
Usage: test-argp [-tvCSOlp?V] [-f FILE] [-o[ARG]] [--test] [--file=FILE]
            [--input=FILE] [--verbose] [--cantiga] [--sonet] [--option]
            [--optional[=ARG]] [--limerick] [--poem] [--help] [--usage]
            [--version] ARGS...
EOT

./test-argp --usage | func_compare || ERR=1

####
# Test working usage-indent format

cat > $TMP <<EOT
Usage: test-argp [-tvCSOlp?V] [-f FILE] [-o[ARG]] [--test] [--file=FILE]
[--input=FILE] [--verbose] [--cantiga] [--sonet] [--option] [--optional[=ARG]]
[--limerick] [--poem] [--help] [--usage] [--version] ARGS...
EOT

ARGP_HELP_FMT='usage-indent=0' ./test-argp --usage | func_compare || ERR=1

####
# Test --help output
cat >$TMP <<EOT
Usage: test-argp [OPTION...] ARGS...
documentation string

 Main options
  -t, --test

 Option Group 1
  -f, --file=FILE, --input=FILE   Option with a mandatory argument
  -v, --verbose              Simple option without arguments

 Option Group 1.1
  -C, --cantiga              create a cantiga
  -S, --sonet                create a sonet

 Option Group 2
  -O, --option               An option

  -o, --optional[=ARG]       Option with an optional argument. ARG is one of
                             the following:

  many                       many units
  one                        one unit
  two                        two units

 Option Group 2.1
  -l, --limerick             create a limerick
  -p, --poem                 create a poem

  -?, --help                 give this help list
      --usage                give a short usage message
  -V, --version              print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

Report bugs to <>.
EOT

./test-argp --help | func_compare || ERR=1

####
# Test ambiguous option handling

./test-argp --optio 2>/dev/null && ERR=1

####
# Run built-in tests
./test-argp || ERR=1

rm $TMP

exit $ERR
