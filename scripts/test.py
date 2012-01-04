#!/usr/bin/env python

from glob import glob
import json

for f in glob('songs/*/*.json'):
    data = {}
    with open(f) as d:
        data = json.loads(d.read())

    if data['changelog']:
        print data['changelog']
