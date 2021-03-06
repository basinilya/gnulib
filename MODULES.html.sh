#!/bin/sh
#
# Copyright (C) 2002-2007 Free Software Foundation, Inc.
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
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Usage: MODULES.html.sh [--cvs-urls|--git-urls] > MODULES.html

# Extend the PATH so that gnulib-tool is found.
PATH=`dirname "$0"`:$PATH; export PATH

POSIX2001_URL='http://www.opengroup.org/susv3'
case $USER in
  bruno )
    POSIX2001_URL='file:/packages/www/www.opengroup.org/susv3' ;;
esac

repo_url_prefix=
repo_url_suffix=
if test $# != 0; then
  case "$1" in
    --cvs-urls)
      # Generate URLs to the official gnulib CVS repository.
      repo_url_prefix='http://cvs.sv.gnu.org/viewcvs/*checkout*/gnulib/'
      repo_url_suffix='?root=gnulib&content-type=text/plain'
      ;;
    --git-urls)
      # Generate URLs to the official gnulib git repository.
      repo_url_prefix='http://git.sv.gnu.org/gitweb/?p=gnulib.git;a=blob_plain;f='
      repo_url_suffix=''
      ;;
  esac
fi
# For sed replacements: Escape the '&'.
repo_url_suffix_repl=`echo "$repo_url_suffix" | sed -e 's,[&],\\\&,'`

