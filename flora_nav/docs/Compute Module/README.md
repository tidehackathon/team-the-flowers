# Compute Module

## Velocity Estimation

Firstly 

---

## Calculating position of UAV

Initial GPS position of drone can be collected from GPS or inserted to a memory of on-board computer manually. 
As well as it’s altitude. These data constitutes the initial coordinate system which is mathematicall interpretation 
for the moving small UAV.

Translation matrix:

    T = | 1 | 0 | 0 | xt |
        | 0 | 1 | 0 | yt |
        | 0 | 0 | 1 | 0  |
        | 0 | 0 | 0 | 1  |

x<sub>t</sub> - value of latitude which you have to move the position of UAV on map after traveling for one second with given velocity.

y<sub>t</sub> - value of longtitude which you have to move the position of UAV on map after traveling for one second with given velocity.

Given the previous data the starting position of UAV will look like this:

    P = | xp |
        | yp |
        | 0  |
        | 1  |

Position of UAV after time interval will be calculated this way:

P<sub>current</sub> = P<sub>UAV</sub> x T