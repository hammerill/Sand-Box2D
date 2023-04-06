# -*- coding: utf-8 -*-
import sandbox2d

EARRAPE_RATE = 100000

for i in range(EARRAPE_RATE):
    sandbox2d.sound()

    if (i % (EARRAPE_RATE / 100)) == 0:
        print(str(i / (EARRAPE_RATE / 100))+"%")

print("100%")