sed_lt='s,<,\&lt;,g'
sed_gt='s,>,\&gt;,g'
sed_escape_dot='s,\.,\\.,g'
sed_escape_slash='s,/,\\/,g'
trnl='\012'
sed_alt1='s,$,\\|,'
sed_alt2='s,^\\|,\\(,'
sed_alt3='s,\\|\\|$,\\),'
posix_headers=`echo '
aio
arpa/inet
assert
complex
cpio
ctype
dirent
dlfcn
errno
fcntl
fenv
float
fmtmsg
fnmatch
ftw
glob
grp
iconv
inttypes
iso646
langinfo
libgen
limits
locale
math
monetary
mqueue
ndbm
net/if
netdb
netinet/in
netinet/tcp
nl_types
poll
pthread
pwd
regex
sched
search
semaphore
setjmp
signal
spawn
stdarg
stdbool
stddef
stdint
stdio
stdlib
string
strings
stropts
sys/ipc
sys/mman
sys/msg
sys/resource
sys/select
sys/sem
sys/shm
sys/socket
sys/stat
sys/statvfs
sys/time
sys/timeb
sys/times
sys/types
sys/uio
sys/un
sys/utsname
sys/wait
syslog
tar
termios
tgmath
time
trace
ucontext
ulimit
unistd
utime
utmpx
wchar
wctype
wordexp
' | sed -e "$sed_alt1" | tr -d "$trnl" | sed -e "$sed_alt2" -e "$sed_alt3"`
posix_functions=`echo '
FD_CLR
FD_ISSET
FD_SET
FD_ZERO
_Exit
_exit
_longjmp
_setjmp
_tolower
_toupper
a64l
abort
abs
accept
access
acos
acosf
acosh
acoshf
acoshl
acosl
aio_cancel
aio_error
aio_fsync
aio_read
aio_return
aio_suspend
aio_write
alarm
asctime
asctime_r
asin
asinf
asinh
asinhf
asinhl
asinl
assert
atan
atan2
atan2f
atan2l
atanf
atanh
atanhf
atanhl
atanl
atexit
atof
atoi
atol
atoll
basename
bcmp
bcopy
bind
bsd_signal
bsearch
btowc
bzero
cabs
cabsf
cabsl
cacos
cacosf
cacosh
cacoshf
cacoshl
cacosl
calloc
carg
cargf
cargl
casin
casinf
casinh
casinhf
casinhl
casinl
catan
catanf
catanh
catanhf
catanhl
catanl
catclose
catgets
catopen
cbrt
cbrtf
cbrtl
ccos
ccosf
ccosh
ccoshf
ccoshl
ccosl
ceil
ceilf
ceill
cexp
cexpf
cexpl
cfgetispeed
cfgetospeed
cfsetispeed
cfsetospeed
chdir
chmod
chown
cimag
cimagf
cimagl
clearerr
clock
clock_getcpuclockid
clock_getres
clock_gettime
clock_nanosleep
clock_settime
clog
clogf
clogl
close
closedir
closelog
confstr
conj
conjf
conjl
connect
copysign
copysignf
copysignl
cos
cosf
cosh
coshf
coshl
cosl
cpow
cpowf
cpowl
cproj
cprojf
cprojl
creal
crealf
creall
creat
crypt
csin
csinf
csinh
csinhf
csinhl
csinl
csqrt
csqrtf
csqrtl
ctan
ctanf
ctanh
ctanhf
ctanhl
ctanl
ctermid
ctime
ctime_r
daylight
dbm_clearerr
dbm_close
dbm_delete
dbm_error
dbm_fetch
dbm_firstkey
dbm_nextkey
dbm_open
dbm_store
difftime
dirname
div
dlclose
dlerror
dlopen
dlsym
drand48
dup
dup2
ecvt
encrypt
endgrent
endhostent
endnetent
endprotoent
endpwent
endservent
endutxent
environ
erand48
erf
erfc
erfcf
erfcl
erff
erfl
errno
execl
execle
execlp
execv
execve
execvp
exit
exp
exp2
exp2f
exp2l
expf
expl
expm1
expm1f
expm1l
fabs
fabsf
fabsl
fattach
fchdir
fchmod
fchown
fclose
fcntl
fcvt
fdatasync
fdetach
fdim
fdimf
fdiml
fdopen
feclearexcept
fegetenv
fegetexceptflag
fegetround
feholdexcept
feof
feraiseexcept
ferror
fesetenv
fesetexceptflag
fesetround
fetestexcept
feupdateenv
fflush
ffs
fgetc
fgetpos
fgets
fgetwc
fgetws
fileno
flockfile
floor
floorf
floorl
fma
fmaf
fmal
fmax
fmaxf
fmaxl
fmin
fminf
fminl
fmod
fmodf
fmodl
fmtmsg
fnmatch
fopen
fork
fpathconf
fpclassify
fprintf
fputc
fputs
fputwc
fputws
fread
free
freeaddrinfo
freopen
frexp
frexpf
frexpl
fscanf
fseek
fseeko
fsetpos
fstat
fstatvfs
fsync
ftell
ftello
ftime
ftok
ftruncate
ftrylockfile
ftw
funlockfile
fwide
fwprintf
fwrite
fwscanf
gai_strerror
gcvt
getaddrinfo
getc
getc_unlocked
getchar
getchar_unlocked
getcontext
getcwd
getdate
getegid
getenv
geteuid
getgid
getgrent
getgrgid
getgrgid_r
getgrnam
getgrnam_r
getgroups
gethostbyaddr
gethostbyname
gethostent
gethostid
gethostname
getitimer
getlogin
getlogin_r
getmsg
getnameinfo
getnetbyaddr
getnetbyname
getnetent
getopt
getpeername
getpgid
getpgrp
getpid
getpmsg
getppid
getpriority
getprotobyname
getprotobynumber
getprotoent
getpwent
getpwnam
getpwnam_r
getpwuid
getpwuid_r
getrlimit
getrusage
gets
getservbyname
getservbyport
getservent
getsid
getsockname
getsockopt
getsubopt
gettimeofday
getuid
getutxent
getutxid
getutxline
getwc
getwchar
getwd
glob
globfree
gmtime
gmtime_r
grantpt
h_errno
hcreate
hdestroy
hsearch
htonl
htons
hypot
hypotf
hypotl
iconv
iconv_close
iconv_open
if_freenameindex
if_indextoname
if_nameindex
if_nametoindex
ilogb
ilogbf
ilogbl
imaxabs
imaxdiv
index
inet_addr
inet_ntoa
inet_ntop
inet_pton
initstate
insque
ioctl
isalnum
isalpha
isascii
isastream
isatty
isblank
iscntrl
isdigit
isfinite
isgraph
isgreater
isgreaterequal
isinf
isless
islessequal
islessgreater
islower
isnan
isnormal
isprint
ispunct
isspace
isunordered
isupper
iswalnum
iswalpha
iswblank
iswcntrl
iswctype
iswdigit
iswgraph
iswlower
iswprint
iswpunct
iswspace
iswupper
iswxdigit
isxdigit
j0
j1
jn
jrand48
kill
killpg
l64a
labs
lchown
lcong48
ldexp
ldexpf
ldexpl
ldiv
lfind
lgamma
lgammaf
lgammal
link
lio_listio
listen
llabs
lldiv
llrint
llrintf
llrintl
llround
llroundf
llroundl
localeconv
localtime
localtime_r
lockf
log
log10
log10f
log10l
log1p
log1pf
log1pl
log2
log2f
log2l
logb
logbf
logbl
logf
logl
longjmp
lrand48
lrint
lrintf
lrintl
lround
lroundf
lroundl
lsearch
lseek
lstat
makecontext
malloc
mblen
mbrlen
mbrtowc
mbsinit
mbsrtowcs
mbstowcs
mbtowc
memccpy
memchr
memcmp
memcpy
memmove
memset
mkdir
mkfifo
mknod
mkstemp
mktemp
mktime
mlock
mlockall
mmap
modf
modff
modfl
mprotect
mq_close
mq_getattr
mq_notify
mq_open
mq_receive
mq_send
mq_setattr
mq_timedreceive
mq_timedsend
mq_unlink
mrand48
msgctl
msgget
msgrcv
msgsnd
msync
munlock
munlockall
munmap
nan
nanf
nanl
nanosleep
nearbyint
nearbyintf
nearbyintl
nextafter
nextafterf
nextafterl
nexttoward
nexttowardf
nexttowardl
nftw
nice
nl_langinfo
nrand48
ntohl
ntohs
open
opendir
openlog
optarg
pathconf
pause
pclose
perror
pipe
poll
popen
posix_fadvise
posix_fallocate
posix_madvise
posix_mem_offset
posix_memalign
posix_openpt
posix_spawn
posix_spawn_file_actions_addclose
posix_spawn_file_actions_adddup2
posix_spawn_file_actions_addopen
posix_spawn_file_actions_destroy
posix_spawn_file_actions_init
posix_spawnattr_destroy
posix_spawnattr_getflags
posix_spawnattr_getpgroup
posix_spawnattr_getschedparam
posix_spawnattr_getschedpolicy
posix_spawnattr_getsigdefault
posix_spawnattr_getsigmask
posix_spawnattr_init
posix_spawnattr_setflags
posix_spawnattr_setpgroup
posix_spawnattr_setschedparam
posix_spawnattr_setschedpolicy
posix_spawnattr_setsigdefault
posix_spawnattr_setsigmask
posix_spawnp
posix_trace_attr_destroy
posix_trace_attr_getclockres
posix_trace_attr_getcreatetime
posix_trace_attr_getgenversion
posix_trace_attr_getinherited
posix_trace_attr_getlogfullpolicy
posix_trace_attr_getlogsize
posix_trace_attr_getmaxdatasize
posix_trace_attr_getmaxsystemeventsize
posix_trace_attr_getmaxusereventsize
posix_trace_attr_getname
posix_trace_attr_getstreamfullpolicy
posix_trace_attr_getstreamsize
posix_trace_attr_init
posix_trace_attr_setinherited
posix_trace_attr_setlogfullpolicy
posix_trace_attr_setlogsize
posix_trace_attr_setmaxdatasize
posix_trace_attr_setname
posix_trace_attr_setstreamfullpolicy
posix_trace_attr_setstreamsize
posix_trace_clear
posix_trace_close
posix_trace_create
posix_trace_create_withlog
posix_trace_event
posix_trace_eventid_equal
posix_trace_eventid_get_name
posix_trace_eventid_open
posix_trace_eventset_add
posix_trace_eventset_del
posix_trace_eventset_empty
posix_trace_eventset_fill
posix_trace_eventset_ismember
posix_trace_eventtypelist_getnext_id
posix_trace_eventtypelist_rewind
posix_trace_flush
posix_trace_get_attr
posix_trace_get_filter
posix_trace_get_status
posix_trace_getnext_event
posix_trace_open
posix_trace_rewind
posix_trace_set_filter
posix_trace_shutdown
posix_trace_start
posix_trace_stop
posix_trace_timedgetnext_event
posix_trace_trid_eventid_open
posix_trace_trygetnext_event
posix_typed_mem_get_info
posix_typed_mem_open
pow
powf
powl
pread
printf
pselect
pthread_atfork
pthread_attr_destroy
pthread_attr_getdetachstate
pthread_attr_getguardsize
pthread_attr_getinheritsched
pthread_attr_getschedparam
pthread_attr_getschedpolicy
pthread_attr_getscope
pthread_attr_getstack
pthread_attr_getstackaddr
pthread_attr_getstacksize
pthread_attr_init
pthread_attr_setdetachstate
pthread_attr_setguardsize
pthread_attr_setinheritsched
pthread_attr_setschedparam
pthread_attr_setschedpolicy
pthread_attr_setscope
pthread_attr_setstack
pthread_attr_setstackaddr
pthread_attr_setstacksize
pthread_barrier_destroy
pthread_barrier_init
pthread_barrier_wait
pthread_barrierattr_destroy
pthread_barrierattr_getpshared
pthread_barrierattr_init
pthread_barrierattr_setpshared
pthread_cancel
pthread_cleanup_pop
pthread_cleanup_push
pthread_cond_broadcast
pthread_cond_destroy
pthread_cond_init
pthread_cond_signal
pthread_cond_timedwait
pthread_cond_wait
pthread_condattr_destroy
pthread_condattr_getclock
pthread_condattr_getpshared
pthread_condattr_init
pthread_condattr_setclock
pthread_condattr_setpshared
pthread_create
pthread_detach
pthread_equal
pthread_exit
pthread_getconcurrency
pthread_getcpuclockid
pthread_getschedparam
pthread_getspecific
pthread_join
pthread_key_create
pthread_key_delete
pthread_kill
pthread_mutex_destroy
pthread_mutex_getprioceiling
pthread_mutex_init
pthread_mutex_lock
pthread_mutex_setprioceiling
pthread_mutex_timedlock
pthread_mutex_trylock
pthread_mutex_unlock
pthread_mutexattr_destroy
pthread_mutexattr_getprioceiling
pthread_mutexattr_getprotocol
pthread_mutexattr_getpshared
pthread_mutexattr_gettype
pthread_mutexattr_init
pthread_mutexattr_setprioceiling
pthread_mutexattr_setprotocol
pthread_mutexattr_setpshared
pthread_mutexattr_settype
pthread_once
pthread_rwlock_destroy
pthread_rwlock_init
pthread_rwlock_rdlock
pthread_rwlock_timedrdlock
pthread_rwlock_timedwrlock
pthread_rwlock_tryrdlock
pthread_rwlock_trywrlock
pthread_rwlock_unlock
pthread_rwlock_wrlock
pthread_rwlockattr_destroy
pthread_rwlockattr_getpshared
pthread_rwlockattr_init
pthread_rwlockattr_setpshared
pthread_self
pthread_setcancelstate
pthread_setcanceltype
pthread_setconcurrency
pthread_setschedparam
pthread_setschedprio
pthread_setspecific
pthread_sigmask
pthread_spin_destroy
pthread_spin_init
pthread_spin_lock
pthread_spin_trylock
pthread_spin_unlock
pthread_testcancel
ptsname
putc
putc_unlocked
putchar
putchar_unlocked
putenv
putmsg
putpmsg
puts
pututxline
putwc
putwchar
pwrite
qsort
raise
rand
rand_r
random
read
readdir
readdir_r
readlink
readv
realloc
realpath
recv
recvfrom
recvmsg
regcomp
regerror
regexec
regfree
remainder
remainderf
remainderl
remove
remque
remquo
remquof
remquol
rename
rewind
rewinddir
rindex
rint
rintf
rintl
rmdir
round
roundf
roundl
scalb
scalbln
scalblnf
scalblnl
scalbn
scalbnf
scalbnl
scanf
sched_get_priority_max
sched_getparam
sched_getscheduler
sched_rr_get_interval
sched_setparam
sched_setscheduler
sched_yield
seed48
seekdir
select
sem_close
sem_destroy
sem_getvalue
sem_init
sem_open
sem_post
sem_timedwait
sem_trywait
sem_unlink
sem_wait
semctl
semget
semop
send
sendmsg
sendto
setbuf
setcontext
setegid
setenv
seteuid
setgid
setgrent
sethostent
setitimer
setjmp
setkey
setlocale
setlogmask
setnetent
setpgid
setpgrp
setpriority
setprotoent
setpwent
setregid
setreuid
setrlimit
setservent
setsid
setsockopt
setstate
setuid
setutxent
setvbuf
shm_open
shm_unlink
shmat
shmctl
shmdt
shmget
shutdown
sigaction
sigaddset
sigaltstack
sigdelset
sigemptyset
sigfillset
sighold
sigignore
siginterrupt
sigismember
siglongjmp
signal
signbit
sigpause
sigpending
sigprocmask
sigqueue
sigrelse
sigset
sigsetjmp
sigsuspend
sigtimedwait
sigwait
sigwaitinfo
sin
sinf
sinh
sinhf
sinhl
sinl
sleep
snprintf
sockatmark
socket
socketpair
sprintf
sqrt
sqrtf
sqrtl
srand
srand48
srandom
sscanf
stat
statvfs
stdin
strcasecmp
strcat
strchr
strcmp
strcoll
strcpy
strcspn
strdup
strerror
strerror_r
strfmon
strftime
strlen
strncasecmp
strncat
strncmp
strncpy
strpbrk
strptime
strrchr
strspn
strstr
strtod
strtof
strtoimax
strtok
strtok_r
strtol
strtold
strtoll
strtoul
strtoull
strtoumax
strxfrm
swab
swapcontext
swprintf
swscanf
symlink
sync
sysconf
syslog
system
tan
tanf
tanh
tanhf
tanhl
tanl
tcdrain
tcflow
tcflush
tcgetattr
tcgetpgrp
tcgetsid
tcsendbreak
tcsetattr
tcsetpgrp
tdelete
telldir
tempnam
tfind
tgamma
tgammaf
tgammal
time
timer_create
timer_delete
timer_getoverrun
timer_settime
times
timezone
tmpfile
tmpnam
toascii
tolower
toupper
towctrans
towlower
towupper
trunc
truncate
truncf
truncl
tsearch
ttyname
ttyname_r
twalk
tzname
tzset
ualarm
ulimit
umask
uname
ungetc
ungetwc
unlink
unlockpt
unsetenv
usleep
utime
utimes
va_arg
va_copy
va_end
va_start
vfork
vfprintf
vfscanf
vfwprintf
vfwscanf
vprintf
vscanf
vsnprintf
vsprintf
vsscanf
vswprintf
vswscanf
vwprintf
vwscanf
wait
waitid
waitpid
wcrtomb
wcscat
wcschr
wcscmp
wcscoll
wcscpy
wcscspn
wcsftime
wcslen
wcsncat
wcsncmp
wcsncpy
wcspbrk
wcsrchr
wcsrtombs
wcsspn
wcsstr
wcstod
wcstof
wcstoimax
wcstok
wcstol
wcstold
wcstoll
wcstombs
wcstoul
wcstoull
wcstoumax
wcswcs
wcswidth
wcsxfrm
wctob
wctomb
wctrans
wctype
wcwidth
wmemchr
wmemcmp
wmemcpy
wmemmove
wmemset
wordexp
wordfree
wprintf
write
writev
wscanf
y0
y1
yn
' | sed -e "$sed_alt1" | tr -d "$trnl" | sed -e "$sed_alt2" -e "$sed_alt3"`

