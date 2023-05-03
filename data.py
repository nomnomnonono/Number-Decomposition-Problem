import random
import os

DIR = "data"

os.makedirs(DIR, exist_ok=True)

for i in range(50):
    f = open(f"{DIR}/data{i}.nl", "w")

    for _ in range(1000):
        now = "0."+"".join([str(random.randint(0, 9)) for _ in range(30)])
        f.write(now+"\n")
    f.close()
