#Report

- **Hardware**
- CPU: 11th Gen Intel(R) Core(TM) i5-11400H @ 2.70GHz
- OS: Ubuntu 24.04.2 LTS

##Test Data:

Image: 762×1309 pixels (24-bit BMP)
Gaussian filter radius: 5px
Sigma: 2.0

##Results
**Sequential**
- Rotation time: 0.00581289 s
- Gauss blur time: 0.259151 s

**Parallel**
- Rotation time: 0.0025688 s
- Gauss blur time: 0.0460461 s

**Speedup**
- Rotation: ~2.26
- Gauss blur: ~ 5.63
