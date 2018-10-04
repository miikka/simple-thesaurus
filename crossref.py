#!/usr/bin/env python

import json
import sys

thesaurus = {}
common = set()

with open('data/words1000.txt', 'r') as f:
    for line in f:
        common.add(line.strip())

with open('data/mthesaur.txt', 'r') as f:
    for line in f:
        words = line.strip().split(',')
        filtered = [word for word in words[1:] if word in common]
        if len(filtered) > 0:
            thesaurus[words[0]] = filtered

json.dump(thesaurus, sys.stdout)