indent=""
seen_modules=
seen_files=

# func_exit STATUS
# exit with status
func_exit ()
{
  (exit $1); exit $1
}

# func_tmpdir
# creates a temporary directory.
# Sets variable
# - tmp             pathname of freshly created temporary directory
func_tmpdir ()
{
  # Use the environment variable TMPDIR, falling back to /tmp. This allows
  # users to specify a different temporary directory, for example, if their
  # /tmp is filled up or too small.
  : ${TMPDIR=/tmp}
  {
    # Use the mktemp program if available. If not available, hide the error
    # message.
    tmp=`(umask 077 && mktemp -d "$TMPDIR/MDXXXXXX") 2>/dev/null` &&
    test -n "$tmp" && test -d "$tmp"
  } ||
  {
    # Use a simple mkdir command. It is guaranteed to fail if the directory
    # already exists.  $RANDOM is bash specific and expands to empty in shells
    # other than bash, ksh and zsh.  Its use does not increase security;
    # rather, it minimizes the probability of failure in a very cluttered /tmp
    # directory.
    tmp=$TMPDIR/MD$$-$RANDOM
    (umask 077 && mkdir "$tmp")
  } ||
  {
    echo "$0: cannot create a temporary directory in $TMPDIR" >&2
    func_exit 1
  }
}

# func_append var value
# appends the given value to the shell variable var.
if ( foo=bar; foo+=baz && test "$foo" = barbaz ) >/dev/null 2>&1; then
  # Use bash's += operator. It reduces complexity of appending repeatedly to
  # a single variable from O(n^2) to O(n).
  func_append ()
  {
    eval "$1+=\"\$2\""
  }
else
  func_append ()
  {
    eval "$1=\"\$$1\$2\""
  }
fi

