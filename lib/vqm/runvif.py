#!/usr/bin/python

import numpy
import sys
import scipy.misc

import vifp

ref_file = sys.argv[1]
dist_file = sys.argv[2]

ref = scipy.misc.imread(ref_file, flatten=True).astype(numpy.float32)
dist = scipy.misc.imread(dist_file, flatten=True).astype(numpy.float32)
print vifp.vifp_mscale(ref, dist)

