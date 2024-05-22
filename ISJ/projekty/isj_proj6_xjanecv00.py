#!/usr/bin/env python3

#Do souboru, nazvaného podle konvence isj_proj6_xnovak00.py,
# implementujte třídu Polynomial, která bude pracovat s polynomy reprezentovanými pomocí seznamů.
# Například 2x^3 - 3x + 1 bude  reprezentováno jako [1,-3,0,2]
# (seznam začíná nejnižším řádem, i když se polynomy většinou zapisují opačně).
"""
#Instance třídy bude možné vytvářet několika různými způsoby:
pol1 = Polynomial([1,-3,0,2])
pol2 = Polynomial(1,-3,0,2)
pol3 = Polynomial(x0=1,x3=2,x1=-3)

Volání funkce print() vypíše polynom v obvyklém formátu:
>>> print(pol2)
2x^3 - 3x + 1

Bude možné porovnávat vektory porovnávat:
>>> pol1 == pol2
True

Polynomy bude možné sčítat a umocňovat nezápornými celými čísly:
>>> print(Polynomial(1,-3,0,2) + Polynomial(0, 2, 1))
2x^3 + x^2 - x + 1
>>> print(Polynomial(-1, 1) ** 2)
x^2 - 2x  + 1

A budou fungovat metody derivative() - derivace a at_value() - hodnota polynomu pro zadané x - obě pouze vrací výsledek, nemění samotný polynom:
>>> print(pol1.derivative())
6x^2 - 3
>>> print(pol1.at_value(2))
11
>>> print(pol1.at_value(2,3))
35
(pokud jsou zadány 2 hodnoty, je výsledkem rozdíl mezi hodnotou at_value() druhého a prvního parametru - může sloužit pro výpočet určitého integrálu, ale ten nemá být implementován)

#Maximální hodnocení bude vyžadovat, abyste:
- uvedli "shebang" jako v předchozích projektech
- důsledně používali dokumentační řetězce a komentovali kód
- nevypisovali žádné ladicí/testovací informace při běžném "import isj_proj6_xnovak00"
- zajistili, že následující platí:
"""


class Polynomial:
    def __init__(self, *args, **kwargs):
        if len(kwargs) > 0:
            self.list = kwargs
            try:
                snake_m = str(max([x for x in kwargs if kwargs[x] != 0]))
            except ValueError:  #  max() arg is an empty sequence
                snake_m = "x0"
            for i in range(int(snake_m[1:])):
                if f"x{i}" not in kwargs:
                    self.list[f"x{i}"] = 0
            #print("kwargs   adwad " + str([x[1] for x in sorted(kwargs.items())]))
            self.list = [x[1] for x in sorted(kwargs.items())]
            #print(self.list[-1])
            while self.list != [] and self.list[-1] == 0:
                del self.list[-1]
        elif len(args) == 1 and isinstance(args[0], list):
            self.list = args[0]
        else:
            self.list = list(args)

    def __str__(self):
        i = len(self.list) - 1
        if i <= 0:
            return "0"
        result = ""
        first = 1
        for snake_x in self.list[::-1]:
            if snake_x != 0:
                if first == 0 and snake_x > 0:
                    result += " + "
                if snake_x < 0:
                    result += " - "
                    snake_x = -1 * snake_x
                if snake_x != 1 or (snake_x == 1 and i == 0):
                    result += str(snake_x)
                if i != 0:
                    result += "x"
                    first = 0
                if i not in (0,1):
                    result += "^" + str(i)
            i -= 1
        return result
    def derivative(self):
        res = Polynomial(self.list[:])
        for i in range(len(res.list)):
            res.list[i] *= i
        res << 1
        return res

    def __eq__(self, other):
        #print(str(self.list) + "==" + str(other.list))
        return self.list == other.list

    def __add__(self, other):
        vetsi = max(len(self.list), len(other.list))
        # rozsireni o nuly
        self.list.extend([0] * (vetsi - len(self.list)))
        other.list.extend([0] * (vetsi - len(other.list)))
        #soucet
        return Polynomial([x + y for x, y in zip(other.list, self.list)])

    def __mul__(self, other):
        for i in range(len(self.list)):
            self.list[i] *= other
        #print(f"{other} {h} * {self.list}")
    def __rshift__(self, number): # real signature unknown
        for _ in range(number):
            self.list.insert(0, 0)
    def __lshift__(self, number): # real signature unknown
        for _ in range(number):
            if self.list:
                del self.list[0]
    def __pow__(self, other):
        res = Polynomial(self.list[:])
        loop = other
        while loop > 1:
            tmp0 = Polynomial(0)
            for i in range(len(self.list)):
                tmp = Polynomial(res.list[:])
                tmp >> i
                tmp * self.list[i]
                tmp0 += tmp
            res = tmp0
            loop -= 1
        return res

    def at_value(self,*number):
        if len(number) == 1:
            #print(f"{number=} {len(number)}")
            res = 0
            for i in range(len(self.list)):
                res += self.list[i]*(number[0]**i)
            return res
        else:
            #print(f"{number=} -- {self.at_value(number[0])}")
            return self.at_value(number[1]) - self.at_value(number[0])

def test():
    assert str(Polynomial(0, 1, 0, -1, 4, -2, 0, 1, 3, 0)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial([-5, 1, 0, -1, 4, -2, 0, 1, 3, 0])) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x - 5"
    assert str(Polynomial(x7=1, x4=4, x8=3, x9=0, x0=0, x5=-2, x3=-1, x1=1)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial(x2=0)) == "0"
    assert str(Polynomial(x0=0)) == "0"
    assert Polynomial(x0=2, x1=0, x3=0, x2=3) == Polynomial(2, 0, 3)
    assert Polynomial(x2=0) == Polynomial(x0=0)
    assert str(Polynomial(x0=1) + Polynomial(x1=1)) == "x + 1"
    assert str(Polynomial([-1, 1, 1, 0]) + Polynomial(1, -1, 1)) == "2x^2"
    pol1 = Polynomial(x2=3, x0=1)
    pol2 = Polynomial(x1=1, x3=0)
    assert str(pol1 + pol2) == "3x^2 + x + 1"
    assert str(pol1 + pol2) == "3x^2 + x + 1"
    assert str(Polynomial(x0=-1, x1=1) ** 1) == "x - 1"
    assert str(Polynomial(x0=-1, x1=1) ** 2) == "x^2 - 2x + 1"
    pol3 = Polynomial(x0=-1, x1=1)
    assert str(pol3 ** 4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(pol3 ** 4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(Polynomial(x0=2).derivative()) == "0"
    assert str(Polynomial(x3=2, x1=3, x0=2).derivative()) == "6x^2 + 3"
    assert str(Polynomial(x3=2, x1=3, x0=2).derivative().derivative()) == "12x"
    pol4 = Polynomial(x3=2, x1=3, x0=2)
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert Polynomial(-2, 3, 4, -5).at_value(0) == -2
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3) == 20
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3, 5) == 44
    pol5 = Polynomial([1, 0, -2])
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-1, 3.6) == -23.92
    assert pol5.at_value(-1, 3.6) == -23.92


if __name__ == '__main__':
    test()