# func_echo line
# outputs line with indentation.
func_echo ()
{
  echo "${indent}$*"
}

# func_indent
# increases the indentation.
func_indent ()
{
  indent="$indent  "
}

# func_unindent
# decreases the indentation.
func_unindent ()
{
  indent=`echo "$indent" | sed -e 's/  $//'`
}

# func_begin tag [attribute...]
# opens a HTML tag.
func_begin ()
{
  func_echo "<$*>"
  func_indent
}

# func_end tag
# closes a HTML tag.
func_end ()
{
  func_unindent
  func_echo "</$1>"
}

# func_wrap tag [attribute...]
# encloses $element in a HTML tag, without line breaks.
func_wrap ()
{
  element="<$*>${element}</$1>"
}

# func_section_wrap sectiontag
# encloses $element in a HTML tag referring to the given tag.
func_section_wrap ()
{
  if test -n "$in_toc"; then
    func_wrap A "HREF=\"#$1\""
  else
    func_wrap A "NAME=\"$1\""
  fi
}

# func_begin_table
func_begin_table ()
{
  func_begin TABLE
  if test -z "$in_toc"; then
    func_begin TR
    func_echo "<TH ALIGN=LEFT>modules/"
    func_echo "<TH ALIGN=LEFT>lib/"
    func_echo "<TH ALIGN=LEFT>lib/"
    func_echo "<TH ALIGN=LEFT>m4/"
    func_echo "<TH ALIGN=LEFT>&nbsp;"
    func_end TR
    func_begin TR
    func_echo "<TH ALIGN=LEFT>Module"
    func_echo "<TH ALIGN=LEFT>Header"
    func_echo "<TH ALIGN=LEFT>Implementation"
    func_echo "<TH ALIGN=LEFT>Autoconf macro"
    func_echo "<TH ALIGN=LEFT>Depends on"
    func_end TR
  fi
}

# func_module module
func_module ()
{
    sed_remove_trailing_empty_line='${
/^$/d
}'

  if test -n "$in_toc"; then

    func_begin TR "WIDTH=\"100%\""

    element='<A HREF="#module='$1'">'$1'</A>'
    func_echo "<TD ALIGN=LEFT VALIGN=TOP WIDTH=\"20%\">$element"

    element=`gnulib-tool --extract-description $1 \
             | sed -e "$sed_lt" -e "$sed_gt" -e "$sed_remove_trailing_empty_line" \
                   -e 's,^, ,' \
                   -e 's,\([^a-zA-Z_]\)'"${posix_functions}"'(),\1<A HREF="'"$POSIX2001_URL"'xsh/\2.html">\2</A>(),g' \
                   -e 's,^ ,,'`
    func_echo "<TD ALIGN=LEFT VALIGN=TOP WIDTH=\"80%\">$element"

    func_end TR

  else

    func_begin TR

    element='<A NAME="module='$1'"></A><A HREF="'$repo_url_prefix'modules/'$1$repo_url_suffix'">'$1'</A>'
    func_echo "<TD ALIGN=LEFT VALIGN=TOP>$element"

    includes=`gnulib-tool --extract-include-directive $1`
    files=`gnulib-tool --extract-filelist $1 \
           | grep -v '^m4/gnulib-common\.m4$'`
    element=`echo "$includes" \
             | sed -e "$sed_lt" -e "$sed_gt" -e "$sed_remove_trailing_empty_line" \
                   -e 's,^#include "\(.*\)"$,#include "<A HREF="'$repo_url_prefix'lib/\1'$repo_url_suffix_repl'">\1</A>",' \
                   -e 's,^#include &lt;'"${posix_headers}"'\.h&gt;$,#include \&lt;<A HREF="'"$POSIX2001_URL"'xbd/\1.h.html">\1.h</A>\&gt;,' \
                   -e 's/$/<BR>/' | tr -d "$trnl" | sed -e 's/<BR>$//'`
    test -n "$element" || element='---'
    func_echo "<TD ALIGN=LEFT VALIGN=TOP>$element"

    sed_choose_unconditional_nonstandard_include='s,^#include "\(.*\)"$,\1,p'
    includefile=`echo "$includes" \
                 | sed -n -e "$sed_choose_unconditional_nonstandard_include" \
                 | sed -e "$sed_escape_dot" | tr -d "$trnl"`
    sed_choose_lib_files='s,^lib/\(.*\)$,\1,p'
    element=`echo "$files" \
             | sed -e '/^$/d' \
             | sed -n -e "$sed_choose_lib_files" \
             | sed -e '\|^'"${includefile}"'$|d' \
                   -e 's,^\(.*\)$,<A HREF="'$repo_url_prefix'lib/\1'$repo_url_suffix_repl'">\1</A>,' \
                   -e 's/$/<BR>/' | tr -d "$trnl" | sed -e 's/<BR>$//'`
    test -n "$element" || element='---'
    func_echo "<TD ALIGN=LEFT VALIGN=TOP>$element"

    sed_choose_m4_files='s,^m4/\(.*\)$,\1,p'
    element=`(echo "$files" \
              | sed -e "$sed_remove_trailing_empty_line" \
              | sed -n -e "$sed_choose_m4_files" \
              | sed -e '/^onceonly/d' \
                    -e 's,^\(.*\)$,<A HREF="'$repo_url_prefix'm4/\1'$repo_url_suffix_repl'">\1</A>,'; \
              gnulib-tool --extract-autoconf-snippet $1 \
              | sed -e "$sed_remove_trailing_empty_line") \
              | sed -e 's/$/<BR>/' | tr -d "$trnl" | sed -e 's/<BR>$//'`
    test -n "$element" || element='---'
    func_echo "<TD ALIGN=LEFT VALIGN=TOP>$element"

    element=`gnulib-tool --extract-dependencies $1 \
             | sed -e "$sed_remove_trailing_empty_line" \
                   -e 's/$/<BR>/' | tr -d "$trnl" | sed -e 's/<BR>$//'`
    test -n "$element" || element='---'
    func_echo "<TD ALIGN=LEFT VALIGN=TOP>$element"

    func_end TR

    func_append seen_modules " $1"
    func_append seen_files " $files"
  fi
}

# func_end_table
func_end_table ()
{
  func_end TABLE
}

