"""Exact describe() extraction from preserved E3 analyze_p0.py."""
import math
import statistics
QUANTILE_METHOD = 'Type 7: h=(n-1)*p; linear interpolation at floor(h), ceil(h)'

def describe(values):
    values = sorted(x for x in values if x is not None)
    if not values:
        return dict(n=0, min=None, median=None, max=None, q1=None, q3=None, iqr=None)

    def quantile(p):
        h = (len(values)-1)*p
        return values[math.floor(h)]+(values[math.ceil(h)]-values[math.floor(h)])*(h-math.floor(h))

    q1, q3 = quantile(.25), quantile(.75)
    return dict(n=len(values), min=values[0], median=statistics.median(values), max=values[-1],
                q1=q1, q3=q3, iqr=q3-q1)

