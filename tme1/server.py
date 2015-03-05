#!/usr/bin/python

import os, time
from struct import calcsize, unpack, pack

f2s = '/tmp/F2S'
s2f = '/tmp/S2F'
req_fmt = "@32ci"
ans_fmt = "@ii"

if not os.path.exists(f2s):
    os.mkfifo(f2s)
if not os.path.exists(s2f):
    os.mkfifo(s2f)

pipe_in = open(f2s, 'r', 0)
pipe_out = open(s2f, 'w')

while 1:
    s = pipe_in.read(calcsize("@1024s"))
    i = pipe_in.read(calcsize("@i"))
    id = unpack("@i", i)
    l = 0
    for i in range(0,1024):
        if s[i] == '\0':
            break
        else:
            l = l+1
    print "len = %d" % l
    print "id = %d" % id
    resp = pack("@2i", id[0], l)
    print resp
    pipe_out.write(resp)
    pipe_out.flush()