# func_all_modules
func_all_modules ()
{
  element="Support for systems lacking ANSI C 89"
  func_section_wrap ansic_sup
  func_wrap H2
  func_echo "$element"

  func_begin_table
  func_module stdlib
  func_module exit
  func_module atexit
  func_module strtod
  func_module strtol
  func_module strtoul
  func_module memchr
  func_module memcmp
  func_module memcpy
  func_module memmove
  func_module memset
  func_module strcspn
  func_module strpbrk
  func_module strerror
  func_module mktime
  func_end_table

  func_echo 'Most of these modules are not listed among dependencies below, for simplicity.'

  element="Enhancements for ANSI C 89 functions"
  func_section_wrap ansic_enh
  func_wrap H2
  func_echo "$element"

  element="Diagnostics <assert.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_enh_assert_diagnostics
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module assert
  func_module verify
  func_end_table

  element="Mathematics <math.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_enh_math
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module fpieee
  func_end_table

  element="Input/output <stdio.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_enh_stdio
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module fflush
  func_module fseterr
  func_module tmpfile
  func_end_table

  element="Memory management functions <stdlib.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_enh_stdlib_memory
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module calloc
  func_module eealloc
  func_module free
  func_module malloc
  func_module realloc
  func_module pagealign_alloc
  func_end_table

  element="Sorting functions <stdlib.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_enh_stdlib_sorting
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module mpsort
  func_end_table

  element="Date and time <time.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_enh_time_datetime
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module fprintftime
  func_module strftime
  func_end_table

  element="Extra functions based on ANSI C 89"
  func_section_wrap ansic_ext
  func_wrap H2
  func_echo "$element"

  element="Memory management functions <stdlib.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_ext_stdlib_memory
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module xsize
  func_module xalloc
  func_module xalloc-die
  func_module alloca
  func_module alloca-opt
  func_module malloca
  func_module xmalloca
  func_end_table

  element="Integer arithmetic functions <stdlib.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_ext_stdlib_arith
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module count-one-bits
  func_module gcd
  func_module minmax
  func_end_table

  element="Environment variables <stdlib.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_ext_stdlib_env
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module putenv
  func_module setenv
  func_module xsetenv
  func_end_table

  element="Character handling <ctype.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_ext_ctype
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module c-ctype
  func_end_table

  element="String handling <string.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_ext_string
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module bcopy
  func_module memmem
  func_module mempcpy
  func_module memrchr
  func_module stpcpy
  func_module stpncpy
  func_module c-strcase
  func_module strcase
  func_module c-strcaseeq
  func_module c-strcasestr
  func_module strcasestr
  func_module strchrnul
  func_module strdup
  func_module streq
  func_module strnlen
  func_module strnlen1
  func_module strndup
  func_module strsep
  func_module c-strstr
  func_module trim
  func_module fstrcmp
  func_module xstrndup
  func_end_table

  element="Mathematics <math.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_ext_math
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module printf-frexp
  func_end_table

  element="Numeric conversion functions <stdlib.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_ext_stdlib_conv
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module c-strtod
  func_module c-strtold
  func_module xstrtod
  func_module xstrtol
  func_module xstrtold
  func_end_table

  element="Date and time <time.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_ext_time_datetime
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module getdate
  func_module timegm
  func_module tzset
  func_end_table

  element="Input/Output <stdio.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_ext_stdio
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module unlocked-io
  func_module fwriteerror
  func_module vasnprintf
  func_module vasprintf
  func_module xvasprintf
  func_end_table

  element="Signal handling <signal.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_ext_signal
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module fatal-signal
  func_module raise
  func_end_table

  element="Command-line arguments"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_ext_argv
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module argmatch
  func_module version-etc
  func_module version-etc-fsf
  func_module long-options
  func_end_table

  element="Container data structures"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_ext_container
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module list
  func_module array-list
  func_module carray-list
  func_module linked-list
  func_module avltree-list
  func_module rbtree-list
  func_module linkedhash-list
  func_module avltreehash-list
  func_module rbtreehash-list
  func_module sublist
  func_module oset
  func_module array-oset
  func_module avltree-oset
  func_module rbtree-oset
  func_end_table

  element="Cryptographic computations (low-level)"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_ext_crypto
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module crypto/arcfour
  func_module crypto/arctwo
  func_module crypto/des
  func_module crypto/hmac-md5
  func_module crypto/hmac-sha1
  func_module crypto/md2
  func_module crypto/md4
  func_module crypto/md5
  func_module crypto/rijndael
  func_module crypto/sha1
  func_end_table

  element="Cryptographic computations (high-level)"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_ext_crypto2
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module crypto/gc
  func_module crypto/gc-arcfour
  func_module crypto/gc-arctwo
  func_module crypto/gc-des
  func_module crypto/gc-hmac-md5
  func_module crypto/gc-hmac-sha1
  func_module crypto/gc-md2
  func_module crypto/gc-md4
  func_module crypto/gc-md5
  func_module crypto/gc-pbkdf2-sha1
  func_module crypto/gc-random
  func_module crypto/gc-rijndael
  func_module crypto/gc-sha1
  func_end_table

  element="Misc"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap ansic_ext_misc
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module base64
  func_module check-version
  func_module crc
  func_module diacrit
  func_module diffseq
  func_module getline
  func_module getdelim
  func_module getnline
  func_module getndelim2
  func_module linebuffer
  func_module memxor
  func_module obstack
  func_module hash-pjw
  func_module hash
  func_module readline
  func_module readtokens
  func_module readtokens0
  func_module strverscmp
  func_end_table

  element="Support for systems lacking ISO C 99"
  func_section_wrap isoc_sup
  func_wrap H2
  func_echo "$element"

  element="Core language properties"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_core_properties
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module flexmember
  func_module fpucw
  func_module inline
  func_module vararrays
  func_end_table

  element="Sizes of integer types <limits.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_sup_limits
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module size_max
  func_end_table

  element="Variable arguments <stdarg.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_sup_stdarg
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module stdarg
  func_end_table

  element="Boolean type and values <stdbool.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_sup_stdbool
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module stdbool
  func_end_table

  element="Integer types and values <stdint.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_sup_stdint
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module stdint
  func_end_table

  element="Input/output <stdio.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_sup_stdio
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module stdio
  func_module snprintf
  func_module vsnprintf
  func_end_table

  element="Numeric conversion functions <stdlib.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_sup_stdlib_conv
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module strtoll
  func_module strtoull
  func_end_table

  element="Functions for greatest-width integer types <inttypes.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_sup_inttypes
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module imaxabs
  func_module imaxdiv
  func_module inttypes
  func_module strtoimax
  func_module strtoumax
  func_end_table

  element="Extended multibyte and wide character utilities <wchar.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_sup_wchar
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module wchar
  func_end_table

  element="Wide character classification and mapping utilities <wctype.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_sup_wctype
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module wctype
  func_end_table

  element="Characteristics of floating types <float.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_sup_float
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module float
  func_end_table

  element="Mathematics <math.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_sup_math
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module ceilf
  func_module ceill
  func_module floorf
  func_module floorl
  func_module frexp
  func_module frexpl
  func_module frexpl-nolibm
  func_module isfinite
  func_module isnan-nolibm
  func_module isnanf-nolibm
  func_module isnanl
  func_module isnanl-nolibm
  func_module ldexpl
  func_module math
  func_module mathl
  func_module round
  func_module roundf
  func_module roundl
  func_module signbit
  func_module trunc
  func_module truncf
  func_module truncl
  func_end_table

  element="Enhancements for ISO C 99 functions"
  func_section_wrap isoc_enh
  func_wrap H2
  func_echo "$element"

  element="Input/output <stdio.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_enh_stdio
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module printf-safe
  func_end_table

  element="Extra functions based on ISO C 99"
  func_section_wrap isoc_ext
  func_wrap H2
  func_echo "$element"

  element="Mathematics <math.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_ext_math
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module printf-frexpl
  func_end_table

  element="Numeric conversion functions <stdlib.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_ext_stdlib_conv
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module intprops
  func_module inttostr
  func_module xstrtoimax
  func_module xstrtoumax
  func_end_table

  element="Extended multibyte and wide character utilities <wchar.h>"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap isoc_ext_wchar_mb
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module mbchar
  func_module mbiter
  func_module mbuiter
  func_module mbfile
  func_end_table

  element="Support for systems lacking POSIX:2001"
  func_section_wrap posix_sup
  func_wrap H2
  func_echo "$element"

  func_begin_table
  func_module arpa_inet
  func_module calloc-posix
  func_module chown
  func_module dup2
  func_module fchdir
  func_module fcntl
  func_module fopen
  func_module fprintf-posix
  func_module freopen
  func_module fseek
  func_module fseeko
  func_module ftell
  func_module ftello
  func_module ftruncate
  func_module getaddrinfo
  func_module getcwd
  func_module getgroups
  func_module gethostname
  func_module getlogin_r
  func_module getsubopt
  func_module gettimeofday
  func_module iconv_open
  func_module inet_ntop
  func_module inet_pton
  func_module locale
  func_module lseek
  func_module lstat
  func_module malloc-posix
  func_module mkdir
  func_module mkdtemp
  func_module mkstemp
  func_module netinet_in
  func_module open
  func_module poll
  func_module printf-posix
  func_module readlink
  func_module realloc-posix
  func_module signal
  func_module sleep
  func_module snprintf-posix
  func_module sprintf-posix
  func_module string
  func_module tempname
  func_module time
  func_module time_r
  func_module timespec
  func_module nanosleep
  func_module regex
  func_module rename
  func_module rmdir
  func_module search
  func_module sigprocmask
  func_module socklen
  func_module ssize_t
  func_module strptime
  func_module strtok_r
  func_module sys_select
  func_module sys_socket
  func_module sys_stat
  func_module sys_time
  func_module tsearch
  func_module unistd
  func_module utime
  func_module vasnprintf-posix
  func_module vasprintf-posix
  func_module vfprintf-posix
  func_module vprintf-posix
  func_module vsnprintf-posix
  func_module vsprintf-posix
  func_module wcwidth
  func_end_table

  element="Compatibility checks for POSIX:2001 functions"
  func_section_wrap posix_compat
  func_wrap H2
  func_echo "$element"

  func_begin_table
  func_module clock-time
  func_module d-ino
  func_module d-type
  func_module link-follow
  func_module rename-dest-slash
  func_module rmdir-errno
  func_module unlink-busy
  func_module winsz-ioctl
  func_module winsz-termios
  func_end_table

  element="Enhancements for POSIX:2001 functions"
  func_section_wrap posix_enh
  func_wrap H2
  func_echo "$element"

  func_begin_table
  func_module chdir-long
  func_module dirname
  func_module getopt
  func_module unistd-safer
  func_module fnmatch
  func_module fnmatch-posix
  func_module fnmatch-gnu
  func_module glob
  func_module exclude
  func_end_table

  element="Extra functions based on POSIX:2001"
  func_section_wrap posix_ext
  func_wrap H2
  func_echo "$element"

  element="Numeric conversion functions"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap posix_ext_conv
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module human
  func_end_table

  element="File system functions"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap posix_ext_filesys
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module acl
  func_module areadlink
  func_module areadlink-with-size
  func_module backupfile
  func_module canonicalize
  func_module canonicalize-lgpl
  func_module chdir-safer
  func_module clean-temp
  func_module copy-file
  func_module fsusage
  func_module dirfd
  func_module double-slash-root
  func_module euidaccess
  func_module file-type
  func_module fileblocks
  func_module filemode
  func_module filename
  func_module filenamecat
  func_module fts
  func_module fts-lgpl
  func_module isdir
  func_module lchmod
  func_module lchown
  func_module mkancesdirs
  func_module mkdir-p
  func_module modechange
  func_module mountlist
  func_module openat
  func_module pathmax
  func_module read-file
  func_module same
  func_module save-cwd
  func_module savedir
  func_module savewd
  func_module stat-time
  func_module tmpdir
  func_module unlinkdir
  func_module utimecmp
  func_module utimens
  func_module write-any-file
  func_module xgetcwd
  func_module xreadlink
  func_end_table

  element="File system as inode set"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap posix_ext_inodeset
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module cycle-check
  func_module dev-ino
  func_module file-set
  func_module hash-triple
  func_module i-ring
  func_module same-inode
  func_end_table

  element="File descriptor based Input/Output"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap posix_ext_filedesc
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module fcntl-safer
  func_module safe-read
  func_module safe-write
  func_module full-read
  func_module full-write
  func_module binary-io
  func_module isapipe
  func_end_table

  element="File stream based Input/Output"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap posix_ext_filestream
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module close-stream
  func_module closein
  func_module closeout
  func_module fbufmode
  func_module fopen-safer
  func_module fpending
  func_module fpurge
  func_module freadable
  func_module freadahead
  func_module freading
  func_module fwritable
  func_module fwriting
  func_module getpass
  func_module getpass-gnu
  func_module stdlib-safer
  func_module tmpfile-safer
  func_end_table

  element="Users and groups"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap posix_ext_uidgid
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module getugroups
  func_module group-member
  func_module idcache
  func_module userspec
  func_end_table

  element="Date and time"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap posix_ext_datetime
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module gethrxtime
  func_module gettime
  func_module settime
  func_module posixtm
  func_module xnanosleep
  func_end_table

  element="Networking functions"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap posix_ext_net
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module xgethostname
  func_module canon-host
  func_end_table

  element="Multithreading"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap posix_ext_thread
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module lock
  func_module tls
  func_module openmp
  func_end_table

  element="Internationalization functions"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap posix_ext_i18n
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module gettext
  func_module gettext-h
  func_module iconv
  func_module striconv
  func_module xstriconv
  func_module striconveh
  func_module striconveha
  func_module localcharset
  func_module hard-locale
  func_module localename
  func_module mbslen
  func_module mbsnlen
  func_module mbschr
  func_module mbsrchr
  func_module mbsstr
  func_module mbscasecmp
  func_module mbsncasecmp
  func_module mbspcasecmp
  func_module mbscasestr
  func_module mbscspn
  func_module mbspbrk
  func_module mbsspn
  func_module mbssep
  func_module mbstok_r
  func_module mbswidth
  func_module memcasecmp
  func_module memcoll
  func_module xmemcoll
  func_module unicodeio
  func_module rpmatch
  func_module yesno
  func_module linebreak
  func_module bison-i18n
  func_end_table

  element="Unicode string functions"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap posix_ext_unicode
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module unitypes
  func_module ucs4-utf8
  func_module ucs4-utf16
  func_module utf8-ucs4-unsafe
  func_module utf16-ucs4-unsafe
  func_module utf8-ucs4
  func_module utf16-ucs4
  func_module unistr/base
  func_module unistr/u8-to-u16
  func_module unistr/u8-to-u32
  func_module unistr/u16-to-u8
  func_module unistr/u16-to-u32
  func_module unistr/u32-to-u8
  func_module unistr/u32-to-u16
  func_module unistr/u8-check
  func_module unistr/u16-check
  func_module unistr/u32-check
  func_module unistr/u8-chr
  func_module unistr/u16-chr
  func_module unistr/u32-chr
  func_module unistr/u8-cmp
  func_module unistr/u16-cmp
  func_module unistr/u32-cmp
  func_module unistr/u8-cpy
  func_module unistr/u16-cpy
  func_module unistr/u32-cpy
  func_module unistr/u8-cpy-alloc
  func_module unistr/u16-cpy-alloc
  func_module unistr/u32-cpy-alloc
  func_module unistr/u8-endswith
  func_module unistr/u16-endswith
  func_module unistr/u32-endswith
  func_module unistr/u8-mblen
  func_module unistr/u16-mblen
  func_module unistr/u32-mblen
  func_module unistr/u8-mbsnlen
  func_module unistr/u16-mbsnlen
  func_module unistr/u32-mbsnlen
  func_module unistr/u8-mbtouc-unsafe
  func_module unistr/u16-mbtouc-unsafe
  func_module unistr/u32-mbtouc-unsafe
  func_module unistr/u8-mbtouc
  func_module unistr/u16-mbtouc
  func_module unistr/u32-mbtouc
  func_module unistr/u8-mbtoucr
  func_module unistr/u16-mbtoucr
  func_module unistr/u32-mbtoucr
  func_module unistr/u8-move
  func_module unistr/u16-move
  func_module unistr/u32-move
  func_module unistr/u8-next
  func_module unistr/u16-next
  func_module unistr/u32-next
  func_module unistr/u8-prev
  func_module unistr/u16-prev
  func_module unistr/u32-prev
  func_module unistr/u8-set
  func_module unistr/u16-set
  func_module unistr/u32-set
  func_module unistr/u8-startswith
  func_module unistr/u16-startswith
  func_module unistr/u32-startswith
  func_module unistr/u8-stpcpy
  func_module unistr/u16-stpcpy
  func_module unistr/u32-stpcpy
  func_module unistr/u8-stpncpy
  func_module unistr/u16-stpncpy
  func_module unistr/u32-stpncpy
  func_module unistr/u8-strcat
  func_module unistr/u16-strcat
  func_module unistr/u32-strcat
  func_module unistr/u8-strchr
  func_module unistr/u16-strchr
  func_module unistr/u32-strchr
  func_module unistr/u8-strcmp
  func_module unistr/u16-strcmp
  func_module unistr/u32-strcmp
  func_module unistr/u8-strcpy
  func_module unistr/u16-strcpy
  func_module unistr/u32-strcpy
  func_module unistr/u8-strcspn
  func_module unistr/u16-strcspn
  func_module unistr/u32-strcspn
  func_module unistr/u8-strdup
  func_module unistr/u16-strdup
  func_module unistr/u32-strdup
  func_module unistr/u8-strlen
  func_module unistr/u16-strlen
  func_module unistr/u32-strlen
  func_module unistr/u8-strmblen
  func_module unistr/u16-strmblen
  func_module unistr/u32-strmblen
  func_module unistr/u8-strmbtouc
  func_module unistr/u16-strmbtouc
  func_module unistr/u32-strmbtouc
  func_module unistr/u8-strncat
  func_module unistr/u16-strncat
  func_module unistr/u32-strncat
  func_module unistr/u8-strncmp
  func_module unistr/u16-strncmp
  func_module unistr/u32-strncmp
  func_module unistr/u8-strncpy
  func_module unistr/u16-strncpy
  func_module unistr/u32-strncpy
  func_module unistr/u8-strnlen
  func_module unistr/u16-strnlen
  func_module unistr/u32-strnlen
  func_module unistr/u8-strpbrk
  func_module unistr/u16-strpbrk
  func_module unistr/u32-strpbrk
  func_module unistr/u8-strrchr
  func_module unistr/u16-strrchr
  func_module unistr/u32-strrchr
  func_module unistr/u8-strspn
  func_module unistr/u16-strspn
  func_module unistr/u32-strspn
  func_module unistr/u8-strstr
  func_module unistr/u16-strstr
  func_module unistr/u32-strstr
  func_module unistr/u8-strtok
  func_module unistr/u16-strtok
  func_module unistr/u32-strtok
  func_module unistr/u8-uctomb
  func_module unistr/u16-uctomb
  func_module unistr/u32-uctomb
  func_module uniconv/base
  func_module uniconv/u8-conv-from-enc
  func_module uniconv/u16-conv-from-enc
  func_module uniconv/u32-conv-from-enc
  func_module uniconv/u8-conv-to-enc
  func_module uniconv/u16-conv-to-enc
  func_module uniconv/u32-conv-to-enc
  func_module uniconv/u8-strconv-from-enc
  func_module uniconv/u16-strconv-from-enc
  func_module uniconv/u32-strconv-from-enc
  func_module uniconv/u8-strconv-to-enc
  func_module uniconv/u16-strconv-to-enc
  func_module uniconv/u32-strconv-to-enc
  func_module uniconv/u8-strconv-from-locale
  func_module uniconv/u16-strconv-from-locale
  func_module uniconv/u32-strconv-from-locale
  func_module uniconv/u8-strconv-to-locale
  func_module uniconv/u16-strconv-to-locale
  func_module uniconv/u32-strconv-to-locale
  func_module unistdio/base
  func_module unistdio/u-printf-args
  func_module unistdio/ulc-asnprintf
  func_module unistdio/ulc-asprintf
  func_module unistdio/ulc-fprintf
  func_module unistdio/ulc-printf-parse
  func_module unistdio/ulc-snprintf
  func_module unistdio/ulc-sprintf
  func_module unistdio/ulc-vasnprintf
  func_module unistdio/ulc-vasprintf
  func_module unistdio/ulc-vfprintf
  func_module unistdio/ulc-vsnprintf
  func_module unistdio/ulc-vsprintf
  func_module unistdio/u8-asnprintf
  func_module unistdio/u8-asprintf
  func_module unistdio/u8-printf-parse
  func_module unistdio/u8-snprintf
  func_module unistdio/u8-sprintf
  func_module unistdio/u8-vasnprintf
  func_module unistdio/u8-vasprintf
  func_module unistdio/u8-vsnprintf
  func_module unistdio/u8-vsprintf
  func_module unistdio/u8-u8-asnprintf
  func_module unistdio/u8-u8-asprintf
  func_module unistdio/u8-u8-snprintf
  func_module unistdio/u8-u8-sprintf
  func_module unistdio/u8-u8-vasnprintf
  func_module unistdio/u8-u8-vasprintf
  func_module unistdio/u8-u8-vsnprintf
  func_module unistdio/u8-u8-vsprintf
  func_module unistdio/u16-asnprintf
  func_module unistdio/u16-asprintf
  func_module unistdio/u16-printf-parse
  func_module unistdio/u16-snprintf
  func_module unistdio/u16-sprintf
  func_module unistdio/u16-vasnprintf
  func_module unistdio/u16-vasprintf
  func_module unistdio/u16-vsnprintf
  func_module unistdio/u16-vsprintf
  func_module unistdio/u16-u16-asnprintf
  func_module unistdio/u16-u16-asprintf
  func_module unistdio/u16-u16-snprintf
  func_module unistdio/u16-u16-sprintf
  func_module unistdio/u16-u16-vasnprintf
  func_module unistdio/u16-u16-vasprintf
  func_module unistdio/u16-u16-vsnprintf
  func_module unistdio/u16-u16-vsprintf
  func_module unistdio/u32-asnprintf
  func_module unistdio/u32-asprintf
  func_module unistdio/u32-printf-parse
  func_module unistdio/u32-snprintf
  func_module unistdio/u32-sprintf
  func_module unistdio/u32-vasnprintf
  func_module unistdio/u32-vasprintf
  func_module unistdio/u32-vsnprintf
  func_module unistdio/u32-vsprintf
  func_module unistdio/u32-u32-asnprintf
  func_module unistdio/u32-u32-asprintf
  func_module unistdio/u32-u32-snprintf
  func_module unistdio/u32-u32-sprintf
  func_module unistdio/u32-u32-vasnprintf
  func_module unistdio/u32-u32-vasprintf
  func_module unistdio/u32-u32-vsnprintf
  func_module unistdio/u32-u32-vsprintf
  func_module uniname/base
  func_module uniname/uniname
  func_module uniwidth/base
  func_module uniwidth/u8-strwidth
  func_module uniwidth/u8-width
  func_module uniwidth/u16-strwidth
  func_module uniwidth/u16-width
  func_module uniwidth/u32-strwidth
  func_module uniwidth/u32-width
  func_module uniwidth/width
  func_end_table

  element="Executing programs"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap posix_ext_exec
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module cloexec
  func_module findprog
  func_module wait-process
  func_module execute
  func_module pipe
  func_module sh-quote
  func_end_table

  element="Java"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap posix_ext_java
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module classpath
  func_module javacomp-script
  func_module javacomp
  func_module javaexec-script
  func_module javaexec
  func_module javaversion
  #func_module gcj
  func_end_table

  element="C#"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap posix_ext_csharp
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module csharpcomp-script
  func_module csharpcomp
  func_module csharpexec-script
  func_module csharpexec
  func_end_table

  element="Misc"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_section_wrap posix_ext_misc
  func_wrap H3
  func_echo "$element"

  func_begin_table
  func_module argp
  func_module argz
  func_module byteswap
  func_module exitfail
  func_module c-stack
  func_module error
  func_module extensions
  func_module getdomainname
  func_module xgetdomainname
  func_module getloadavg
  func_module getpagesize
  func_module getusershell
  func_module physmem
  func_module posixver
  func_module progname
  func_module quotearg
  func_module quote
  func_module readutmp
  func_module sig2str
  func_module sysexits
  func_module verror
  func_module visibility
  func_end_table

  element="Support for building libraries and executables"
  func_section_wrap build_lib
  func_wrap H2
  func_echo "$element"

  func_begin_table
  func_module absolute-header
  func_module config-h
  func_module configmake
  func_module dummy
  func_module elisp-comp
  func_module havelib
  func_module include_next
  func_module ldd
  func_module lib-ignore
  func_module link-warning
  func_module no-c++
  func_module relocatable-lib
  func_module relocatable-lib-lgpl
  func_module relocatable-prog
  func_module relocatable-prog-wrapper
  func_module relocatable-script
  func_end_table

  element="Support for building documentation"
  func_section_wrap build_doc
  func_wrap H2
  func_echo "$element"

  func_begin_table
  func_module fdl
  func_module gendocs
  func_module gpl-2.0
  func_module gpl-3.0
  func_module lgpl-2.1
  func_module regexprops-generic
  func_end_table

  element="Support for maintaining and releasing projects"
  func_section_wrap maintain
  func_wrap H2
  func_echo "$element"

  func_begin_table
  func_module announce-gen
  func_module autobuild
  func_module git-version-gen
  func_module gnupload
  func_module maintainer-makefile
  func_end_table

  element="Misc"
  func_section_wrap misc
  func_wrap H2
  func_echo "$element"

  func_begin_table
  func_module gnu-make
  func_module host-os
  func_module perl
  func_module posix-shell
  func_module uptime
  func_end_table
}


