# plane landing (25 points) #

from math import cos, tan, atan, sin, radians, degrees

def solve(distance: int, speed: int, time: int, offset: int) -> str:
    
    
    air_distance = distance/cos(radians(3))
    print(air_distance)
    air_dist_covered = speed*time
    air_dist_left = air_distance-air_dist_covered
    print(air_dist_left)
    
    ground_dist_left = air_dist_left * cos(radians(3))
    altitude = air_dist_left * sin(radians(3))
    print(altitude)
    altitude += offset
    print(altitude)
    
    new_angle = degrees( atan(altitude/ground_dist_left))

    print(new_angle)
    
    if (new_angle < 3): return "??"
    if (new_angle < 4): return "Perfect"
    if (new_angle < 5): return "Okay"
    if (new_angle > 5): return "Danger"