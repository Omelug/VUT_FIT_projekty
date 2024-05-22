class Point:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

    def __sub__(self, other):
        sub = Point(self.x, self.y)
        sub.x -= other.x
        sub.y -= other.y
        return sub

    def __str__(self):
        return f"Point({self.x},{self.y})"


p0 = Point()
print(p0)  # should be: Point(0,0)
p1 = Point(3, 4)
result = p0 - p1
print(result)  # should be: Point(-3,-4)