func_tmpdir
trap 'exit_status=$?
      if test "$signal" != 0; then
        echo "caught signal $signal" >&2
      fi
      rm -rf "$tmp"
      exit $exit_status' 0
for signal in 1 2 3 13 15; do
  trap '{ signal='$signal'; func_exit 1; }' $signal
done
signal=0

echo '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">'
func_begin HTML

func_begin HEAD

element="Gnulib Module List"
func_wrap TITLE
func_echo "$element"

modules=`gnulib-tool --list`
modules=`for m in $modules; do printf "%s, " $m; done | sed -e 's/, $//'`
func_echo '<META NAME="keywords" CONTENT="'"${modules}"'">'

func_end HEAD

func_begin BODY

element="Gnulib Module List"
func_wrap H1
func_echo "$element"

func_echo 'This is a list of the modules which make up gnulib, with dependencies.'

in_toc=yes
func_all_modules

in_toc=
func_all_modules


gnulib-tool --list > "$tmp/all-modules"
missed_modules=`for module in $seen_modules; do echo $module; done \
		  | LC_ALL=C sort -u \
		  | LC_ALL=C join -v 2 - "$tmp/all-modules"`

if test -n "$missed_modules"; then

  element="Unclassified modules - please update MODULES.html.sh"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_wrap H2
  func_echo "$element"

  func_begin_table
  for module in $missed_modules; do
    func_module $module
  done
  func_end_table

fi

{ find lib -type f -print; find m4 -type f -print; } | LC_ALL=C sort | sed -e '/\/\./d' -e /CVS/d -e /README/d -e /ChangeLog/d -e /Makefile/d -e /TODO/d -e '/tags$/d' -e '/TAGS$/d' -e '/~$/d' > "$tmp/all-files"
missed_files=`for file in $seen_files; do echo $file; done \
		| LC_ALL=C sort -u \
		| LC_ALL=C join -v 2 - "$tmp/all-files"`

if test -n "$missed_files"; then

  element="Lone files - please create new modules containing them"
  element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
  func_wrap H2
  func_echo "$element"

  func_echo '<PRE>'
  echo "$missed_files" | sed -e 's,^\(.*\)$,<A HREF="'$repo_url_prefix'\1'$repo_url_suffix_repl'">\1</A>,'
  echo '</PRE>'

fi

element="Future developments"
element=`printf "%s" "$element" | sed -e "$sed_lt" -e "$sed_gt"`
func_wrap H2
func_echo "$element"

func_echo 'Ideally a module could consist of:'
func_begin UL
func_echo '<LI>A header file: lib/<VAR>module</VAR>.h'
func_echo '<LI>One or more implementation files: lib/<VAR>module</VAR>.c et al.'
func_echo '<LI>One or more autoconf macro files: m4/<VAR>module</VAR>.m4 et al.'
func_echo '<LI>A configure.ac fragment, Makefile.am fragment, dependency list: modules/<VAR>module</VAR>'
func_echo '<LI>A testsuite: source files in tests/ and metainformation (a configure.ac fragment, Makefile.am fragment, dependency list) in modules/<VAR>module</VAR>-tests'
func_echo '<LI>Some documentation'
func_echo '<LI>A POT file and some PO files'
func_end UL

func_echo '<HR>'
func_echo 'Generated from <CODE>MODULES.html.sh</CODE> on '`LC_ALL=C date +"%e %B %Y"`.

func_end BODY

func_end HTML

rm -rf "$tmp"
# Undo the effect of the previous 'trap' command.
trap '' 0
trap 'func_exit $?' 1 2 3 13 15

exit 0

# Local Variables:
# indent-tabs-mode: nil
# whitespace-check-buffer-indent: nil
# End:
